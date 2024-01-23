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
	Revision 2.6  2013/11/22 20:18:32  marpas
	coding best practices applied
	
	Revision 2.5  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.4  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.3  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.2  2006/05/19 18:58:48  marpas
	non  blocking i/o - SpawnProc used
	
	Revision 2.1  2006/05/18 15:11:46  marpas
	ftailstream class family added (implemented only for input - iftailstream)
	

*/

#ifndef _ftailstream_H_
#define _ftailstream_H_

#include <SpawnProc.h>
#include <exException.h>

#include <fstream>
#include <sstream>
#include <cstdio>


_ACS_BEGIN_NAMESPACE(acs)

/*! \class iftailstream
	
	Utility class to support creation of temporary output files
*/
class iftailstream // PRQA S 2109
{
public:

	static const bool defaultCerrOnCout = true ;

	explicit iftailstream(const std::string &, bool cerrOnCout = defaultCerrOnCout) ;
	iftailstream() ;

	~iftailstream() ;
	
	void close() throw() ;  // PRQA S 2502
	void open(const std::string &, bool cerrOnCout = defaultCerrOnCout) ;
	bool hasData() const ;
	std::string &getline(std::string &) ; // PRQA S 2502
	
private:
	// declared but not defined
	iftailstream(const iftailstream &) ;
	iftailstream &operator=(const iftailstream &) ;
    
    // reset the filedescriptors
    void resetFDs() throw();
	
private:
	
	std::string _path ;
	bool _cerrOnCout ;
	std::istringstream *_inb ;
	int _filedes[2] ;   // PRQA S 4403
	SpawnProc *_proc ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL (iftailstream);
} ;

_ACS_END_NAMESPACE

#endif	
