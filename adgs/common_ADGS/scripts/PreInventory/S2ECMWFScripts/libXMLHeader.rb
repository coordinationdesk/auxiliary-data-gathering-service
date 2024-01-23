require 'rubygems'
require 'nokogiri'

class LibXMLHeader
  def initialize()
  end

  def find_file_class(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/File_Class").text.strip
  end

  def find_file_type(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/File_Type").text.strip
  end

  def find_validity_start(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/Validity_Period/Validity_Start").text.strip
  end

  def find_validity_stop(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/Validity_Period/Validity_Stop").text.strip
  end

  def find_file_version(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/File_Version").text.strip
  end

  def find_system(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/Source/System").text.strip
  end

  def find_generation_time(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/Source/Creation_Date").text.strip
  end

  def find_source(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/Source/Creator").text.strip
  end

  def find_source_sw_version(doc)
    doc.at_xpath("//Earth_Explorer_Header/Fixed_Header/Source/Creator_Version").text.strip
  end

  def find_product_version (name)
    return name[-4,4]
  end
end
