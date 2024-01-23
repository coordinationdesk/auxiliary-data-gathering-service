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

# Constants for ReportGeneratorTest
module ReportGeneratorTestConstants
  include DBOperations
  include ReportOperations

  # Paths inside configuration files must not change due to initialization
  SINGLE_SAT_SINGLE_FILE_CONF_PATH = File.join(__dir__, 'conf', 'single_sat_single_file.conf')
  SINGLE_SAT_MULTI_FILE_CONF_PATH = File.join(__dir__, 'conf', 'single_sat_multiple_file.conf')
  MULTI_SAT_MULTI_FILE_CONF_PATH = File.join(__dir__, 'conf', 'multiple_sat_multiple_file.conf')
  NO_EMPTY_ON_MISSING_CONF_PATH = File.join(__dir__, 'conf', 'no_empty_on_missing.conf')
  INVALID_FILE_TYPE_CONF_PATH = File.join(__dir__, 'conf', 'invalid_file_type.conf')

  SAT1_NAME = 'Test1'
  SAT1_ACR = 'TS1'
  SAT1 = { name: SAT1_NAME, acr: SAT1_ACR }.freeze

  SAT2_NAME = 'Test2'
  SAT2_ACR = 'TS2'
  SAT2 = { name: SAT2_NAME, acr: SAT2_ACR }.freeze

  FILE_TYPE = 'TestFType'

  F1_NAME = 'TS1_TYP_N01_FILE_NAME'
  F1_FILE_TYPE = 'TYP_N01'
  F1_TYPE = 'TYP_E01'
  F1_START_S = DateTime.new(2021, 6, 2, 1, 2, 3).strftime('%Y-%m-%dT%H:%M:%S')
  F1_STOP_S = DateTime.new(2021, 6, 12, 4, 5, 6).strftime('%Y-%m-%dT%H:%M:%S')
  F1_CREATION_S = DateTime.new(2021, 5, 11, 7, 8, 9).strftime('%Y-%m-%dT%H:%M:%S')
  F1 = { name: F1_NAME, type: F1_TYPE, start: F1_START_S, stop: F1_STOP_S, creation: F1_CREATION_S,
         attr: "[{\"Name\": \"productType\",\"Value\": \"#{F1_TYPE}\", \"ValueType\": \"String\"}]" }.freeze

  F2_1_NAME = 'TS1_TYP_N02_FILE_NAME'
  F2_2_NAME = 'TS2_TYP_N02_FILE_NAME'
  F2_TYPE = 'TYP_E02'
  F2_START_S = DateTime.new(2022, 6, 2, 1, 2, 3).strftime('%Y-%m-%dT%H:%M:%S')
  F2_STOP_S = DateTime.new(2022, 6, 12, 4, 5, 6).strftime('%Y-%m-%dT%H:%M:%S')
  F2_CREATION_S = DateTime.new(2022, 5, 11, 7, 8, 9).strftime('%Y-%m-%dT%H:%M:%S')
  F2_1 = { name: F2_1_NAME, type: F2_TYPE, start: F2_START_S, stop: F2_STOP_S, creation: F2_CREATION_S,
           attr: "[{\"Name\": \"productType\",\"Value\": \"#{F2_TYPE}\", \"ValueType\": \"String\"}]" }.freeze
  F2_2 = { name: F2_2_NAME, type: F2_TYPE, start: F2_START_S, stop: F2_STOP_S, creation: F2_CREATION_S,
           attr: "[{\"Name\": \"productType\",\"Value\": \"#{F2_TYPE}\", \"ValueType\": \"String\"}]" }.freeze

  F3_NAME = 'TS1_TYP_N03_FILE_NAME'
  F3_TYPE = 'TYP_E03'
  F3_START_S = DateTime.new(2023, 6, 2, 1, 2, 3).strftime('%Y-%m-%dT%H:%M:%S')
  F3_STOP_S = DateTime.new(2023, 6, 12, 4, 5, 6).strftime('%Y-%m-%dT%H:%M:%S')
  F3_CREATION_S = DateTime.new(2020, 5, 11, 7, 8, 9).strftime('%Y-%m-%dT%H:%M:%S')
  F3 = { name: F3_NAME, type: F3_TYPE, start: F3_START_S, stop: F3_STOP_S, creation: F3_CREATION_S,
         attr: "[{\"Name\": \"productType\",\"Value\": \"#{F3_TYPE}\", \"ValueType\": \"String\"}]" }.freeze

  F_NO_TYPE = { name: F1_NAME, type: '', start: F1_START_S, stop: F1_STOP_S, creation: F1_CREATION_S,
                attr: '[]' }.freeze
  F_EMPTY = { name: '', type: F1_FILE_TYPE, start: '', stop: '', creation: '' }.freeze
end

# Test for ReportHandler
class ReportGeneratorTest < Test::Unit::TestCase
  include ReportGeneratorTestConstants

  # Test initialization
  def setup
    # Paths must not change between configuration files
    @conf = ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(SINGLE_SAT_SINGLE_FILE_CONF_PATH)

    @log = RotatingLogger.new(@conf.logger_file, level: @conf.logger_level,
                                                 rotation_period: @conf.logger_rotation_period,
                                                 max_file_num: @conf.logger_files_to_keep,
                                                 text_file_num: @conf.logger_text_files)

    clean_test_folders
  end

  # Test finalization
  def teardown
    delete_records_to_remove
    clean_test_folders
  end

  # Create a valid report with one satellite and one file type
  def test_report_generation_one_sat_one_file
    type_id = insert_file_type(FILE_TYPE)
    reference = create_case(SAT1, [F1], type_id: type_id)

    report_generator = ADGSBaselineReportGenerator.new(SINGLE_SAT_SINGLE_FILE_CONF_PATH)
    assert_equal(true, report_generator.execute_job)

    check_file(SAT1_ACR, reference)
  end

  # Create a valid report with one satellite and two file type
  def test_report_generation_one_sat_two_files
    type_id = insert_file_type(FILE_TYPE)
    reference = create_case(SAT1, [F1, F2_1], type_id: type_id)

    report_generator = ADGSBaselineReportGenerator.new(SINGLE_SAT_MULTI_FILE_CONF_PATH)
    assert_equal(true, report_generator.execute_job)

    check_file(SAT1_ACR, reference)
  end

  # Create a valid report with two satellite and two file type each
  def test_report_generation_two_sat_two_files
    type_id = insert_file_type(FILE_TYPE)
    reference1 = create_case(SAT1, [F1, F2_1], type_id: type_id)
    reference2 = create_case(SAT2, [F2_2, F3], type_id: type_id)

    report_generator = ADGSBaselineReportGenerator.new(MULTI_SAT_MULTI_FILE_CONF_PATH)
    assert_equal(true, report_generator.execute_job)

    check_file(SAT1_ACR, reference1)
    check_file(SAT2_ACR, reference2)
  end

  # Create a valid report without the file type
  def test_report_generation_empty_file_type
    type_id = insert_file_type(FILE_TYPE)
    reference = create_case(SAT1, [F_NO_TYPE], type_id: type_id)

    report_generator = ADGSBaselineReportGenerator.new(SINGLE_SAT_SINGLE_FILE_CONF_PATH)
    assert_equal(true, report_generator.execute_job)

    check_file(SAT1_ACR, reference)
  end

  # Create a valid report with one satellite and one empty file type (no data on db)
  def test_report_generation_empty_file
    reference = create_case(SAT1, [F_EMPTY], insert_files: false)

    report_generator = ADGSBaselineReportGenerator.new(SINGLE_SAT_SINGLE_FILE_CONF_PATH)
    assert_equal(true, report_generator.execute_job)

    check_file(SAT1_ACR, reference)
  end

  # Create a empty report with one satellite and no file type
  def test_report_generation_no_entry
    reference = create_case(SAT1, [], insert_files: false)

    report_generator = ADGSBaselineReportGenerator.new(NO_EMPTY_ON_MISSING_CONF_PATH)
    assert_equal(true, report_generator.execute_job)

    check_file(SAT1_ACR, reference)
  end

  # Missing satellite error case
  def test_missing_satellite
    report_generator = ADGSBaselineReportGenerator.new(SINGLE_SAT_SINGLE_FILE_CONF_PATH)
    assert_equal(false, report_generator.execute_job)

    report_file = find_report(@conf.output, SAT1_ACR)
    assert_equal(0, report_file.size)
  end

  # Create a valid report without the file type
  def test_invalid_file_type
    type_id = insert_file_type(FILE_TYPE)
    reference = create_case(SAT1, [F1], type_id: type_id)

    report_generator = ADGSBaselineReportGenerator.new(INVALID_FILE_TYPE_CONF_PATH)
    assert_equal(false, report_generator.execute_job)

    check_file(SAT1_ACR, reference)
  end

  private

  # Create the DB records and the relative reference report
  def create_case(sat, fls, type_id: 0, insert_files: true)
    sat_id = insert_sat(sat[:name], sat[:acr])
    if insert_files
      fls.each do |file|
        insert_inventory(file[:name], file[:creation], file[:start], file[:stop], type_id, sat_id, file[:attr])
      end
    end

    # Create the reference report without the 'CreationDate' field (cant be reconstructed in this type of test)
    remove_creaton_date(fls.empty? ? create_empty_reference(sat[:name], '-') : create_reference(sat[:name], '-', fls))
  end

  # Remove the 'CreationDate' field from a report
  def remove_creaton_date(report)
    report.gsub(%r{(<CreationDate>).*(</CreationDate>)}, '\1\2')
  end

  # Compare the expected result with the file on output path
  def check_file(sat, reference)
    report_file = find_report(@conf.output, sat)
    assert_equal(1, report_file.size)
    assert_equal(reference, remove_creaton_date(File.read(report_file[0])))
  end
end
