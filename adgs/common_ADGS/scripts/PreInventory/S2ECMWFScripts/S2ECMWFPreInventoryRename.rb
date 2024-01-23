#!/usr/bin/env ruby
require 'rubygems'
require 'pathname'
require 'yaml'
require 'socket'
require 'nokogiri'
require 'fileutils'


scriptPath=File.expand_path(File.dirname(__FILE__))
libPath=scriptPath+"/../lib/"
$LOAD_PATH << libPath
configPath="/data/ADGS/conf/"

require 'libDB'
require 'libReadDBConf'
require 'libMetaData'
require 'libXMLHeader'

class ECMWFMTDReader
  def initialize()
  end
  def find_file_name(doc)
    doc.at_xpath("//Inventory_Metadata/File_Name").text.strip
  end

  def find_generation_time(doc)
    doc.at_xpath("//Inventory_Metadata/Generation_Time").text.strip
  end

  def find_validity_start(doc)
    doc.at_xpath("//Inventory_Metadata/Validity_Start").text.strip
  end

  def find_validity_stop(doc)
    doc.at_xpath("//Inventory_Metadata/Validity_Stop").text.strip
  end

  def find_file_version(doc)
    doc.at_xpath("//Inventory_Metadata/File_Version").text.strip
  end

  def find_data_size(doc)
    doc.at_xpath("//Inventory_Metadata/Data_Size").text.strip
  end

  def find_file_class(doc)
    doc.at_xpath("//Inventory_Metadata/File_Class").text.strip
  end

  def find_origindate(doc)
    doc.at_xpath("//Inventory_Metadata/Equator_X_Time").text.strip
  end
end


def format_message_GPICD(message,msg_class)
  themassage = Time.now.strftime("%Y-%m-%dT%H:%m:%S.000000 ") + Socket.gethostname + " #{File.basename(__FILE__)} 02.00 [" + Process::pid.to_s + "]: [#{msg_class}] " +  message
  return themassage
end

def utc_to_file_time(utime)
#UTC=2004-07-23T12:00:00
	utime[4,4]+utime[9,2]+utime[12,5]+utime[18,2]+utime[21,2]
end

def check_usage
  unless ARGV.size == 1
    puts "Usage: #{File.basename(__FILE__)} <JobOrder>"
    exit 1
  end
end

if $0 == __FILE__
  check_usage
  begin
    puts format_message_GPICD("Starting S2 ECMWF Preinventory Rename","I")
    job_order = ARGV.shift
    input_mtd = Dir.glob("*.MTD")[0]
    input_mtd_doc = Nokogiri::XML(File.open(input_mtd))
    input_mtd_doc.remove_namespaces!()
    dbConf = LibReadDBConf.new(configPath+"database/db.conf")
    dbConf.redconf()
    dbGet=LibDB.new(dbConf.hostname(),dbConf.database(),dbConf.user(),dbConf.password(),dbConf.port())

    ecmwf_mtd = ECMWFMTDReader.new()
    ecmwf_input_file_name =  ecmwf_mtd.find_file_name(input_mtd_doc)
    ecmwf_input_generation_time = ecmwf_mtd.find_generation_time(input_mtd_doc)
    ecmwf_input_validity_start =  ecmwf_mtd.find_validity_start(input_mtd_doc)
    ecmwf_input_validity_stop = ecmwf_mtd.find_validity_stop(input_mtd_doc)
    ecmwf_input_file_version =  ecmwf_mtd.find_file_version(input_mtd_doc)
    ecmwf_input_data_size =  ecmwf_mtd.find_data_size(input_mtd_doc)
    ecmwf_input_file_class =  ecmwf_mtd.find_file_class(input_mtd_doc)
    ecmwf_input_origindate =  ecmwf_mtd.find_origindate(input_mtd_doc)

    #Rename Product
    final_file_name = "S2__" + ecmwf_input_file_class + "_AUX_ECMWFD_"+"PDMC_"+ utc_to_file_time(ecmwf_input_generation_time) + 
    "_V" + utc_to_file_time(ecmwf_input_validity_start) + "_" +  utc_to_file_time(ecmwf_input_validity_stop)
	FileUtils.mv(ecmwf_input_file_name,final_file_name)
	
	#Build MTD
	libMetaData=LibMetaData.new()
	libMetaData.filename=final_file_name
	libMetaData.fileversion=ecmwf_input_file_version
	libMetaData.gentime=ecmwf_input_generation_time
	libMetaData.validitystart = ecmwf_input_validity_start
	libMetaData.validitystop = ecmwf_input_validity_stop
	libMetaData.validationdate = ecmwf_input_generation_time
	libMetaData.filetype = "AUX_ECMWFD" # Constant 
	libMetaData.fileclass = ecmwf_input_file_class
	libMetaData.sateliteid=dbGet.getSatID("S2_")
	libMetaData.mission=1
	libMetaData.origindate=ecmwf_input_origindate
	libMetaData.datasize=ecmwf_input_data_size
	libMetaData.headersize=0
	libMetaData.preiventory_mtd()
	# Save and remove old MDT
	libMetaData.writeFile(final_file_name)
	FileUtils.rm(input_mtd)	
	#Remove Old List and create New One
	FileUtils.rm Dir.glob('*.LIST')
	list_name = Process.pid.to_s + ".LIST"
	File.open(list_name,"w") do |file|
	  file.puts(final_file_name)
	end
    puts format_message_GPICD("Completed S2 ECMWF Preinventory Rename","I")
  rescue Exception => ex
    puts format_message_GPICD(ex.message,"E")
    puts ex.backtrace
    exit(150)
  end
end
