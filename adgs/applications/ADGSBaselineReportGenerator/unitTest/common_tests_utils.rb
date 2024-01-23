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

# $Prod: ADGSBaselineReportGenerator Tests $

# :nocov:
# Add the main dir to load path if not already included
path_to_test = File.expand_path(File.join(__dir__, '..'))
$LOAD_PATH.unshift(path_to_test) unless $LOAD_PATH.include?(path_to_test)

# Add RubyLibs dir to load path if not already included
path_to_ruby_libs = File.expand_path(File.join(__dir__, '..', '..', 'lib'))
$LOAD_PATH.unshift(path_to_ruby_libs) unless $LOAD_PATH.include?(path_to_ruby_libs)

begin
  require 'simplecov'
rescue LoadError => e
  puts "Gem 'simplecov' not installed. Impossible to generate coverage report.\n#{e}"
end
begin
  require 'test/unit'
rescue LoadError => e
  puts "Gem 'test-unit' not installed. Impossible to run tests.\n#{e}"
  exit 1
end

require 'adgs_baseline_report_generator'

require 'pg'

require 'db_handler'
require 'common_utils'

# clean the test folder from previous run files
def clean_test_folders
  each_in_dir(File.join(__dir__, 'outputs')) { |path, file| FileUtils.rm(path) unless file == '.gitignore' }
end

# Module for create, delete and check DB rows
module DBOperations
  SAT_TAB = 't_satellites'
  SAT_ID = 'satelliteid'
  SAT_NAME = 'satellitename'
  SAT_ACR = 'satelliteacronym'

  FTY_TAB = 't_filetypes'
  FTY_ID = 'id_filetype'
  FTY_TYPE = 'filetype'
  FTY_GROUP = '"group"'

  INV_TAB = 't_srv_inventory'
  INV_ID = 'id'
  INV_NAME = '"name"'
  INV_CONT_TYPE = 'contenttype'
  INV_CONT_LEN = 'contentlength'
  INV_CREAT = 'creationdate'
  INV_CS = 'checksum'
  INV_VAL_START = 'validitystart'
  INV_VAL_STOP = 'validitystop'
  INV_TYPE = 'filetype_id'
  INV_SAT = 'satellite_id'
  INV_ATR = '"attributes"'

  private

  # Execute a query
  def exec_query(query)
    db_conf = DBHandler::DBConfiguration.new(@conf.db_conf_file, host_index: @conf.db_host_index,
                                                                 decrypter: @conf.db_pwd_decrypter)
    conn = PG.connect(host: db_conf.hostname, dbname: db_conf.schema, user: db_conf.user, password: db_conf.password)

    conn.exec(query)
  rescue StandardError => e
    print_error(e)
    raise
  ensure
    # rubocop:disable Style/SafeNavigation
    conn.close unless conn.nil?
    # rubocop:enable Style/SafeNavigation
  end

  # Add a record to the list of those to be removed and return its id
  def add_record_to_remove(tab, field, id)
    @records_to_remove = [] if @records_to_remove.nil?

    @records_to_remove.unshift({ table: tab, field: field, id: id })

    id
  end

  # Delete the records on the list of those to be removed
  def delete_records_to_remove
    (@records_to_remove || []).delete_if do |rc|
      query = %(DELETE FROM #{rc[:table]}
                WHERE #{rc[:field]} = #{rc[:id]};).gsub(/\s+/, ' ').strip

      begin
        @log.debug "Executing query: #{query}"
        exec_query(query)
      rescue StandardError
        @log.error "Error deleting record with '#{rc[:field]}' == '#{rc[:id]}' from table '#{rc[:table]}'"
      end
    end
  end

  # Inserta a satellite
  def insert_sat(name, acronym)
    query = %(
      INSERT INTO	#{SAT_TAB}
        ( #{SAT_ID}, #{SAT_NAME}, #{SAT_ACR} )
      SELECT
        MAX(st.#{SAT_ID}) + 1, '#{name}', '#{acronym}'
      FROM #{SAT_TAB} st
      RETURNING #{SAT_ID};
      ).gsub(/\s+/, ' ').strip

    @log.debug "Executing query: #{query}"
    add_record_to_remove(SAT_TAB, SAT_ID, exec_query(query)[0][SAT_ID])
  end

  # Inserta a file_type
  def insert_file_type(type, group = 'test')
    query = %(
      INSERT INTO	#{FTY_TAB}
        ( #{FTY_TYPE}, #{FTY_GROUP} )
      VALUES
        ( '#{type}', '#{group}' )
      RETURNING #{FTY_ID};
      ).gsub(/\s+/, ' ').strip

    @log.debug "Executing query: #{query}"
    add_record_to_remove(FTY_TAB, FTY_ID, exec_query(query)[0][FTY_ID])
  end

  # rubocop:disable Metrics/ParameterLists

  # Inserta a row in inventory
  def insert_inventory(name, creation, val_start, val_stop, type_id, sat_id, attributes, content_type: 'test',
                       content_length: 0, checksum: '{}')
    # rubocop:enable Metrics/ParameterLists
    query = %(
      INSERT INTO	#{INV_TAB}
        ( #{INV_NAME}, #{INV_CONT_TYPE}, #{INV_CONT_LEN}, #{INV_CREAT}, #{INV_CS}, #{INV_VAL_START}, #{INV_VAL_STOP},
          #{INV_TYPE}, #{INV_SAT}, #{INV_ATR} )
      VALUES
        ( '#{name}', '#{content_type}', #{content_length}, '#{creation}', '#{checksum}', '#{val_start}', '#{val_stop}',
          #{type_id}, #{sat_id}, #{attributes.nil? ? 'NULL' : "'#{attributes}'"} )
      RETURNING #{INV_ID};
      ).gsub(/\s+/, ' ').strip

    @log.debug "Executing query: #{query}"
    add_record_to_remove(INV_TAB, INV_ID, exec_query(query)[0][INV_ID])
  end
end

# Module for create reference reports from template and compare with generated ones
module ReportOperations
  private

  REPORT_GENERAL_TEMPLATE_PATH = File.absolute_path(File.join(__dir__, 'data', 'report_general_template.xml'))
  REPORT_ENTRY_TEMPLATE_PATH = File.absolute_path(File.join(__dir__, 'data', 'report_entry_template.xml'))
  REPORT_EMPTY_TEMPLATE_PATH = File.absolute_path(File.join(__dir__, 'data', 'report_empty_template.xml'))

  # General template placeholders
  SAT_KEY = /__SAT__/.freeze
  REPORT_V_START_KEY = /__REPORT_V_START__/.freeze
  REPORT_V_STOP_KEY = /__REPORT_V_STOP__/.freeze
  REPORT_CREATION_KEY = /__REPORT_CREATION__/.freeze
  ENTRY_COUNT_KEY = /__ENTRY_COUNT__/.freeze
  ENTRIES_KEY = /__ENTRIES__/.freeze

  # Entry template placeholders
  FILE_NAME_KEY = /__FILE_NAME__/.freeze
  FILE_TYPE_KEY = /__FILE_TYPE__/.freeze
  FILE_V_START_KEY = /__FILE_V_START__/.freeze
  FILE_V_STOP_KEY = /__FILE_V_STOP__/.freeze
  FILE_CREATION_KEY = /__FILE_CREATION__/.freeze

  # Create a reference report based on input data and template files
  def create_reference(sat, creation, entries)
    File.read(REPORT_GENERAL_TEMPLATE_PATH)
        .gsub(SAT_KEY, sat)
        .gsub(REPORT_V_START_KEY, ADGSBaselineReportGenerator::ReportHandler::REPORT_VALIDITY_START_DATE)
        .gsub(REPORT_V_STOP_KEY, ADGSBaselineReportGenerator::ReportHandler::REPORT_VALIDITY_STOP_DATE)
        .gsub(REPORT_CREATION_KEY, creation)
        .gsub(ENTRY_COUNT_KEY, entries.size.to_s)
        .gsub(ENTRIES_KEY, create_reference_entries(entries))
        .gsub(%r{<(.+)></\1>}, '<\1/>')
  end

  # Create a reference entry for the reference report based on input data and template file
  def create_reference_entries(entries)
    template_entry = File.read(REPORT_ENTRY_TEMPLATE_PATH)
    entries.inject('') do |report_entry, entry|
      "#{report_entry.empty? ? '' : "#{report_entry}\n"}#{template_entry.dup
                                                                        .gsub(FILE_NAME_KEY, entry[:name])
                                                                        .gsub(FILE_TYPE_KEY, entry[:type])
                                                                        .gsub(FILE_V_START_KEY, entry[:start])
                                                                        .gsub(FILE_V_STOP_KEY, entry[:stop])
                                                                        .gsub(FILE_CREATION_KEY, entry[:creation])}"
    end
  end

  # Create a empty reference report based on input data and template file
  def create_empty_reference(sat, creation)
    File.read(REPORT_EMPTY_TEMPLATE_PATH)
        .gsub(SAT_KEY, sat)
        .gsub(REPORT_V_START_KEY, ADGSBaselineReportGenerator::ReportHandler::REPORT_VALIDITY_START_DATE)
        .gsub(REPORT_V_STOP_KEY, ADGSBaselineReportGenerator::ReportHandler::REPORT_VALIDITY_STOP_DATE)
        .gsub(REPORT_CREATION_KEY, creation)
  end

  # Find the report for a given satellite acronym
  def find_report(output_path, sat_acronym)
    Dir.glob(File.join(output_path, "#{sat_acronym}_ADFBASELINE_*"))
  end
end

# :nocov:
