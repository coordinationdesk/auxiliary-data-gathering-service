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

require 'fileutils'

# Test for ReportGeneratorConfiguration
class ReportGeneratorConfigurationTest < Test::Unit::TestCase
  VALID_COMPLETE_PATH = File.join(__dir__, 'conf', 'valid_complete.conf')
  VALID_NO_OPTIONAL_PATH = File.join(__dir__, 'conf', 'valid_no_optional.conf')
  VALID_EMPTY_FILE_TYPE_PATH = File.join(__dir__, 'conf', 'valid_empty_file_type.conf')
  INVALID_MISSING_OUTPUT_PATH = File.join(__dir__, 'conf', 'invalid_missing_output.conf')
  INVALID_NOT_EXISTING_OUTPUT_PATH = File.join(__dir__, 'conf', 'invalid_not_existing_output.conf')
  INVALID_MISSING_SATELLITE_PATH = File.join(__dir__, 'conf', 'invalid_missing_satellite.conf')

  # Test valid configuration file with all fields set
  def test_valid_conf
    conf = ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(VALID_COMPLETE_PATH)
    assert_equal(File.expand_path("#{__dir__}/../unitTest/outputs"), conf.output)
    assert_equal(true, conf.add_empty_entries)
    assert_equal(2, conf.reports.size)
    check_report({ satellite: 'TS1', file_types: %w[TST_001 TST_002 TST_003] }, conf.reports[0])
    check_report({ satellite: 'TS2', file_types: %w[TST_003 TST_004] }, conf.reports[1])
  end

  # Test valid configuration file with only mandatory fields set
  def test_no_optionals_conf
    conf = ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(VALID_NO_OPTIONAL_PATH)
    assert_equal(File.expand_path("#{__dir__}/../unitTest/outputs"), conf.output)
    assert_equal(false, conf.add_empty_entries)
    assert_equal(0, conf.reports.size)
  end

  # Test valid configuration file with all fields set except for a FileType Tag leave empty
  def test_empty_file_type_conf
    conf = ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(VALID_EMPTY_FILE_TYPE_PATH)
    assert_equal(2, conf.reports.size)
    assert_equal('TS1', conf.reports[0][:satellite])
    assert_equal(true, conf.reports[0][:file_types].empty?)
    check_report({ satellite: 'TS2', file_types: %w[TST_003 TST_004] }, conf.reports[1])
  end

  # Test invalid configuration files
  def test_invalid_configurations
    assert_raise(RuntimeError) do
      ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(INVALID_MISSING_OUTPUT_PATH)
    end
    assert_raise(IOError) do
      ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(INVALID_NOT_EXISTING_OUTPUT_PATH)
    end
    assert_raise(ArgumentError) do
      ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(INVALID_MISSING_SATELLITE_PATH)
    end
  end

  # Test invalid write permission on output path
  def test_invalid_output_write_permission
    FileUtils.chmod('ug-w', File.expand_path("#{__dir__}/../unitTest/outputs"))
    assert_raise(IOError) do
      ADGSBaselineReportGenerator::ReportGeneratorConfiguration.new(VALID_COMPLETE_PATH)
    end
  ensure
    FileUtils.chmod('ug+w', File.expand_path("#{__dir__}/../unitTest/outputs"))
  end

  private

  def check_report(expected, report)
    assert_equal(expected[:satellite], report[:satellite])
    expected[:file_types].each_with_index do |file_type, index|
      assert_equal(file_type, report[:file_types][index])
    end
  end
end
