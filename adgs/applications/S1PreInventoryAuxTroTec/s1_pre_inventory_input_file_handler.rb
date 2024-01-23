# frozen_string_literal: true

# Copyright 2021-2021, Exprivia SpA.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com

# All Rights Reserved.

# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Exprivia SpA.

# $Prod: S1PreInventoryAuxTroTec $

require 'date'
require 'rexml/document'

# require 'common_utils'

# Collector for all S2PreInventoryAux components
module S2PreInventoryAuxComponents
  # Tags for the GenericInputWrapper class
  module GenericInputWrapperTags
    WRAPPER_BASE_TAG = 'Inventory_Metadata'
    FILE_NAME_TAG = 'File_Name'
    FILE_VERSION_TAG = 'File_Version'
    SOURCE_TAG = 'Source'
    SOURCE_SW_VERSION_TAG = 'Source_Sw_Version'
    GENERATION_TIME_TAG = 'Generation_Time'
    VALIDITY_START_TAG = 'Validity_Start'
    VALIDITY_STOP_TAG = 'Validity_Stop'
    START_ORBIT_NUMBER_TAG = 'Start_Orbit_Number'
    STOP_ORBIT_NUMBER_TAG = 'Stop_Orbit_Number'
    GEOGRAPHIC_LOCALIZATION_TAG = 'Geographic_Localization'
    GEO_TYPE_TAG = 'Geo_Type'
    GEO_TYPE_POINT = 'Point'
    TIME_RANGE_TAG = 'Time_Range'
    START_TIME_TAG = 'Start_Time'
    STOP_TIME_TAG = 'Stop_Time'
    LIST_OF_GEO_PNT_TAG = 'List_Of_Geo_Pnt'
    LIST_OF_GEO_PNT_COUNT_ATTR = 'count'
    QUALITY_INFO_TAG = 'Quality_Info'
    VALIDITY_FLAG_TAG = 'Validity_Flag'
    VALIDATION_DATE_TAG = 'Validation_Date'
    HEADER_SIZE_TAG = 'Header_Size'
    DATA_SIZE_TAG = 'Data_Size'
    FILE_TYPE_TAG = 'File_Type'
    FILE_CLASS_TAG = 'File_Class'
    SENSOR_ID_TAG = 'Sensor_Id'
    ACQUISITION_STATION_ID_TAG = 'Acquisition_Station_Id'
    PROCESSING_STATION_ID_TAG = 'Processing_Station_Id'
    SENSOR_MODE_TAG = 'Sensor_Mode'
    PHASE_TAG = 'Phase'
    SATELLITE_ID_TAG = 'Satellite_Id'
    ASCENDING_FLAG_TAG = 'Ascending_Flag'
    EQUATOR_X_LONGITUDE_TAG = 'Equator_X_Longitude'
    EQUATOR_X_TIME_TAG = 'Equator_X_Time'
    ORDER_ID_TAG = 'Order_Id'
    MISSION_TAG = 'Mission'
    SESSION_ID_TAG = 'Session_Id'
    GRANULE_NUMBER_TAG = 'Granule_Number'
    GRANULE_POSITION_TAG = 'Granule_Position'
    DUMP_START_TAG = 'Dump_Start'
    TAG_MAP = [
      { tag: FILE_NAME_TAG, func: 'file_name', mandatory: true, nested: false },
      { tag: FILE_VERSION_TAG, func: 'file_version', mandatory: true, nested: false },
      { tag: SOURCE_TAG, func: 'source', mandatory: true, nested: false },
      { tag: SOURCE_SW_VERSION_TAG, func: 'source_sw_version', mandatory: true, nested: false },
      { tag: GENERATION_TIME_TAG, func: 'generation_time', mandatory: true, nested: false },
      { tag: VALIDITY_START_TAG, func: 'validity_start', mandatory: true, nested: false },
      { tag: VALIDITY_STOP_TAG, func: 'validity_stop', mandatory: true, nested: false },
      { tag: START_ORBIT_NUMBER_TAG, func: 'start_orbit_number', mandatory: true, nested: false },
      { tag: STOP_ORBIT_NUMBER_TAG, func: 'stop_orbit_number', mandatory: true, nested: false },
      { tag: GEOGRAPHIC_LOCALIZATION_TAG, func: 'geographic_localization', mandatory: true, nested: true },
      { tag: QUALITY_INFO_TAG, func: 'quality_info', mandatory: true, nested: false },
      { tag: VALIDITY_FLAG_TAG, func: 'validity_flag', mandatory: true, nested: false },
      { tag: VALIDATION_DATE_TAG, func: 'validation_date', mandatory: true, nested: false },
      { tag: HEADER_SIZE_TAG, func: 'header_size', mandatory: true, nested: false },
      { tag: DATA_SIZE_TAG, func: 'data_size', mandatory: true, nested: false },
      { tag: FILE_TYPE_TAG, func: 'file_type', mandatory: true, nested: false },
      { tag: FILE_CLASS_TAG, func: 'file_class', mandatory: true, nested: false },
      { tag: SENSOR_ID_TAG, func: 'sensor_id', mandatory: true, nested: false },
      { tag: ACQUISITION_STATION_ID_TAG, func: 'acquisition_station_id', mandatory: true, nested: false },
      { tag: PROCESSING_STATION_ID_TAG, func: 'processing_station_id', mandatory: true, nested: false },
      { tag: SENSOR_MODE_TAG, func: 'sensor_mode', mandatory: true, nested: false },
      { tag: PHASE_TAG, func: 'phase', mandatory: true, nested: false },
      { tag: SATELLITE_ID_TAG, func: 'satellite_id', mandatory: true, nested: false },
      { tag: ASCENDING_FLAG_TAG, func: 'ascending_flag', mandatory: true, nested: false },
      { tag: EQUATOR_X_LONGITUDE_TAG, func: 'equator_x_longitude', mandatory: true, nested: false },
      { tag: EQUATOR_X_TIME_TAG, func: 'equator_x_time', mandatory: true, nested: false },
      { tag: ORDER_ID_TAG, func: 'order_id', mandatory: true, nested: false },
      { tag: MISSION_TAG, func: 'mission', mandatory: true, nested: false },
      { tag: SESSION_ID_TAG, func: 'session_id', mandatory: false, nested: false },
      { tag: GRANULE_NUMBER_TAG, func: 'granule_number', mandatory: false, nested: false },
      { tag: GRANULE_POSITION_TAG, func: 'granule_position', mandatory: false, nested: false },
      { tag: DUMP_START_TAG, func: 'dump_start', mandatory: false, nested: false }
    ].freeze

    # Generate all elements of the final MTD file
    def generate_elements(final_xml)
      TAG_MAP.each do |el|
        val = send(el[:func])
        raise "Mandatory tag '#{el[:tag]}' not set" if val.nil? && el[:mandatory]

        add_tag(final_xml, el[:tag], val, el[:nested]) unless val.nil?
      end
    end

    # Add a single pair tag/value to the final XML
    def add_tag(final_xml, tag, val, nested)
      element = final_xml.root.add_element(tag)
      if nested
        val.each { |v| element.add_element(v) }
      else
        element.text = val
      end
    end
  end

  # Defaults values for all Input File Handlers
  module GenericInputWrapperDefaults
    include REXML
    include GenericInputWrapperTags

    # validation_date always not set, return the default value
    def validation_date
      DateTime.now.new_offset(0).strftime('UTC=%FT%T.%6N')
    end

    # geographic_localization always not set, return the default value
    def geographic_localization
      geo = Element.new(GEO_TYPE_TAG)
      geo.text = GEO_TYPE_POINT
      time = Element.new(TIME_RANGE_TAG)
      time.add_element(START_TIME_TAG)
      time.add_element(STOP_TIME_TAG)
      list = Element.new(LIST_OF_GEO_PNT_TAG)
      list.attributes[LIST_OF_GEO_PNT_COUNT_ATTR] = 0

      [geo, time, list]
    end

    private

    # Set the fixed defaults values for the mandatory fields
    def mandatory_fixed_defaults
      @file_version = '1.0'
      @source_sw_version = '1.0'
      @validity_flag = 'TRUE'
      @header_size = 0
      @file_class = 'OPER'
      @satellite_id = 34 # => S1_ in t_satellites
      @mission = 1
    end

    # Set an empty value for the mandatory fields
    def mandatory_empty_defaults
      @start_orbit_number = @stop_orbit_number = @quality_info = @sensor_id = @acquisition_station_id = ''
      @processing_station_id = @sensor_mode = @phase = @ascending_flag = @equator_x_longitude = @order_id = ''
      @session_id = @granule_number = @granule_position = @dump_start = ''
    end
  end

  # Base module for all the Input File Handlers
  module GenericInputWrapper
    attr_reader :file_name, :file_version, :source, :source_sw_version, :start_orbit_number, :stop_orbit_number,
                :quality_info, :validity_flag, :header_size, :data_size, :file_type, :file_class, :sensor_id,
                :acquisition_station_id, :processing_station_id, :sensor_mode, :phase, :satellite_id, :ascending_flag,
                :equator_x_longitude, :order_id, :mission, :session_id, :granule_number, :granule_position, :dump_start

    include REXML
    include GenericInputWrapperDefaults

    # Initialize all the defaults values
    def initialize_variables(file, file_type)
      check_type(file_type, 'FileType', [String])

      @file_name = File.basename(file)
      @data_size = file.size
      @file_type = file_type
      @equator_x_time = DateTime.parse(file.mtime.to_s)

      mandatory_fixed_defaults
      mandatory_empty_defaults
    end

    # Create the final MTD
    def generate_mtd(mtd_path)
      final_xml = Document.new
      final_xml.add_element(WRAPPER_BASE_TAG)
      generate_elements(final_xml)
      f = Formatters::Pretty.new(4)
      f.compact = true
      f.width = 999_999_999_999
      File.open(mtd_path, File::WRONLY | File::CREAT) { |file| f.write(final_xml.root, file) }
    end

    # Get the value of generation_time correctly formatted
    def generation_time
      @generation_time.new_offset(0).strftime('UTC=%FT%T.%6N')
    end

    # Get the value of validity_start correctly formatted
    def validity_start
      @validity_start.new_offset(0).strftime('UTC=%FT%T.%6N')
    end

    # Get the value of validity_stop correctly formatted, or the default value if not provided
    def validity_stop
      @validity_stop.new_offset(0).strftime('UTC=%FT%T.%6N')
    end

    # equator_x_time converted to standard time format
    def equator_x_time
      @equator_x_time.new_offset(0).strftime('UTC=%FT%T.%6N')
    end
  end
end
