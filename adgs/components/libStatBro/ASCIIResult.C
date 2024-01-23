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
	Revision 2.8  2013/04/12 12:20:07  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.7  2013/03/26 17:59:05  marpas
	heavy refactoring, breaking comaptibility with Qt applications and StatisticalReportGenerator in favour f StatBroEngine
	
	Revision 2.6  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.5  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.4  2010/04/26 10:26:02  marpas
	preparation for ORACLE integration. (need new libDbGeo ORACLE aware)
	
	Revision 2.3  2008/10/08 15:52:49  crivig
	porting to postgres8
	
	Revision 2.2  2006/04/12 17:52:17  paoscu
	addResult method fixed,
	
	Revision 2.1  2006/03/09 16:37:50  marpas
	new libDbClient.dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.16  2005/10/21 18:57:32  marpas
	DBEXPRESS compatibility
	
	Revision 1.15  2005/10/20 11:53:20  marpas
	DBEXPRESS compatibility - temporary patch for dbGeoObject
	
	Revision 1.14  2005/09/20 13:44:51  marpas
	dependencies on OO Informix types removed
	
	Revision 1.13  2005/09/06 15:53:48  marpas
	new dbRow::getColumnType method used
	
	Revision 1.12  2004/12/06 14:24:07  marpas
	headers fixed
	
	Revision 1.11  2004/10/07 16:01:30  marpas
	multiset queries patched
	
	Revision 1.10  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.9  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.8  2004/04/26 17:26:03  paoscu
	insertHeader implemented
	
	Revision 1.7  2004/01/23 15:20:52  paoscu
	insertHeader added.
	
	Revision 1.6  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.5  2003/09/26 10:08:43  paoscu
	setQueryClass method added.
	
	Revision 1.4  2003/04/30 16:55:05  marpas
	using namespace std no longer into acs includes
	
	Revision 1.3  2003/04/17 14:09:15  marpas
	GCC3.2
	
	Revision 1.2  2003/03/26 16:30:37  paoscu
	working path configurable
	
	Revision 1.1  2003/02/26 13:47:33  marpas
	stable
	
	
	

*/
#include <ASCIIResult.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbGeoObject.h>
#include <Filterables.h>
#include <Application.h>
#include <dbConnectionPool.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ASCIIResult)

ASCIIResult::ASCIIResult(const string &workingPath) :
	ShowResult(),
	_theStream("/tmp/stat","txt"),
	_workingPath(workingPath)
 
{
}


ASCIIResult::~ASCIIResult() throw() {}

void ASCIIResult::clear()
{
	_theStream.close() ;
	_theStream.open(_workingPath+"/stat","txt") ;
}

void ASCIIResult::addResult(dbSet &set)
{
    string connKey = "" ;
    if (Application::instance() && Application::instance()->getDefaultConnectionPool()) {
        connKey = Application::instance()->getDefaultConnectionPool()->getKey() ;
    }
	dbRow	row(connKey) ;
	bool title = false ;
	
	while (set.getNextRow(row))
	{
		uint16_t ncol = row.columns() ;

		if (!title)
		{	
			for (uint16_t j = 0; j < ncol; ++j)
			{
				if (j != 0 ) { // PRQA S 3000
					_theStream() << "," ;
                }

				string colName = getNthElement(j,getLabels(0)) ;
				if(colName.empty()) {
					colName = row.columnName(j) ;
                }

				_theStream() << colName ;
			}
			_theStream() << "\n" ;
			
			title = true ;
		}
		for (uint16_t j = 0; j < ncol; ++j)
		{
			if (j != 0) { // PRQA S 3000
				_theStream() << "," ;
            }

			string value ;
			try {
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
					}
					break ;
				default: 
                    {
					    value = row.column(j,value) ;
					    break ;
                    }
				}
			} catch (dbRow::NullValueException &e) {
				value = "null" ;	
			}
			_theStream() << value ; 
		}
		_theStream() << "\n" ;
	}
}


void ASCIIResult::getResult(string &result)
{
	_theStream.close() ;
	ifstream in(_theStream.path().c_str()) ;
	ostringstream out ;
	int c = 0 ;
	while ((c = in.get()) != EOF) { // PRQA S 4244
		out.put(c) ; // PRQA S 3010
    }
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Writing: /tmp/ASCIIResult.txt file") ;
		ifstream ind(_theStream.path().c_str()) ;
		ofstream outd("/tmp/ASCIIResult.txt") ; ;
		int cd = 0 ;
		while ((cd = ind.get()) != EOF) { // PRQA S 4244
			outd.put(cd) ;	// PRQA S 3010
        }	
	ACS_CLASS_END_DEBUG
	
	result = out.str() ;
}


void ASCIIResult::setOutTitle(const string &descr)
{
	_theStream() << descr << "\n" ;
}


void  ASCIIResult::insertHeader(const DateTime & validityStart ,  const DateTime & validityStop)
{
	DateTime currentTime ;
	string header = "Created on ";
    string tmpString ;
	header += currentTime.utcs(tmpString) ;
	header += " Validity Start=" + validityStart.utcs(tmpString) ;
	header += " Validity Stop="  +validityStop.utcs(tmpString) ;

	_theStream() << header << "\n" ;
}




void ASCIIResult::setSubTitle(const string &subt)
{
	_theStream() << subt << "\n" ;
}


_ACS_END_NAMESPACE
