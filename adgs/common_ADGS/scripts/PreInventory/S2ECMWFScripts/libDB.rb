require 'rubygems'
require 'pg'

class LibDB
  def initialize(hostname,database,user,password,port)
    @conn = PGconn.connect(hostname, port, '', '', database, user, password)
  end

  def close_db()
    @conn.close
  end

  def execute_query(query)
    begin
      res = @conn.exec(query)
      return res
    rescue Exception => ex
      puts "#{query}"
      raise ex.message
    end
  end
  
  def update_requested_sor(requestid,status)
    res=execute_query("update t_sor_requests set status = '#{status}', statusupdated = now() where requestid = '#{requestid}';")
  end
  
  def insert_sent_sor(spacecraftid,requestid,originator,status,emailtitle,emailbody)
    res=execute_query("update t_sor_requests set spacecraftid = '#{spacecraftid}' , originator = '#{originator}', status = '#{status}', emailtitle = '#{emailtitle}', emailbody = '#{emailbody}' where requestid = '#{requestid}';")
  end
  
  def getSorID()
	tries ||= 5
    res=execute_query("select f_sor_generation() ;") 
  rescue Exception => e
    retry unless (tries -= 1).zero?   
  else
    return res[0]['f_sor_generation']
  end

  def getSatID(satname)
    res=execute_query("select satelliteid from t_satellites where satelliteacronym='#{satname}';")[0]
    return res['satelliteid']
  end
  
  def getQualifiedID(qualitystatus)
    res=execute_query("select id from t_qualifiedflag where name='#{qualitystatus}';")[0]
    return res['id']
  end
  
  def getManageExtention(filetype)
    res=execute_query("select mngsngext from t_filetypes where filetype='#{filetype}';")[0]
    return res['mngsngext']
  end


  def getOriginator(originator)
    res=execute_query("select id from t_originators where name='#{originator}';")[0]
    return res['id']

  end

  def getStationID(station)
    res=execute_query("select stationid from t_stations where stationacronym='#{station}';")[0]
    return res['stationid']
  end

  def getBand(band)
    res=execute_query("select id from t_bands where name='#{band}';")[0]
    return res['id']
  end
end
