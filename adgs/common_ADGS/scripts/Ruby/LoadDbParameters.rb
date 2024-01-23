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
  opts.banner = "Usage: #{executable_name} [options] database_name table_name CSV_file_name"
  opts.banner = <<-EOS
  
  Load Values inside a table of a schema starting from a CSV file.
  The first line not blank or commented(#) must contain the table fields.
  The following rows may contain string (''enclosed) numbers or empty values meaning NULL value.
  The string fields may be a text, an sql statement returning single value enclosed by () or a full path name
  to Load cblob.
  
  "Usage: #{executable_name} [options] database_name table_name CSV_file_name"

EOS
  opts.on("-a","--append",
          'Indicates that table will not be cleaned before tuples insertion') do 
    $options[:append] = true
  end
end


def check_usage
  unless ARGV.size >= 3
    puts $option_parser.help
    exit 150
  end
end
	

def read_params(file_name)
	first_column=true 
  columns = []
  table = []  
  File.open(file_name) do|f|
    until f.eof?
      line = f.readline.chomp
      next if line =~ /^(#|,)/ || line.empty?
      if first_column
        columns = line.split(',')
        first_column=false
      else
        table <<  line.split(',')
      end
    end
  end
  return columns, table
end


def execute_query(schema,query)
  begin
    db = PGconn.open(:dbname => schema)
    res = db.exec(query)
    db.close
  rescue Exception => ex
    puts "[#{schema}] #{query}"
    raise ex.message
  end
end

def clean_table (schema,table_name)
  begin
    sql_statement = "delete from #{table_name}"
    execute_query(schema,sql_statement)
  rescue Exception => ex
    puts sql_statement
    raise ex.message
  end
end

def load_cblob(file_name)
  contents = File.open(file_name, 'rb') { |f| f.read }
  escaped_content = "(E'#{contents.gsub("'"){|m| "\\#{m}"}}'),"  
end

def make_sql(schema,table_name,columns, table)  
  sql_statement1 = "insert into #{table_name}("
  columns_counter=0
  columns.each do |field|
    if not field.nil? and not field.to_s.empty?
      sql_statement1 << field.gsub("'","") << " ,"
      columns_counter += 1
    end
  end
  sql_statement1[-1] =") values("
  table.each do |row|
    sql_statement2 = String.new
    (0...columns_counter).each do |index|
      value = row[index]
      if value.nil? || value.empty? || value == "''" || value=~/null/ || value=~/NULL/
        sql_statement2 << "NULL,"
      else
        value.strip!
        if value[1...-1] =~ /^\(.*\)$/
          sql_statement2 << "#{value[1...-1]},"
        elsif  value[1...-1] =~ /^%(\/|\.\.\/)[:print:]*([\/][:print:]+)?/
          sql_statement2 << load_cblob(value[1...-1].sub("%",""))
        elsif value =~ /_empty_/
           sql_statement2 << "'',"
        else
          sql_statement2 << "#{value},"
        end
      end
    end
    sql_statement2[-1] =");"
    sql_statement = sql_statement1 + sql_statement2
    begin
      execute_query(schema,sql_statement)
    rescue Exception => ex
      puts "The Following SQL statement[#{sql_statement}] failed skipped!!!" 
    end  
  end
end


if $0 == __FILE__
  check_usage
  begin
    $option_parser.parse!
    schema = ARGV.shift
    table_name = ARGV.shift
    file_name = ARGV.shift
    columns,table = read_params(file_name)
    # This script will use postgres personality to connect DB without passwd assuming it is run as admin
    Process.euid = Etc.getpwnam("postgres").uid
    unless $options[:append]
      clean_table(schema,table_name)
    end
    make_sql(schema,table_name,columns,table)

  rescue Exception => ex
    puts ex
    puts ex.backtrace
    exit(150)
  end
end
