#ifndef _LTAJsonProductAttributesV_H_
#define _LTAJsonProductAttributesV_H_

namespace acs {

// support class for versioning

class LTAJsonProductAttributesV {
public:
	LTAJsonProductAttributesV() throw() ;
	~LTAJsonProductAttributesV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _LTAJsonProductAttributesV_H_
