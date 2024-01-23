// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
 * 	$Prod$
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 2.1  2012/02/09 17:54:40  marpas
 * 	refactoring in progress
 * 	
 * 	Revision 2.0  2006/02/28 15:51:48  paoscu
 * 	Exception 2.1 I/F adopted
 * 	
 * 	Revision 1.3  2004/12/01 15:05:56  enrcar
 * 	New header
 * 	
 * 	Revision 1.2  2003/01/31 15:25:53  enrcar
 * 	*** empty log message ***
 * 	
 * 	Revision 1.1  2003/01/31 15:24:14  enrcar
 * 	Import
 * 	
 * 	Revision 1.1.1.1  2003/01/16 11:53:12  crivig
 * 	coding...
 * 	
 */


#ifndef _PackagerOptimiserAppV_H_
#define _PackagerOptimiserAppV_H_

namespace acs {

// support class for versioning

class PackagerOptimiserAppV { // PRQA S 2109
public:
	PackagerOptimiserAppV() throw() ;
	~PackagerOptimiserAppV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	PackagerOptimiserAppV(const PackagerOptimiserAppV &) ; // not implemented 
	PackagerOptimiserAppV& operator= (const PackagerOptimiserAppV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _PackagerOptimiserAppV_H_



