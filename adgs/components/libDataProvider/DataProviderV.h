/*
 * 
 * 	Copyright 1995-2010, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: libDataProvider $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 *	$Log$
 *	Revision 5.0  2013/06/06 18:51:19  marpas
 *	adopting libException 5.x standards
 *	
 *	Revision 1.1  2010/10/28 15:37:35  marpas
 *	version file name (and class) changed
 *	
 *
 */

 
#ifndef _DataProviderV_H_
#define _DataProviderV_H_

namespace acs {

// support class for versioning

class DataProviderV {
public:
	DataProviderV() {}
	~DataProviderV() {}
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	DataProviderV(const DataProviderV &);
	DataProviderV &operator=(const DataProviderV &) ;
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _DataProviderV_H_



