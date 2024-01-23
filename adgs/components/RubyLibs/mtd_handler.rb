# frozen_string_literal: true

# Copyright 2021-2023, Exprivia SpA.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Exprivia SpA.
#
# $Prod: RubyLibs $

require 'date'
require 'rexml/document'

# Handle the generation of a MTD file
module MTDHandler
  # Tags for the Wrapper class
  module MTDTags
    WRAPPER_BASE_TAG = 'Inventory_Metadata'
    FILE_NAME_TAG = 'File_Name'
    FILE_VERSION_TAG = 'File_Version'
    UNIQUE_ID_TAG = 'Unique_Id'
    LOGICAL_ID_TAG = 'Logical_Id'
    PARENT_ID_TAG = 'Parent_Id'
    QUALITY_VALUE_TAG = 'Quality_Value'
    INVENTORY_EXTRA_TAG = 'Inventory_Extra'
    SESSION_ID_TAG = 'Session_Id'
    GRANULE_NUMBER_TAG = 'Granule_Number'
    GRANULE_POSITION_TAG = 'Granule_Position'
    DUMP_START_TAG = 'Dump_Start'
    MISSION_DATA_TAKE_ID_TAG = 'Mission_Data_Take_Id'
    GROUP_ID_TAG = 'Group_Id'
    SYSTEM_TAG = 'System'
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
    GEO_TYPE_LINESEG = 'Lineseg'
    GEO_TYPE_POLYGON = 'Polygon'
    TIME_RANGE_TAG = 'Time_Range'
    START_TIME_TAG = 'Start_Time'
    STOP_TIME_TAG = 'Stop_Time'
    LIST_OF_GEO_PNT_TAG = 'List_Of_Geo_Pnt'
    LIST_OF_GEO_PNT_COUNT_ATTR = 'count'
    GEO_PNT_TAG = 'Geo_Pnt'
    LATITUDE_TAG = 'Latitude'
    LONGITUDE_TAG = 'Longitude'
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
    QUALIFIED_TAG = 'Qualified'
    BAND_TAG = 'Band'
    TDI_TAG = 'Tdi'
    CLOUD_PERCENTAGE_TAG = 'CloudPercentage'
    PRODUCT_SPECIFICATION_TAG = 'ProductSpecification'
    BASELINE_TAG = 'Baseline'
    RELATION_TAG = 'Relation'
    RELATION_TYPE_TAG = 'RelationType'
    METADATA_VERSION_TAG = 'Metadata_Version'
    MD5_TAG = 'Md5'
  end

  # Automatically create the MTD structure
  module MTDComposer
    include MTDTags

    # Handle a generic MTD entry
    class MTDEntry
      attr_reader :tag, :func, :mandatory, :nullable, :nested

      # Bind TAG name, access function and options
      def initialize(tag, func, mandatory: false, nullable: false, nested: false)
        @tag = tag
        @func = func
        @mandatory = mandatory
        @nullable = nullable
        @nested = nested
      end

      # Add this element to the final xml
      def add_to_mtd(final_xml, wrapper)
        val = wrapper.send(@func)

        raise "Mandatory tag '#{@tag}' not set" if val.nil? && @mandatory

        set_element(final_xml.root.add_element(@tag), val) unless val.nil? || (empty_string?(val) && !@mandatory)
      end

      private

      # Set the element value
      def set_element(element, val)
        if @nested
          val.each { |v| element.add_element(v) }
        else
          raise "Not nullable tag '#{@tag}' is empty" if empty_string?(val) && !@nullable

          element.text = val unless empty_string?(val)
        end
      end

      # Check if the value is an empty String
      def empty_string?(val)
        val.is_a?(String) && val.empty?
      end
    end

    # Bind TAG name, access function and options
    TAG_MAP = [
      MTDEntry.new(FILE_NAME_TAG, 'file_name', mandatory: true),
      MTDEntry.new(FILE_VERSION_TAG, 'file_version', mandatory: true),
      MTDEntry.new(UNIQUE_ID_TAG, 'unique_id'),
      MTDEntry.new(LOGICAL_ID_TAG, 'logical_id'),
      MTDEntry.new(PARENT_ID_TAG, 'parent_id'),
      MTDEntry.new(QUALITY_VALUE_TAG, 'quality_value'),
      MTDEntry.new(INVENTORY_EXTRA_TAG, 'inventory_extra'),
      MTDEntry.new(SESSION_ID_TAG, 'session_id'),
      MTDEntry.new(GRANULE_NUMBER_TAG, 'granule_number'),
      MTDEntry.new(GRANULE_POSITION_TAG, 'granule_position'),
      MTDEntry.new(DUMP_START_TAG, 'dump_start'),
      MTDEntry.new(MISSION_DATA_TAKE_ID_TAG, 'mission_data_take_id'),
      MTDEntry.new(GROUP_ID_TAG, 'group_id'),
      MTDEntry.new(SYSTEM_TAG, 'system'),
      MTDEntry.new(SOURCE_TAG, 'source', mandatory: true, nullable: true),
      MTDEntry.new(SOURCE_SW_VERSION_TAG, 'source_sw_version', mandatory: true, nullable: true),
      MTDEntry.new(GENERATION_TIME_TAG, 'generation_time', mandatory: true),
      MTDEntry.new(VALIDITY_START_TAG, 'validity_start', mandatory: true),
      MTDEntry.new(VALIDITY_STOP_TAG, 'validity_stop', mandatory: true),
      MTDEntry.new(START_ORBIT_NUMBER_TAG, 'start_orbit_number', mandatory: true, nullable: true),
      MTDEntry.new(STOP_ORBIT_NUMBER_TAG, 'stop_orbit_number', mandatory: true, nullable: true),
      MTDEntry.new(GEOGRAPHIC_LOCALIZATION_TAG, 'geographic_localization', mandatory: true, nested: true),
      MTDEntry.new(QUALITY_INFO_TAG, 'quality_info', mandatory: true, nullable: true),
      MTDEntry.new(VALIDITY_FLAG_TAG, 'validity_flag', mandatory: true, nullable: true),
      MTDEntry.new(VALIDATION_DATE_TAG, 'validation_date', mandatory: true, nullable: true),
      MTDEntry.new(HEADER_SIZE_TAG, 'header_size', mandatory: true, nullable: true),
      MTDEntry.new(DATA_SIZE_TAG, 'data_size', mandatory: true, nullable: true),
      MTDEntry.new(FILE_TYPE_TAG, 'file_type', mandatory: true),
      MTDEntry.new(FILE_CLASS_TAG, 'file_class', mandatory: true),
      MTDEntry.new(SENSOR_ID_TAG, 'sensor_id', mandatory: true, nullable: true),
      MTDEntry.new(ACQUISITION_STATION_ID_TAG, 'acquisition_station_id', mandatory: true, nullable: true),
      MTDEntry.new(PROCESSING_STATION_ID_TAG, 'processing_station_id', mandatory: true, nullable: true),
      MTDEntry.new(SENSOR_MODE_TAG, 'sensor_mode', mandatory: true, nullable: true),
      MTDEntry.new(PHASE_TAG, 'phase', mandatory: true, nullable: true),
      MTDEntry.new(SATELLITE_ID_TAG, 'satellite_id', mandatory: true, nullable: true),
      MTDEntry.new(ASCENDING_FLAG_TAG, 'ascending_flag', mandatory: true, nullable: true),
      MTDEntry.new(EQUATOR_X_LONGITUDE_TAG, 'equator_x_longitude', mandatory: true, nullable: true),
      MTDEntry.new(EQUATOR_X_TIME_TAG, 'equator_x_time', mandatory: true, nullable: true),
      MTDEntry.new(ORDER_ID_TAG, 'order_id', mandatory: true, nullable: true),
      MTDEntry.new(MISSION_TAG, 'mission', mandatory: true, nullable: true),
      MTDEntry.new(QUALIFIED_TAG, 'qualified'),
      MTDEntry.new(BAND_TAG, 'band'),
      MTDEntry.new(TDI_TAG, 'tdi'),
      MTDEntry.new(CLOUD_PERCENTAGE_TAG, 'cloud_percentage'),
      MTDEntry.new(PRODUCT_SPECIFICATION_TAG, 'product_specification'),
      MTDEntry.new(BASELINE_TAG, 'baseline'),
      MTDEntry.new(RELATION_TAG, 'relation'),
      MTDEntry.new(RELATION_TYPE_TAG, 'relation_type'),
      MTDEntry.new(METADATA_VERSION_TAG, 'metadata_version'),
      MTDEntry.new(MD5_TAG, 'md5')
    ].freeze
  end

  # Base module for the MTD Handlers
  module Wrapper
    attr_accessor :file_name, :file_version, :unique_id, :logical_id, :parent_id, :quality_value, :inventory_extra,
                  :session_id, :granule_number, :granule_position, :mission_data_take_id, :group_id, :system, :source,
                  :source_sw_version, :start_orbit_number, :quality_info, :validity_flag, :header_size, :data_size,
                  :file_type, :file_class, :sensor_id, :acquisition_station_id, :processing_station_id, :sensor_mode,
                  :phase, :satellite_id, :ascending_flag, :equator_x_longitude, :order_id, :mission, :qualified, :band,
                  :tdi, :cloud_percentage, :product_specification, :baseline, :relation, :relation_type,
                  :metadata_version, :md5

    attr_writer :dump_start, :generation_time, :validity_start, :validity_stop, :stop_orbit_number,
                :geographic_localization, :validation_date, :equator_x_time

    include REXML
    include MTDComposer

    # Define all the mandatory field that are also nullable to a empty string
    def empty_nullable_mandatory
      TAG_MAP.each { |el| send("#{el.func}=", '') if el.mandatory && el.nullable }
    end

    # Create the final MTD
    def generate_mtd(mtd_path)
      final_xml = Document.new
      final_xml.add_element(WRAPPER_BASE_TAG)
      TAG_MAP.each { |el| el.add_to_mtd(final_xml, self) }
      f = Formatters::Pretty.new(4)
      f.compact = true
      f.width = 999_999_999_999
      File.open(mtd_path, File::WRONLY | File::CREAT) { |file| f.write(final_xml.root, file) }
    end

    # Get the value of dump_start correctly formatted
    def dump_start
      convert_date(@dump_start)
    end

    # Get the value of generation_time correctly formatted
    def generation_time
      convert_date(@generation_time)
    end

    # Get the value of validity_start correctly formatted
    def validity_start
      convert_date(@validity_start)
    end

    # Get the value of validity_stop correctly formatted
    def validity_stop
      convert_date(@validity_stop)
    end

    # Get the default stop_orbit_number (= start_orbit_number) if not set
    def stop_orbit_number
      @stop_orbit_number.nil? ? @start_orbit_number : @stop_orbit_number
    end

    # Create the geographic_localization tag
    def geographic_localization
      num_of_point = @geographic_localization.nil? ? 0 : @geographic_localization.count
      geo = Element.new(GEO_TYPE_TAG)
      geo.text = GEO_TYPE_POINT
      time = Element.new(TIME_RANGE_TAG)
      start = time.add_element(START_TIME_TAG)
      stop = time.add_element(STOP_TIME_TAG)
      list = Element.new(LIST_OF_GEO_PNT_TAG)
      list.attributes[LIST_OF_GEO_PNT_COUNT_ATTR] = num_of_point
      set_geo_point(geo, start, stop, list, num_of_point) if num_of_point > 0

      [geo, time, list]
    end

    # Get the value of validation_date correctly formatted
    def validation_date
      convert_date(@validation_date)
    end

    # equator_x_time converted to standard time format
    def equator_x_time
      convert_date(@equator_x_time)
    end

    private

    # Set the geographic_localization points
    def set_geo_point(geo, start, stop, list, num_of_point)
      geo.text = GEO_TYPE_LINESEG if num_of_point == 2
      geo.text = GEO_TYPE_POLYGON if num_of_point > 3
      start.text = validity_start
      stop.text = validity_stop

      @geographic_localization.each do |coord|
        point = list.add_element(GEO_PNT_TAG)
        point.add_element(LATITUDE_TAG).text = coord[0]
        point.add_element(LONGITUDE_TAG).text = coord[1]
      end
    end

    # Convert a DateTime object in a correctly formatted string
    def convert_date(date)
      return date if date.nil? || date == ''

      date.new_offset(0).strftime('UTC=%FT%T.%6N')
    end
  end
end
