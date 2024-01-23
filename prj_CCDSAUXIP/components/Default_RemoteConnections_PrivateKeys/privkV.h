/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Default RemoteConnections_PrivateKeys Header $

	$Id$

	$Author$

	$Log$
		

*/
 
#ifndef _privkV_H_
#define _privkV_H_

namespace acs {

// support class for versioning

class privkV {
public:
	privkV() = default ;
	~privkV() = default ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _privkV_H_



