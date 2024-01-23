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

	$Prod: A.C.S. fileOperation01 Handler - HEADER FILE $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/20 16:43:54  marpas
	adoption of libException 5.x in progress
	
	Revision 2.4  2012/11/29 19:31:30  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.3  2012/02/07 17:57:44  marpas
	refactoring in progress
	
	Revision 2.2  2009/05/14 14:00:10  enrcar
	EC:: c++ rules
	
	Revision 2.1  2008/06/04 15:11:24  enrcar
	EC:: fileOperation01, fileOperation01Factory added
	

*/


#ifndef _fileOperation01_H_
#define _fileOperation01_H_

#include <acs_c++config.hpp>
#include <Operation01.h>
#include <exMacros.h>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class fileOperation01
//
//
//
class fileOperation01: public Operation01   // PRQA S 2109
{
public:

	fileOperation01(const std::string &feature,
				  const std::string &instance) ; // max 8 chars
	~fileOperation01() override = default ;

	fileOperation01(const fileOperation01 &) = default ;
	fileOperation01 &operator=(const fileOperation01 &) = default ;

	// returns how much data have been set
	virtual size_t get(void *, size_t bufSize, std::string &, const std::vector<std::string> &mods) const ;
	
	virtual void set(const void *, size_t size, const std::string &) const ;
	
	virtual bool remove(const std::string &mod) const ; // PRQA S 2502

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(fileOperation01) ;
} ;


_ACS_END_NAMESPACE
 
#endif	/* _fileOperation01_H_ */

