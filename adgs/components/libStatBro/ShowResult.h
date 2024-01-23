// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 5.1  2015/06/12 15:01:40  marpas
	APF-269 work in progress
	
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
	
	Revision 2.1  2010/04/26 10:26:03  marpas
	preparation for ORACLE integration. (need new libDbGeo ORACLE aware)
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2004/12/06 14:24:07  marpas
	headers fixed
	
	Revision 1.11  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.10  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.9  2004/01/23 15:20:52  paoscu
	insertHeader added.
	
	Revision 1.8  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.7  2003/10/07 16:39:47  paoscu
	init method added.
	
	Revision 1.6  2003/09/26 10:08:43  paoscu
	setQueryClass method added.
	
	Revision 1.5  2003/04/17 14:09:15  marpas
	GCC3.2
	
	Revision 1.4  2003/03/07 10:23:35  marpas
	ShowResult rebirth
	
	Revision 1.1  2003/03/07 10:06:44  paoscu
	ShowResult moved from libStatBro
	
	Revision 1.2  2003/02/26 13:47:33  marpas
	stable
	
	Revision 1.1.1.1  2003/02/14 10:16:00  marpas
	Import libStatBro
	
	

*/

#ifndef _ShowResult_H_
#define _ShowResult_H_

#include <acs_c++config.hpp>
#include <DateTime.h>
#include <exException.h>
#include <exStream.h>

#include <string>
#include <vector>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;

class dbSet ;

class ShowResult // PRQA S 2109
{
public:

	/*! class InvalidUse declaration */
	exDECLARE_EXCEPTION(InvalidUse,exException) ; // PRQA S 2131, 2502

    ShowResult();
    virtual ~ShowResult() throw() ;

    enum XmlDateFormat {
        DateWUtc, // utc=YYYY-MM-DDThh:mm:ss.uuuuuu
        PlainDateToSecZ // YYYY-MM-DDThh:mm:ssZ
    } ;
public:
	virtual void clear() = 0 ;
	virtual void addResult(dbSet &) = 0 ;
	virtual void getResult(std::string &) = 0 ;
	virtual void showResult(const std::string &) ;
	virtual void setOutTitle(const std::string &) = 0 ;
	virtual void insertHeader(const DateTime & validityStart ,  const DateTime & validityStop) =0 ;
	virtual void setSubTitle(const std::string &) = 0 ;

	virtual void setQueryClass(const std::string &) ;
	virtual const std::string getQueryClass() const ;
	
	virtual void setNumShowQueries(int) ;
	void addShowRecords(size_t r) throw() { _numOfShowRecords+= r ; } // PRQA S 3084
    size_t getShowRecords() const throw() { return _numOfShowRecords ; }
	int getNumShowQueries() const throw() { return _numOfShowQueries ; }
	
	virtual void init(rsResourceSet &) ;
	void parse(rsResourceSet &, const std::string &) ;
    void setDateFormat(XmlDateFormat) throw() ;
    void setDateFormat(const std::string &) ;
    XmlDateFormat getDateFormat() const throw() ;

protected:
	static std::string getNthElement(unsigned long i,std::vector<std::string> const &elements) ;
	const std::vector<std::string> &getLabels(size_t i) ;
	const std::vector<std::string> &getAttributes(size_t i) ;
	const std::string &getSubRecordLabel(size_t i) ;
private:
	// functions declared but not implemented
	ShowResult &operator=(const ShowResult &) ;
	ShowResult (const ShowResult &) ;
	
	void parseSet(rsResourceSet &, size_t ) ;

private:
	std::map<size_t,std::vector<std::string> > _labels ;
	std::map<size_t,std::vector<std::string> > _attributes ;
	std::map<size_t,std::string> _subRecords ;
	std::string _queryClass ;
    XmlDateFormat _dateFormat ;
    int _numOfShowQueries ;
    size_t _numOfShowRecords ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ShowResult) ;

};

std::ostream &operator << (std::ostream &, ShowResult::XmlDateFormat) ;
exostream &operator << (exostream &, ShowResult::XmlDateFormat) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif // _ShowResult_H_
