// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S. Data Consumer Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.0  2013/06/06 20:55:20  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1.1.1  2010/11/04 10:18:56  micmaz
 *  create a new library
 *
 *
 *
 */

#ifndef _DataConsumerV_H_
#define _DataConsumerV_H_

namespace acs {
namespace dc{

// support class for versioning

class DataConsumerV { // PRQA S 2109
public:
	DataConsumerV() ;
	~DataConsumerV() throw() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	DataConsumerV(const DataConsumerV &) ;
	const DataConsumerV &operator=(const DataConsumerV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;
} // Close namespace
} // Close namespace

#endif // _DataConsumerV_H_

