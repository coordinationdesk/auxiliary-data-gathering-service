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

def check_usage
  unless ARGV.size >= 3
    puts "Usage: #{File.basename($PROGRAM_NAME)} database_name table_name CSV_file_name"
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



def load_cblob(file_name)
  contents = File.open(file_name, 'rb') { |f| f.read }
  escaped_content = "(E'#{contents.gsub("'"){|m| "\\#{m}"}}')"
end


def reformat_field(value)
  r_value = String.new
  if value.nil? || value.empty? || value == "''" || value=~/null/ || value=~/NULL/
    r_value << "NULL"
  else
    value.strip!
    if value[1...-1] =~ /^\(.*\)$/
      r_value << "#{value[1...-1]}"
    elsif  value[1...-1] =~ /^%(\/|\.\.\/)[:print:]*([\/][:print:]+)?/
      r_value << load_cblob(value[1...-1].sub("%",""))
    elsif value =~ /_empty_/
       r_value << "''"
    else
      r_value << "#{value}"
    end
  end
  return r_value
end

def make_update_sql(schema,table_name,columns, table)
   update_sql_statement = "update #{table_name} "
   columns_counter=0
   columns_pairs = Array.new
   columns.each do |field|
     if not field.nil? and not field.to_s.empty?
       field.gsub!("'","").strip!
       if field =~ /^&.*/
         key_pair = ['K',field[1..-1]]
         puts "PAIR: #{key_pair}"
       else
         key_pair = ['U',field]
         puts "PAIR: #{key_pair}"
       end
       columns_pairs << key_pair
       columns_counter += 1
     end
   end
   table.each do |row|
      set_sql_statement = " set "
	  where_sql_statement = " where "
     (0...columns_counter).each do |index|
       r_value = row[index]
       r_value = 'NULL' if r_value.nil? || r_value == "''" || r_value=~/null/ || r_value=~/NULL/
       if  columns_pairs[index][0] == 'K'    #expected a key field
         where_sql_statement << (columns_pairs[index][1] + " = " + r_value + " AND ")
       else
         set_sql_statement << (columns_pairs[index][1] + " = " + reformat_field(r_value) + " , ")
       end
     end
     where_sql_statement = where_sql_statement[0...-4] + ";" #remove last && and close the statement
     set_sql_statement = set_sql_statement[0...-2]  #remove last comma leaving a space
     sql_statement = update_sql_statement + set_sql_statement + where_sql_statement
     execute_query(schema,sql_statement)
   end
end



if $0 == __FILE__
  check_usage
  begin
    schema = ARGV.shift
    table_name = ARGV.shift
    file_name = ARGV.shift
    columns,table = read_params(file_name)
    # This script will use postgres personality to connect DB without passwd assuming it is run as admin
    Process.euid = Etc.getpwnam("postgres").uid
    make_update_sql(schema,table_name,columns,table)
  rescue Exception => ex
    puts ex
    puts ex.backtrace
    exit(150)
  end
end
