#!/usr/bin/env ruby

require 'time'
require 'pg'
require 'fileutils'
require 'socket'
require 'json'
require 'securerandom'
require 'digest'
require 'rexml/document'
include REXML

EXTENSION_PATTERN = '*.{TGZ,tar,zip,EOF,gz}'

def format_message_GPICD(message,msg_class)
    themessage = Time.now.strftime("%Y-%m-%dT%H:%M:%S.000000 ") + Socket.gethostname + " #{File.basename(__FILE__)} 02.00 [" + Process::pid.to_s + "]: [#{msg_class}] " +  message
    return themessage
end

def check_usage
    unless ARGV.size == 1
        puts "Usage: #{File.basename(__FILE__)} <JobOrder>"
        exit 1
    end
end

def get_time_hash(start,stop)
  tempDateHash = Hash.new
  tempDateHash["Start"]=start.strftime('%Y-%m-%dT%H:%M:%S.%3NZ')
  tempDateHash["End"]=stop.strftime('%Y-%m-%dT%H:%M:%S.%3NZ')
  return tempDateHash
end

def s1_eof_start_stop(filename)
  #Porcata per il FOS
  if filename =~ /MPL_[OTM]/ || filename =~ /REP__SUP/ || filename =~ /TLM__REQ/ || 
     filename =~ /REP__MACP/ || filename =~ /REP__MCSF/ || filename =~ /REP__SMPR/ || 
     filename =~ /L_PREORB/  || filename =~ /MP_ALL__MTL/
    start = Time.parse(filename[20,15])
    if filename[36,15]  =~ /9999/
      stop=Time.new("2100-01-01 00:00:00")
    else
      stop=Time.parse(filename[36,15])
    end
  elsif filename =~ /AUX_OBMEMC/
    start = Time.parse(filename[25,15])
    stop = Time.new("2100-01-01 00:00:00")
  elsif filename =~ /MP_FULL/
    start = Time.parse(filename[12,15])
    stop=Time.parse(filename[28,15])
  else
    start=Time.parse(filename[42,15])
    stop=Time.parse(filename[58,15])
  end
  return get_time_hash(start,stop)
end

def s3_eof_start_stop(filename)
  #Porcata per il FOS
  if filename =~ /MPL_[_D]/ ||
     filename  =~ /OPER_REP__SUP/
    start=Time.parse(filename[20,15])
    stop=Time.parse(filename[36,15])
  else
    start=Time.parse(filename[42,15])
    stop=Time.parse(filename[58,15])
  end
  return get_time_hash(start,stop)
end

def s5_eof_start_stop(filename)
  start=Time.parse(filename[20,15])
  stop=Time.parse(filename[36,15])
  return get_time_hash(start,stop)
end

def s12_gip_start_stop(filename)
  start=Time.parse(filename[42,15])
  if filename[58,15] =~ /9999/
    stop=Time.new("2100-01-01 00:00:00")
  else
    stop=Time.parse(filename[58,15])
  end
  return get_time_hash(start,stop)
end

def s12_hktmsad_start_stop(filename)
  start=Time.parse(filename[20,15])
  if filename[36,15] =~ /9999/
    stop=Time.new("2100-01-01 00:00:00")
  else
    stop=Time.parse(filename[36,15])
  end
  return get_time_hash(start,stop)
end

def s2_extract_metadata(file)
  filename=File.basename(file,File.extname(file))
  extract_command = "tar --extract  --strip-components 1 --file=#{file} #{filename}/Inventory_Metadata.xml"
  result = system(extract_command)
  raise "Failed to execute command [#{extract_command}] at Line[#{__LINE__}]" if result == false
end

def s1_extract_manifest(file)
  extract_command = "7z e #{file} -aoa -o./ manifest.safe -r &> /dev/null"
  result = system(extract_command)
  raise "Failed to execute command [#{extract_command}] at Line[#{__LINE__}]" if result == false
end

def s3_extract_manifest(file)
  extract_command = "7z e #{file} -aoa -o./ xfdumanifest.xml -r &> /dev/null"
  result = system(extract_command)
  raise "Failed to execute command [#{extract_command}] at Line[#{__LINE__}]" if result == false
end

def s1_prod_manifest_start_stop(manifest_meta)
  xmlfile = File.new(manifest_meta)
  xmldoc = Document.new(xmlfile)
  start_s=xmldoc.text("//safe:startTime")
  stop_s=xmldoc.text("//safe:stopTime")
  if start_s.nil? || stop_s.nil? #L0 Products
    start_s=xmldoc.text("//startTime")
    stop_s=xmldoc.text("//stopTime")
  end
  if start_s.nil? || stop_s.nil? #AUXILIARY
    start_s=xmldoc.text("//s1auxsar:validity")
    stop_s=xmldoc.text("//s1auxsar:validity")
  end
  if start_s.nil? || stop_s.nil? #AUXILIARY S1__AUX_SCS_V20171017T080000_G20171016T150910.SAFE.zip NOT in Specific
    start_s=xmldoc.text("//ns1:validity")
    stop_s=xmldoc.text("//ns1:validity")
  end
  start=Time.parse(start_s)
  stop=Time.parse(stop_s)
  return get_time_hash(start,stop)
end

def s2_tarprod_start_stop(inv_meta)
  xmlfile = File.new(inv_meta)
  xmldoc = Document.new(xmlfile)
  start=Time.parse(xmldoc.text("/Inventory_Metadata/Validity_Start"))
  stop=Time.parse(xmldoc.text("/Inventory_Metadata/Validity_Stop"))
  return get_time_hash(start,stop)
end

def s3_prod_manifest_start_stop(manifest_meta)
  xmlfile = File.new(manifest_meta)
  xmldoc = Document.new(xmlfile)
  start_s=xmldoc.text("//sentinel-safe:startTime")
  stop_s=xmldoc.text("//sentinel-safe:stopTime")
  if start_s.nil? || stop_s.nil?
    start_s=xmldoc.text("//sentinel3aux:validityStartTime")
    stop_s=xmldoc.text("//sentinel3aux:validityStopTime")
  end
  start=Time.parse(start_s)
  stop=Time.parse(stop_s)
  return get_time_hash(start,stop)
end

def gml_to_lta(pol_string)
  lta_s = String.new
  gml_pol = pol_string.split(" ")
  (0..gml_pol.length - 1).step(2).each do |i|
    gml_pol[i], gml_pol[i+1] = gml_pol[i+1], gml_pol[i]
    lta_s << "#{gml_pol[i]} " << "#{gml_pol[i+1]}, "
  end
  return  lta_s[0..-3]
end
def s1_gml_to_lta(pol_string,filename)
  lta_s = String.new
  gml_pol = pol_string.split(" ")
  gml_pol.each do |tuple|
    lat,lon = tuple.split(",")
    lta_s << "#{lon} " << "#{lat}, "
  end
  if   File.basename(filename) =~ /RAW/
    return  lta_s[0..-3]
  else
    first_point = lta_s.split(",")
    return  lta_s + first_point[0]
  end
end

def s3_manifest_footprint(manifest_meta)
  xmlfile = File.new(manifest_meta)
  xmldoc = Document.new(xmlfile)
  pol_string = xmldoc.text("//sentinel-safe:frameSet/sentinel-safe:footPrint/gml:posList")
  return "geography'SRID=4326;Polygon(("+ gml_to_lta(pol_string) + "))'" unless pol_string.nil?
end

def s1_manifest_footprint(manifest_meta,filename)
  xmlfile = File.new(manifest_meta)
  xmldoc = Document.new(xmlfile)
  pol_string = xmldoc.text("//safe:frameSet/safe:frame/safe:footPrint/gml:coordinates")
  if pol_string.nil?
    pol_string = xmldoc.text("//frameSet/frame/footPrint/coordinates")
  end
  unless  pol_string.nil?
    lta_coord = s1_gml_to_lta(pol_string,filename)
    if lta_coord.split(",").size == 2
      return
    else
      return "geography'SRID=4326;Polygon((" + lta_coord + "))'"
    end
  end
end

def s1_manifest_footprint_wv(manifest_meta,filename)
  xmlfile = File.new(manifest_meta)
  xmldoc = Document.new(xmlfile)
  first_vign_node = xmldoc.elements.to_a("//safe:frameSet/safe:frame/safe:footPrint/gml:coordinates")[0]
  last_vign_node  = xmldoc.elements.to_a("//safe:frameSet/safe:frame/safe:footPrint/gml:coordinates")[-1]
  unless first_vign_node.nil? && last_vign_node.nil? #Avoid RAW data
    first_vign_string = first_vign_node.text
    last_vign_string  = last_vign_node.text
    lta_s = String.new
    first_vign_pol = first_vign_string.split(" ")
    last_vign_pol = last_vign_string.split(" ")
    gml_pol = [first_vign_pol[0],first_vign_pol[1],last_vign_pol[-1],last_vign_pol[-2]]
    gml_pol.each do |tuple|
      lat,lon = tuple.split(",")
      lta_s << "#{lon} " << "#{lat}, "
    end
    first_point = lta_s.split(",")
    return "geography'SRID=4326;Polygon((" + lta_s + first_point[0] + "))'"
   end
end

def s2_invmeta_footprint(inv_meta)
  xmlfile = File.new(inv_meta)
  xmldoc = Document.new(xmlfile)
  lon_a = Array.new
  lat_a = Array.new
  xmldoc.elements.each("//Geo_Pnt/LONGITUDE") do |lon|
    lon_a << lon.text
  end
  xmldoc.elements.each("//Geo_Pnt/LATITUDE") do |lat|
    lat_a << lat.text
  end
  pol_string = String.new
  lon_a.each_index do |i|
    pol_string << lon_a[i] << " " << lat_a[i] << ", "
  end
  return "geography'SRID=4326;Polygon((" + pol_string[0..-3] + "))'" unless pol_string.empty?
end

def gen_val_start_stop(file)
  filename=File.basename(file)
  sat=filename[0,2]
  case sat
    when "S3"
      if filename =~ /EOF/ || filename =~ /TGZ/
        return s3_eof_start_stop(filename)
      else
        s3_extract_manifest(file)
        return s3_prod_manifest_start_stop("xfdumanifest.xml")
      end
    when "S5"
      return s5_eof_start_stop(filename)
    when "S1"
      if filename =~/EOF/ || filename =~/TGZ/ || filename =~/xml/ || filename =~/csv/
        return s1_eof_start_stop(filename)
      elsif filename =~/DBL/ || filename =~/MPL_TIMELINE/
        return s12_hktmsad_start_stop(filename)
      elsif filename =~/REP_MP_MP/
        return s12_gip_start_stop(filename)
      else
        s1_extract_manifest(file)
        return s1_prod_manifest_start_stop("manifest.safe")
      end
    when "S2"
      if filename=~/_GIP_/ || filename=~/_AUX_/
        return s12_gip_start_stop(filename)
      end
      if filename=~/_HKTM_/ || filename=~/_SAD_/ || filename=~/TLM__REQ/ || filename=~/EOF/ || filename=~/TGZ/ || filename =~/DBL/ || filename =~/MPL_TIMELINE/
        return s12_hktmsad_start_stop(filename)
      end
      if filename=~/.tar$/
        s2_extract_metadata(file)
        return s2_tarprod_start_stop("Inventory_Metadata.xml")
      end
    else
     raise "Satellite Not Managed"
    end
end

def get_satellite_id(full_sat)
  return 34 if full_sat == 'S1_'
  return 35 if full_sat == 'S1A'
  return 36 if full_sat == 'S1B'
  return 37 if full_sat == 'S2_'
  return 38 if full_sat == 'S2A'
  return 39 if full_sat == 'S2B'
  return 40 if full_sat == 'S3_'
  return 41 if full_sat == 'S3A'
  return 42 if full_sat == 'S3B'
  return 46 if full_sat == 'S5P'
end
def gen_xxh(filename)
  value=`xxhsum #{filename}`
  value.split(' ')[0].strip
end
def get_esc_prefix_name(prod_name)
  prod_name.split('.')[0].gsub('_','\_')
end
def get_origin_date(prod_name)
  begin
    db = PGconn.open(:host => $db_hostname, :dbname => $db_schema, :user => $db_user, :password => $db_pass)
    query="SELECT equatorxtime FROM t_inventory WHERE filename LIKE '#{get_esc_prefix_name(prod_name)+'%'}'"
    res = db.exec(query)
    origin_date = res[0]['equatorxtime'] if res.ntuples == 1
    db.close
    return origin_date
  rescue Exception => ex
    puts "[#{$db_schema}] #{query}"
    raise ex.message
  end
end

def gen_metadata_auxip(filename,prod_origin_date)
  tempHash = Hash.new
  tempHash["Id"] = SecureRandom.uuid
  tempHash["Name"] = filename
  tempHash["ContentType"] = "application/octet-stream"
  tempHash["ContentLength"] = File.stat(filename).size
  #This time is actually used as origin date to be retrieved by query  on PDS
  tempHash["PublicationDate"] = prod_origin_date
  tempMD5Hash = Hash.new
  tempMD5Hash["Algorithm"]="MD5"
  tempMD5Hash["Value"]=Digest::MD5.file(filename).hexdigest
  tempMD5Hash["ChecksumDate"]= Time.now.strftime('%Y-%m-%dT%H:%M:%S.%3NZ')
  tempXXHHash = Hash.new
  tempXXHHash["Algorithm"]="XXH"
  tempXXHHash["Value"]=gen_xxh(filename)
  tempXXHHash["ChecksumDate"]= Time.now.strftime('%Y-%m-%dT%H:%M:%S.%3NZ')
  tempCSArray=Array.new
  tempCSArray[0]=tempMD5Hash
  tempCSArray[1]=tempXXHHash
  tempHash['Checksum'] = tempCSArray
  tempHash['ContentDate']=gen_val_start_stop(filename)
  sat=File.basename(filename)[0,2]
  case sat
    when "S3"
      if File.basename(filename) !~ /EOF/ &&
         File.basename(filename) !~ /TGZ/ &&
         File.basename(filename) !~ /SR_2_SI[C|F][N|S]AX/
        foot_print = s3_manifest_footprint("xfdumanifest.xml")
      end
      tempHash["Mission"] = 3   #Sentinel 3A/B
    when "S1"
      if File.basename(filename) !~ /EOF/ &&
         File.basename(filename) !~ /xml/ &&
         File.basename(filename) !~ /DBL/ &&
         File.basename(filename) !~ /MPL_TIMELINE/ &&
         File.basename(filename) !~ /TGZ/ &&
         File.basename(filename) !~ /csv/ &&
         File.basename(filename) !~ /_WV_...__(1|2)/ &&
         File.basename(filename) !~ /REP_MP_MP/
        foot_print = s1_manifest_footprint("manifest.safe",filename)
      end
      if File.basename(filename) =~ /_WV_...__(1|2)/
        foot_print = s1_manifest_footprint_wv("manifest.safe",filename)
      end
      tempHash["Mission"] = 1   #Sentinel 1A/B
    when "S2"
      file = File.basename(filename)
      if  file =~ /tar/ && file !~ /AUX/ && file !~ /HKTM/ && file !~ /MPL_TIMELINE/
        foot_print = s2_invmeta_footprint("Inventory_Metadata.xml")
      end
      tempHash["Mission"] = 2   #Sentinel 2A/B
    when "S5"
      tempHash["Mission"] = 5
    else
     raise "Satellite Not Managed"
  end
  tempHash["Footprint"] = foot_print unless foot_print.nil?
  full_sat = File.basename(filename)[0,3]
  tempHash["Satellite_Id"] = get_satellite_id(full_sat)
  return tempHash
end

if $0 == __FILE__
    check_usage
    begin
    puts format_message_GPICD("Starting #{File.basename(__FILE__)}  LTAPreinventory","I")
    #Load DB Conf File Parameters
    db_conf_file = File.dirname(__FILE__) + "/../conf/database/db.conf"
    db_conf = File.read(db_conf_file)
    $db_hostname = db_conf.scan(/Postgres\.host\[0\]\.dbConnectionArbiterCParam\.hostname:(.*)\n/).first[0].strip
    #Porcata schema del PDS scolpito assumendo che PDS e SRV schema siano sullo stesso motore SQL
    $db_schema = 'adgsdb'
    $db_user = 'pdsuser'
    #Porcata non so decriptare la Password messa in chiaro
    $db_pass = '12qwas'

    job_order = ARGV.shift
    xmlfile = File.new(job_order)
    xmldoc = Document.new(xmlfile)
    file=xmldoc.text("//Input/List_of_File_Names/File_Name")
    prod_file = File.basename(file)
    meta_name = prod_file + ".METADATA"
    origin_date=nil
    #origin_date = get_origin_date(prod_file)
    unless origin_date.nil?
      prod_origin_date = Time.parse(origin_date).strftime('%Y-%m-%dT%H:%M:%S.%3NZ')
    else
      prod_origin_date = Time.now.strftime('%Y-%m-%dT%H:%M:%S.%3NZ')
    end
    meta_hash = gen_metadata_auxip(prod_file,prod_origin_date)
    out_dir = "./"
    File.write(File.join(out_dir,meta_name),JSON.pretty_generate(meta_hash))
    puts format_message_GPICD("Completed #{File.basename(__FILE__)} LTAPreinventory","I")
  rescue Exception => ex
        puts format_message_GPICD(ex.message,"E")
        puts ex.backtrace
        exit(150)
  end
end
