// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Version File $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 08:31:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/02/19 13:26:07  enrcar
	Modified for SGI compatibility
	
	Revision 1.1  2003/07/18 09:01:47  enrcar
	Add versioning files
	
	

*/

#ifndef _crcV_H_
#define _crcV_H_

#ifdef __cplusplus

namespace acs {

/* support class for versioning */

class crcV { // PRQA S 2109
public:
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }

private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif

#ifdef __cplusplus
extern "C" {
#endif

	const char *crcVbindTag() ;
	const char *crcVbindName() ;
#ifdef __cplusplus
}
#endif

#endif /* _crcV_H_ */

