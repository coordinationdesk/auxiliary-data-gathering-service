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

# Store the default method for log
module DefaultLogger
  module_function

  # Log a DEBUG message
  def debug(*args, &block)
    @debug[*args, &block]
  end

  # Set the DEBUG handler
  def debug=(val)
    @debug = val
  end

  # Log a DEBUG message with the default logger
  def default_debug(*args, &block)
    args_val = args.join(', ')
    block_val = block_given? ? block.call : ''
    scoped = !args_val.empty? && !block_val.empty?
    puts "DEBUG: #{scoped ? '(' : ''}#{args_val}#{scoped ? ') ' : ''}#{block_val}"
  end

  # Log a INFO message
  def info(*args, &block)
    @info[*args, &block]
  end

  # Set the INFO handler
  def info=(val)
    @info = val
  end

  # Log a INFO message with the default logger
  def default_info(*args, &block)
    args_val = args.join(', ')
    block_val = block_given? ? block.call : ''
    scoped = !args_val.empty? && !block_val.empty?
    puts "INFO: #{scoped ? '(' : ''}#{args_val}#{scoped ? ') ' : ''}#{block_val}"
  end

  # Log a WARN message
  def warn(*args, &block)
    @warn[*args, &block]
  end

  # Set the WARN handler
  def warn=(val)
    @warn = val
  end

  # Log a WARN message with the default logger
  def default_warn(*args, &block)
    args_val = args.join(', ')
    block_val = block_given? ? block.call : ''
    scoped = !args_val.empty? && !block_val.empty?
    puts "WARN: #{scoped ? '(' : ''}#{args_val}#{scoped ? ') ' : ''}#{block_val}"
  end

  # Log a ERROR message
  def error(*args, &block)
    @error[*args, &block]
  end

  # Set the ERROR handler
  def error=(val)
    @error = val
  end

  # Log a ERROR message with the default logger
  def default_error(*args, &block)
    args_val = args.join(', ')
    block_val = block_given? ? block.call : ''
    scoped = !args_val.empty? && !block_val.empty?
    # rubocop:disable Style/StderrPuts
    $stderr.puts "ERROR: #{scoped ? '(' : ''}#{args_val}#{scoped ? ') ' : ''}#{block_val}"
    # rubocop:enable Style/StderrPuts
  end

  # Log a FATAL message
  def fatal(*args, &block)
    @fatal[*args, &block]
  end

  # Set the FATAL handler
  def fatal=(val)
    @fatal = val
  end

  # Log a FATAL message with the default logger
  def default_fatal(*args, &block)
    args_val = args.join(', ')
    block_val = block_given? ? block.call : ''
    scoped = !args_val.empty? && !block_val.empty?
    # rubocop:disable Style/StderrPuts
    $stderr.puts "FATAL: #{scoped ? '(' : ''}#{args_val}#{scoped ? ') ' : ''}#{block_val}"
    # rubocop:enable Style/StderrPuts
  end

  # Log a UNKNOWN message
  def unknown(*args, &block)
    @unknown[*args, &block]
  end

  # Set the UNKNOWN handler
  def unknown=(val)
    @unknown = val
  end

  # Log a UNKNOWN message with the default logger
  def default_unknown(*args, &block)
    args_val = args.join(', ')
    block_val = block_given? ? block.call : ''
    scoped = !args_val.empty? && !block_val.empty?
    puts "UNKNOWN: #{scoped ? '(' : ''}#{args_val}#{scoped ? ') ' : ''}#{block_val}"
  end

  # Reset logger methods to defaults values
  def reset_defaults
    @debug = method(:default_debug)
    @info = method(:default_info)
    @warn = method(:default_warn)
    @error = method(:default_error)
    @fatal = method(:default_fatal)
    @unknown = method(:default_unknown)
  end

  # Call the reset_defaults for initialization
  reset_defaults
end

# Print an error message in a standardized way
def print_error(err, msg = nil, logger_method: DefaultLogger.method(:error))
  return unless err.is_a?(Exception)

  final_msg = ''

  # Print custom message
  final_msg += "#{msg}\n" unless msg.nil?

  # Print the error
  final_msg += "An error of type #{err.class} happened, message is:\n  #{err.message.gsub("\n", "\n  ")}\nat\n"
  err.backtrace.each { |stack| final_msg += "  #{stack}\n" }
  logger_method[final_msg]
end

# Check the type of an input
def check_type(input, name, classes, nil_allowed: false, boolean_allowed: false)
  return if classes.any? { |el| input.is_a?(el) }
  return if nil_allowed && input.nil?
  return if boolean_allowed && [true, false].include?(input)

  raise_type_error(name, classes, nil_allowed, boolean_allowed)
end

def raise_type_error(name, classes, nil_allowed, boolean_allowed)
  classes.push('Boolean') if boolean_allowed
  msg = classes.empty? ? '' : 'must be '
  classes.push('must have nil value') if nil_allowed
  raise TypeError, "Argument '#{name}' #{msg}#{classes.join(', ').sub(/(.*)\b, \b/, '\1 or ')}"
end

# Check if a directory is empty
def dir_empty?(dir)
  (Dir.entries(dir) - %w[. ..]).empty?
end

# Loop inside a folder and perform some standard actions
def each_in_dir(path, check_writable: false)
  # Check if the path is writable
  raise IOError, "Directory '#{path}' not exist." unless File.exist?(path)
  raise IOError, "Directory '#{path}' is not a directory." unless File.directory?(path)
  raise IOError, "Directory '#{path}' not writable." if check_writable && !File.writable?(path)

  # Loop all the subfolder
  Dir.foreach(path) { |el| yield(File.join(path, el), el) unless ['.', '..'].include?(el) }
end

# Create a TAR file
require 'open3'
def create_tar(options, folder, tar_name, files)
  raise IOError, "Path '#{folder}' is not an existing directory" unless File.exist?(folder) && File.directory?(folder)

  files = files.join(' ') if files.is_a?(Array)

  data = ''
  Dir.chdir(folder) do
    raise IOError, "File '#{tar_name}' already exists" if File.exist?(tar_name)

    cmd = "tar #{options} '#{tar_name}' #{files}"
    data, err, status = Open3.capture3(cmd)

    # Check the result
    raise "Command '#{cmd}' fail with status #{status} and error: '#{err}'" unless err.empty? && status.success?
  end
  data
end
