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

	$Prod: A.C.S. StatBro Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/06 14:24:07  marpas
	headers fixed
	
	Revision 1.3  2003/07/11 21:07:53  marpas
	exException::clone implemented
	
	Revision 1.2  2003/04/30 15:51:13  marpas
	using namespace std no longer found into acs includes
	
	Revision 1.1.1.1  2003/02/14 10:16:00  marpas
	Import libStatBro
	
	

*/

#ifndef _ParamParser_H_
#define _ParamParser_H_

#include <acs_c++config.hpp>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class ParamParser // PRQA S 2109
{
public:
	/*! class NoParamFound declaration */
	exDECLARE_EXCEPTION(NoParamFound,exException) ; // PRQA S 2131, 2502

public:
    explicit ParamParser(const char delim = '%');
    ~ParamParser() throw() ;

public:
	char delimiter() const throw() ;
	void delimiter(const char) ;
	const std::string &regExpr() const throw() ;
	void regExpr(const std::string &) ;
	
	std::string getVariable(std::string &, int num = 1) const ;
	std::string &subVariable(std::string &, const std::string &, const std::string &) const ;

private:
	// functions declared but not implemented
	ParamParser &operator=(const ParamParser &) ;
	ParamParser (const ParamParser &) ;

private:
	char	_delimiter ;
	std::string	_regExpr ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ParamParser) ;
};


_ACS_END_NAMESPACE

#endif // _ParamParser_H_
