/*

	Copyright 2020, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: SRVDownloadInventory $
	

*/

#ifndef _sdiV_H_
#define _sdiV_H_

namespace acs {

// support class for versioning

class sdiV {
public:
	sdiV() throw() ;
	~sdiV() throw() ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _sdiV_H_

