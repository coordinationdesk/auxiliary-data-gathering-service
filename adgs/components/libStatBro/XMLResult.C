// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 5.2  2015/06/12 15:01:40  marpas
	APF-269 work in progress
	
	Revision 5.1  2013/07/12 10:32:55  marpas
	null value management explicit
	
	Revision 5.0  2013/07/02 11:22:11  marpas
	adopting libException 5.x standards
	coding best practices
	qa warnings
	
	Revision 2.9  2013/04/12 12:20:07  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.8  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.7  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.6  2012/02/13 17:04:33  marpas
	removing compiler warnings
	
	Revision 2.5  2010/04/26 10:26:03  marpas
	preparation for ORACLE integration. (need new libDbGeo ORACLE aware)
	
	Revision 2.4  2008/10/08 15:52:49  crivig
	porting to postgres8
	
	Revision 2.3  2006/06/22 14:39:10  marpas
	temporary version, aligned with new libDateTime I/F
	
	Revision 2.2  2006/04/12 17:52:17  paoscu
	addResult method fixed,
	
	Revision 2.1  2006/03/09 16:37:50  marpas
	new libDbClient.dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.24  2005/10/21 18:57:32  marpas
	DBEXPRESS compatibility
	
	Revision 1.23  2005/10/20 11:53:20  marpas
	DBEXPRESS compatibility - temporary patch for dbGeoObject
	
	Revision 1.22  2005/09/20 13:44:51  marpas
	dependencies on OO Informix types removed
	
	Revision 1.21  2005/09/06 15:53:48  marpas
	new dbRow::getColumnType method used
	
	Revision 1.20  2005/09/05 17:15:12  marpas
	string error fixed
	
	Revision 1.19  2004/12/07 20:17:20  marpas
	Fix for QueryResult tag missing in StatBro reports
	
	Revision 1.18  2004/12/06 14:23:50  marpas
	debug improved
	
	Revision 1.17  2004/10/15 16:54:03  marpas
	label IgnoredColumn supported to ignore a column an force MULTISET not empty
	trailing and leading spaces from labels stripped away
	
	Revision 1.16  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.15  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.14  2004/05/07 16:11:33  marpas
	Fixed header support implemented
	
	Revision 1.13  2004/01/23 15:20:05  paoscu
	insertHeader implemented.
	Diffrent behaviour for clear() method if XML stream is from outside.
	
	Revision 1.12  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.11  2003/10/23 17:20:01  paoscu
	New header style.
	
	Revision 1.10  2003/10/23 17:18:51  paoscu
	Now it's possible to avoid parameters replacement.
	
	Revision 1.9  2003/09/29 17:42:28  paoscu
	Subtitle tag moved outside results vector.
	
	Revision 1.8  2003/09/26 10:27:02  paoscu
	setQueryClass method added.
	queryClass is used as an attribute in "query" tag.
	
	Revision 1.7  2003/07/11 21:07:53  marpas
	exException::clone implemented
	
	Revision 1.6  2003/04/30 16:55:05  marpas
	using namespace std no longer into acs includes
	
	Revision 1.5  2003/04/17 14:09:15  marpas
	GCC3.2
	
	Revision 1.4  2003/03/26 16:30:37  paoscu
	working path configurable
	
	Revision 1.3  2003/03/11 13:43:23  marpas
	some string::lenght changed in string::empty
	
	Revision 1.2  2003/02/26 13:48:23  marpas
	Very Small Bug Fixed
	
	Revision 1.1  2003/02/26 13:47:33  marpas
	stable
	

*/



#include <XMLResult.h>
#include <XMLOstream.h>
#include <Application.h>


#include <dbConnectionPool.h>

#include <dbRow.h>
#include <dbSet.h>
#include <Filterables.h>

#include <dbGeoObject.h>
#include <sstream>




_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    string nameDate(DateTime const &d)
    {
	    //Datetime exported in format CCSDS compact.
	    ostringstream os;
        DateTime::UTCD utcd = d ; // PRQA S 3050
	    os
	       <<"UTC=" 
	       << setfill('0')
	       << setw(4) << utcd.year << "-" // PRQA S 4400 6
	       << setw(2) << utcd.month << "-" 
	       << setw(2) << utcd.day <<"T"
	       << setw(2) << utcd.hour << ":" 
	       << setw(2) << utcd.min << ":" 
	       << setw(2) << utcd.sec;

	    return os.str();

    }

}

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLResult)

XMLResult::XMLResult(const string &workingPath ) :
	ShowResult(),
    _theStream(),
	_theXMLStream(0) ,
    _tagStack(),
    _subtitle(),
	_externalXml(false),
	_workingPath(workingPath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	XMLResult::clear() ;
}

XMLResult::XMLResult(XMLOstream & xml) :
	ShowResult(),
    _theStream(),
	_theXMLStream(&xml) ,
    _tagStack(),
    _subtitle(),
	_externalXml(true) ,
	_workingPath()

{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	//In this case "QueryResult" is not the root tag but just a simple one 
	_tagStack.push(new XMLOstream::Tag(*_theXMLStream, "QueryResult") ) ;
}

XMLResult::~XMLResult() throw() 
{
    // PRQA S 4631 L1
    try {
	    clearAndDestroy() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}

void XMLResult::clearAndDestroy()
{
	_subtitle = "" ;
	if (_theXMLStream)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "deleting _tagStack. ") ;

		while (_tagStack.size() > 0) {
			delete _tagStack.top() ;
			_tagStack.pop() ;
		}
		
		if(!_externalXml){
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "deleting _theXMLStream. ") ;
			delete _theXMLStream ;
			_theXMLStream = 0 ;
		}
	}
}


void XMLResult::clear() // PRQA S 4020
{
	//NOTE if _externalXml is not possible to clear anytjing since XML already contains
	//something else
	
	if(_externalXml) {
		return ;
    }
		
	clearAndDestroy() ;

	_theStream.close() ;
	_theStream.open(_workingPath+"/stat","txt") ;

	_theXMLStream = new XMLOstream(_theStream(),"QueryResult") ;
}


void XMLResult::addResult(dbSet &set)
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	string queryClassAtt;
	if(!getQueryClass().empty()) {
		queryClassAtt = "queryClass=\"" + getQueryClass() + "\"";
    }
	XMLOstream::Tag qtag(*_theXMLStream, "query" , "" , queryClassAtt) ; 

	if (!_subtitle.empty()) {	
		XMLOstream::Tag subtag(*_theXMLStream, "Subtitle",_subtitle, "", true) ;
		_subtitle="" ;
	}

    string connKey = "" ;
    if (Application::instance() && Application::instance()->getDefaultConnectionPool()) {
        connKey = Application::instance()->getDefaultConnectionPool()->getKey() ;
    }
	ostringstream oss ;
	oss << "count=\"" << set.size() << "\"" ;
	XMLOstream::Tag ltag(*_theXMLStream, "List_of_records","", oss.str()) ; 
	dbRow		row(connKey) ;
	
	while (set.getNextRow(row))
	{
		uint16_t ncol = row.columns() ;
		XMLOstream::Tag tag(*_theXMLStream, "record") ; 

		for (uint16_t j = 0; j < ncol; ++j)
		{
			string value ;
			string name ;
			string attribute ;

			try {
				name=getNthElement(j,getLabels(0)) ;
				attribute=getNthElement(j,getAttributes(0)) ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "j="<<j<<" getNthResult gave \""<<name<<"\".") ;
				if(name.empty()) {
					name = row.columnName(j) ;
                }
				
				switch(row.getColumnType(j)) {
				case dbSpecificColumnType::BLOB:
                    {
					    value = "<blob data>" ;
					    break ;
                    }
				case dbSpecificColumnType::GEOOBJECT:
					{
						dbGeoObject *geoObj = 0 ;
						try {

							string geo ;
							row.column(j,geo) ;
							geoObj = dbGeoObject::newDbGeo(geo) ;

							ostringstream os ;
							geoObj->getSqlString(os,SQLDialectFactory::dialectName(connKey)) ;
							value = os.str() ;
							delete geoObj ;
						}
						catch (...) {
							delete geoObj ;
							throw ;
						}
					    break ;
					}
				case dbSpecificColumnType::DATETIME:
					{
                        switch(getDateFormat()) {
                        case DateWUtc:
                            {
						        DateTime dt;
						        row.column(j,dt) ;
						        ostringstream os;
						        os << "UTC=" ;
						        dt.asISO8601(os,6) ; // PRQA S 4400
						        value =os.str() ;
                                break ;
                            }
                        case PlainDateToSecZ:
                            {
						        DateTime dt;
						        row.column(j,dt) ;
						        ostringstream os;
						        dt.asISO8601(os,0) ;
                                os << "Z" ;
						        value =os.str() ;
                                break ;
                            }
                        default:
                            {
                                ACS_THROW(InvalidUse("invalid date format: " + getDateFormat())) ; // PRQA S 3081
					        }
                            break ;
                        }
					    break ;
                    }

				default:
                    {
                        bool isNull = false ;
					    value = row.column(j,value,&isNull) ;
                        if (isNull) {
                            value = "null" ;
                        }
					    break ;
                    }
				}
			
			} catch (dbRow::NullValueException &e) {
				value = "null" ;	
			}
			XMLOstream::Tag tag1(*_theXMLStream, name,value, attribute, true) ; 
		}
	}
}


void XMLResult::getResult(string &result)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	ACS_COND_THROW(_externalXml,InvalidUse("Cannot use XMLResult::getResult if _externalXml==true")); // PRQA S 3081
	clearAndDestroy() ;
	_theStream.close() ;
	ifstream in(_theStream.path().c_str()) ;
	ostringstream out ;
	int c = 0;
	while ((c = in.get()) != EOF)  { // PRQA S 4244
		out.put(c) ; // PRQA S 3010
    }		
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Writing: ./XMLResult.txt file") ;
		ifstream ind(_theStream.path().c_str()) ;
		ofstream outd("./XMLResult.txt") ; ;
		int cd = 0;
		while ((cd = ind.get()) != EOF) { // PRQA S 4244
			outd.put(cd) ;    // PRQA S 3010	
        }
	ACS_CLASS_END_DEBUG
	
	result = out.str() ;
}


void XMLResult::setOutTitle(const string &descr)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "title: \"" << descr << "\"") ;
	XMLOstream::Tag tag(*_theXMLStream, "Title",descr, "", true) ;
}


void  XMLResult::insertHeader(const DateTime & validityStart ,  const DateTime & validityStop)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	string header;
	header+="Created on ";
	header+=nameDate(DateTime());
	header+=" Validity Start="+nameDate(validityStart);
	header+=" Validity Stop="+nameDate(validityStop);
	XMLOstream::Tag tag(*_theXMLStream, "Header",header, "", true) ;
}


void XMLResult::setSubTitle(const string &subt)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "subt: \"" << subt << "\"") ;
	_subtitle = subt ;
}

void XMLResult::setNumShowQueries(int num) 
{
    ShowResult::setNumShowQueries(num) ;
    
	ostringstream oss ;
	oss << "count=\"" << num << "\"" ;
	_tagStack.push(new XMLOstream::Tag(*_theXMLStream, "List_of_queries","",oss.str())) ;
}


_ACS_END_NAMESPACE
