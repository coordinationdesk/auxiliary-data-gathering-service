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

# :nocov:
# Add the lib main to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'logger'
require 'socket'
require 'date'
require 'fileutils'
require 'open3'

require 'common_utils'

# Constants for RotatingLogger
module RotatingLoggerConstants
  # Initialize all the levels
  DEBUG = Logger::DEBUG
  INFO = Logger::INFO
  WARN = Logger::WARN
  ERROR = Logger::ERROR
  FATAL = Logger::FATAL
  UNKNOWN = Logger::UNKNOWN

  # Initialize fixed values
  HOST_NAME = Socket.gethostname
  PROG_NAME = File.basename($PROGRAM_NAME, '.rb')
  PROCESS_ID = Process.pid
end

# Implement a logger with a custom rotation policy
class RotatingLogger
  include RotatingLoggerConstants

  # Initialize the logger.
  # Parameter:
  # - file_name: the name of the current file to use, rotated files will have the same name with some suffix
  # - level: minimum log level, levels below this values will be skipped (default: INFO)
  # - rotation_period: time between rotations in days, if negative rotation is disabled (default: disabled)
  # - max_file_num: max number of old files to keep, if negative keep all files (default: 20)
  # - text_file_num: number of files to keep not compressed, if negative no files will be compressed (default: 1)
  def initialize(file_name, level: INFO, rotation_period: -1.0, max_file_num: 20, text_file_num: 1)
    check_vars(file_name, level, rotation_period, max_file_num, text_file_num)

    @file_name = file_name
    @file_path = File.dirname(file_name)
    @level = level
    @rotation_period = rotation_period
    @max_file_num = max_file_num
    @text_file_num = text_file_num

    create_logger
  end

  # Assign this logger instance as the default logger
  def set_as_default_logger
    DefaultLogger.debug = method(:debug)
    DefaultLogger.info = method(:info)
    DefaultLogger.warn = method(:warn)
    DefaultLogger.error = method(:error)
    DefaultLogger.fatal = method(:fatal)
    DefaultLogger.unknown = method(:unknown)
  end

  # Log a debug message
  def debug(*args, &block)
    check_rotation if @rotation_period > 0
    @logger.send(:debug, *args, &block)
  end

  # Log a info message
  def info(*args, &block)
    check_rotation if @rotation_period > 0
    @logger.send(:info, *args, &block)
  end

  # Log a warn message
  def warn(*args, &block)
    check_rotation if @rotation_period > 0
    @logger.send(:warn, *args, &block)
  end

  # Log a error message
  def error(*args, &block)
    check_rotation if @rotation_period > 0
    @logger.send(:error, *args, &block)
  end

  # Log a fatal message
  def fatal(*args, &block)
    check_rotation if @rotation_period > 0
    @logger.send(:fatal, *args, &block)
  end

  # Log a unknown message
  def unknown(*args, &block)
    check_rotation if @rotation_period > 0
    @logger.send(:unknown, *args, &block)
  end

  private

  # Initialize the class variables
  def check_vars(file_name, level, rotation_period, max_file_num, text_file_num)
    check_type(file_name, 'file_name', [String])
    allowed_levels = [DEBUG, INFO, WARN, ERROR, FATAL]
    raise ArgumentError, "Argument 'level' must be one of: #{allowed_levels}" unless allowed_levels.include?(level)

    check_type(rotation_period, 'rotation_period', [Integer, Float])
    check_type(max_file_num, 'max_file_num', [Integer])
    check_type(text_file_num, 'text_file_num', [Integer])
  end

  # Set the custom messages format
  def create_logger
    @logger.close unless @logger.nil?
    @logger = Logger.new(@file_name)
    @logger.level = @level
    @logger.formatter = proc do |level, date_time, scope_name, msg|
      time = date_time.strftime('%FT%T.%L')
      scope = "#{scope_name} - " if scope_name
      "#{time} #{HOST_NAME} #{PROG_NAME} #{PROCESS_ID} [#{level}] #{scope}#{msg}\n"
    end
    @next_rotation = nil
  end

  # Set the last rotation date
  def compute_next_rotation
    date_str = /\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2} [+-]\d{4}/.match(IO.readlines(@file_name)[0])
    @next_rotation = date_str ? DateTime.strptime(date_str[0], '%F %T %z') + @rotation_period : DateTime.now - 1
  end

  # Rotate log if needed
  def check_rotation
    return create_logger unless File.exist?(@file_name)

    compute_next_rotation if @next_rotation.nil?
    return unless DateTime.now >= @next_rotation

    @logger.close
    rotate_files(0)
    create_logger
  end

  # Rotate the file with the given index. The file must exist
  def rotate_files(index)
    this_file = rotated_file_name(index)
    if @max_file_num > index || @max_file_num < 0
      next_file = rotated_file_name(index + 1)
      rotate_files(index + 1) if File.exist?(next_file)
      index == @text_file_num ? compress_log(this_file, next_file) : FileUtils.mv(this_file, next_file)
    else
      FileUtils.rm_f(this_file)
    end
  end

  # Compress the log file
  def compress_log(source, dest)
    create_tar('-czf', @file_path, File.basename(dest), File.basename(source))
  end

  # Return the file name of rotated file with a given index
  def rotated_file_name(index)
    return @file_name if index < 1
    return "#{@file_name}.#{index}" if index <= @text_file_num || @text_file_num < 0

    "#{@file_name}.#{index}.gz"
  end
end
