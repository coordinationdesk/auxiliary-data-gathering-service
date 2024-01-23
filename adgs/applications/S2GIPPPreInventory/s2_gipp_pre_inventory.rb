#!/usr/bin/env ruby
# frozen_string_literal: true

# Copyright 2021-2021, Exprivia SpA.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Exprivia SpA.
#
# $Prod: S2GIPPPreInventory $

# :nocov:
# Add the RubyLibs dir to load path if not already included
libs_path = File.expand_path(File.join(__dir__, '..', 'lib'))
$LOAD_PATH.unshift(libs_path) unless $LOAD_PATH.include?(libs_path)
# # Add the other S2PreInventoryAuxTEC module parts dir to load path if not already included
# $LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'optparse'
require 'open3'
require 'syslog'
require 'socket'
require 'date'

require 'nokogiri'

require 'common_utils'
require 'job_order_handler'
require 'configuration_handler'
require 'db_handler'
require 'mtd_handler'

# Collector for all S2GIPPPreInventory components
module S2GIPPPreInventoryComponents
  DEFAULT_CONF_FILE = File.join('..', 'conf', 's2_gipp_pre_inventory.conf')

  HOST_NAME = Socket.gethostname
  SCRIPT_NAME = File.basename(__FILE__)
  PID = Process.pid.to_s

  def self.log(lvl, msg)
    puts "#{Time.now.strftime('%FT%T.%6N')} #{HOST_NAME} #{SCRIPT_NAME} 02.00 [#{PID}]: [#{lvl}] " + msg
  end

  # Handle the S2 Pre Inventory GIPP configuration
  class S2GIPPPreInventoryConf < ConfigurationHandler
    include DBConfigurationParams

    SCHEMA_ROOT_TAG = 'SchemaRoot'
    FILE_TYPE_SCHEMA_MAP_TAG = 'FileTypeSchemaMap'
    FILE_TYPE_TAG = 'FileType'
    SCHEMA_TAG = 'Schema'

    # Class initialization
    def initialize(conf_file)
      # Load the configuration
      super(conf_file)

      load_file_type_schema

      db_params
    end

    # Return the schema file for the selected file_type
    def file_type_schema(file_type)
      @file_type_schema_map[file_type]
    end

    private

    # Load the map of file_type - schema file
    def load_file_type_schema
      @file_type_schema_map = {}

      schema_root_path = get(SCHEMA_ROOT_TAG, mandatory: false)
      return if schema_root_path.nil?

      schema_root_path = File.absolute_path(schema_root_path, __dir__)

      (get(FILE_TYPE_SCHEMA_MAP_TAG, mandatory: false) || []).each do |rule|
        unless rule.key?(FILE_TYPE_TAG) && rule.key?(SCHEMA_TAG)
          raise IOError, "Invalid rule '#{rule}' in tag '#{FILE_TYPE_SCHEMA_MAP_TAG}'"
        end

        @file_type_schema_map[rule[FILE_TYPE_TAG]] = File.join(schema_root_path, rule[SCHEMA_TAG])
      end
    end
  end

  # Query the DB for S2GIPPPreInventory operations
  class S2GIPPPreInventoryDB < DBHandler::DB
    # Initialize the DB
    def initialize(conf)
      check_type(conf, 'conf', [S2GIPPPreInventoryConf])

      # Initialize the DB
      super(conf.db_conf_file, host_index: conf.db_host_index, decrypter: conf.db_pwd_decrypter)
    end

    # Return the mngsngext field of the t_filetypes table associated to the given filetype
    def manage_extension(file_type)
      get_field_from_table_where('mngsngext', 't_filetypes', 'filetype', file_type) == 't'
    end

    # Return the satelliteid on the t_satellites table associated to the given acronym
    def get_satellite_id(acronym)
      get_field_from_table_where('satelliteid', 't_satellites', 'satelliteacronym', acronym)
    end

    # Return the id on the t_bands table associated to the given band name
    def get_band_id(band)
      get_field_from_table_where('id', 't_bands', 'name', band)
    end

    # Return the id on the t_qualifiedflag table associated to the given qualified name
    def get_qualified(qualified)
      get_field_from_table_where('id', 't_qualifiedflag', 'name', qualified)
    end

    private

    # Get the value of the specified field in the specified table that have the the search_filed matching the given data
    def get_field_from_table_where(field, table, search_filed, data)
      # SELECT
      query_select = DBHandler::Query.new(type: :select)
      query_select.fields = field
      # FROM
      query_select.from = table
      # WHERE
      query_select.where = DBHandler::WhereClause.new.add_element(search_filed, :eq, "'#{data}'")

      # Compose and execute the select query
      rows = exec(query_select)

      return rows[0][field] unless rows.nil? || rows.ntuples != 1 || !rows[0].key?(field)

      raise "Error getting '#{field}' from '#{table}' where '#{search_filed}' is equal to '#{data}'"
    end
  end

  # Read all values of the GIPP file and generate the MTD
  class S2GIPPPreInventoryWrapper
    include MTDHandler::Wrapper

    FIXED_HEADER_PATH = '//Earth_Explorer_Header/Fixed_Header'
    FILE_TYPE_PATH = '//File_Type'
    FILE_VERSION_PATH = '//File_Version'
    SOURCE_PATH = '//Source'
    SYSTEM_PATH = '//System'
    CREATOR_PATH = '//Creator'
    CREATOR_VERSION_PATH = '//Creator_Version'
    CREATION_DATE_PATH = '//Creation_Date'
    VALIDITY_PERIOD_PATH = '//Validity_Period'
    VALIDITY_START_PATH = '//Validity_Start'
    VALIDITY_STOP_PATH = '//Validity_Stop'

    # Extract all the needed value from the metadata
    def initialize(metadata, file_path, file_type, conf, db)
      check_type(conf, 'conf', [S2GIPPPreInventoryConf])
      check_type(db, 'db', [S2GIPPPreInventoryDB])

      metadata = Nokogiri::XML(metadata)
      @header = metadata.dup.remove_namespaces!.at_xpath(FIXED_HEADER_PATH)

      set_defaults

      parse_file_name(db, file_path, file_type)

      parse_metadata

      @qualified = db.get_qualified(validate_xml(metadata, conf.file_type_schema(get_value(FILE_TYPE_PATH)), file_path))
    end

    private

    # Set all non used values of the MTD to the default value
    def set_defaults
      @start_orbit_number = @stop_orbit_number = @quality_info = @sensor_id = @acquisition_station_id = ''
      @processing_station_id = @sensor_mode = @phase = @ascending_flag = @equator_x_longitude = @equator_x_time = ''
      @order_id = ''
      @geographic_localization = []
      @validity_flag = 'TRUE'
      @header_size = 0
      @mission = 1
    end

    # Set the file_name, file_type and derived values
    def parse_file_name(db, file_path, file_type)
      @file_name = File.basename(file_path, db.manage_extension(file_type) ? '.*' : '')
      @data_size = File.size(file_path)
      @file_type = file_type
      @file_class = @file_name[4, 4]
      @satellite_id = db.get_satellite_id(@file_name[0, 3])
      @band = db.get_band_id(File.basename(file_path, '.*')[-3, 3]) if file_type =~ /GIP/
    end

    # Return the value of the given tag
    def get_element(xpath, element: @header)
      element.at_xpath(xpath)
    end

    # Return the value of the given tag
    def get_value(xpath, element: @header)
      get_element(xpath, element: element).text.strip
    end

    # Parse all the needed data on the fixed header
    def parse_metadata
      @file_version = get_value(FILE_VERSION_PATH)
      source = get_element(SOURCE_PATH)
      @system = get_value(SYSTEM_PATH, element: source)
      @source = get_value(CREATOR_PATH, element: source)
      @source_sw_version = get_value(CREATOR_VERSION_PATH, element: source)
      @generation_time = @validation_date = fix_date_time(get_value(CREATION_DATE_PATH, element: source))
      validity_period = get_element(VALIDITY_PERIOD_PATH)
      @validity_start = fix_date_time(get_value(VALIDITY_START_PATH, element: validity_period))
      @validity_stop = fix_date_time(get_value(VALIDITY_STOP_PATH, element: validity_period))
    end

    # Validate the file xml schema, if provided
    def validate_xml(metadata, schema, file_path)
      return 'NOT_AVAILABLE' if schema.nil?

      raise IOError, "Provided schema file '#{schema}' does not exists" unless File.exist?(schema)

      errors = Nokogiri::XML::Schema(File.read(schema)).validate(metadata)

      return 'VALID_PRODUCT' if errors.empty?

      Syslog.open(S2GIPPPreInventoryComponents::SCRIPT_NAME, Syslog::LOG_PID, Syslog::LOG_DAEMON)
      Syslog.log(Syslog::LOG_ALERT, "The [#{file_path}] DO NOT VALIDATE: #{errors.join(' - ')}")
      Syslog.close

      'VALIDATION_FAILURE'
    end

    # Fix incorrect date and time and transform it from string to DateTime object
    def fix_date_time(str)
      return DateTime.new(2000, 1, 1, 0, 0, 0) if str =~ /0000-00-00T00:00:00/
      return DateTime.new(2100, 1, 1, 0, 0, 0) if str =~ /9999-99-99T99:99:99/

      DateTime.strptime(str, 'UTC=%FT%T')
    end
  end

  # Perform the Pre Inventory of the files specified in the JobOrder
  class S2GIPPPreInventory
    # Class initialization
    def initialize(options)
      parse_joborder(options[:job_order])

      @conf = S2GIPPPreInventoryConf.new(options[:conf])
      @db = S2GIPPPreInventoryDB.new(@conf)
    end

    # Execute the required job
    def execute_job
      raise IOError, "File '#{@mtd_path}' already exist" if File.exist?(@mtd_path)
      raise IOError, "File '#{@list_path}' already exist" if File.exist?(@list_path)

      wrapper = S2GIPPPreInventoryWrapper.new(extract_metadata, @file_path, @file_type, @conf, @db)
      wrapper.generate_mtd(@mtd_path)

      File.write(@list_path, "#{@file_name}\n")
    end

    private

    # Parse JobOrder and extract file name and type
    def parse_joborder(job_order)
      job = JobOrder::JobOrderHandler.new(job_order)

      @file_path = job.file_name(1, 1, 1)
      raise IOError, "Cannot find '#{@file_path}' file" unless File.exist?(@file_path)

      @file_type = job.files_type(1, 1)

      @file_name = File.basename(@file_path)
      @file_ext = File.extname(@file_name)
      @file_name_without_ext = File.basename(@file_name, @file_ext)
      @file_ext = @file_ext.strip.upcase.sub(/\./, '')

      @mtd_path = "#{@file_path}.MTD"
      @list_path = File.join(File.dirname(@file_path), "#{S2GIPPPreInventoryComponents::PID}.LIST")
    end

    # Extract the metadata from the input file
    def extract_metadata
      return File.read(@file_path) if @file_ext == 'EOF'

      cmd = "tar -xO#{@file_ext == 'TGZ' ? 'z' : ''}f '#{@file_path}' '#{@file_name_without_ext}.HDR'"
      data, err, status = Open3.capture3(cmd)

      # Check the result
      raise "Command '#{cmd}' fail with status #{status} and error: '#{err}'" unless err.empty? && status.success?

      data
    end
  end
end

# :nocov:
# If the executed tile is this file
if $PROGRAM_NAME == __FILE__

  options = {}
  opt_parser = OptionParser.new do |opts|
    opts.banner = "Usage: #{__FILE__} JOB_ORDER [-c CONF_FILE]\n" \
                  '    JOB_ORDER                        Sets the JobOrder to use'

    options[:conf] = File.absolute_path(S2GIPPPreInventoryComponents::DEFAULT_CONF_FILE)
    opts.on('-c', '--conf CONF_FILE', 'Sets the configuration file path') { |c| options[:conf] = File.absolute_path(c) }
  end

  begin
    opt_parser.parse!
    raise OptionParser::MissingArgument, 'Job Order' if ARGV.empty?

    options[:job_order] = File.absolute_path(ARGV.shift)
  rescue OptionParser::InvalidOption, OptionParser::MissingArgument => e
    puts "Error on options parsing: #{e}"
    puts opt_parser

    # Exit with failure status
    exit(false)
  end

  begin
    S2GIPPPreInventoryComponents.log('I', "Starting #{S2GIPPPreInventoryComponents::SCRIPT_NAME} PreInventory")

    # Create the Pre Inventory object and execute the job
    gipp_pre_inventory = S2GIPPPreInventoryComponents::S2GIPPPreInventory.new(options)
    gipp_pre_inventory.execute_job

    S2GIPPPreInventoryComponents.log('I', "Completed #{S2GIPPPreInventoryComponents::SCRIPT_NAME} PreInventory")
  rescue StandardError => e
    # Print the error
    S2GIPPPreInventoryComponents.log('E', e.message)
    puts e.backtrace

    # Exit with failure status
    exit(150)
  end

  # Exit with status derived from job execution result
  exit(true)
end
# :nocov:
