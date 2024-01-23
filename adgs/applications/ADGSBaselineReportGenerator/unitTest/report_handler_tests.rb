#!/usr/bin/env ruby
# frozen_string_literal: true

# Copyright 2023-2023, Exprivia SpA.
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
# $Prod: ADGSBaselineReportGenerator Tests $

# :nocov:
# Add current dir to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'common_tests_utils'
# Set the simplecov suit name
SimpleCov.command_name File.absolute_path(__FILE__) if defined?(SimpleCov)

# Test for ReportHandler
class ReportHandlerTest < Test::Unit::TestCase
  include ReportOperations

  CONF_PATH = File.join(__dir__, 'conf', 'valid_complete.conf')

  SAT_ACR = 'TS1'
  SAT_NAME = 'Test1'

  F1_NAME = 'TS1_TYP_E01_FILE_NAME'
  F1_TYPE = 'TYP_E01'
  F1_START = DateTime.new(2021, 6, 2)
  F1_START_S = F1_START.strftime('%Y-%m-%dT%H:%M:%S')
  F1_STOP = DateTime.new(2021, 6, 12)
  F1_STOP_S = F1_STOP.strftime('%Y-%m-%dT%H:%M:%S')
  F1_CREATION = DateTime.new(2023, 5, 11)
  F1_CREATION_S = F1_CREATION.strftime('%Y-%m-%dT%H:%M:%S')
  F1 = { name: F1_NAME, type: F1_TYPE, start: F1_START_S, stop: F1_STOP_S, creation: F1_CREATION_S }.freeze

  F2_NAME = 'TS1_TYP_E02_FILE_NAME'
  F2_TYPE = 'TYP_E02'
  F2_START = DateTime.new(2022, 6, 2)
  F2_START_S = F2_START.strftime('%Y-%m-%dT%H:%M:%S')
  F2_STOP = DateTime.new(2022, 6, 12)
  F2_STOP_S = F2_STOP.strftime('%Y-%m-%dT%H:%M:%S')
  F2_CREATION = DateTime.new(2022, 5, 11)
  F2_CREATION_S = F2_CREATION.strftime('%Y-%m-%dT%H:%M:%S')
  F2 = { name: F2_NAME, type: F2_TYPE, start: F2_START_S, stop: F2_STOP_S, creation: F2_CREATION_S }.freeze

  # Test initialization
  def setup
    @conf = ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(CONF_PATH)

    @log = RotatingLogger.new(@conf.logger_file, level: @conf.logger_level,
                                                 rotation_period: @conf.logger_rotation_period,
                                                 max_file_num: @conf.logger_files_to_keep,
                                                 text_file_num: @conf.logger_text_files)
    clean_test_folders
  end

  # Test finalization
  def teardown
    clean_test_folders
  end

  # Create a valid report with one entry
  def test_report_generation_one_file
    report = ADGSBaselineReportGenerator::ReportHandler.new(@conf.output, SAT_ACR, SAT_NAME, @log)
    report.add_entry(F1_NAME, F1_TYPE, F1_START, F1_STOP, F1_CREATION)
    report.close

    report_file = find_report(@conf.output, SAT_ACR)
    assert_equal(1, report_file.size)
    assert_equal(create_reference(SAT_NAME, creation_date(report), [F1]), File.read(report_file[0]))
  end

  # Create a valid report with two entry
  def test_report_generation_two_files
    report = ADGSBaselineReportGenerator::ReportHandler.new(@conf.output, SAT_ACR, SAT_NAME, @log)
    report.add_entry(F1_NAME, F1_TYPE, F1_START, F1_STOP, F1_CREATION)
    report.add_entry(F2_NAME, F2_TYPE, F2_START, F2_STOP, F2_CREATION)
    report.close

    report_file = find_report(@conf.output, SAT_ACR)
    assert_equal(1, report_file.size)
    assert_equal(create_reference(SAT_NAME, creation_date(report), [F1, F2]), File.read(report_file[0]))
  end

  # Create a valid report with no entry
  def test_report_generation_no_files
    report = ADGSBaselineReportGenerator::ReportHandler.new(@conf.output, SAT_ACR, SAT_NAME, @log)
    report.close

    report_file = find_report(@conf.output, SAT_ACR)
    assert_equal(1, report_file.size)
    assert_equal(create_empty_reference(SAT_NAME, creation_date(report)), File.read(report_file[0]))
  end

  # Check already existing error
  def test_already_existing_report
    # This test may fail due to second changed just in the middle of the two initialization, so it is try 20 times
    # and must be verified at least 19
    assert_block { (1..20).count { double_run } >= 19 }
  end

  private

  # Extract the report creation date from the ReportHandler object
  def creation_date(report)
    report.instance_variable_get(:@report_creation_date).strftime('%Y-%m-%dT%H:%M:%S')
  end

  # Run two times ReportHandler (for test_already_existing_report)
  def double_run
    # One of the two initialization must raise IOError:
    # - the second one will raise IOError at first run and at every second change
    # - the first one will raise IOError at all the other runs
    ADGSBaselineReportGenerator::ReportHandler.new(@conf.output, SAT_ACR, SAT_NAME, @log)
    ADGSBaselineReportGenerator::ReportHandler.new(@conf.output, SAT_ACR, SAT_NAME, @log)
    false
  rescue IOError
    true
  end
end
