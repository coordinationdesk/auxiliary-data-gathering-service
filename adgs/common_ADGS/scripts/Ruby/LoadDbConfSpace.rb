#!/usr/bin/env ruby

#Copyright 1995-2013, Advanced Computer Systems , Inc.
#Via Della Bufalotta, 378 - 00139 Roma - Italy
#http://www.acsys.it

#All Rights Reserved.

#This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#the contents of this file may not be disclosed to third parties, copied or
#duplicated in any form, in whole or in part, without the prior written
#permission of Advanced Computer Systems, Inc.

require 'rubygems'
require 'optparse'
require 'pg'
require 'etc'
require 'pathname'

$options = {}
$option_parser = OptionParser.new do |opts|
  executable_name = File.basename($PROGRAM_NAME)
  opts.banner = "Usage: #{executable_name} [options] database_name [conf_space_name] [conf_space_file] [version]"
  opts.banner = <<-EOS
  
  Load configuration space from a specific file or in bulk mode from a directory 
  
  "Usage: #{executable_name} [options] database_name [conf_space_name] [conf_space_file] [version]"
EOS
  opts.on("-d full_path_dir ","--dir full_path_dir",
          'all files inside the directory [<name>.xml] are loaded as configuration spaces with corresponging <name>') do |full_path_dir| 
    $options[:full_path_dir] = full_path_dir
  end
end

def check_usage
  unless ARGV.size >= 1
    puts $option_parser.help
    exit 150
  end
end

def execute_query(schema,query)
  begin
    db = PGconn.open(:dbname => schema)
    res = db.exec(query)
    db.close
    return res
  rescue Exception => ex
    puts "[#{schema}] #{query}"
    raise ex.message
  end
end

def get_confspace_version(schema,confspace_name)
   begin
    sql_statement = "select version from t_configurationspaces where namespace = '#{confspace_name.gsub("'","")}' order by version DESC limit 1"
    res = execute_query(schema,sql_statement)
    if res.ntuples >= 1
      return res[0]['version'].to_f
    else
      return 0.0
    end  
  rescue Exception => ex
    puts sql_statement
    raise ex.message
  end
end

def load_confspace(schema,confspace_name,file_name,version=0.0 )
  begin
    current_version = get_confspace_version(schema,confspace_name)
    if  version.nil? || version.to_f <= current_version
      delta = current_version.ceil - current_version
      if delta > 0.01 || delta == 0.0
        version = current_version + 0.01
      else 
        version = current_version.ceil.to_f
      end
    else
      version = version.to_f
    end
    string_version =  "%4.2f" % version
    contents = File.open(file_name, 'rb') { |f| f.read }
    sql_statement = "insert into t_configurationspaces(namespace,version,space) values('#{confspace_name}','#{string_version}',(E'#{contents.gsub("'"){|m| "\\#{m}"}}'));"
    execute_query(schema,sql_statement)
    max_version = get_confspace_version(schema,confspace_name)
    if (max_version - version).abs >  0.01
       raise "WARNING The actual max version [#{max_version}] Is different from the uploaded one [#{version}]"
    end
  rescue Exception => ex
    raise ex.message
  end  
end

if $0 == __FILE__
  check_usage
  begin
    $option_parser.parse!
    schema = ARGV.shift
    if ARGV.size >= 2
      confspace_name = ARGV.shift
      file_name = ARGV.shift    
      input_version = ARGV.shift
    end
    # This script will use postgres personality to connect DB without passwd assuming it is run as admin
    Process.euid = Etc.getpwnam("postgres").uid
    unless $options[:full_path_dir]
      load_confspace(schema,confspace_name,file_name, input_version)
    else
      source_dir = $options[:full_path_dir]
      conf_spaces = Dir.glob("#{source_dir}/*.xml")
      conf_spaces.each do |file|
        conf_space = File.basename(file)
        conf_name = conf_space.clone
        conf_name.slice!(".xml")
        load_confspace(schema,conf_name,file)
      end
      puts 
    end
  rescue Exception => ex
    puts ex
    puts ex.backtrace
    exit(150)
  end
end
