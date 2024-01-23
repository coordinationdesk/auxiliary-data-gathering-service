class LibMetaData
  attr_accessor :filename,:fileversion,:inventoryextra,:system,:source,:sourceversion,:gentime,:validitystart,
  :validitystart,:validitystop,:startorbit,:stoporbit,:qualityinfo,:validityflag,:validationdate,
  :headersize,:datasize,:filetype,:fileclass,:sensorid,:acquisitionid,:processingid,
  :sensormode,:phase,:qualified,:band,:sateliteid,:mission,:orderid,:relation,:origindate,:relationtype
  def initialize()
    @filename = ""
    @fileversion=""
    @inventoryextra=""
    @system=""
    @source=""
    @sourceversion=""
    @gentime=""
    @validitystart=""
    @validitystop=""
    @startorbit=""
    @stoporbit=""
    @qualityinfo
    @validityflag="TRUE"
    @validationdate=""
    @headersize=""
    @datasize=""
    @filetype=""
    @fileclass=""
    @sensorid=""
    @acquisitionid=""
    @processingid=""
    @sensormode=""
    @phase=""
    @qualified=""
    @band=""
    @sateliteid=""
    @mission=""
    @orderid=""
    @relation=""
    @origindate=""
    @preinventory_body=""
  end

  def preiventory_mtd()
    @preinventory_body = <<END_OF_STRING
<?xml version ="1.0" encoding="UTF-8"?>
<Inventory_Metadata>
            <File_Name>#{@filename}</File_Name>
            <File_Version>#{@fileversion}</File_Version>
            <Inventory_Extra>#{@inventoryextra}</Inventory_Extra>
            <System>#{@system}</System>
            <Source>#{@source}</Source>
            <Source_Sw_Version>#{@sourceversion}</Source_Sw_Version>
            <Generation_Time>#{@gentime}</Generation_Time>
            <Validity_Start>#{@validitystart}</Validity_Start>
            <Validity_Stop>#{@validitystop}</Validity_Stop>
            <Start_Orbit_Number>#{@startorbit}</Start_Orbit_Number>
            <Stop_Orbit_Number>#{@stoporbit}</Stop_Orbit_Number>
            <Geographic_Localization>
                    <Geo_Type></Geo_Type>
                    <Time_Range>
                            <Start_Time></Start_Time>
                            <Stop_Time></Stop_Time>
                    </Time_Range>
                    <List_Of_Geo_Pnt count="0">
                    </List_Of_Geo_Pnt>
            </Geographic_Localization>
            <Quality_Info>#{@qualityinfo}</Quality_Info>
            <Validity_Flag>#{@validityflag}</Validity_Flag>
            <Validation_Date>#{@validationdate}</Validation_Date>
            <Header_Size>#{@headersize}</Header_Size>
            <Data_Size>#{@datasize}</Data_Size>
            <File_Type>#{@filetype}</File_Type>
            <File_Class>#{@fileclass}</File_Class>
            <Sensor_Id>#{@sensorid}</Sensor_Id>
            <Acquisition_Station_Id>#{@acquisitionid}</Acquisition_Station_Id>
            <Processing_Station_Id>#{@processingid}</Processing_Station_Id>
            <Sensor_Mode>#{@sensormode}</Sensor_Mode>
            <Phase></Phase>
            <Qualified>#{@qualified}</Qualified>
            <Band>#{@band}</Band>
            <Satellite_Id>#{@sateliteid}</Satellite_Id>
            <Ascending_Flag></Ascending_Flag>
            <Equator_X_Longitude></Equator_X_Longitude>
            <Equator_X_Time>#{@origindate}</Equator_X_Time>
            <Order_Id>#{@orderid}</Order_Id>
            <Mission>#{@mission}</Mission>
            <Relation>#{@relation}</Relation>
            <RelationType>#{@relationtype}</RelationType>
            <List_Of_Gaps count="0">
            </List_Of_Gaps>
            <List_Of_MDSs count="0">
            </List_Of_MDSs>
</Inventory_Metadata>
END_OF_STRING
  end

  
  def getMDTfilename(filename,manage_extention)
    if manage_extention == 't' # PostgreSQL return a string 't' for a true bulean
      return filename.split('.')[-2]
    end
    return filename
  end
  
  def writeFile(input_file)
    preinventory_file_name = input_file + ".MTD"
    File.open(preinventory_file_name,"w") do |file|
      file.write(@preinventory_body)
    end
    #Save List file
    list_name = Process.pid.to_s + ".LIST"
    File.open(list_name,"w") do |file|
      file.puts(input_file)
    end
  end
  def bounding_times(time)
    if (time == "UTC=0000-00-00T00:00:00")
      return "UTC=2000-01-01T00:00:00"
    elsif (time =~ /UTC=9999-99-99T99:99:99/)
      return "UTC=2100-01-01T00:00:00"
    else
      return time
    end
  end

end
