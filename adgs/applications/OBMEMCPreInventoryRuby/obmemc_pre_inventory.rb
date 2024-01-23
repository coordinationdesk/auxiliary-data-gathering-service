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
# $Prod: OBMEMCPreInventory $

# :nocov:
# Add the RubyLibs dir to load path if not already included
libs_path = File.expand_path(File.join(__dir__, '..', 'lib'))
$LOAD_PATH.unshift(libs_path) unless $LOAD_PATH.include?(libs_path)
# :nocov:

require 'optparse'
require 'syslog'
require 'socket'
require 'date'

require 'nokogiri'

require 'common_utils'
require 'job_order_handler'
require 'configuration_handler'
require 'mtd_handler'

# Collector for all OBMEMCPreInventory components
module OBMEMCPreInventoryComponents
  DEFAULT_CONF_FILE = File.join('..', 'conf', 'obmemc_pre_inventory.conf')

  HOST_NAME = Socket.gethostname
  SCRIPT_NAME = File.basename(__FILE__)
  PID = Process.pid.to_s

  def self.log(lvl, msg)
    puts "#{Time.now.strftime('%FT%T.%6N')} #{HOST_NAME} #{SCRIPT_NAME} 02.00 [#{PID}]: [#{lvl}] #{msg}"
  end

  # Handle the OBMEMC Pre Inventory configuration
  class OBMEMCPreInventoryConf < ConfigurationHandler
    SOURCE_TAG = 'Source'
    FILE_CLASS_TAG = 'FileClass'
    SATELLITE_ID_TAG = 'SatelliteID'
    MISSION_TAG = 'Mission'

    attr_reader :source, :file_class, :satellite_id, :mission

    # Class initialization
    def initialize(conf_file)
      # Load the configuration
      super(conf_file)

      @source = get(SOURCE_TAG)
      @file_class = get(FILE_CLASS_TAG)
      @satellite_id = get(SATELLITE_ID_TAG)
      @mission = get(MISSION_TAG)
    end
  end

  # Read all values of the GIPP file and generate the MTD
  class OBMEMCWrapper
    include MTDHandler::Wrapper

    CREATION_TIME_PATH = '//OnBoardMemoryConfigurationUpdate/Creation_Time'
    VALIDITY_START_PATH = '//OnBoardMemoryConfigurationUpdate/Validity_Start'
    VALIDITY_STOP_PATH = '//OnBoardMemoryConfigurationUpdate/Validity_Stop'

    # Extract all the needed value from the metadata
    def initialize(file_path, file_type, conf)
      check_type(conf, 'conf', [OBMEMCPreInventoryConf])

      empty_defaults_values
      fixed_defaults_values

      values_from_conf(conf)

      parse_file(file_path)

      @file_type = file_type
    end

    private

    # Set all non used values of the MTD to the default empty value
    def empty_defaults_values
      @source_sw_version = @start_orbit_number = @stop_orbit_number = @quality_info = @sensor_id = ''
      @acquisition_station_id = @processing_station_id = @sensor_mode = @phase = @ascending_flag = ''
      @equator_x_longitude = @equator_x_time = @order_id = ''
    end

    # Set all non used values of the MTD to the default fixed value
    def fixed_defaults_values
      @file_version = '1.0'
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
      raise IOError, "Cannot find '#{file_path}' file" unless File.exist?(file_path)

      @file_name = File.basename(file_path)
      @data_size = File.size(file_path)

      doc = Nokogiri::XML(File.read(file_path))
      @generation_time = @validation_date = extract_date_time(doc, CREATION_TIME_PATH)
      @validity_start = extract_date_time(doc, VALIDITY_START_PATH)
      @validity_stop = extract_date_time(doc, VALIDITY_STOP_PATH)
    end

    # Fix incorrect date and time and transform it from string to DateTime object
    def extract_date_time(doc, xpath)
      elem = doc.at_xpath(xpath)
      raise "Cannot find '#{xpath}' tag inside '#{@file_name}' file" if elem.nil?

      str = elem.text.strip
      return DateTime.new(2000, 1, 1, 0, 0, 0) if str =~ /0000-00-00T00:00:00/
      return DateTime.new(2100, 1, 1, 0, 0, 0) if str =~ /9999-99-99T99:99:99/

      DateTime.parse(str)
    end
  end

  # Perform the Pre Inventory of the files specified in the JobOrder
  class OBMEMCPreInventory
    # Class initialization
    def initialize(options)
      parse_joborder(options[:job_order])

      @conf = OBMEMCPreInventoryConf.new(options[:conf])
    end

    # Execute the required job
    def execute_job
      raise IOError, "File '#{@mtd_path}' already exist" if File.exist?(@mtd_path)
      raise IOError, "File '#{@list_path}' already exist" if File.exist?(@list_path)

      wrapper = OBMEMCWrapper.new(@file_path, @file_type, @conf)
      wrapper.generate_mtd(@mtd_path)

      File.write(@list_path, "#{@file_name}\n")
    end

    private

    # Parse JobOrder and extract file name and type
    def parse_joborder(job_order)
      job = JobOrder::JobOrderHandler.new(job_order)

      @file_path = job.file_name(1, 1, 1)
      @file_type = job.files_type(1, 1)

      @file_name = File.basename(@file_path)

      @mtd_path = "#{@file_path}.MTD"
      @list_path = File.join(File.dirname(@file_path), "#{OBMEMCPreInventoryComponents::PID}.LIST")
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

    options[:conf] = File.absolute_path(OBMEMCPreInventoryComponents::DEFAULT_CONF_FILE)
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
    OBMEMCPreInventoryComponents.log('I', "Starting #{OBMEMCPreInventoryComponents::SCRIPT_NAME} PreInventory")

    # Create the Pre Inventory object and execute the job
    pre_inventory = OBMEMCPreInventoryComponents::OBMEMCPreInventory.new(options)
    pre_inventory.execute_job

    OBMEMCPreInventoryComponents.log('I', "Completed #{OBMEMCPreInventoryComponents::SCRIPT_NAME} PreInventory")
  rescue StandardError => e
    # Print the error
    OBMEMCPreInventoryComponents.log('E', e.message)
    puts e.backtrace

    # Exit with failure status
    exit(150)
  end

  # Exit with status derived from job execution result
  exit(true)
end
# :nocov:
