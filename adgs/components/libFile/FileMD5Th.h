// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/20 16:43:53  marpas
	adoption of libException 5.x in progress
	
	Revision 2.4  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.3  2009/06/26 10:42:32  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/13 15:26:45  marpas
	some rule violations fixed
	
	Revision 2.1  2007/04/13 17:17:51  marpas
	new class to compute md5 in a separate thread
	
	
*/
 
#ifndef _FileMD5Th_H_
#define _FileMD5Th_H_


#include <acs_c++config.hpp>

#include <Thread.h>


_ACS_BEGIN_NAMESPACE(acs)



/*! \class FileMD5Th
	
	Utility class to support dir convenience functions.
	
	virtual inheritance from Thred is harmless
*/
class FileMD5Th : public Thread // PRQA S 2109
{
public:
	FileMD5Th(const std::string &, size_t safeThr) ;
	virtual ~FileMD5Th() throw() ;
	
	virtual void run() ;
	std::string getResult() ;
	
private:
	FileMD5Th() ;
	FileMD5Th(const FileMD5Th &) ;
	FileMD5Th &operator=(const FileMD5Th &) ;

private:
	std::string _path ;
	size_t _threshold ;
	std::string _result ;
	bool _end ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FileMD5Th) ;
} ;
_ACS_END_NAMESPACE

#endif	/* _FileMD5Th_H_ */
