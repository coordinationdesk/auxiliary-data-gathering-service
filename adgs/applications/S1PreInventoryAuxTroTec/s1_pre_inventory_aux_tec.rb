#!/usr/bin/env ruby
# frozen_string_literal: true

# Copyright 2021-2021, Exprivia SpA.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com

# All Rights Reserved.

# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Exprivia SpA.

# $Prod: S1PreInventoryAuxTroTec $

# :nocov:
# Add the RubyLibs dir to load path if not already included
libs_path = File.expand_path(File.join(__dir__, '..', 'lib'))
$LOAD_PATH.unshift(libs_path) unless $LOAD_PATH.include?(libs_path)
# Add the other S2PreInventoryAuxTEC module parts dir to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'open3'

require 'common_utils'
require 'job_order_handler'

require 's1_pre_inventory_input_file_handler'

# Collector for all S2PreInventoryAux components
module S2PreInventoryAuxComponents
  # Read all values of the AUX_TEC file and generate the MTD
  class AuxTECWrapper
    include GenericInputWrapper

    # Case 1 regex: e.g. codg2220.19i.Z
    REGEXP_NAME_1 = /\w{4}(\d{3})\w\.(\d{2})\w\.Z$/.freeze
    # Case 1 regex: e.g. COD0OPSFIN_20223310000_01D_01H_GIM.INX
    REGEXP_NAME_2 = /\w{10}_(\d{4})(\d{3})\w{4}_\w{3}_\w{3}_\w{3}\.INX\.gz$/.freeze
    REGEXP_VALIDITY_START = /(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+EPOCH OF FIRST MAP/.freeze
    REGEXP_VALIDITY_STOP = /(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+EPOCH OF LAST MAP/.freeze

    # Class initialization
    def initialize(file_path, file_type)
      raise IOError, "File '#{file_path}' does not exists" unless File.exist?(file_path)

      initialize_variables(File.new(file_path), file_type)

      @source = 'AGS'

      @generation_time = compute_generation_time(file_path)
      extract_validity_times(file_path)
    end

    private

    # Check the file format and extract the generation time from the file name
    def compute_generation_time(file_path)
      match = REGEXP_NAME_1.match(file_path)

      return DateTime.strptime("#{match[2]}#{match[1]}", '%y%j') unless match.nil?

      match = REGEXP_NAME_2.match(file_path)

      raise ArgumentError, "File '#{file_path}' have a wrong name format" if match.nil?

      DateTime.strptime("#{match[1]}#{match[2]}", '%Y%j')
    end

    # Extract the validity times from within the file
    def extract_validity_times(file_path)
      cmd = "7z e -so '#{file_path}' | grep -E 'EPOCH OF (FIRST|LAST) MAP'"
      data, err, status = Open3.capture3(cmd)

      # Check the result
      raise "Command '#{cmd}' fail with status #{status} and error: '#{err}'" unless err.empty? && status.success?
      raise IOError, "Cannot read metadata in the file '#{file_path}'" if data.empty?

      parse_validity_start(file_path, data)
      parse_validity_stop(file_path, data)
    end

    # Parse the extracted validity times
    def parse_validity_start(file_path, data)
      date = REGEXP_VALIDITY_START.match(data)
      raise "Cannot find 'EPOCH OF FIRST MAP' field inside the file '#{file_path}'" if date.nil?

      @validity_start = DateTime.new(date[1].to_i, date[2].to_i, date[3].to_i, date[4].to_i, date[5].to_i, date[6].to_i)
    end

    # Parse the extracted validity times
    def parse_validity_stop(file_path, data)
      date = REGEXP_VALIDITY_STOP.match(data)
      raise "Cannot find 'EPOCH OF LAST MAP' field inside the file '#{file_path}'" if date.nil?

      @validity_stop = DateTime.new(date[1].to_i, date[2].to_i, date[3].to_i, date[4].to_i, date[5].to_i, date[6].to_i)
    end
  end

  # Perform the Pre Inventory of the files specified in the JobOrder
  class S2PreInventoryAuxTEC
    # Class initialization
    def initialize(job_order)
      job = JobOrder::JobOrderHandler.new(job_order, mission: :S1)

      @file_path = job.file_name(1, 1, 1)
      @file_type = job.files_type(1, 1)
      @mtd_path = "#{@file_path}.MTD"
      @list_path = "#{@file_path}.LIST"
    end

    # Execute the required job
    def execute_job
      puts "Start handling file '#{@file_path}'"

      do_job

      puts "Job done on file '#{@file_path}'\n"

      true
    rescue StandardError => e
      print_error(e)

      false
    end

    private

    # Perform all the needed operations
    def do_job
      raise IOError, "File '#{@mtd_path}' already exists" if File.exist?(@mtd_path)
      raise IOError, "File '#{@list_path}' already exists" if File.exist?(@list_path)

      wrapper = AuxTECWrapper.new(@file_path, @file_type)
      wrapper.generate_mtd(@mtd_path)
      File.write(@list_path, File.basename(@file_path))
    end
  end
end

# :nocov:
# If the executed tile is this file
if $PROGRAM_NAME == __FILE__
  valid = false
  begin
    raise ArgumentError, "Job Order not provided.\nusage: #{__FILE__} <JobOrder>" if ARGV.empty?

    # Get the JobOrder
    job_order = File.absolute_path(ARGV.shift)

    # Create the Pre Inventory object and execute the job
    tec_pre_inventory = S2PreInventoryAuxComponents::S2PreInventoryAuxTEC.new(job_order)
    valid = tec_pre_inventory.execute_job
  rescue StandardError => e
    # Print the error
    print_error(e)

    # Exit with failure status
    exit(false)
  end

  # Exit with status derived from job execution result
  exit(valid)
end
# :nocov:
