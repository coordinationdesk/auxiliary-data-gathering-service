#!/usr/bin/env ruby
# frozen_string_literal: true

#
#	Copyright 2023-2023, Exprivia - DFDA-AS
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Prod: ADGSBaselineReportGenerator $
#

# :nocov:
# Add the RubyLibs dir to load path if not already included
libs_path = File.expand_path(File.join(__dir__, '..', 'lib'))
$LOAD_PATH.unshift(libs_path) unless $LOAD_PATH.include?(libs_path)
# :nocov:

require 'date'
require 'json'
require 'rexml/document'

require 'common_utils'
require 'configuration_handler'
require 'db_handler'
require 'rotating_logger'

###############################
# ADGSBaselineReportGenerator #
###############################

# Collector for all ADGS Baseline Report Generator main component
class ADGSBaselineReportGenerator
  #############
  # CONSTANTS #
  #############

  DEFAULT_CONF_FILE = File.join(__dir__, '..', 'conf', 'adgs_baseline_report_generator.conf').freeze
  REPORT_LAST_DATE = Date.today.prev_day.strftime('%Y-%m-%d 23:59:59.999').freeze

  ################################
  # ReportGeneratorConfiguration #
  ################################

  # Handle the ADGS Baseline Report Generator configuration
  class ReportGeneratorConfiguration < ConfigurationHandler
    include LoggerConfigurationParams
    include DBConfigurationParams

    CONF_OUTPUT_TAG = 'Output'
    CONF_EMPTY_ENTRY_TAG = 'EmptyEntryIfMissing'
    CONF_EMPTY_ENTRY_DEFAULT = false
    CONF_REPORTS_TAG = 'Reports'
    CONF_SATELLITE_TAG = 'Satellite'
    CONF_FILE_TYPE_TAG = 'FileType'

    attr_reader :output, :reports, :add_empty_entries

    # Class initialization
    def initialize(conf_file)
      # Load the configuration
      super(conf_file)

      logger_params(base_dir: __dir__)
      db_params(base_dir: __dir__)

      @output = File.absolute_path(get(CONF_OUTPUT_TAG, mandatory: true), __dir__)
      raise IOError, "Output path '#{@output}' is not a valid output directory" unless File.directory?(@output)
      raise IOError, "Output path '#{@output}' is not writable" unless File.writable?(@output)

      @add_empty_entries = get(CONF_EMPTY_ENTRY_TAG, mandatory: false) || CONF_EMPTY_ENTRY_DEFAULT

      extract_reports
    end

    private

    # Extract the list of reports to be generated for each satellite
    def extract_reports
      @reports = []
      get(CONF_REPORTS_TAG, mandatory: true).each do |report|
        unless report.key?(CONF_SATELLITE_TAG)
          raise ArgumentError, "Invalid report '#{report}' in tag '#{CONF_REPORTS_TAG}':"\
                               " tag #{CONF_SATELLITE_TAG} must be set"
        end

        @reports.push({ satellite: report[CONF_SATELLITE_TAG],
                        file_types: report[CONF_FILE_TYPE_TAG] || [] })
      end
    end
  end

  #####################
  # ReportGeneratorDB #
  #####################

  # Query the DB for ADGS Baseline Report Generator operations
  class ReportGeneratorDB < DBHandler::DB
    # Satellites table
    SAT_TAB = 't_satellites'
    private_constant(:SAT_TAB)

    # Inventory table
    INV_TAB = 't_srv_inventory'
    private_constant(:INV_TAB)

    # Fields to select on inventory table
    FILE_SELECT_FIELDS = %W[#{INV_TAB}.name #{INV_TAB}.attributes
                            to_char(#{INV_TAB}.validitystart,\ 'YYYY-MM-DD\ HH24:MI:SS')\ validitystart
                            to_char(#{INV_TAB}.validitystop,\ 'YYYY-MM-DD\ HH24:MI:SS')\ validitystop
                            to_char(#{INV_TAB}.creationdate,\ 'YYYY-MM-DD\ HH24:MI:SS.MS')\ creationdate].freeze
    private_constant(:FILE_SELECT_FIELDS)

    # Initialize the DB
    def initialize(conf, logger)
      check_type(conf, 'conf', [ReportGeneratorConfiguration])
      check_type(logger, 'logger', [RotatingLogger])

      @log = logger

      # Initialize the DB
      super(conf.db_conf_file, host_index: conf.db_host_index, decrypter: conf.db_pwd_decrypter)
    end

    def get_satellite_data(sat_acr)
      # Compose select query
      query = compose_satellite_select_query(sat_acr)

      @log.debug { "Executing satellite select query \"#{query.compose}\"" }

      # Execute select query
      rows = exec(query, no_throw: false)

      # Check the query result
      raise "Invalid query result for satellite acronym '#{sat_acr}'" if rows.nil? || rows.ntuples != 1

      @log.debug do
        "Satellite select query result for satellite '#{sat_acr}': id #{rows[0]['satelliteid']}; " \
                   "name = '#{rows[0]['satellitename']}'"
      end

      # Return the query result
      [rows[0]['satelliteid'], sat_acr, rows[0]['satellitename']]
    end

    # Perform a select query on the DB in order to retrieve the data for a specific satellite and file type combination
    def get_file_data(sat_id, file_type, &block)
      # Compose select query
      query = compose_file_select_query(sat_id, file_type)

      @log.debug { "Executing file select query \"#{query.compose}\"" }

      # Execute the select query
      rows = exec(query, no_throw: false)

      # Handle the selected row
      handle_selected_row(rows[0], &block) if rows.ntuples == 1

      # Return the total number of rows selected
      rows.ntuples
    end

    private

    # Create the Select query for satellite data retrieving
    def compose_satellite_select_query(sat_acr)
      # SELECT
      satellite_query_select = DBHandler::Query.new(type: :select)
      satellite_query_select.fields = %W[#{SAT_TAB}.satelliteid #{SAT_TAB}.satellitename]
      # FROM
      satellite_query_select.from = SAT_TAB
      # WHERE
      where = DBHandler::WhereClause.new.add_element("#{SAT_TAB}.satelliteacronym", :eq, "'#{sat_acr}'")
      satellite_query_select.where = where

      # Return the query
      satellite_query_select
    end

    # Create the Select Query
    def compose_file_select_query(sat_id, file_type)
      # SELECT
      file_query_select = DBHandler::Query.new(type: :select)
      file_query_select.fields = FILE_SELECT_FIELDS
      # FROM
      file_query_select.from = INV_TAB
      # WHERE
      where = DBHandler::WhereClause.new.add_element("#{INV_TAB}.satellite_id", :eq, "'#{sat_id}'")
      where.add_element("#{INV_TAB}.name", :like, "'%#{file_type}%'")
      where.add_element("#{INV_TAB}.creationdate", :le, "'#{REPORT_LAST_DATE}'")
      file_query_select.where = where
      # ORDER BY
      file_query_select.order_by = "#{INV_TAB}.creationdate DESC"
      # LIMIT
      file_query_select.limit = 1

      # Return the query
      file_query_select
    end

    # Handle a row retrieved by the select query
    def handle_selected_row(row, &block)
      type = parse_attributes(row['attributes'])

      @log.debug do
        "File select query result: name = '#{row['name']}'; type = #{type.nil? ? 'NULL' : "'#{type}'"};" \
                " validity start = '#{row['validitystart']}'; validity stop = '#{row['validitystop']}';" \
                " creation date = '#{row['creationdate']}'"
      end

      # Perform the block operations
      block.call(row['name'], type, DateTime.strptime(row['validitystart'], '%F %T'),
                 DateTime.strptime(row['validitystop'], '%F %T'), DateTime.strptime(row['creationdate'], '%F %T.%L'))
    end

    # Parse attributes field and extract file_type
    def parse_attributes(attributes)
      return nil if attributes.nil?

      js = JSON.parse(attributes)
      sel_obj = js.find { |obj| obj['Name'] == 'productType' }
      sel_obj.nil? ? nil : sel_obj['Value']
    end
  end

  #################
  # ReportHandler #
  #################

  # Handle a single ADGS Baseline Report
  class ReportHandler
    include REXML

    ROOT_TAG = 'ADGSBaseline'
    SATELLITE_TAG = 'Satellite'
    VALIDITY_START_TAG = 'ValidityStart'
    VALIDITY_STOP_TAG = 'ValidityEnd'
    CREATION_DATE_TAG = 'CreationDate'
    LAST_PUBLISHED_FILES_TAG = 'LastPublishedFiles'
    LAST_PUBLISHED_FILES_COUNT_ATTRIBUTE = 'count'
    AUXILIARY_DATA_FILE_TAG = 'AuxiliaryDataFile'
    AUXILIARY_FILENAME_TAG = 'Filename'
    AUXILIARY_FILETYPE_TAG = 'Filetype'
    AUXILIARY_VALIDITY_WINDOW_TAG = 'ValidityWindow'
    AUXILIARY_VALIDITY_START_TAG = 'ValidityStart'
    AUXILIARY_VALIDITY_STOP_TAG = 'ValidityEnd'
    AUXILIARY_PUBLICATION_DATE_TAG = 'PublicationDate'

    REPORT_VALIDITY_START_DATE = Date.today.prev_day.strftime('%Y-%m-%dT00:00:00').freeze
    REPORT_VALIDITY_START_NAME = Date.today.prev_day.strftime('%Y%m%dT000000').freeze
    REPORT_VALIDITY_STOP_DATE = Date.today.strftime('%Y-%m-%dT00:00:00').freeze
    REPORT_VALIDITY_STOP_NAME = Date.today.strftime('%Y%m%dT000000').freeze

    # Class initialization
    def initialize(output, sat_acr, sat_name, logger)
      check_type(output, 'output', [String])
      check_type(sat_acr, 'sat_acr', [String])
      check_type(sat_name, 'sat_name', [String])
      check_type(logger, 'logger', [RotatingLogger])

      @log = logger
      @report_creation_date = DateTime.now

      check_output(output, sat_acr)

      initialize_document(sat_name)
    end

    # Add an entry on the report
    def add_entry(file_name, type, validity_start, validity_stop, creation_date)
      auxiliary_data_file = @last_published_files.add_element(AUXILIARY_DATA_FILE_TAG)
      auxiliary_data_file.add_element(AUXILIARY_FILENAME_TAG).text = file_name
      auxiliary_data_file.add_element(AUXILIARY_FILETYPE_TAG).text = type
      set_validity(auxiliary_data_file, validity_start, validity_stop)
      auxiliary_data_file.add_element(AUXILIARY_PUBLICATION_DATE_TAG).text = format_data(creation_date)
      @log.debug { "Added new file entry with name '#{file_name}' in the report '#{@output_path}'" }
      @count += 1
    end

    # Close the report
    def close
      @log.debug { "Closing and writing the report '#{@output_path}'" }
      @last_published_files.attributes[LAST_PUBLISHED_FILES_COUNT_ATTRIBUTE] = @count

      formatter = Formatters::Pretty.new(4)
      formatter.compact = true
      formatter.width = 999_999_999_999
      File.open(@output_path, File::WRONLY) do |file|
        formatter.write(@final_document.root, file)
      end

      @log.debug { "Report '#{@output_path}' closed" }
      @count
    end

    private

    # Compose the final output file path and initialize it
    def check_output(output, sat_acr)
      @output_path = File.join(output, "#{sat_acr}_ADFBASELINE_#{REPORT_VALIDITY_START_NAME}_" \
                                       "#{REPORT_VALIDITY_STOP_NAME}_" \
                                       "#{@report_creation_date.strftime('%Y%m%dT%H%M%S')}.xml")

      # Check if the file already exists
      raise IOError, "Output File '#{@output_path}' already exists" if File.exist?(@output_path)

      # Create the file, prevent subsequent creation of similar files
      File.write(@output_path, '')
    end

    # Initialize the XML document
    def initialize_document(sat_name)
      @log.debug { "Initializing the report '#{@output_path}'" }
      @final_document = Document.new
      root = @final_document.add_element(ROOT_TAG)
      root.add_element(SATELLITE_TAG).text = sat_name
      root.add_element(VALIDITY_START_TAG).text = REPORT_VALIDITY_START_DATE
      root.add_element(VALIDITY_STOP_TAG).text = REPORT_VALIDITY_STOP_DATE
      root.add_element(CREATION_DATE_TAG).text = format_data(@report_creation_date)
      @last_published_files = root.add_element(LAST_PUBLISHED_FILES_TAG)
      @count = 0
      @log.debug { "Report '#{@output_path}' initialized" }
    end

    # Format a DateTime to be compliant with the report formats
    def format_data(data)
      data.is_a?(DateTime) ? data.strftime('%Y-%m-%dT%H:%M:%S') : data
    end

    # Set the validity start of a given auxiliary file entry
    def set_validity(auxiliary_data_file, validity_start, validity_stop)
      validity_window = auxiliary_data_file.add_element(AUXILIARY_VALIDITY_WINDOW_TAG)
      validity_window.add_element(AUXILIARY_VALIDITY_START_TAG).text = format_data(validity_start)
      validity_window.add_element(AUXILIARY_VALIDITY_STOP_TAG).text = format_data(validity_stop)
    end
  end

  ####################
  # INSTANCE METHODS #
  ####################

  # Initialize the report generator with the configuration file provided
  def initialize(conf_file)
    check_type(conf_file, 'conf_file', [String])
    @conf = ReportGeneratorConfiguration.new(conf_file)

    @log = RotatingLogger.new(@conf.logger_file, level: @conf.logger_level,
                                                 rotation_period: @conf.logger_rotation_period,
                                                 max_file_num: @conf.logger_files_to_keep,
                                                 text_file_num: @conf.logger_text_files)
    @log.set_as_default_logger

    @db = ReportGeneratorDB.new(@conf, @log)
  end

  # Execute the required job
  def execute_job
    @log.info { 'Reports generation start' }

    @errors = false
    @conf.reports.each { |conf_report| handle_satellite(conf_report) }

    @log.info { "Report generation ended #{@errors ? 'with errors' : 'cleanly'}" }
    !@errors
  end

  private

  def handle_satellite(conf_report)
    @log.info { "Starting handling report for #{current_sat(conf_report[:satellite])}" }

    num_of_files = generate_satellite_report(conf_report)

    # rubocop:disable Style/NumericPredicate
    if num_of_files == 0
      # rubocop:enable Style/NumericPredicate
      @log.warn { "No file published for #{current_sat(conf_report[:satellite])}" }
    else
      @log.info { "End handling report for #{current_sat(conf_report[:satellite])}, #{num_of_files} files added" }
    end
  rescue StandardError => e
    print_error(e, "Error handling report for #{current_sat(conf_report[:satellite])}")
    @errors = true
  end

  # Generate the report for a satellite
  def generate_satellite_report(conf_report)
    sat_id, sat_acr, sat_name = @db.get_satellite_data(conf_report[:satellite])
    report = ReportHandler.new(@conf.output, sat_acr, sat_name, @log)

    conf_report[:file_types].each { |file_type| handle_file_type(report, sat_acr, sat_id, file_type) }

    report.close
  end

  # Handle a file type for the given satellite
  def handle_file_type(report, sat_acr, sat_id, file_type)
    @log.debug { "Start handling #{current_type_and_sat(file_type, sat_acr)}" }

    add_report_entry(report, sat_acr, sat_id, file_type)

    @log.debug { "End handling #{current_type_and_sat(file_type, sat_acr)}" }
  rescue StandardError => e
    print_error(e, "Error handling #{current_type_and_sat(file_type, sat_acr)}",
                logger_method: @log.method(:warn))
    @errors = true
  end

  # Generate the report entry for a single file_type
  def add_report_entry(report, sat_acr, sat_id, file_type)
    num_row = @db.get_file_data(sat_id, file_type) do |name, type, validity_start, validity_stop, creation_date|
      if type.nil?
        @log.warn { "No productType attribute for file '#{name}' of #{current_type_and_sat(file_type, sat_acr)}" }
      end

      report.add_entry(name, type, validity_start, validity_stop, creation_date)
    end

    return if num_row == 1

    report.add_entry(nil, file_type, nil, nil, nil) if @conf.add_empty_entries
    @log.warn { "No data for #{current_type_and_sat(file_type, sat_acr)}" }
  end

  # Fixed log message for current satellite
  def current_sat(sat_acr)
    "satellite '#{sat_acr}'"
  end

  # Fixed log message for current file type and satellite
  def current_type_and_sat(file_type, sat_acr)
    "file_type '#{file_type}' for #{current_sat(sat_acr)}"
  end
end

# :nocov:
# If the executed tile is this file
if $PROGRAM_NAME == __FILE__
  valid = false
  begin
    # Get the configuration file
    conf_file = File.absolute_path(ARGV.empty? ? ADGSBaselineReportGenerator::DEFAULT_CONF_FILE : ARGV.shift)

    # Create the Pre Inventory object and execute the job
    generator = ADGSBaselineReportGenerator.new(File.absolute_path(conf_file, __dir__))
    valid = generator.execute_job
  rescue StandardError => e
    # Print the error in both file and stdout
    print_error(e, logger_method: DefaultLogger.method(:fatal))
    unless DefaultLogger.instance_variable_get(:@fatal) == DefaultLogger.method(:default_fatal)
      print_error(e, logger_method: DefaultLogger.method(:default_fatal))
    end

    # Exit with failure status
    exit(false)
  end

  # Exit with status derived from job execution result
  exit(valid)
end
# :nocov:
