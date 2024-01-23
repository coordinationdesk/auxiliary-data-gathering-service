#!/usr/bin/env ruby
# frozen_string_literal: true

# Copyright 2022-2022, Exprivia SpA.
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
# $Prod: ICECONCPreInventory $

# :nocov:
# Add the RubyLibs dir to load path if not already included
libs_path = File.expand_path(File.join(__dir__, '..', 'lib'))
$LOAD_PATH.unshift(libs_path) unless $LOAD_PATH.include?(libs_path)
# :nocov:

require 'optparse'
require 'socket'
require 'date'

require 'common_utils'
require 'job_order_handler'
require 'configuration_handler'
require 'mtd_handler'

# Collector for all ICECONCPreInventory components
module ICECONCPreInventoryComponents
  DEFAULT_CONF_FILE = File.join('..', 'conf', 'ice_conc_pre_inventory.conf')
  REGEX_FILE_NAME = /\Aice_conc_[ns]h_ease2-250_(?:cdr|icdr|icdrft)-v[23]p0_
                     (?<year>\d{4})(?<month>\d{2})(?<day>\d{2})(?<hour>\d{2})(?<minute>\d{2})\.nc\z/x.freeze
  private_constant(:REGEX_FILE_NAME)

  HOST_NAME = Socket.gethostname
  SCRIPT_NAME = File.basename(__FILE__)
  PID = Process.pid.to_s

  def self.log(lvl, msg)
    puts "#{Time.now.strftime('%FT%T.%6N')} #{HOST_NAME} #{SCRIPT_NAME} 02.00 [#{PID}]: [#{lvl}] #{msg}"
  end

  # Handle the ICE CONC Pre Inventory configuration
  class ICECONCPreInventoryConf < ConfigurationHandler
    SOURCE_TAG = 'Source'
    FILE_CLASS_TAG = 'FileClass'
    SATELLITE_ID_TAG = 'SatelliteID'
    MISSION_TAG = 'Mission'

    attr_reader :source, :file_class, :satellite_id, :mission

    # Class initialization
    def initialize(conf_file)
      # Load the configuration
      super(conf_file)

      @source = get(SOURCE_TAG, mandatory: false) || ''
      @file_class = get(FILE_CLASS_TAG)
      @satellite_id = get(SATELLITE_ID_TAG, mandatory: false) || ''
      @mission = get(MISSION_TAG, mandatory: false) || ''
    end
  end

  # Read all values of the ICE CONC file and generate the MTD
  class ICECONCWrapper
    include MTDHandler::Wrapper

    # Extract all the needed value from the metadata
    def initialize(file_path, file_type, conf)
      check_type(conf, 'conf', [ICECONCPreInventoryConf])
      raise IOError, "Cannot find '#{file_path}' file" unless File.exist?(file_path)

      empty_nullable_mandatory
      fixed_defaults_values

      values_from_conf(conf)

      parse_file(file_path)

      @file_type = file_type
    end

    private

    # Set all non used values of the MTD to the default fixed value
    def fixed_defaults_values
      @file_version = '0001'
      @geographic_localization = []
      @validity_flag = 'TRUE'
      @validation_date = DateTime.now
      @header_size = 0
    end

    # Set the values fixed from conf
    def values_from_conf(conf)
      @source = conf.source
      @file_class = conf.file_class
      @satellite_id = conf.satellite_id
      @mission = conf.mission
    end

    # Set all the data retrieved from the file
    def parse_file(file_path)
      @file_name = File.basename(file_path)
      @data_size = File.size(file_path)

      date = REGEX_FILE_NAME.match(@file_name)
      raise IOError, "File '#{file_path}' have an invalid name" if date.nil?

      extract_date_time(date[:year].to_i, date[:month].to_i, date[:day].to_i, date[:hour].to_i, date[:minute].to_i)
    end

    # Fix incorrect date and time and transform it from string to DateTime object
    def extract_date_time(year, month, day, hour, minute)
      @generation_time = DateTime.new(year, month, day, hour, minute)
      @validity_start = DateTime.new(year, month, day)
      @validity_stop = ((@validity_start + 1).to_time - 1 / 1_000_001.0).to_datetime
    end
  end

  # Perform the Pre Inventory of the files specified in the JobOrder
  class ICECONCPreInventory
    # Class initialization
    def initialize(options)
      parse_joborder(options[:job_order])

      @conf = ICECONCPreInventoryConf.new(options[:conf])
    end

    # Execute the required job
    def execute_job
      raise IOError, "File '#{@mtd_path}' already exist" if File.exist?(@mtd_path)
      raise IOError, "File '#{@list_path}' already exist" if File.exist?(@list_path)

      wrapper = ICECONCWrapper.new(@file_path, @file_type, @conf)
      wrapper.generate_mtd(@mtd_path)

      File.write(@list_path, "#{@file_name}\n")
    end

    private

    # Parse JobOrder and extract file name and type
    def parse_joborder(job_order)
      raise IOError, "JobOrder '#{job_order}' does not exist" unless File.exist?(job_order)

      job = JobOrder::JobOrderHandler.new(job_order)

      @file_path = job.file_name(1, 1, 1)
      @file_type = job.files_type(1, 1)

      @file_name = File.basename(@file_path)

      @mtd_path = "#{@file_path}.MTD"
      @list_path = File.join(File.dirname(@file_path), "#{ICECONCPreInventoryComponents::PID}.LIST")
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

    options[:conf] = File.absolute_path(ICECONCPreInventoryComponents::DEFAULT_CONF_FILE)
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
    ICECONCPreInventoryComponents.log('I', "Starting #{ICECONCPreInventoryComponents::SCRIPT_NAME} PreInventory")

    # Create the Pre Inventory object and execute the job
    pre_inventory = ICECONCPreInventoryComponents::ICECONCPreInventory.new(options)
    pre_inventory.execute_job

    ICECONCPreInventoryComponents.log('I', "Completed #{ICECONCPreInventoryComponents::SCRIPT_NAME} PreInventory")
  rescue StandardError => e
    # Print the error
    ICECONCPreInventoryComponents.log('E', e.message)
    puts e.backtrace

    # Exit with failure status
    exit(150)
  end

  # Exit with status derived from job execution result
  exit(true)
end
# :nocov:
