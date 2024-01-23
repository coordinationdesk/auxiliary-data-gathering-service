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
	Revision 5.0  2013/07/02 11:22:11  marpas
	adopting libException 5.x standards
	coding best practices
	qa warnings
	
	Revision 2.3  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.2  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.1  2012/02/13 17:04:33  marpas
	removing compiler warnings
	
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

#include <ParamParser.h>
#include <RegExp.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ParamParser)
ParamParser::ParamParser(const char delim) :
	_delimiter(delim),
    _regExpr()
{
	delimiter(_delimiter) ;
}


ParamParser::~ParamParser() throw() 
{

}


char ParamParser::delimiter() const throw() // PRQA S 4120
{
	return _delimiter ;
}


void ParamParser::delimiter(const char d)
{
	_delimiter = d ;
	_regExpr = _delimiter ;
	_regExpr += "[A-Za-z0-9_]*" ;
	_regExpr += _delimiter ;
}


const string &ParamParser::regExpr() const throw()  // PRQA S 4120
{
	return _regExpr ;
}


void ParamParser::regExpr(const string &r)  // PRQA S 4121
{
	_regExpr = r ;
}

string ParamParser::getVariable(string &str, int num) const // PRQA S 4214
{
	RegExp rg(_regExpr) ;

    regoff_t start = 0 ;
    regoff_t end = 0 ;
    regoff_t pos = 0 ;

	for (int i = 0; i < num; i++)
	{
		// serch the 'i' occurence of the regular expression
		//start = rg.match(str.c_str()+pos,start,end) ;
		//.index(rg, &size, pos) ;

		if (!rg.match(str.c_str()+pos,start,end))
		{
			ACS_THROW(NoParamFound("No more parameter")) ; // PRQA S 3081
		}
		
		// move current position after the first occurrence
		pos += start + 1 ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "start " << start << ": " << str.c_str()+pos) ;
	}

	
	// The size of the pattern is equal to the size of variable plus
	// the delimiter; remove the delimiter from the size
	regoff_t size = end - start - 2 ;

	// The first character starting from this index is the delimiter;
	// remove it to return the variable without start delimiter
	start++ ;

	// Extract the variable name
	return string(str,pos,size) ; // PRQA S 3000
}


string &ParamParser::subVariable(string &str, const string &var, const string &val) const // PRQA S 4214
{
	string substr ;
	substr += _delimiter ;
	substr += var ;
	substr += _delimiter ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "searching " << substr << " in \"" << str << "\"") ;
	RegExp rg(substr) ;

	regoff_t start = 0;
    regoff_t end = 0 ;

	if (!rg.match(str.c_str(),start,end))
	{
		ACS_THROW(NoParamFound("Variable not found")) ; // PRQA S 3081
	}

	str.erase(start, end-start) ; // PRQA S 3000
	
	str.insert(start, val) ; // PRQA S 3000

	return str ; // PRQA S 4028
}


_ACS_END_NAMESPACE
