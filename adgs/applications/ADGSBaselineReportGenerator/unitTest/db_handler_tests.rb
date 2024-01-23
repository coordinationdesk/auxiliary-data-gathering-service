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

# Test for ReportGeneratorDB
class ReportGeneratorDBTest < Test::Unit::TestCase
  include DBOperations

  CONF_PATH = File.join(__dir__, 'conf', 'valid_complete.conf')

  SAT_NAME = 'TestSat'
  SAT_ACR = 'TST'

  FILE_TYPE = 'TestFType'

  NAME = 'TST_TEST_FILE_TST_TYP_0123456'
  NAME_LIKE = 'TST_TYP'
  YESTERDAY = DateTime.now.prev_day.freeze
  CREATION_DATE = DateTime.new(YESTERDAY.year, YESTERDAY.month, YESTERDAY.day, 22, 59, 59.999).freeze
  CREATION_DATE_S = CREATION_DATE.strftime('%FT%T.%L').freeze
  VAL_START = DateTime.new(YESTERDAY.year, YESTERDAY.month, YESTERDAY.day, 20, 59, 59).freeze
  VAL_START_S = VAL_START.strftime('%FT%T.%L').freeze
  VAL_STOP = DateTime.new(YESTERDAY.year, YESTERDAY.month, YESTERDAY.day, 21, 59, 59).freeze
  VAL_STOP_S = VAL_STOP.strftime('%FT%T.%L').freeze
  TYPE = 'TestType'
  ATTR = "[{\"Name\": \"SomeName\"},{\"Name\": \"productType\",\"Value\": \"#{TYPE}\", \"ValueType\": \"String\"}]"
  ATTR_NO_PROD_TYPE = "[{\"Name\": \"SomeName\"},{\"Name\": \"product\",\"Value\": \"#{TYPE}\"}]"

  WRONG_NAME = 'TST_TEST_FILE_TST_TYP_987654'
  WRONG_CREATION_DATE_S = DateTime.now.strftime('%FT%T.%L').freeze
  OLD_NAME = 'TST_TEST_FILE_TST_TYP_000000'
  OLD_CREATION_DATE_S = DateTime.new(2021, 6, 12).strftime('%FT%T.%L').freeze

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
    delete_records_to_remove
    clean_test_folders
  end

  # Test initialize
  def test_initialization
    assert_raise(TypeError) { ADGSBaselineReportGenerator::ReportGeneratorDB.new(nil, @log) }
    assert_raise(TypeError) { ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, nil) }
    assert_nothing_raised { ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, @log) }
  end

  # Test the get_satellite_data method
  def test_get_satellite_data
    db = ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, @log)

    assert_raise(RuntimeError) { db.get_satellite_data(SAT_ACR) }

    exp_id = insert_sat(SAT_NAME, SAT_ACR)

    id, acr, name = db.get_satellite_data(SAT_ACR)
    assert_equal(exp_id, id)
    assert_equal(SAT_ACR, acr)
    assert_equal(SAT_NAME, name)
  end

  # Test the get_file_data method
  def test_get_file_data
    db = ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, @log)

    sat_id = insert_sat(SAT_NAME, SAT_ACR)
    type_id = insert_file_type(FILE_TYPE)
    assert_equal(0, db.get_file_data(sat_id, NAME_LIKE))

    insert_inventory(NAME, CREATION_DATE_S, VAL_START_S, VAL_STOP, type_id, sat_id, ATTR)
    insert_inventory(OLD_NAME, OLD_CREATION_DATE_S, VAL_START_S, VAL_STOP, type_id, sat_id, ATTR)
    insert_inventory(WRONG_NAME, WRONG_CREATION_DATE_S, VAL_START_S, VAL_STOP, type_id, sat_id, ATTR)

    assert_equal(1, db.get_file_data(sat_id, NAME_LIKE, &method(:check_inventory)))
  end

  # Test the get_file_data method with nullo attributes
  def test_get_file_data_null_attribute
    db = ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, @log)

    sat_id = insert_sat(SAT_NAME, SAT_ACR)
    type_id = insert_file_type(FILE_TYPE)
    assert_equal(0, db.get_file_data(sat_id, NAME_LIKE))

    insert_inventory(NAME, CREATION_DATE_S, VAL_START_S, VAL_STOP, type_id, sat_id, nil)

    num_rows = db.get_file_data(sat_id, NAME_LIKE) do |_name, type, _validity_start, _validity_stop, _creation_date|
      assert_equal(nil, type)
    end

    assert_equal(1, num_rows)
  end

  # Test the get_file_data method with empty attributes
  def test_get_file_data_empty_attribute
    db = ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, @log)

    sat_id = insert_sat(SAT_NAME, SAT_ACR)
    type_id = insert_file_type(FILE_TYPE)
    assert_equal(0, db.get_file_data(sat_id, NAME_LIKE))

    insert_inventory(NAME, CREATION_DATE_S, VAL_START_S, VAL_STOP, type_id, sat_id, '[]')

    num_rows = db.get_file_data(sat_id, NAME_LIKE) do |_name, type, _validity_start, _validity_stop, _creation_date|
      assert_equal(nil, type)
    end

    assert_equal(1, num_rows)
  end

  # Test the get_file_data method with invalid attributes
  def test_get_file_data_invalid_attribute
    db = ADGSBaselineReportGenerator::ReportGeneratorDB.new(@conf, @log)

    sat_id = insert_sat(SAT_NAME, SAT_ACR)
    type_id = insert_file_type(FILE_TYPE)
    assert_equal(0, db.get_file_data(sat_id, NAME_LIKE))

    insert_inventory(NAME, CREATION_DATE_S, VAL_START_S, VAL_STOP, type_id, sat_id, ATTR_NO_PROD_TYPE)

    num_rows = db.get_file_data(sat_id, NAME_LIKE) do |_name, type, _validity_start, _validity_stop, _creation_date|
      assert_equal(nil, type)
    end

    assert_equal(1, num_rows)
  end

  private

  # Check if the data read is correct
  def check_inventory(name, type, validity_start, validity_stop, creation_date)
    assert_equal(NAME, name)
    assert_equal(TYPE, type)
    assert_equal(VAL_START, validity_start)
    assert_equal(VAL_STOP, validity_stop)
    assert_equal(CREATION_DATE, creation_date)
  end
end
