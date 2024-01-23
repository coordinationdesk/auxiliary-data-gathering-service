#!/usr/bin/env ruby
# frozen_string_literal: true

require 'fileutils'
require 'date'
require 'digest'
require 'rexml/document'

# Pack the ECMWF files
module ECMWFReFormatter
  ########################
  ### MODULE CONSTANTS ###
  ########################

  # Defaults values
  DEFAULT_CONF_FILE = File.expand_path(File.join(File.dirname(__FILE__), '..', 'conf',
                                                 's2_ECMWF_re_formatter_task.conf')).freeze
  DEFAULT_HDR_TEMPLATE = './Template.HDR'
  DEFAULT_HDR_SYSTEM = 'VPMC'
  DEFAULT_HDR_CREATOR = 'MCC_'
  DEFAULT_NUM_OF_FILES = 14
  DEFAULT_TIMEOUT = 3600
  DEFAULT_POLLING = 60
  DEFAULT_ECMWFD_FIRST_LETTER = 'S'

  # Configuration tags
  ECMWF_WORK_DIR_TAG = 'ECMWFWorkDir'
  FINAL_BASKET_TAG = 'FinalBasket'
  HDR_TEMPLATE_TAG = %w[HDR Template].freeze
  HDR_SYSTEM_TAG = %w[HDR System].freeze
  HDR_SYSTEM_MAX_LENGTH = 4
  HDR_CREATOR_TAG = %w[HDR Creator].freeze
  NUM_OF_FILES_TAG = 'NumOfFiles'
  TIMEOUT_TAG = 'Timeout'
  POLLING_TAG = 'Polling'
  ECMWFD_FIRST_LETTER_TAG = 'ECMWFDFirstLetter'

  # Header file token for substitution
  HDR_EOFFS_NAME_TOKEN = '__eoffs_name__'
  HDR_START_DATE_TOKEN = '__start_date__'
  HDR_STOP_DATE_TOKEN = '__stop_date__'
  HDR_SYSTEM_TOKEN = '__system__'
  HDR_CREATOR_TOKEN = '__creator__'
  HDR_CREATION_DATE_TOKEN = '__creation_date__'

  # Complete regexp with all files for extracting creation date and forecast date
  S_REGEX_EXTRACT_DATE = Regexp.new('S2D(\d{8})(\d{8})\d\z').freeze
  D_REGEX_EXTRACT_DATE = Regexp.new('D2D(\d{8})(\d{8})\d\z').freeze

  ######################
  ### MODULE METHODS ###
  ######################

  # Extract the creation date and the validity dates
  def self.extract_dates(name, regex_extract_date, mandatory = true)
    match_result = regex_extract_date.match(name)
    return nil if match_result.nil? && !mandatory
    raise "The file '#{name}' is not one of the files to take" if match_result.nil?

    creation = DateTime.strptime(match_result[1], '%m%d%H%M')

    start = DateTime.strptime(match_result[2], '%m%d%H%M')
    start = add_years(start, 1) if start.month < creation.month

    stop = add_hours(start, 3)

    return [creation, start, stop]
  end

  # Add some year to a DateTime object
  def self.add_years(data, num)
    DateTime.new(data.year + num, data.month, data.day, data.hour, data.min, data.sec)
  end

  # Add some hours to a DateTime object
  def self.add_hours(data, num)
    DateTime.new(data.year, data.month, data.day, data.hour + num, data.min, data.sec)
  end

  # Move a source file to the final dest folder. If destination folder does not exists, it will be created
  def self.move_file(source, dest)
    raise IOError, "Source file '#{source}' does not exists" unless File.exist?(source)

    # Create the folders if needed
    FileUtils.mkdir_p(File.dirname(dest))

    # Check if destination already exists
    raise IOError, "File '#{File.basename(source)}' already exists in '#{dest}' folder" if File.exist?(dest)

    # Move the file
    FileUtils.mv(source, dest)
    puts "File '#{File.basename(source)}' moved to '#{dest}'"
  end

  # Create a TAR file
  require 'open3'
  def self.create_tar(options, folder, tar_name, files)
    raise IOError, "Path '#{folder}' is not an existing directory" unless File.exist?(folder) && File.directory?(folder)

    files = files.join(' ') if files.is_a?(Array)

    data = ''
    Dir.chdir(folder) do
      raise IOError, "File '#{tar_name}' already exists" if File.exist?(tar_name)

      cmd = "tar #{options} '#{tar_name}' #{files}"
      _stdin, stdout, stderr = Open3.popen3(cmd)
      data = stdout.read
      err = stderr.read

      # Check the result
      raise "Command '#{cmd}' fail with error: '#{err}'" unless err.empty?
    end
    data
  end

  ######################
  ### MODULE CLASSES ###
  ######################

  ############################
  # ReFormatterConfiguration #
  ############################

  require 'yaml'

  # Configuration Handler for the ECMWFReFormatter
  class ReFormatterConfiguration
    attr_reader :ecmwf_work_dir, :final_basket, :hdr_template, :hdr_system, :hdr_creator, :num_of_files, :timeout,
                :polling, :regex_extract_date

    def initialize(conf_file)
      # Load the configuration
      raise IOError, "Invalid configuration file '#{conf_file}'" unless File.exist?(conf_file)

      @config = YAML.load_file(conf_file)

      extract_paths
      extract_hdr_data
      extract_end_conditions
    end

    # Get a key value. If key is an Array it will be treated as a sequence of nested keys (e.g. [a, b, c] => a.b.c).
    def get(key, mandatory = true)
      get_sub_key(@config, key, mandatory)
    end

    private

    # Manage nested keys in get
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

    # Extract the paths from configuration and check them
    def extract_paths
      @ecmwf_work_dir = File.expand_path(get(ECMWFReFormatter::ECMWF_WORK_DIR_TAG), File.dirname(__FILE__))
      raise IOError, "Working folder '#{@ecmwf_work_dir}' does not exists" unless File.exist?(@ecmwf_work_dir)

      @final_basket = File.expand_path(get(ECMWFReFormatter::FINAL_BASKET_TAG), File.dirname(__FILE__))
      raise IOError, "Final basket folder '#{@final_basket}' does not exists" unless File.exist?(@final_basket)
    end

    # Extract the fixed header data from configuration
    def extract_hdr_data
      @hdr_template = get(ECMWFReFormatter::HDR_TEMPLATE_TAG, false) || ECMWFReFormatter::DEFAULT_HDR_TEMPLATE
      @hdr_template = File.expand_path(@hdr_template, File.dirname(__FILE__))
      raise IOError, "Template header file '#{@hdr_template}' does not exists" unless File.exist?(@hdr_template)

      @hdr_system = get(ECMWFReFormatter::HDR_SYSTEM_TAG, false) || ECMWFReFormatter::DEFAULT_HDR_SYSTEM
      if @hdr_system.size > ECMWFReFormatter::HDR_SYSTEM_MAX_LENGTH
        puts "Invalid header system '#{@hdr_system}': max length is #{ECMWFReFormatter::HDR_SYSTEM_MAX_LENGTH}, " \
        "going on using default value '#{ECMWFReFormatter::DEFAULT_HDR_SYSTEM}'"
        @hdr_system = ECMWFReFormatter::DEFAULT_HDR_SYSTEM
      end

      @hdr_creator = get(ECMWFReFormatter::HDR_CREATOR_TAG, false) || ECMWFReFormatter::DEFAULT_HDR_CREATOR
    end

    # Extract the number of files and the timeout to be used for final tar creation
    def extract_end_conditions
      @num_of_files = get(ECMWFReFormatter::NUM_OF_FILES_TAG, false) || ECMWFReFormatter::DEFAULT_NUM_OF_FILES
      @timeout = get(ECMWFReFormatter::TIMEOUT_TAG, false) || ECMWFReFormatter::DEFAULT_TIMEOUT
      @polling = get(ECMWFReFormatter::POLLING_TAG, false) || ECMWFReFormatter::DEFAULT_POLLING
      letter = get(ECMWFReFormatter::ECMWFD_FIRST_LETTER_TAG, false) || ECMWFReFormatter::DEFAULT_ECMWFD_FIRST_LETTER
      @regex_extract_date = letter.upcase == "D" ? D_REGEX_EXTRACT_DATE : S_REGEX_EXTRACT_DATE
    end
  end

  ##############################
  # ReFormatterJobOrderHandler #
  ##############################

  # Module for XML JobOrder handler
  class ReFormatterJobOrderHandler
    include REXML

    attr_reader :source_path, :source_name, :creation_date, :creation_date_utc, :start_date, :stop_date,
                :working_folder, :data_path, :destination, :timeout_lock_file, :final_lock_file

    def initialize(job_order, conf)
      raise TypeError, "Argument 'job_order' must be a String" unless job_order.is_a?(String)
      raise TypeError, "Argument 'conf' must be a ReFormatterConfiguration" unless conf.is_a?(ReFormatterConfiguration)
      raise IOError, "JobOrder file '#{job_order}' does not exists" unless File.exist?(job_order)

      job_xml = Document.new(File.new(job_order))
      @source_path = job_xml.text('Ipf_Job_Order/List_of_Ipf_Procs/Ipf_Proc/List_of_Inputs/Input/' \
        'List_of_File_Names/File_Name')
      raise IOError, "File '#{@source_path}' does not exists" unless File.exist?(@source_path)

      puts "File to process: '#{@source_path}'"

      extract_data(conf.ecmwf_work_dir, conf.regex_extract_date)
    end

    private

    # Extract the data from the filename
    def extract_data(work_dir, regex_extract_date)
      @source_name = File.basename(@source_path)

      match_result = ECMWFReFormatter.extract_dates(@source_name, regex_extract_date)

      @creation_date = match_result[0].strftime('%Y%m%dT%H%M%S')
      @creation_date_utc = match_result[0].strftime('UTC=%Y-%m-%dT%H:%M:%S')

      @start_date = match_result[1].strftime('%Y%m%dT%H%M%S')
      @stop_date = match_result[2].strftime('%Y%m%dT%H%M%S')

      compute_final_names(work_dir)
    end

    # Compute final files names
    def compute_final_names(work_dir)
      @working_folder = File.join(work_dir, @creation_date)
      @data_path = File.join(@working_folder, 'data')
      @destination = File.join(@data_path, @source_name)
      @timeout_lock_file = File.join(@working_folder, '.timeout_lock')
      @final_lock_file = File.join(@working_folder, '.final_lock')
    end
  end

  ##########################
  # S2ECMWFReFormatterTask #
  ##########################

  # Main class for ECMWF files conversion
  class S2ECMWFReFormatterTask
    def initialize(job_order, conf_file)
      @conf = ReFormatterConfiguration.new(conf_file)
      @job = ReFormatterJobOrderHandler.new(job_order, @conf)

      execute_job
    end

    private

    # Execute the given job
    def execute_job
      puts "[#{@job.source_name}] - Start processing group with creation date #{@job.creation_date}"

      # Move the current file in the working directory
      ECMWFReFormatter.move_file(@job.source_path, @job.destination)

      if working_folder_complete?
        puts "[#{@job.source_name}] - All check passed for group with date #{@job.creation_date}"
        finalize_packet
      else
        puts "[#{@job.source_name}] - Check for group with date #{@job.creation_date} not passed, try to go on polling"
        try_polling
      end

      puts "[#{@job.source_name}] - End processing group with date #{@job.creation_date}"
    end

    # Check if the current working folder is complete
    def working_folder_complete?
      @files = Dir.glob(File.join(@job.data_path, '*')).uniq

      if @files.count < @conf.num_of_files
        puts "[#{@job.source_name}] - Group with date #{@job.creation_date} not completed: #{@files.count} " \
             "files found, #{@conf.num_of_files} expected"
        return false
      end

      if extract_overall_interval != @conf.num_of_files
        raise "[#{@job.source_name}] - Group with date #{@job.creation_date} have some unknown file " \
              "and then it will be skipped"
      end

      true
    end

    # Extract the minimum validity start and the maximum validity stop, and return the count of data parsed
    def extract_overall_interval
      beginning = nil
      ending = nil

      count = 0
      @files.each do |name|
        match_result = ECMWFReFormatter.extract_dates(name, @conf.regex_extract_date, false)
        next if match_result.nil?

        beginning = beginning.nil? ? match_result[1] : [match_result[1], beginning].min
        ending = ending.nil? ? match_result[2] : [match_result[2], ending].max
        count += 1
      end
      @beginning_date = beginning.strftime('%Y%m%dT%H%M%S') unless beginning.nil?
      @ending_date = ending.strftime('%Y%m%dT%H%M%S') unless ending.nil?
      @beginning_date_utc = beginning.strftime('UTC=%Y-%m-%dT%H:%M:%S') unless beginning.nil?
      @ending_date_utc = ending.strftime('UTC=%Y-%m-%dT%H:%M:%S') unless ending.nil?

      count
    end

    # Try to go on polling
    def try_polling
      ensure_unique(@job.timeout_lock_file) do
        puts "[#{@job.source_name}] - Start polling for group with date #{@job.creation_date}"

        start_polling = DateTime.now
        while DateTime.now - start_polling < @conf.timeout / 86_400.0
          sleep([[@conf.polling, @conf.timeout - (DateTime.now - start_polling) * 86_400.0].min, 0].max)

          unless File.exist?(@job.data_path)
            puts "[#{@job.source_name}] - Stop polling for group with date #{@job.creation_date} " \
                 'because the data folder does not exists anymore'
            return
          end

          break if working_folder_complete?
        end

        # Create the TAR file in any case
        extract_overall_interval
        finalize_packet
      end
    end

    # Try to create the final tar after having get the lock on the final lock file
    def finalize_packet
      # :nocov:
      # Defensive coding
      return unless File.exist?(@job.data_path)
      # :nocov:

      puts "[#{@job.source_name}] - Try to create the final TGZ"

      ensure_unique(@job.final_lock_file) do
        create_tgz
        move_to_final_destination
      end
    end

    # Get the lock on the current working directory
    def ensure_unique(lock_file)
      File.open(lock_file, File::CREAT | File::RDWR) do |lock|
        begin
          if lock.flock(File::LOCK_EX | File::LOCK_NB)
            puts "[#{@job.source_name}] - Lock acquired on '#{lock_file}' file"
            yield
          else
            puts "[#{@job.source_name}] - Cannot lock the '#{lock_file}' file"
          end
        ensure
          lock.flock(File::LOCK_UN)
        end
      end
    end

    # Create the final tgz
    def create_tgz
      compute_final_names

      ECMWFReFormatter.create_tar('-cf', @job.data_path, @dbl_name, '*')
      FileUtils.mv(File.join(@job.data_path, @dbl_name), File.join(@job.working_folder, @dbl_name))
      puts "[#{@job.source_name}] - DBL file '#{@dbl_name}' created"

      create_header
      puts "[#{@job.source_name}] - Header file '#{@hdr_name}' created"

      ECMWFReFormatter.create_tar('-czf', @job.working_folder, @tgz_name, [@dbl_name, @hdr_name])
      puts "[#{@job.source_name}] - TGZ file '#{@tgz_name}' created"
    end

    # Compute the final names and paths
    def compute_final_names
      @eoffs_name = "S2__OPER_AUX_ECMWFD_#{@conf.hdr_system}_#{@job.creation_date}_V#{@beginning_date}_#{@ending_date}"
      @dbl_name = "#{@eoffs_name}.DBL"
      @hdr_name = "#{@eoffs_name}.HDR"
      @hdr_path = File.join(@job.working_folder, @hdr_name)
      @tgz_name = "#{@eoffs_name}.TGZ"
      @tgz_working_path = File.join(@job.working_folder, @tgz_name)
      @tgz_final_path = File.join(@conf.final_basket, @tgz_name)
    end

    # Create the header file
    def create_header
      raise IOError, "[#{@job.source_name}] - Header file '#{@hdr_path}' already exists" if File.exist?(@hdr_path)

      f = File.open(@hdr_path, 'w')
      f.write(template_data)
      f.close
    end

    # Load template file and substitute fields
    def template_data
      data = File.read(@conf.hdr_template)
      data.gsub!(/#{ECMWFReFormatter::HDR_EOFFS_NAME_TOKEN}/, @eoffs_name)
      data.gsub!(/#{ECMWFReFormatter::HDR_START_DATE_TOKEN}/, @beginning_date_utc)
      data.gsub!(/#{ECMWFReFormatter::HDR_STOP_DATE_TOKEN}/, @ending_date_utc)
      data.gsub!(/#{ECMWFReFormatter::HDR_SYSTEM_TOKEN}/, @conf.hdr_system)
      data.gsub!(/#{ECMWFReFormatter::HDR_CREATOR_TOKEN}/, @conf.hdr_creator)
      data.gsub!(/#{ECMWFReFormatter::HDR_CREATION_DATE_TOKEN}/, @job.creation_date_utc)
      data
    end

    # Move the tgz file to the final destination and clean the working directory
    def move_to_final_destination
      raise IOError, "[#{@job.source_name}] - File '#{@tgz_final_path}' already exists" if File.exist?(@tgz_final_path)

      FileUtils.mv(@tgz_working_path, @tgz_final_path)
      FileUtils.rm_rf(@job.working_folder)

      puts "[#{@job.source_name}] - TGZ file '#{File.basename(@tgz_working_path)}' moved to the final destination " \
           "'#{@tgz_final_path}'"
    end
  end
end

# :nocov:
# If the executed tile is this file
if $PROGRAM_NAME == __FILE__
  begin
    raise ArgumentError, "Job Order not provided.\nusage: #{__FILE__} JobOrder <conf>" if ARGV.empty?

    job_order = ARGV.shift

    # Get the configuration file
    conf_file = ARGV.empty? ? ECMWFReFormatter::DEFAULT_CONF_FILE : ARGV.shift

    # Convert files
    ECMWFReFormatter::S2ECMWFReFormatterTask.new(job_order, conf_file)
  rescue StandardError => e
    # Print the error
    puts "An error of type #{e.class} happened, message is:\n  #{e.message.gsub("\n", "\n  ")}\nat"
    e.backtrace.each { |stack| puts "  #{stack}" }
    puts

    # Exit with failure status
    exit(false)
  end

  # Exit with normal status
  exit(true)
end
# :nocov:
