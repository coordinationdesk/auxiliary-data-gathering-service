# frozen_string_literal: true

# Copyright 2021-2023, Exprivia SpA.
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
# $Prod: RubyLibs $

require 'json'

# Load a given configuration file and store its values
class ConfigurationHandler
  ####################
  # INSTANCE METHODS #
  ####################

  # Parse the configuration
  def initialize(file_name)
    raise IOError, "Invalid configuration file '#{file_name}'" unless File.exist?(file_name)

    @config = JSON.parse(File.read(file_name))
  end

  # Get a key value. If key is an Array it will be treated as a sequence of nested keys (e.g. [a, b, c] => a.b.c).
  def get(key, mandatory: true)
    get_sub_key(@config, key, mandatory)
  end

  ############################
  # PRIVATE INSTANCE METHODS #
  ############################
  def get_sub_key(obj, key, mandatory)
    next_key = key.is_a?(Array) ? key[0] : key

    # If the key exists, return its value
    if obj.is_a?(Hash) && obj.key?(next_key)
      return key.is_a?(Array) && key.size > 1 ? get_sub_key(obj[next_key], key[1..-1], mandatory) : obj[next_key]
    end

    # Raise an exception if the key does not exist and is mandatory
    raise "Invalid key '#{next_key}'" if mandatory

    # Return nil if the key is not mandatory
    nil
  end
end

# Extension module for DB configuration
module LoggerConfigurationParams
  attr_reader :logger_file, :logger_level, :logger_rotation_period, :logger_files_to_keep, :logger_text_files

  CONF_LOGGER_FILE_PATH_TAG = %w[Logger FilePath].freeze
  CONF_LOGGER_FILE_PATH_DEFAULT = File.join('..', 'log', "#{File.basename($PROGRAM_NAME, '.rb')}.log")
  CONF_LOGGER_LEVEL_TAG = %w[Logger Level].freeze
  CONF_LOGGER_LEVEL_DEFAULT = 1
  CONF_LOGGER_ROTATION_PERIOD_TAG = %w[Logger RotationPeriod].freeze
  CONF_LOGGER_ROTATION_PERIOD_DEFAULT = 1.0
  CONF_LOGGER_FILES_TO_KEEP_TAG = %w[Logger FilesToKeep].freeze
  CONF_LOGGER_FILES_TO_KEEP_DEFAULT = 20
  CONF_LOGGER_TEXT_FILES_TO_KEEP_TAG = %w[Logger TextFilesToKeep].freeze
  CONF_LOGGER_TEXT_FILES_TO_KEEP_DEFAULT = 1

  private

  # Set the configured logger as default
  def logger_params(base_dir: File.dirname($PROGRAM_NAME))
    @logger_file = File.absolute_path(get(CONF_LOGGER_FILE_PATH_TAG, mandatory: false) || CONF_LOGGER_FILE_PATH_DEFAULT,
                                      base_dir)
    @logger_level = get(CONF_LOGGER_LEVEL_TAG, mandatory: false) || CONF_LOGGER_LEVEL_DEFAULT
    @logger_rotation_period = get(CONF_LOGGER_ROTATION_PERIOD_TAG, mandatory: false) ||
                              CONF_LOGGER_ROTATION_PERIOD_DEFAULT
    @logger_files_to_keep = get(CONF_LOGGER_FILES_TO_KEEP_TAG, mandatory: false) || CONF_LOGGER_FILES_TO_KEEP_DEFAULT
    @logger_text_files = get(CONF_LOGGER_TEXT_FILES_TO_KEEP_TAG, mandatory: false) ||
                         CONF_LOGGER_TEXT_FILES_TO_KEEP_DEFAULT
  end
end

# Extension module for DB configuration
module DBConfigurationParams
  attr_reader :db_conf_file, :db_host_index, :db_pwd_decrypter

  CONF_DB_CONF_FILE_TAG = %w[DB ConfFile].freeze
  CONF_DB_CONF_FILE_DEFAULT = File.join('..', 'conf', 'database', 'db.conf')
  CONF_DB_HOST_INDEX_TAG = %w[DB HostIndex].freeze
  CONF_DB_HOST_INDEX_DEFAULT = 0
  CONF_DB_PWD_DECRYPTER_TAG = %w[DB PasswordDecrypter].freeze
  CONF_DB_PWD_DECRYPTER_DEFAULT = ''

  private

  # Set the db configuration parameters
  def db_params(base_dir: File.dirname($PROGRAM_NAME))
    @db_conf_file = File.absolute_path(get(CONF_DB_CONF_FILE_TAG, mandatory: false) || CONF_DB_CONF_FILE_DEFAULT,
                                       base_dir)

    @db_host_index = get(CONF_DB_HOST_INDEX_TAG, mandatory: false) || CONF_DB_HOST_INDEX_DEFAULT
    @db_pwd_decrypter = get(CONF_DB_PWD_DECRYPTER_TAG, mandatory: false) || CONF_DB_PWD_DECRYPTER_DEFAULT

    return if @db_pwd_decrypter.empty? || !@db_pwd_decrypter.include?(File::SEPARATOR)

    @db_pwd_decrypter = File.absolute_path(@db_pwd_decrypter, base_dir)
  end
end
