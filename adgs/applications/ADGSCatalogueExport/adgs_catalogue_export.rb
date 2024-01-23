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
# $Prod: ADGSCatalogueExport $

# :nocov:
# Add the RubyLibs dir to load path if not already included
libs_path = File.expand_path(File.join(__dir__, '..', 'lib'))
$LOAD_PATH.unshift(libs_path) unless $LOAD_PATH.include?(libs_path)
# :nocov:

require 'fileutils'
require 'db_handler'
require 'splunk_handler'
require 'configuration_handler'
require 'json'
require 'net/http'
require 'openssl'
require 'common_utils'

# Store and handle the catalogue
module ADGSCatalogueExport
  ########################
  ### MODULE CONSTANTS ###
  ########################

  # Valid statuses for ADGSCatalogueExport SPLUNK message
  CATALOGUE_STAT = {
    started: 'STARTED',
    failed: 'FAILED',
    completed: 'COMPLETED'
  }.freeze

  # Store the reference date and the snapshot date
  REF_DATE = Date.today.prev_day.strftime('%Y-%m-%d')
  REF_DATE_NEXT = Date.today.strftime('%Y-%m-%d')
  SNAPSHOT_DATE = Date.today.prev_day.strftime('%Y%m%d235959')

  SAT_ACRONYMS = %w[S1_ S1A S1B S2_ S2A S2B S3_ S3A S3B].freeze

  ######################
  ### MODULE METHODS ###
  ######################

  # Compose and send a ADGSCatalogueExport syslog message
  def self.emit_splunk_message(status: nil, num_change: nil)
    # Check status
    raise TypeError, "Argument 'status' must be one of #{CATALOGUE_STAT.keys}" unless CATALOGUE_STAT.key?(status)

    # Check num_change. Must be nil or integer
    num_change = Integer(num_change) unless num_change.nil?

    # Compose message
    smt_msg = "M&C|AuxData Gathering|ADGS|PROGRESS|jobid=\"CE-#{REF_DATE}\"|"
    smt_msg += "dateschanged=#{num_change}|" unless num_change.nil?
    smt_msg += "status=\"#{CATALOGUE_STAT[status]}\"|"

    SPLUNKHandler.emit_syslog('ADGSCatalogueExport', smt_msg)
  end

  ######################
  ### MODULE CLASSES ###
  ######################

  ##########################
  # CatalogueConfiguration #
  ##########################

  # Configuration Handler for the catalogue
  class CatalogueConfiguration < ConfigurationHandler
    attr_reader :workroot, :output_tar_path, :sys, :prov, :snapshot_to_keep,
                :remote_data_limit, :remote_base_url, :remote_auth

    include DBConfigurationParams

    def initialize(conf_file)
      # Load the configuration
      super(conf_file)

      # Set base working dir
      @workroot = File.absolute_path(get('Workroot'), __dir__)
      @output_tar_path = File.absolute_path(get('OutputTarPath'), __dir__)

      # Set Catalogue parameters
      set_catalogue

      # Set DB parameters
      db_params

      # Set Remote parameters
      set_remote
    end

    private

    # Set Catalogue parameters
    def set_catalogue
      # Catalogue parameters
      @sys = get(%w[Catalogue System])
      @prov = get(%w[Catalogue Provider])
      @snapshot_to_keep = get(%w[Catalogue SnapshotToKeep], mandatory: false)
      if snapshot_to_keep.nil?
        puts 'Set Catalogue.SnapshotToKeep to default value 3'
        @snapshot_to_keep = 3
      elsif snapshot_to_keep < 1
        puts 'Catalogue.SnapshotToKeep must be >= 1. Set to 1'
        @snapshot_to_keep = 1
      end
    end

    # Set Remote parameters
    def set_remote
      @remote_base_url = File.join(get(%w[Remote BaseURL]))
      @remote_auth = set_remote_auth

      set_remote_data_limit
    end

    # Extract the Remote Auth data
    def set_remote_auth
      basic_auth = get(%w[Remote BasicAuth], mandatory: false)
      return { type: :basic, data: basic_auth } unless basic_auth.nil?

      remote_user = get(%w[Remote User])
      remote_password = get(%w[Remote Password])
      { type: :user_pwd, user: remote_user, pwd: remote_password }
    end

    # Set the Remote data limit value
    def set_remote_data_limit
      @remote_data_limit = get(%w[Remote DataLimit], mandatory: false)
      if @remote_data_limit.nil?
        puts 'Set Remote.DataLimit to default value 1000'
      else
        puts 'Remote.DataLimit must be >= 1. Set to 1' if @remote_data_limit < 1
        puts 'Remote.DataLimit must be <= 1000. Set to 1000' if @remote_data_limit > 1000
      end
      @remote_data_limit = [[@remote_data_limit || 1000, 1000].min, 1].max
    end
  end

  ########################
  # CatalogueFileManager #
  ########################

  # Manage the files in the catalogue
  class CatalogueFileManager
    TEMP_SNAPSHOT_NAME = '.TEMP_SNAPSHOT'
    private_constant(:TEMP_SNAPSHOT_NAME)

    # Initialize the file manager
    def initialize(conf)
      check_type(conf, 'conf', [CatalogueConfiguration])

      @conf = conf
      @valid_snapshot_path = File.join(@conf.workroot, "SNAPSHOT_#{ADGSCatalogueExport::REF_DATE}")
    end

    # Check if the final snapshot folder already exists. If not, initialize the temporary snapshot folder
    def check_current_valid_snapshot
      raise IOError, "The snapshot '#{@valid_snapshot_path}' already exist" if File.exist?(@valid_snapshot_path)

      FileUtils.mkdir_p(temp_snapshot_path)
    end

    # Check if is a valid sat short name
    def valid_sat?(sat)
      SAT_ACRONYMS.include?(sat)
    end

    # Check if is a valid year
    def valid_year?(year)
      !/\A\d{4}\z/.match(year).nil?
    end

    # Check if is a valid month
    def valid_month?(month)
      !/\A\d{2}\z/.match(month).nil? && month.to_i.between?(1, 12)
    end

    # Check if is a valid day
    def valid_day?(day)
      !/\A\d{2}\z/.match(day).nil? && day.to_i.between?(1, 31)
    end

    # Check data validity. Defaults pass the check
    def check_valid_input(sat, year = '2020', month = '01', day = '01')
      raise RangeError, "Invalid 'sat' value '#{sat}'" unless valid_sat?(sat)
      raise RangeError, "Invalid 'year' value '#{year}'" unless valid_year?(year)
      raise RangeError, "Invalid 'month' value '#{month}'" unless valid_month?(month)
      raise RangeError, "Invalid 'day' value '#{day}'" unless valid_day?(day)
    end

    # Check if the file is in the temporary folder
    def check_in_temporary(file_name)
      return if File.expand_path(file_name).start_with?(File.expand_path(temp_snapshot_path))

      raise ArgumentError, "The file '#{file_name}' is not in the temporary snapshot "
    end

    # Return the path of the temporary file in the temporary snapshot folder
    def temp_snapshot_temp_file_path(sat, year, month, day)
      check_valid_input(sat, year, month, day)

      File.join(temp_snapshot_path(sat, year, month), ".#{sat}_#{year}#{month}#{day}.tmp")
    end

    # Return a regex to test if the filename is correct and extract the date as a group
    def valid_file_name_regex(sat, year, month)
      check_valid_input(sat, year, month) # Fake day

      /\A#{sat}_#{year}#{month}(\d{2})_#{@conf.sys}_#{@conf.prov}_catalogue_(\d{4})(\d{2})(\d{2})235959.json\z/.freeze
    end

    # Get the current snapshot in the workroot
    def current_snapshots
      snapshots = []
      temp = nil
      each_in_dir(@conf.workroot, check_writable: true) do |path, snapshot|
        snapshots << path if !/\ASNAPSHOT_\d{4}-\d{2}-\d{2}\z/.match(snapshot).nil? && File.directory?(path)
        temp = path if snapshot == TEMP_SNAPSHOT_NAME && File.directory?(path)
      end
      [snapshots.sort!, temp]
    end

    # Remove a file if in the temp directory
    def remove_temporary(file_name)
      check_in_temporary(file_name)
      FileUtils.rm_rf(file_name)
    end

    # Create the path in the temporary snapshot
    def create_temp_path(sat, year, month)
      check_valid_input(sat, year, month) # Fake day

      FileUtils.mkdir_p(temp_snapshot_path(sat, year, month))
    end

    # Promote a temp file or a old file to the new valid file
    def promote_temp_to_valid_file(from, sat, year, month, day)
      check_in_temporary(from)

      File.rename(from, temp_snapshot_valid_file_path(sat, year, month, day))
    end

    # Hard link an old file
    def hard_link_file(from, sat, year, month, day)
      check_valid_input(sat, year, month, day)

      FileUtils.ln(from, temp_snapshot_valid_file_path(sat, year, month, day))
    end

    # Promote the temporary snapshot folder to a new valid snapshot folder
    def promote_temp_to_valid_snapshot
      tmp_path = temp_snapshot_path
      return unless File.exist?(tmp_path)

      # Move the snapshot from temporary to valid folder
      FileUtils.mv(tmp_path, @valid_snapshot_path)

      # Check the temporary tart path
      tar = "#{@conf.output_tar_path}_TMP"
      FileUtils.rm_rf(tar) if File.exist?(tar)

      # Cerate a tar.gz of the snapshot
      create_tar('-czf', @valid_snapshot_path, tar, dir_empty?(@valid_snapshot_path) ? '.' : '*')
      FileUtils.mv(tar, @conf.output_tar_path)
    end

    # Clean the old snapshots
    def clean_old_snapshots
      # Get the current snapshots
      snapshot, = current_snapshots

      # Keep only the first 'snapshot_to_keep' files
      FileUtils.rm_rf(snapshot.shift) while snapshot.size > @conf.snapshot_to_keep
    end

    # Return the folder in the temp snapshot based on the given parameters
    def temp_snapshot_path(sat = nil, year = nil, month = nil)
      path = File.join(@conf.workroot, TEMP_SNAPSHOT_NAME)
      valid_sat?(sat) ? (path = File.join(path, sat)) : (return path)
      valid_year?(year) ? (path = File.join(path, year)) : (return path)
      valid_month?(month) ? File.join(path, month) : path
    end

    # Return the new valid file name
    def temp_snapshot_valid_file_path(sat, year, month, day)
      check_valid_input(sat, year, month, day)

      file_name = "#{sat}_#{year}#{month}#{day}_#{@conf.sys}_#{@conf.prov}_catalogue_#{SNAPSHOT_DATE}.json"
      File.join(temp_snapshot_path(sat, year, month), file_name)
    end
  end

  ####################
  # CatalogueElement #
  ####################

  # Single element of the catalogue
  class CatalogueElement
    attr_accessor :on_dir, :id, :on_db, :file, :temp, :processed

    def initialize
      @on_db = false
      @id = []
      @on_dir = false
      @file = nil
      @temp = false
      @processed = false
    end

    # Print the element informations
    def print_data(tabs = '  ', tabs_start = '')
      "#{tabs_start}Object #{@object_id}: (\n" \
      "#{tabs}on_db     = #{on_db}\n" \
      "#{tabs}id        = [#{id.sort.join(', ')}]\n" \
      "#{tabs}on_dir    = #{on_dir}\n" \
      "#{tabs}file      = '#{file}'\n" \
      "#{tabs}temp      = #{temp}\n" \
      "#{tabs}processed = #{processed}\n" \
      "#{tabs[0...-2]})\n"
    end

    # Set the element to on_db and save the record id
    def add_on_db(new_id)
      unless (new_id.is_a?(Integer) && new_id > 0) || (new_id.is_a?(String) && new_id.to_i > 0)
        raise TypeError, "Argument 'new_id' must be a Integer or String representing a positive number"
      end

      @on_db = true
      @id.push(new_id.to_i)
      @id.size == 1 ? 1 : 0
    end

    # Set the file on_dir and set the filename and if is in a temporary folder
    def add_on_dir(new_file, new_temp)
      check_type(new_file, 'new_file', [String])
      check_type(new_temp, 'new_temp', [], boolean_allowed: true)

      @on_dir = true
      @temp ||= new_temp
      @file = new_file if file.nil? || new_temp
    end
  end

  #################
  # CatalogueData #
  #################

  # The catalogue
  class CatalogueData
    attr_accessor :data

    # Initialize the data structure
    def initialize(cfm)
      check_type(cfm, 'cfm', [CatalogueFileManager])

      # Store the CatalogueFileManager
      @cfm = cfm

      # Initialize data
      initialize_data

      # Add local files to data
      add_local_files
    end

    # Perform operation on each element of the catalogue
    def each
      @data.each do |sat, year_d|
        year_d.each do |year, month_d|
          month_d.each do |month, day_d|
            day_d.each { |day, el| yield(sat, year, month, day, el) }
          end
        end
      end
    end

    # Set on_db for a leaf of the data structure.
    def set_on_db(id, sat, year, month, day)
      @cfm.check_valid_input(sat, year, month, day)

      @data[sat][year][month][day].add_on_db(id)
    end

    # Check if all filet that have to be downloader are correctly processed
    def all_processed?
      all = true
      each { |_sat, _year, _month, _day, el| all &&= el.processed }
      all
    end

    # Print the data structure
    def print_data(data = @data, tabs = '  ')
      if data.is_a?(Hash)
        result = "{\n"
        data.each { |k, v| result = "#{result}#{tabs}'#{k}' = #{print_data(v, "#{tabs}  ")}" }
        "#{result}#{tabs[0...-2]}}\n"
      else
        data.print_data(tabs)
      end
    end

    private

    # Initialize data
    def initialize_data
      # Set default value as a new CatalogueElement
      @data = Hash.new do |h_d, k_s|
        h_d[k_s] = Hash.new do |h_s, k_y|
          h_s[k_y] = Hash.new { |h_y, k_m| h_y[k_m] = Hash.new { |h_m, k_d| h_m[k_d] = CatalogueElement.new } }
        end
      end
    end

    # Add the locals files to data
    def add_local_files
      # Find temp folder and last snapshot folder
      snapshots, temp = @cfm.current_snapshots

      # Add to the data tree the files on the last snapshot folder
      add_satellites(snapshots[-1], false) unless snapshots.empty?

      # Add to the data tree the files on the temp folder, if exists
      add_satellites(temp, true) unless temp.nil?
    end

    # Add all the 'sat' folders
    def add_satellites(path_snapshot, temp)
      # Add to the data tree all the 'sat' folder in the given snapshot folder
      each_in_dir(path_snapshot, check_writable: true) do |path, sat|
        # If its valid data, add all years subfolders, else delete the file if in temp tree
        if @cfm.valid_sat?(sat) && File.directory?(path)
          add_years(path, temp, @data[sat], sat)
        elsif temp
          @cfm.remove_temporary(path)
        end
      end
    end

    # Add all the 'year' folders
    def add_years(path_sat, temp, sat_d, sat)
      # Add to the data tree all years folder in the given sat folder
      each_in_dir(path_sat, check_writable: true) do |path, year|
        # If its valid data, add all months subfolders, else delete the file if in temp tree
        if @cfm.valid_year?(year) && File.directory?(path)
          add_months(path, temp, sat_d[year], sat, year)
        elsif temp
          @cfm.remove_temporary(path)
        end
      end
    end

    # Add all the 'month' folders
    def add_months(path_year, temp, year_d, sat, year)
      # Add to the data tree all months folder in the given year folder
      each_in_dir(path_year, check_writable: true) do |path, month|
        # If its valid data, add all days files, else delete the file if in temp tree
        if @cfm.valid_month?(month) && File.directory?(path)
          file_name_regex = @cfm.valid_file_name_regex(sat, year, month)
          add_days(path, temp, year_d[month], file_name_regex)
        elsif temp
          @cfm.remove_temporary(path)
        end
      end
    end

    # Add all the 'day' files
    def add_days(path_month, temp, month_d, file_name_regex)
      # Add to the data tree all days files in the given month folder
      each_in_dir(path_month, check_writable: true) do |path, file|
        # If its valid data, add all days files, else delete the file if in temp tree
        m = file_name_regex.match(file)
        if File.file?(path) && !m.nil? && (temp || path_month.include?("#{m[2]}-#{m[3]}-#{m[4]}"))
          month_d[m[1]].add_on_dir(path, temp)
        elsif temp
          @cfm.remove_temporary(path)
        end
      end
    end
  end

  ###############
  # CatalogueDB #
  ###############

  # Query the DB for ADGSCatalogueExport operations
  class CatalogueDB < DBHandler::DB
    # Catalogue table
    CAT_TAB = 't_srv_catalogue_change_registry'
    private_constant(:CAT_TAB)

    # Satellites table
    SAT_TAB = 't_satellites'
    private_constant(:SAT_TAB)

    # Regular expression for date check and year/month/day extraction
    REGEX_DATE = /\A(?<year>\d{4})-(?<month>\d{2})-(?<day>\d{2}) \d{2}:\d{2}:\d{2}\z/.freeze
    private_constant(:REGEX_DATE)

    # Initialize the DB
    def initialize(conf)
      check_type(conf, 'conf', [CatalogueConfiguration])

      # Initialize the DB
      super(conf.db_conf_file, host_index: conf.db_host_index, decrypter: conf.db_pwd_decrypter)
    end

    # Perform a select Query on the DB
    def data_from_db(&block)
      # Compose and execute the select query
      rows = exec(compose_select_query) { |row| handle_selected_row(row, &block) }

      # Check if the query works
      raise 'Error in select query to DB.' if rows.nil?

      # Return the total number of rows selected
      rows.ntuples
    end

    # Delete from the DB the processed rows
    def delete_rows(id_list)
      check_type(id_list, 'id', [Array])
      unless id_list.all? { |id| (id.is_a?(Integer) && id > 0) || (id.is_a?(String) && id.to_i > 0) }
        raise TypeError, "Each element of argument 'id' must be a positive Integer or a String"
      end

      # Compose and execute the select query
      rows = exec(compose_delete_query(id_list))

      # Check if the query works
      check_delete_query_result(id_list, rows)

      rows.ntuples
    end

    private

    # Create the Select Query
    def compose_select_query
      # SELECT
      query_select = DBHandler::Query.new(type: :select)
      query_select.fields = ["#{CAT_TAB}.id", "#{CAT_TAB}.sensingstartdate", "#{SAT_TAB}.satelliteacronym"]
      # FROM
      query_select.from = CAT_TAB
      # JOIN
      join_on = DBHandler::WhereClause.new.add_element("#{SAT_TAB}.satelliteid", :eq, "#{CAT_TAB}.satellite_id")
      query_select.join = DBHandler::JoinClause.new.add_join(SAT_TAB, on: join_on)
      # WHERE
      where = DBHandler::WhereClause.new.add_element("#{SAT_TAB}.satelliteacronym", :in, SAT_ACRONYMS)
      where.add_element("#{CAT_TAB}.modificationDate", :lt, "'#{ADGSCatalogueExport::REF_DATE_NEXT} 00:00:00.000'")
      query_select.where = where
      # ORDER BY
      query_select.order_by = "#{CAT_TAB}.sensingstartdate"

      # Return the query
      query_select
    end

    # Handle a row retrieved by the select query
    def handle_selected_row(row, &block)
      data = REGEX_DATE.match(row['sensingstartdate'])

      # Perform the block operations
      block.call(row['id'], row['satelliteacronym'], data[:year], data[:month], data[:day])
    rescue StandardError => e
      # In case of error print a warning
      print_error(e, "error handling record with id #{row['id']} on '#{CAT_TAB}':",
                  logger_method: DefaultLogger.method(:warn))
    end

    # Create the Delete Query
    def compose_delete_query(id_list)
      query_delete = DBHandler::Query.new(type: :delete)
      # FROM
      query_delete.from = CAT_TAB
      # WHERE
      query_delete.where = DBHandler::WhereClause.new.add_element("#{CAT_TAB}.id", :in, id_list)
      query_delete.returning = "#{CAT_TAB}.id"

      # Return the query
      query_delete
    end

    # Check the returning list of deleted rows
    def check_delete_query_result(id_list, rows)
      return if !rows.nil? && rows.ntuples == id_list.size

      ids = id_list.join(', ')
      raise "Error deleting IDs [#{ids}], nothing has been deleted." if rows.nil?

      raise "Error deleting IDs [#{ids}], only IDs [#{rows.map { |row| row['id'] }.join(', ')}] deleted."
    end
  end

  ########################
  # CatalogueJsonHandler #
  ########################

  # JSON file handler which permit to append data in the "value" tag of the given string to the "value" tag of a json
  #  file. If the file does not exist it will be created, otherwise it will be wiped.
  class CatalogueJsonHandler
    # "value" tag to search
    VALUE_TAG = 'value'
    private_constant(:VALUE_TAG)

    # Preamble of the file
    PREAMBLE = '{"@odata.context":"$metadata#Products","value":['
    private_constant(:PREAMBLE)

    # Closure of the file
    CLOSURE = ']}'
    private_constant(:CLOSURE)

    attr_reader :file_name

    # Initialize the JSON handler, internally reset the file and write the preamble.
    def initialize(file_name)
      @f = nil
      @first = true
      self.file_name = file_name
    end

    # Set a new file name if its a valid path. Parent directory and file (if exists) must be writable. This method also
    # reset the file and write the preamble.
    def file_name=(new_val)
      close_file

      # Check value
      if File.exist?(new_val)
        raise IOError, "File '#{new_val}' not writable." unless File.writable?(new_val)
      else
        parent_name = File.dirname(new_val)
        raise IOError, "Directory '#{parent_name}' not writable." unless File.writable?(parent_name)
      end

      # Store the file name and write the new data
      @file_name = new_val
      @f = File.open(@file_name, 'w')
      write_preamble
    end

    # Add the given data to the file.
    def add_data(data)
      # Check input
      raise TypeError, "Argument 'data' must be a String containing valid JSON" unless data.is_a?(String)
      raise IOError, 'File not open.' if @f.nil?

      # Parse new data
      json_obj = JSON.parse(data)[VALUE_TAG]
      raise ArgumentError, "Argument 'data' = '#{data}' must be a JSON with tag '#{VALUE_TAG}'" if json_obj.nil?

      # Prevent add empty data
      num_of_obj = json_obj.size
      return 0 if num_of_obj.zero?

      # Write new data to the file
      @f.write(',') unless @first
      @f.write(JSON.generate(json_obj)[1...-1])

      # Reset first so ',' is prepend to the data
      @first = false

      # Return the number of object added
      num_of_obj
    end

    # Write the closure of the file and close it.
    def write_closure
      if @f.nil?
        puts "WARNING: File '#{@file_name}' not open."
        return
      end

      # Create a new file
      @f.write(CLOSURE)
      close_file
    end

    private

    # Write the preamble of the file
    def write_preamble
      @f.write(PREAMBLE)
      @first = true
    end

    # close the file and reset the value
    def close_file
      @f.close unless @f.nil?
      @f = nil
    end
  end

  ####################
  # CatalogueHandler #
  ####################

  # Handle the catalogue
  class CatalogueHandler
    attr_accessor :records_to_handle, :records_handled, :files_to_update, :files_updated

    # Catalogue Handler initialization.
    def initialize(conf_file)
      # Store configuration handler
      @conf = CatalogueConfiguration.new(conf_file)

      # Create the Catalogue File Manager
      @cfm = CatalogueFileManager.new(@conf)

      # Create the Catalogue Data
      @cat = CatalogueData.new(@cfm)

      # Create the DB handler
      @db = CatalogueDB.new(@conf)

      # Reset counters
      @records_to_handle = @records_handled = @files_to_update = @files_updated = 0
    end

    # Print the data structure
    def print_data
      @cat.print_data
    end

    # Perform all the needed operations for create the archive
    def create_catalogue
      # Check if the final snapshot folder already exists
      @cfm.check_current_valid_snapshot

      # Get the records to handle and the files to update
      @records_to_handle = @db.data_from_db { |id, s, y, m, d| @files_to_update += @cat.set_on_db(id, s, y, m, d) }

      # Update the catalogue
      update_catalogue

      # Returns if any of the files were not processed or if the count of files to update does not match
      unless @cat.all_processed? && @records_to_handle == @records_handled && @files_to_update == @files_updated
        raise 'Catalogue not completely updated'
      end

      # If everything went well, make valid the temporary folder and create the tar.gz
      @cfm.promote_temp_to_valid_snapshot

      # Clean the old snapshots
      @cfm.clean_old_snapshots
    end

    private

    # Update the Catalogue based on the required actions.
    def update_catalogue
      @cat.each do |sat, year, month, day, el|
        if el.on_db
          create_new_element(sat, year, month, day, el)
        elsif el.temp
          rename_element(sat, year, month, day, el)
        else
          update_element(sat, year, month, day, el)
        end
      end
    end

    # Create a new file by downloading data from Remote.
    def create_new_element(sat, year, month, day, element)
      # Delete the previous file if on temporary folder or create the directory tree if doesn't exists
      element.temp ? @cfm.remove_temporary(element.file) : @cfm.create_temp_path(sat, year, month)

      # Get the temporary file path
      tmp_file = @cfm.temp_snapshot_temp_file_path(sat, year, month, day)

      # Download data
      download_to_file(tmp_file, sat, year, month, day)

      # Move temporary file to regular file
      @cfm.promote_temp_to_valid_file(tmp_file, sat, year, month, day)

      # Remove the record from the DB
      @records_handled += @db.delete_rows(element.id)
    rescue StandardError => e
      print_error(e, "Unable to download data: sat '#{sat}', sensing '#{year}-#{month}-#{day}'",
                  logger_method: DefaultLogger.method(:warn))
    else
      # Set the element as correctly processed
      element.processed = true
      @files_updated += 1
    end

    # Download the selected data to the temporary file
    def download_to_file(tmp_file, sat, year, month, day)
      # Create the file
      js = CatalogueJsonHandler.new(tmp_file)

      # Set the sensing limits
      sensing_start, sensing_start_next = sensing_limits(year, month, day)

      # Download all pieces
      offset = 0
      loop do
        limit = js.add_data(data_from_remote(offset, sat, sensing_start, sensing_start_next))
        offset += limit
        break if limit != @conf.remote_data_limit
      end

      # Close the file
      js.write_closure
    end

    # Calculate the sensing limits
    def sensing_limits(year, month, day)
      sensing_start = "#{year}-#{month}-#{day}"
      sensing_start_next = Date.new(year.to_i, month.to_i, day.to_i).next_day.strftime('%Y-%m-%d')
      [sensing_start, sensing_start_next]
    end

    # Request all data
    def data_from_remote(offset, sat, sensing_start, sensing_start_next)
      # Create the request uri
      uri = compose_uri(offset, sat, sensing_start, sensing_start_next)

      # Create the request command
      request = create_request(uri)

      req_options = { use_ssl: uri.scheme == 'https', verify_mode: OpenSSL::SSL::VERIFY_NONE }

      # Execute the command
      resp = Net::HTTP.start(uri.hostname, uri.port, req_options) { |http| http.request(request) }

      # Check the response
      raise "Request to uri '#{uri}' return code is #{resp.code}, message: #{resp.body}" unless resp.code == '200'

      # Return the downloaded data
      resp.body
    end

    # Create the request command
    def compose_uri(offset, sat, sensing_start, sensing_start_next)
      URI.parse("#{File.join(@conf.remote_base_url, 'Products')}?$expand=Attributes&" \
      "$filter=startswith(Name,%20%27#{sat}%27)%20and%20" \
      "(PublicationDate%20lt%20#{ADGSCatalogueExport::REF_DATE_NEXT}T00:00:00.000Z)%20and%20" \
      "(ContentDate%2FStart%20lt%20#{sensing_start_next}T00:00:00.000Z)%20and%20" \
      "(ContentDate%2FStart%20ge%20#{sensing_start}T00:00:00.000Z)&" \
      "$top=#{@conf.remote_data_limit}&$skip=#{offset}&$orderby=ContentDate%2FStart%20desc&dwh=true")
    end

    # Create a new request with the given uri
    def create_request(uri)
      request = Net::HTTP::Get.new(uri)
      request['Authorization'] = "Basic #{@conf.remote_auth[:data]}" if @conf.remote_auth[:type] == :basic
      request.basic_auth(@conf.remote_auth[:user], @conf.remote_auth[:pwd]) if @conf.remote_auth[:type] == :user_pwd
      request
    end

    # Rename an element in the temporary folder that is the result of a previous incomplete run
    def rename_element(sat, year, month, day, element)
      # Rename the file
      @cfm.promote_temp_to_valid_file(element.file, sat, year, month, day)
    rescue StandardError => e
      # :nocov:
      # Defensive coding
      print_error(e, "WARNING: Unable to rename the file '#{element.file}':" \
                     " sat '#{sat}', sensing '#{year}-#{month}-#{day}'. Exception:")
    # :nocov:
    else
      # Set the element as correctly processed
      element.processed = true
    end

    # Update an element by hard link the new file to the last one.
    def update_element(sat, year, month, day, element)
      # Create the folder, if needed
      @cfm.create_temp_path(sat, year, month)

      # Create the hard link
      @cfm.hard_link_file(element.file, sat, year, month, day)
    rescue StandardError => e
      # :nocov:
      # Defensive coding
      print_error(e, "WARNING: Unable to hard link the file '#{element.file}':" \
                     " sat '#{sat}', sensing '#{year}-#{month}-#{day}'. Exception:")
    # :nocov:
    else
      # Set the element as correctly processed
      element.processed = true
    end
  end
end

# :nocov:
# If the executed tile is this file
if $PROGRAM_NAME == __FILE__
  DefaultLogger.error = method(:warn)

  # Emit the first SPLUNK message with started status
  ADGSCatalogueExport.emit_splunk_message(status: :started)

  # Initialize the number of elements updated
  num_change = nil

  begin
    # Get the configuration file
    conf_file = ARGV.empty? ? File.join(__dir__, '..', 'conf', 'adgs_catalogue_export.conf') : ARGV[0]

    # Read local catalogue and import DB changes
    catalogue = ADGSCatalogueExport::CatalogueHandler.new(conf_file)
    catalogue.create_catalogue

    # End log
    puts "Number of records to handle: #{catalogue.records_to_handle}; " \
         "Number of records handled: #{catalogue.records_handled}; " \
         "Number of files to update: #{catalogue.files_to_update}; " \
         "Number of files handled: #{catalogue.files_updated}"

    # Store the changes for SPLUNK message
    num_change = catalogue.files_updated
  rescue StandardError => e
    # Print the error
    print_error(e)

    unless catalogue.nil?
      # End log
      puts "Number of records to handle: #{catalogue.records_to_handle}; " \
           "Number of records handled: #{catalogue.records_handled}; " \
           "Number of files to update: #{catalogue.files_to_update}; " \
           "Number of files handled: #{catalogue.files_updated}"

      # Store the changes for SPLUNK message if possible
      num_change = catalogue.files_updated
    end

    # Emit the second SPLUNK message with error status
    ADGSCatalogueExport.emit_splunk_message(status: :failed, num_change: num_change)

    # Exit with failure status
    exit(false)
  end

  # Emit the second SPLUNK message with completed status
  ADGSCatalogueExport.emit_splunk_message(status: :completed, num_change: num_change)

  # Exit with normal status
  exit(true)
end
# :nocov:
