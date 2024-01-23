#!/usr/bin/env ruby
# frozen_string_literal: true

require 'fileutils'
require 'date'
require 'digest'
require 'rexml/document'

# Pack the CAMS files
module CAMSReFormatter
  ########################
  ### MODULE CONSTANTS ###
  ########################

  # Defaults values
  DEFAULT_CONF_FILE = File.expand_path(File.join(File.dirname(__FILE__), '..', 'conf',
                                                 's2_CAMS_re_formatter_task.conf')).freeze
  DEFAULT_HDR_TEMPLATE = './Template.HDR'
  DEFAULT_HDR_SYSTEM = 'ADG_'
  DEFAULT_HDR_CREATOR = 'MCC_'

  # Configuration tags
  CAMS_WORK_DIR_TAG = 'CAMSWorkDir'
  FINAL_BASKET_TAG = 'FinalBasket'
  HDR_TEMPLATE_TAG = %w[HDR Template].freeze
  HDR_SYSTEM_TAG = %w[HDR System].freeze
  HDR_SYSTEM_MAX_LENGTH = 4
  HDR_CREATOR_TAG = %w[HDR Creator].freeze

  # Header file token for substitution
  HDR_EOFFS_NAME_TOKEN = '__eoffs_name__'
  HDR_START_DATE_TOKEN = '__start_date__'
  HDR_STOP_DATE_TOKEN = '__stop_date__'
  HDR_SYSTEM_TOKEN = '__system__'
  HDR_CREATOR_TOKEN = '__creator__'
  HDR_CREATION_DATE_TOKEN = '__creation_date__'

  # Complete regexp with all files, including manifest for extracting date
  REGEX_EXTRACT_DATE = Regexp.new('/z_cams_c_ecmf_([0-9]{14})_prod(?:\\.manifest|_(?:an_sfc_000_z|' \
                                  'fc_sfc_0(?:[0-3][0-9]|4[0-8])_(?:aod550|bcaod550|duaod550|omaod550|ssaod550|' \
                                  'suaod550|aod469|aod670|aod865|aod1240))\\.grib)\\z').freeze

  # Total number of files to find
  NUM_OF_FILES = 492

  ######################
  ### MODULE METHODS ###
  ######################

  # Complete regexp with all files, including manifest with a fixed date
  def self.complete_regex(date)
    Regexp.new("/z_cams_c_ecmf_#{date}_prod(?:\\.manifest|_(?:an_sfc_000_z|fc_sfc_0(?:[0-3][0-9]|4[0-8])_"\
               '(?:aod550|bcaod550|duaod550|omaod550|ssaod550|suaod550|aod469|aod670|aod865|aod1240))\.grib)\z').freeze
  end

  # Regex for manifest file with a chosen date
  def self.regex_manifest(date)
    %r{/z_cams_c_ecmf_#{date}_prod\.manifest\z}.freeze
  end

  # Regex for extracting md5 and file name pairs from manifest
  def self.regex_files_md5(date)
    Regexp.new("^([a-z0-9]{32}) +(z_cams_c_ecmf_#{date}_prod_(?:an_sfc_000_z|" \
               'fc_sfc_0(?:[0-3][0-9]|4[0-8])_(?:aod550|bcaod550|duaod550|omaod550|ssaod550|' \
               'suaod550|aod469|aod670|aod865|aod1240))\\.grib)$').freeze
  end

  ######################
  ### MODULE CLASSES ###
  ######################

  ############################
  # ReFormatterConfiguration #
  ############################

  require 'yaml'

  # Configuration Handler for the CAMSReFormatter
  class ReFormatterConfiguration
    attr_reader :cams_work_dir, :final_basket, :hdr_template, :hdr_system, :hdr_creator

    def initialize(conf_file)
      # Load the configuration
      raise IOError, "Invalid configuration file '#{conf_file}'" unless File.exist?(conf_file)

      @config = YAML::load_file(conf_file)
      
      extract_paths
      extract_hdr_data
    end

    private

    # Get a key value. If key is an Array it will be treated as a sequence of nested keys (e.g. [a, b, c] => a.b.c).
    def get(key, mandatory = true)
      get_sub_key(@config, key, mandatory)
    end

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
      @cams_work_dir = File.expand_path(get(CAMSReFormatter::CAMS_WORK_DIR_TAG), File.dirname(__FILE__))
      raise IOError, "Working folder '#{@cams_work_dir}' does not exists" unless File.exist?(@cams_work_dir)

      @final_basket = File.expand_path(get(CAMSReFormatter::FINAL_BASKET_TAG), File.dirname(__FILE__))
      raise IOError, "Final basket folder '#{@final_basket}' does not exists" unless File.exist?(@final_basket)
    end

    # Extract the fixed header data from configuration
    def extract_hdr_data
      @hdr_template = get(CAMSReFormatter::HDR_TEMPLATE_TAG, false) || CAMSReFormatter::DEFAULT_HDR_TEMPLATE
      @hdr_template = File.expand_path(@hdr_template, File.dirname(__FILE__))
      raise IOError, "Template header file '#{@hdr_template}' does not exists" unless File.exist?(@hdr_template)

      @hdr_system = get(CAMSReFormatter::HDR_SYSTEM_TAG, false) || CAMSReFormatter::DEFAULT_HDR_SYSTEM
      if @hdr_system.size > CAMSReFormatter::HDR_SYSTEM_MAX_LENGTH
        puts "Invalid header system '#{@hdr_system}': max length is #{CAMSReFormatter::HDR_SYSTEM_MAX_LENGTH}, " \
        "going on using default value '#{CAMSReFormatter::DEFAULT_HDR_SYSTEM}'"
        @hdr_system = CAMSReFormatter::DEFAULT_HDR_SYSTEM
      end

      @hdr_creator = get(CAMSReFormatter::HDR_CREATOR_TAG, false) || CAMSReFormatter::DEFAULT_HDR_CREATOR
    end
  end

  ##############################
  # ReFormatterJobOrderHandler #
  ##############################

  # Module for XML JobOrder handler
  class ReFormatterJobOrderHandler
    include REXML

    attr_reader :source_path, :date, :working_folder, :start_date, :start_date_utc, :stop_date, :stop_date_utc,
                :eoffs_name, :eoffs_folder_path, :lock_file, :dbl_name, :hdr_name, :tgz_name, :tgz_working_path,
                :tgz_final_path

    def initialize(job_order, conf)
      raise TypeError, "Argument 'job_order' must be a String" unless job_order.is_a?(String)
      raise TypeError, "Argument 'conf' must be a ReFormatterConfiguration" unless conf.is_a?(ReFormatterConfiguration)
      raise IOError, "JobOrder file '#{job_order}' does not exists" unless File.exist?(job_order)

      job_xml = Document.new(File.new(job_order))
      @source_path = job_xml.text('Ipf_Job_Order/List_of_Ipf_Procs/Ipf_Proc/List_of_Inputs/Input/'\
        'List_of_File_Names/File_Name')
      raise IOError, "File '#{@source_path}' does not exists" unless File.exist?(@source_path)

      puts "File to process: '#{@source_path}'"

      extract_date(conf.cams_work_dir, conf.hdr_system)
      compute_final_files_names(conf.final_basket)
    end

    private

    # Extract the data from the filename
    def extract_date(work_dir, system)
      match_result = REGEX_EXTRACT_DATE.match(@source_path)
      raise "The file '#{@source_path}' is not one of the files to take" if match_result.nil?

      @date = match_result[1]

      @working_folder = File.join(work_dir, @date)
      date = DateTime.strptime(@date, '%Y%m%d%H%M%S')
      compute_dates(date)
      @eoffs_name = "S2__OPER_AUX_CAMSFO_#{system}_#{@start_date}_V#{@start_date}_#{@stop_date}"
      @eoffs_folder_path = File.join(@working_folder, @eoffs_name)
      @lock_file = File.join(@working_folder, '.lock')
    end

    # Compute dates
    def compute_dates(date)
      delta_day = (date.hour == 23) ? 3 : 2
      delta_hour = (date.hour == 23) ? -23 : 1
      date_stop = DateTime.new(date.year, date.month, date.day, date.hour + delta_hour, date.min, date.sec) + delta_day
      @stop_date = date_stop.strftime('%Y%m%dT%H%M%S')
      @stop_date_utc = date_stop.strftime('UTC=%Y-%m-%dT%H:%M:%S')
      @start_date = date.strftime('%Y%m%dT%H%M%S')
      @start_date_utc = date.strftime('UTC=%Y-%m-%dT%H:%M:%S')
    end

    # Compute final files names
    def compute_final_files_names(final_basket)
      @dbl_name = "#{@eoffs_name}.DBL"
      @hdr_name = "#{@eoffs_name}.HDR"
      @tgz_name = "#{@eoffs_name}.TGZ"
      @tgz_working_path = File.join(@working_folder, @tgz_name)
      @tgz_final_path = File.join(final_basket, @tgz_name)
    end
  end

  #########################
  # S2CAMSReFormatterTask #
  #########################

  # Main class for CAMS files conversion
  class S2CAMSReFormatterTask
    def initialize(job_order, conf_file)
      conf = ReFormatterConfiguration.new(conf_file)
      job = ReFormatterJobOrderHandler.new(job_order, conf)

      puts "Start processing group with date #{job.date}"
      execute_job(job, conf)
      puts "End processing group with date #{job.date}"
    end

    private

    # Execute the given job
    def execute_job(job, conf)
      dest = job.eoffs_folder_path
      date = job.date

      # Move the current file in the working directory
      move_file(job.source_path, dest)

      return unless working_folder_complete?(dest, date) && manifest_valid?(date) && working_folder_valid?(dest, date)

      puts "All check passed for group with date #{date}, try to create the final EOFFS"
      ensure_unique(job.lock_file, job.working_folder) do
        create_eoffs(job, conf)
        move_to_final_destination(job.tgz_working_path, job.tgz_final_path, job.working_folder)
      end
    end
    
    # Move a source file to the final dest folder. If destination folder does not exists, it will be created
    def move_file(source, dest)
      # Create the folders if needed
      FileUtils.mkdir_p(dest)

      raise IOError, "Source file '#{source}' does not exists" unless File.exist?(source)

      name = File.basename(source)
      raise IOError, "File '#{name}' already exists in '#{dest}' folder" if File.exist?(File.join(dest, name))

      FileUtils.mv(source, dest)
      puts "File '#{name}' moved to '#{dest}'"
    end

    # Check if the current working folder is complete
    def working_folder_complete?(path, date)
      @list_of_files = Dir.glob(File.join(path, '*'))
      if @list_of_files.count < CAMSReFormatter::NUM_OF_FILES
        puts "Group with date #{date} not completed: #{@list_of_files.count} files found, "\
             "#{CAMSReFormatter::NUM_OF_FILES} expected"
        return false
      end

      result = @list_of_files.grep(CAMSReFormatter.complete_regex(date)).count == CAMSReFormatter::NUM_OF_FILES
      raise "Group with date #{date} have some unknown file and then it will be skipped" unless result

      result
    end

    # Check if manifest is valid
    def manifest_valid?(date)
      @manifest = @list_of_files.grep(CAMSReFormatter.regex_manifest(date))[0]
      @md5_and_files = File.new(@manifest).read.scan(CAMSReFormatter.regex_files_md5(date))

      result = @md5_and_files.size == CAMSReFormatter::NUM_OF_FILES - 1

      unless result
        puts "Missing files in manifest file '#{@manifest}' for group with date #{date}: " \
             "#{@md5_and_files.size} files found, #{CAMSReFormatter::NUM_OF_FILES - 1} expected"
      end

      result
    end

    # Check if the files in the current working folder are valid (all whit correct md5)
    def working_folder_valid?(path, date)
      result = true
      @md5_and_files.each do |md5, file|
        file_path = File.join(path, file)
        file_md5 = Digest::MD5.file(file_path).hexdigest if File.exist?(file_path)
        if file_md5 != md5
          puts "Invalid MD5 for file '#{file}' of group with date #{date}: #{file_md5} computed, #{md5} expected"
          result = false
        end
      end

      result
    end

    # Get the lock on the current working directory
    def ensure_unique(lock_file, working_folder)
      if !File.exist?(working_folder)
        puts "Cannot open the '#{working_folder}' folder, another instance have already finalize the EOFFS"
        return
      end
      File.open(lock_file, File::CREAT | File::RDWR) do |lock|
        begin
          if lock.flock(File::LOCK_EX | File::LOCK_NB)
            puts "Lock acquired on '#{working_folder}' folder"
            yield
          else
            puts "Cannot lock the '#{working_folder}' folder, another instance will finalize the EOFFS"
          end
        ensure
          lock.flock(File::LOCK_UN)
        end
      end
    end

    # Create the final eoffs
    def create_eoffs(job, conf)
      working_folder = job.working_folder
      create_tar('-cf', job.eoffs_folder_path, job.dbl_name, '*')
      FileUtils.mv(File.join(job.eoffs_folder_path, job.dbl_name), File.join(working_folder, job.dbl_name))
      puts "DBL file '#{job.dbl_name}' created"
      create_header(job, conf)
      puts "Header file '#{job.hdr_name}' created"
      create_tar('-czf', working_folder, job.tgz_name, [job.dbl_name, job.hdr_name])
      puts "EOFFS file '#{job.tgz_name}' created"
    end

    # Create a TAR file
    require 'open3'
    def create_tar(options, folder, tar_name, files, relative = true)
      raise IOError, "Path '#{folder}' is not an existing directory" unless File.exist?(folder) && File.directory?(folder)

      files = files.join(' ') if files.is_a?(Array)

      data = ''
      Dir.chdir(folder) do
        raise IOError, "File '#{tar_name}' already exists" if File.exist?(tar_name)

        cmd = "tar #{options} '#{tar_name}'"
        cmd = relative ? "#{cmd} #{files}" : "#{cmd} -C #{files} ."
        stdin, stdout, stderr = Open3.popen3(cmd)
        data = stdout.read
        err = stderr.read

        # Check the result
        raise "Command '#{cmd}' fail with error: '#{err}'" unless err.empty?
      end
      data
    end
    
    # Create the header file
    def create_header(job, conf)
      hdr_path = File.join(job.working_folder, job.hdr_name)
      raise IOError, "Header file '#{hdr_path}' already exists" if File.exist?(hdr_path)

      f = File.open(hdr_path, 'w')
      f.write(template_data(job, conf))
      f.close
    end

    # Load template file and substitute fields
    def template_data(job, conf)
      data = File.read(conf.hdr_template)
      data.gsub!(/#{CAMSReFormatter::HDR_EOFFS_NAME_TOKEN}/, job.eoffs_name)
      data.gsub!(/#{CAMSReFormatter::HDR_START_DATE_TOKEN}/, job.start_date_utc)
      data.gsub!(/#{CAMSReFormatter::HDR_STOP_DATE_TOKEN}/, job.stop_date_utc)
      data.gsub!(/#{CAMSReFormatter::HDR_SYSTEM_TOKEN}/, conf.hdr_system)
      data.gsub!(/#{CAMSReFormatter::HDR_CREATOR_TOKEN}/, conf.hdr_creator)
      data.gsub!(/#{CAMSReFormatter::HDR_CREATION_DATE_TOKEN}/, job.start_date_utc)
      data
    end

    # Move the tgz file to the final destination and clean the working directory
    def move_to_final_destination(tgz, final_path, working_folder)
      raise IOError, "File '#{final_path}' already exists" if File.exist?(final_path)

      FileUtils.mv(tgz, final_path)
      FileUtils.rm_rf(working_folder)

      puts "EOFFS file '#{File.basename(tgz)}' moved to the final destination '#{final_path}'"
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
    conf_file = ARGV.empty? ? CAMSReFormatter::DEFAULT_CONF_FILE : ARGV.shift

    # Convert files
    CAMSReFormatter::S2CAMSReFormatterTask.new(job_order, conf_file)
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
