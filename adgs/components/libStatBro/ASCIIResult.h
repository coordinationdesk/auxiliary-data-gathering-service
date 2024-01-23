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

	$Prod: A.C.S. Stat Bro Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.2  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.1  2010/04/26 10:26:02  marpas
	preparation for ORACLE integration. (need new libDbGeo ORACLE aware)
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2004/12/06 14:24:07  marpas
	headers fixed
	
	Revision 1.8  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.7  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.6  2004/01/23 15:20:52  paoscu
	insertHeader added.
	
	Revision 1.5  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.4  2003/09/26 10:08:43  paoscu
	setQueryClass method added.
	
	Revision 1.3  2003/04/30 16:55:05  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/03/26 16:30:37  paoscu
	working path configurable
	
	Revision 1.1  2003/02/26 13:47:33  marpas
	stable
	
	
	

*/

#ifndef _ASCIIResult_H_
#define _ASCIIResult_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <ShowResult.h>
#include <ftmpstream.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbSet ;

class ASCIIResult : public ShowResult // PRQA S 2109
{
public:
    explicit ASCIIResult(const std::string &workingPath="/tmp/");
    virtual ~ASCIIResult() throw() ;

public:
	// inherited from ShowResult
	virtual void clear() ;
	virtual void addResult(dbSet &) ;
	virtual void getResult(std::string &);
	virtual void setOutTitle(const std::string &);
	virtual void insertHeader(const DateTime & validityStart ,  const DateTime & validityStop) ;
	virtual void setSubTitle(const std::string &);

private:
	// functions declared but not implemented
	ASCIIResult &operator=(const ASCIIResult &) ;
	ASCIIResult (const ASCIIResult &) ;

private:
	oftmpstream _theStream ;
	std::string _workingPath;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ASCIIResult) ;
};

_ACS_END_NAMESPACE

#endif // _ASCIIResult_H_
