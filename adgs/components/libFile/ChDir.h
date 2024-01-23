// PRQA S 1050 EOF
/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 2.3  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.2  2012/02/13 10:59:04  marpas
	refactoring in progress
	
	Revision 2.1  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/30 22:06:41  marpas
	headers fixed
	
	Revision 1.2  2004/04/29 13:16:02  marpas
	getCwd method added
	
	Revision 1.1  2004/04/21 09:44:59  marpas
	new class added
	

*/
 
#ifndef _ChDir_H_
#define _ChDir_H_

#include <acs_c++config.hpp>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

/*! \class ChDir
	
	Utility class to change directory and return to the old one when destroyed
*/
class ChDir 
{
public:
	ChDir() ;
	explicit ChDir(const std::string &, bool autoReturn = true) ;
	ChDir(const ChDir &) ;
	ChDir &operator=(const ChDir &) ;
	~ChDir() throw() ;
	static std::string getCwd() ;
private:
	std::string _oldp ;
	mutable bool _autoReturn ;
} ;

_ACS_END_NAMESPACE



#endif /* _ChDir_H_ */
