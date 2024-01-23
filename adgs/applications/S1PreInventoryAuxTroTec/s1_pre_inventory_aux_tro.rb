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
# Add the other S2PreInventoryAuxTRO module parts dir to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'common_utils'
require 'job_order_handler'

require 's1_pre_inventory_input_file_handler'

# Collector for all S2PreInventoryAux components
module S2PreInventoryAuxComponents
  # Read all values of the AUX_TRO file and generate the MTD
  class AuxTROWrapper
    include GenericInputWrapper

    REGEXP_NAME = /ECMWF_(\w{4})_ML00_(\d\d)H_.{3}_GP_.{4}_(\d{8}T\d{6})/.freeze

    # Class initialization
    def initialize(file_path, file_type)
      match = REGEXP_NAME.match(file_path)
      raise ArgumentError, "File '#{file_path}' have a wrong name format" if match.nil?
      raise IOError, "File '#{file_path}' does not exists" unless File.exist?(file_path)

      file = File.new(file_path)

      initialize_variables(file, file_type)

      @source = 'ECMWF'
      @generation_time = @equator_x_time

      @validity_start = DateTime.strptime(match[3], '%Y%m%dT%H%M%S')
      @validity_stop = @validity_start + (match[2].to_i / 24.0)

      @file_class = match[1]
    end
  end

  # Perform the Pre Inventory of the files specified in the JobOrder
  class S2PreInventoryAuxTRO
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

      wrapper = AuxTROWrapper.new(@file_path, @file_type)
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
    tro_pre_inventory = S2PreInventoryAuxComponents::S2PreInventoryAuxTRO.new(job_order)
    valid = tro_pre_inventory.execute_job
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
