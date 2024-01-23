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

# :nocov:
# Add the lib main to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'json'
require 'rexml/document'

require 'common_utils'

# Container for JobOrder handler methods
module JobOrder
  ##################################
  # CONFIGURATION EXTENSION MODULE #
  ##################################

  # Handle the Configuration part of the JobOrder
  module JobOrderConfigurationHandler
    CONF_TAG = 'Ipf_Conf'
    PROCESSOR_NAME_TAG = 'Processor_Name'
    LIST_DYN_PROC_PAR_TAG_S1 = 'List_of_Dynamic_Processing_Parameters|Dynamic_Processing_Parameters'
    DYN_PROC_PAR_TAG_S1 = 'Dynamic_Processing_Parameter|Processing_Parameter'
    LIST_DYN_PROC_PAR_TAG_S3 = 'List_of_Dyn_Processing_Parameters'
    DYN_PROC_PAR_TAG_S3 = 'Dyn_Processing_Parameter'
    LIST_DYN_PROC_PAR_COUNT_ATTR = 'count'
    DYN_PROC_PAR_NAME_TAG = 'Name'
    DYN_PROC_PAR_VALUE_TAG = 'Value'

    # Initialize the conf variable
    def initialize_conf(job_doc, mission)
      mission = mission.upcase.to_sym if mission.is_a?(String)
      raise ArgumentError, "Argument 'mission' must be 'S1' or 'S3'" unless %i[S1 S3].include?(mission)

      @conf = job_doc.elements[CONF_TAG]
      @mission = mission
    end

    # Return the Processor_Name value
    def processor_name
      @conf.elements[PROCESSOR_NAME_TAG].text
    end

    # Return the list of Dynamic_Processing_Parameters as a map of the type 'name' => 'value' where 'name' is a symbol
    def dynamic_processing_parameters(mission: @mission)
      list, element_tag = dynamic_parsing_parameters_mission_switch(mission)

      return nil unless list

      hash = {}
      list.each_element(element_tag) do |param|
        name = param.elements[DYN_PROC_PAR_NAME_TAG].text.to_sym
        raise "Duplicated '#{param.elements[DYN_PROC_PAR_NAME_TAG]}' for '#{element_tag}' tags" if hash.key?(name)

        hash[name] = param.elements[DYN_PROC_PAR_VALUE_TAG].text
      end
      hash
    end

    # Return the specified of Dynamic_Processing_Parameters value
    def dynamic_processing_parameter(ref, mission: @mission)
      list, element_tag = dynamic_parsing_parameters_mission_switch(mission)

      case ref
      when String, Symbol
        dynamic_processing_parameter_by_name(ref, list, element_tag)
      when Integer
        dynamic_processing_parameter_by_index(ref, list)
      else
        raise TypeError, "Argument 'ref' must be a String, a Symbol or a Integer"
      end
    end

    private

    # Get the dynamic_parsing_parameters element based on the mission
    def dynamic_parsing_parameters_mission_switch(mission)
      mission = mission.upcase.to_sym if mission.is_a?(String)
      raise ArgumentError, "Argument 'mission' must be 'S1' or 'S3'" unless %i[S1 S3].include?(mission)

      if mission == :S3
        list = @conf.elements[LIST_DYN_PROC_PAR_TAG_S3]
        element_tag = DYN_PROC_PAR_TAG_S3
      else
        list = @conf.elements[LIST_DYN_PROC_PAR_TAG_S1]
        element_tag = DYN_PROC_PAR_TAG_S1
      end

      [list, element_tag]
    end

    # Get the chosen element by name
    def dynamic_processing_parameter_by_name(name, list, element_tag)
      return nil unless list

      name = name.to_s
      list.each_element(element_tag) do |param|
        return param.elements[DYN_PROC_PAR_VALUE_TAG].text if name == param.elements[DYN_PROC_PAR_NAME_TAG].text
      end
      nil
    end

    # Get the chosen element (by index)
    def dynamic_processing_parameter_by_index(index, list)
      return nil unless list

      count = list.attributes[LIST_DYN_PROC_PAR_COUNT_ATTR].to_i
      return nil if count == 0

      raise ArgumentError, "Argument 'index' must be in [1, #{count}" unless index.between?(1, count)

      list.elements[index].elements[DYN_PROC_PAR_VALUE_TAG].text
    end
  end

  ##############################
  # IPF PROCS EXTENSION MODULE #
  ##############################

  # Constants for JobOrderIpfProcsHandler
  module JobOrderIpfProcsHandlerConstants
    LIST_IPF_PROCS_TAG = 'List_of_Ipf_Procs'
    LIST_IPF_PROCS_COUNT_ATTR = 'count'
    IPF_PROC_ELEM_TAG = 'Ipf_Proc'
    IPF_PROC_ELEM_NAME_TAG = 'Task_Name'
    LIST_INPUTS_TAG = 'List_of_Inputs'
    LIST_INPUTS_COUNT_ATTR = 'count'
    INPUT_TAG = 'Input'
    FILE_TYPE_TAG = 'File_Type'
    FILE_NAME_TYPE_TAG = 'File_Name_Type'
    LIST_FILE_NAMES_TAG = 'List_of_File_Names'
    LIST_FILE_NAMES_COUNT_ATTR = 'count'
    FILE_NAME_TAG = 'File_Name'
  end

  # Handle the Ipf Procs part of the JobOrder
  module JobOrderIpfProcsHandler
    include JobOrderIpfProcsHandlerConstants

    # Save the list of processors
    def initialize_ipf_procs(job_doc)
      @ipf_procs_list = job_doc.elements[LIST_IPF_PROCS_TAG]
    end

    # Get the number of processors
    def ipf_procs_num
      @ipf_procs_list.attributes[LIST_IPF_PROCS_COUNT_ATTR].to_i
    end

    # Return the list of processors as a map of the type 'name' => 'element' where 'name' is a symbol
    def ipf_procs
      hash = {}
      @ipf_procs_list.each_element(IPF_PROC_ELEM_TAG) do |elem|
        name = elem.elements[IPF_PROC_ELEM_NAME_TAG].text.to_sym
        raise "Duplicated '#{elem.elements[IPF_PROC_ELEM_NAME_TAG]}' for '#{IPF_PROC_ELEM_TAG}' tags" if hash.key?(name)

        hash[name] = elem
      end
      hash
    end

    # Return the chosen Ipf_Proc
    def ipf_proc(ipf_proc_ref)
      return ipf_proc_by_name(ipf_proc_ref) if ipf_proc_ref.is_a?(String) || ipf_proc_ref.is_a?(Symbol)
      return ipf_proc_by_index(ipf_proc_ref) if ipf_proc_ref.is_a?(Integer)

      raise TypeError, "Argument 'ipf_proc_ref' must be a String, a Symbol or a Integer"
    end

    # Get the number of Inputs for the specified Ipf_Proc
    def inputs_num(ipf_proc_ref)
      ipf_proc(ipf_proc_ref).elements[LIST_INPUTS_TAG].attributes[LIST_INPUTS_COUNT_ATTR].to_i
    end

    # Return the list of Inputs of the given Ipf_Proc as a map of the type 'type' => 'element' where 'type' is a symbol
    def inputs(ipf_proc_ref)
      ipf_proc_elem = ipf_proc(ipf_proc_ref)
      return nil unless ipf_proc_elem

      hash = {}
      ipf_proc_elem.elements[LIST_INPUTS_TAG].each_element(INPUT_TAG) do |input_elem|
        type = input_elem.elements[FILE_TYPE_TAG].text.to_sym
        raise "Duplicated '#{input_elem.elements[FILE_TYPE_TAG]}' for '#{INPUT_TAG}' tags" if hash.key?(type)

        hash[type] = input_elem
      end
      hash
    end

    # Return the chosen Input of the given Ipf_Proc
    def input(ipf_proc_ref, input_ref)
      ipf_proc_elem = ipf_proc(ipf_proc_ref)
      return nil unless ipf_proc_elem

      return input_by_type(ipf_proc_elem, input_ref) if input_ref.is_a?(String) || input_ref.is_a?(Symbol)
      return input_by_index(ipf_proc_elem, input_ref) if input_ref.is_a?(Integer)

      raise TypeError, "Argument 'input_ref' must be a String, a Symbol or a Integer"
    end

    # Get the files type for the specified Ipf_Proc and Input
    def files_type(ipf_proc_ref, input_ref)
      input_elem = input(ipf_proc_ref, input_ref)

      return nil unless input_elem

      input_elem.elements[FILE_TYPE_TAG].text
    end

    # Get the files type for the specified Ipf_Proc and Input
    def files_names_type(ipf_proc_ref, input_ref)
      input_elem = input(ipf_proc_ref, input_ref)

      return nil unless input_elem

      input_elem.elements[FILE_NAME_TYPE_TAG].text
    end

    # Get the number of files in the list of File Names for the specified Ipf_Proc and Input
    def files_num(ipf_proc_ref, input_ref)
      input_elem = input(ipf_proc_ref, input_ref)

      return nil unless input_elem

      input_elem.elements[LIST_FILE_NAMES_TAG].attributes[LIST_FILE_NAMES_COUNT_ATTR].to_i
    end

    # Return the list of File Names
    def file_names(ipf_proc_ref, input_ref)
      input_elem = input(ipf_proc_ref, input_ref)

      return nil unless input_elem

      list = []
      input_elem.elements[LIST_FILE_NAMES_TAG].each_element(FILE_NAME_TAG) { |file_name| list.push(file_name.text) }
      list
    end

    # Return the chosen File Name
    def file_name(ipf_proc_ref, input_ref, index)
      check_type(index, 'index', [Integer])

      input_elem = input(ipf_proc_ref, input_ref)

      return nil unless input_elem

      list = input_elem.elements[LIST_FILE_NAMES_TAG]
      count = list.attributes[LIST_FILE_NAMES_COUNT_ATTR].to_i
      return nil if count == 0

      raise ArgumentError, "Argument 'index' must be in [1, #{count}]" unless index.between?(1, count)

      list.elements[index].text
    end

    private

    # Get the chosen Ipf_Proc (by name)
    def ipf_proc_by_name(ipf_proc_name)
      @ipf_procs_list.each_element(IPF_PROC_ELEM_TAG) do |ipf_proc_elem|
        return ipf_proc_elem if ipf_proc_name.to_s == ipf_proc_elem.elements[IPF_PROC_ELEM_NAME_TAG].text
      end
      nil
    end

    # Get the chosen Ipf_Proc (by index)
    def ipf_proc_by_index(index)
      count = ipf_procs_num
      return nil if count == 0

      raise ArgumentError, "Argument 'index' must be in [1, #{count}]" unless index.between?(1, count)

      @ipf_procs_list.elements[index]
    end

    # Get the chosen Input (by type)
    def input_by_type(ipf_proc_elem, input_type)
      ipf_proc_elem.elements[LIST_INPUTS_TAG].each_element(INPUT_TAG) do |input_elem|
        return input_elem if input_type.to_s == input_elem.elements[FILE_TYPE_TAG].text
      end
      nil
    end

    # Get the chosen Input (by index)
    def input_by_index(ipf_proc_elem, index)
      list = ipf_proc_elem.elements[LIST_INPUTS_TAG]
      count = list.attributes[LIST_INPUTS_COUNT_ATTR].to_i
      return nil if count == 0

      raise ArgumentError, "Argument 'index' must be in [1, #{count}]" unless index.between?(1, count)

      list.elements[index]
    end
  end

  ##############
  # MAIN CLASS #
  ##############

  # Handle a given JobOrder
  class JobOrderHandler
    include REXML
    include JobOrderConfigurationHandler
    include JobOrderIpfProcsHandler

    BASE_TAG = 'Ipf_Job_Order'

    # Parse the JobOrder
    def initialize(job_order_file, mission: :S1)
      check_type(job_order_file, 'job_order_file', [String])
      raise IOError, "JobOrder file '#{job_order_file}' does not exists" unless File.exist?(job_order_file)

      job_doc = Document.new(File.new(job_order_file)).elements[BASE_TAG]

      initialize_conf(job_doc, mission)
      initialize_ipf_procs(job_doc)
    end
  end
end
