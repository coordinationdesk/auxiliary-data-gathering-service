/*

	Copyright (c) 2021 EXPRIVIA
	License: Proprietary

    ADGS RemoteConnections_PrivateKeys Header version file
*/

#ifndef _privkV_H_
#define _privkV_H_

namespace acs {

// support class for versioning

class privkV {
public:
	privkV() ;
	~privkV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _privkV_H_



