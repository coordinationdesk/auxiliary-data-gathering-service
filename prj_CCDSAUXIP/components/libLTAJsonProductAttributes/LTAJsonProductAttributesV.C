#include <auto_version_file>
#include <LTAJsonProductAttributesV.h>

using namespace acs ;

const char* LTAJsonProductAttributesV::_name = auto_version_file::name;
const char* LTAJsonProductAttributesV::_tag = auto_version_file::tag;


LTAJsonProductAttributesV::LTAJsonProductAttributesV() throw() {}
LTAJsonProductAttributesV::~LTAJsonProductAttributesV() throw() {}

const char* LTAJsonProductAttributesV::tag() throw() { return _tag ; }
const char* LTAJsonProductAttributesV::name() throw() { return _name ; }
