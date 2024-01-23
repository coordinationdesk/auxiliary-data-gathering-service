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

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2016/03/14 15:38:59  marpas
	qa warning fixed
	
	Revision 5.5  2015/04/22 08:14:47  enrcar
	Interface slightly modified to avoid regressions
	
	Revision 5.4  2015/04/21 15:05:33  enrcar
	dbQueryParamter now recursive
	
	Revision 5.3  2014/02/07 18:36:53  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/07/15 16:19:13  marpas
	deprecating methods having default db key string
	
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.10  2013/06/03 12:39:44  marpas
	coding best practices partially applied
	qa rules
	
	Revision 4.9  2013/05/29 10:53:08  marpas
	i/f rationalization
	
	Revision 4.8  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.7  2013/03/29 19:47:21  marpas
	getting rid of Html related functions and libraries
	
	Revision 4.6  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.5  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.4  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.6  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.5  2009/06/19 17:47:57  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.4  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.3  2008/11/04 13:42:10  marpas
	make up and useless stuff deletion
	
	Revision 2.2  2006/06/26 12:07:55  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/20 16:35:12  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2006/01/17 18:20:38  marpas
	Following methods added:
		ParamOperator getParamOperator() const ;
	void setParamOperator(ParamOperator) ;
	
	Revision 1.6  2006/01/13 18:05:35  paoscu
	Query parameters implementation completely changed.
	
	Revision 1.5  2005/10/24 16:24:44  marpas
	GEOOBJECT operators (dbParam ones) fixed
	
	Revision 1.4  2005/09/08 12:02:36  paoscu
	dbType class (from libDbBasic) removed. dbColumn::ColumnType used instead.
	
	Revision 1.26  2005/09/06 16:20:37  paoscu
	Method getColumnType now uses dbColumn::getColumnType .
	
	Revision 1.25  2005/07/13 14:04:24  marpas
	bug fixed when no verification on parameters
	
	Revision 1.24  2005/07/12 16:16:11  marpas
	introducing dbParam::ISNOTNULL
	dbParam::verifyDbValues flag to avoid checking into dbvalues when composing complex where clauses
	
	Revision 1.23  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.22  2004/09/08 08:44:33  ivafam
	Exception message modified
	
	Revision 1.21  2004/07/20 10:15:11  marpas
	IN added to dbParam
	some minor changes and fixes
	
	Revision 1.20  2004/05/03 16:13:28  paoscu
	More debug
	
	Revision 1.19  2004/04/14 15:39:00  marpas
	NameIdAssociator is a template
	
	Revision 1.18  2004/04/13 17:10:27  ivafam
	NameIdAssociator.h and NameIdAssociator.C contain only class NameIdAssociator
	
	Revision 1.17  2004/04/13 08:30:54  marpas
	documentation updated
	
	Revision 1.16  2003/09/03 13:55:59  paoscu
	Export methods are now const.
	
	Revision 1.15  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.14  2003/05/02 19:01:57  paoscu
	getParameter now is const
	
	Revision 1.13  2003/04/15 20:21:37  marpas
	GCC3.2
	
	Revision 1.12  2003/03/19 15:10:15  paoscu
	Avoid value check if operation=ISNULL
	
	Revision 1.11  2003/03/14 19:16:30  paoscu
	Master/Slave policy implemented
	
	Revision 1.10  2003/03/11 16:18:38  paoscu
	Arbiter almost complete
	
	Revision 1.9  2003/02/20 18:12:12  paoscu
	Easter is closer so ...
	
	Revision 1.8  2003/02/20 17:42:25  paoscu
	numerical check now works (integral/floating not literal)
	
	Revision 1.7  2003/02/19 20:54:10  paoscu
	Better debug.
	
	Revision 1.6  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.5  2003/01/31 15:07:36  paoscu
	IS_NULL operator added
	
	Revision 1.4  2002/12/16 11:29:39  paoscu
	Export and import methods modified.
	
	Revision 1.3  2002/12/04 16:09:20  paoscu
	Changed importObj and exportObj methods.
	
	Revision 1.2  2002/11/29 17:53:38  danalt
	added exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#include <dbQueryParameters.h>
#include <dbSchemaMetaDataProvider.h>

#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <StringUtils.h>
#include <Filterables.h>

#include <sstream>

#ifdef __linux__
  #if (__GNUC__ < 3)
	#include <algorithm>
  #endif
#endif

using namespace acs ;
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbQueryParameters) 

dbQueryParameters::dbQueryParameters() :
    _params(),
    _qparams(),
	_paramOperator(AND),
	_key()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


dbQueryParameters::dbQueryParameters(const string &key) :
    _params(),
    _qparams(),
	_paramOperator(AND),
	_key(key)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

dbQueryParameters::~dbQueryParameters() throw() {}

dbQueryParameters::dbQueryParameters(const dbQueryParameters &p) :
    _params(p._params),
    _qparams(p._qparams),
	_paramOperator(p._paramOperator),
	_key(p._key)
{
}

dbQueryParameters &dbQueryParameters::operator=(const dbQueryParameters &p)
{
	if(this != &p) 
	{
		_params = p._params ;
		_qparams = p._qparams ;
		_paramOperator = p._paramOperator ;
		_key = p._key ; 
		
	}
	return *this; 
}


void dbQueryParameters::resetParameters() 
{
	_params.clear() ; 
	_qparams.clear() ; 
	_paramOperator = AND ;
}

dbQueryParameters::ParamOperator dbQueryParameters::getParamOperator() const // PRQA S 4120
{
	return _paramOperator ;
}

void dbQueryParameters::setParamOperator(ParamOperator p) // PRQA S 4121
{
	_paramOperator = p ;
}



bool dbQueryParameters::has(const std::string & tableName , const std::string & fieldName) const // PRQA S 4020
{
	ACS_COND_THROW(not _qparams.empty(),InvalidValue("Method not implemented")) ; // PRQA S 3081

//W8115_OFF: multiple returns
	for (size_t i=0; i < _params.size(); i++) {
		if ((_params[i].tableName() == tableName) && 
            (_params[i].fieldName() == fieldName)) {
			return true ;
        }
    }
	return false ;
}



void dbQueryParameters::deleteParameter(const std::string & tableName , const std::string & fieldName) 
{
	ACS_COND_THROW(not _qparams.empty(),InvalidValue("Method not implemented")) ; // PRQA S 3081

	// we create a temporary dbParam object, to destroy all parameters 
	// contained in vector	
	while (has(tableName, fieldName))
	{
		for (vector<dbParam>::iterator i = _params.begin() ; i != _params.end(); ++i) { // PRQA S 4238
			if ((*i).tableName() == tableName && (*i).fieldName() == fieldName) {
				_params.erase(i) ; 
				break ;
			}
        }
	}
}


// Add a parameters.
// The order is given by CompareTo function
void dbQueryParameters::addParameter(const dbParam &par) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding parameter \""<< par.composeTableAndColumn() <<"\".") ;

	ACS_COND_THROW(not _qparams.empty(),InvalidValue("Unable to mix dbParam and dbQueryParameter in the same object")) ; // PRQA S 3081

	_params.push_back(par) ; 
}

void dbQueryParameters::addQueryParameter(const dbQueryParameters &par, dbConnection *conn) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding parameter \""<< par.getSqlString() <<"\".") ;

	ACS_COND_THROW(not _params.empty(),InvalidValue("Unable to mix dbParam and dbQueryParameter in the same object")) ; // PRQA S 3081

	_qparams.push_back(par) ; 
}

const dbParam &dbQueryParameters::getParameter(size_t pos) const
{ 
	
	if (pos >= _params.size()) // boundary check
	{
		ostringstream os  ;
		os << "Out of bound. Requested position is: " << pos << " allowed " << _params.size() ;
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081 
	}
	return _params[pos] ; 
}



const dbQueryParameters &dbQueryParameters::getQueryParameter(size_t pos) const
{ 
		
	if (pos >= _qparams.size()) // boundary check
	{
		ostringstream os  ;
		os << "Out of bound. Requested position is: " << pos << " allowed " << _qparams.size() ;
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081 
	}
	return _qparams[pos] ; 
}


// Output to stream
ostream &acs::operator<<(ostream &str, const dbQueryParameters &par)
{
	ACS_COND_THROW(not par._qparams.empty(),dbQueryParameters::InvalidValue("Method not implemented")) ; // PRQA S 3081

	for(unsigned long i=0 ; i < par. _params.size() ; i++)
	{
		str << par._params[i] ; 
		ACS_COND_THROW(0 != str.rdstate(),exIOException("Write error",errno)) ; // PRQA S 3081
	}
	return str ; // PRQA S 4028
}


exostream &acs::operator<<(exostream &str, const dbQueryParameters &par) // PRQA S 4222
{
	ACS_COND_THROW(not par._qparams.empty(),dbQueryParameters::InvalidValue("Method not implemented")) ; // PRQA S 3081

	try {
		for(unsigned long i=0 ; i < par. _params.size() ; i++) // each param
		{
			str << par._params[i] ; 
		}
	}
	catch(exception &x) {
		ACS_THROW(exIOException(x,"Query parameters write error",errno)) ;
	}
	return str ; // retrun the stream  // PRQA S 4028
}


// Input from stream
istream &acs::operator>>(istream &str, dbQueryParameters &par) // PRQA S 4020
{
// W8115_OFF multiple returns
	dbParam tmpParam(par._key) ;
		
	while (!str.eof()) // until eof
	{
		str >> tmpParam ; 
		if (str.rdstate() != 0) // some state notice ?
		{
			if (str.eof() || str.fail())
			{
				return str ; // nothing important ... go ahead // PRQA S 4028
			} 
			else // an error
			{
				ACS_THROW(exIOException("Read error",errno)) ;
			}
		}
		par.addParameter(tmpParam) ; 
	}
	return str ;  // PRQA S 4028
}



// Costruisce la parte di clausola WHERE riguardante i parametri inseriti.
// Raggruppa i parametri mettendo in OR quelli che hanno nome parametro 
// uguale

// WHERE clause creation
// Parameters are grouped (in OR) if have same name
string dbQueryParameters::getSqlString() const // PRQA S 4020, 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	string	oldStr; 
	string	sqlString(" ((") ; 
	
	if((_params.size() == 0) && (_qparams.size() == 0)){
		return "" ; 
    }

	if (not _params.empty())
	{
		// NOTE: Once there was a mechanism to have parameters in OR
		// in that case the same parameter name would have been used twice
		for(unsigned long i=0 ; i < _params.size() ; i++)
		{
			sqlString += _params[i].getSqlString() ;
			if( i < _params.size() - 1 ) { // PRQA S 3084
				sqlString += (_paramOperator == AND ) ? " ) AND ( " : " ) OR ( "  ; // PRQA S 3380
        	}
		}
	} else {
	
		// NOTE: Once there was a mechanism to have parameters in OR
		// in that case the same parameter name would have been used twice
        size_t qpsize = _qparams.size() ;
		for(unsigned long i=0 ; i < qpsize ; ++i)
		{
			sqlString += _qparams[i].getSqlString() ;
			if( i < qpsize - 1 ) { // PRQA S 3084
				sqlString += (_paramOperator == AND ) ? " ) AND ( " : " ) OR ( "  ; // PRQA S 3380
        	}
		}
	
	}
	
	sqlString += " ))" ; 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SQL string is \"" << sqlString <<"\".") ;

	
	return sqlString ; 
}




void dbQueryParameters::translate(const dbParam &par, string &description, string &expression) 
{
// W8111_OFF: complex method
	description = "" ; 
	expression = "" ; 
		
	switch(par.operation()) // which clause ? 
	{ // grouped by syntax form
	case dbParam::ISNULL:
	case dbParam::ISNOTNULL: // no arg clauses
    {
		// es. IS NOT NULL
		expression = dbParam::getOperatorName(par.getKey(), par.operation()) ; 
		break ; 				
    }
	case dbParam::NONE:
	case dbParam::EQUAL_TO:
	case dbParam::LIKE:
	case dbParam::MINOR_OF:
	case dbParam::MAJOR_OF:
	case dbParam::MAJOR_OR_EQUAL_OF:
	case dbParam::MINOR_OR_EQUAL_OF:
	case dbParam::DIFFERENT_FROM:
	case dbParam::INSIDE:			
	case dbParam::INTERSECT:
	case dbParam::OUTSIDE:	// single arg clauses
    {
		// es. EQUAL_TO <value> 

		expression = dbParam::getOperatorName(par.getKey(), par.operation()) ; 
		expression += par.value() ; 
		break ; 				
    }
	case dbParam::BETWEEN:
	case dbParam::NOT_BETWEEN: // double args clauses
    {
		expression += dbParam::getOperatorName(par.getKey(), par.operation()) ; 
		expression += par.value() ; 
		expression += " AND " ; 
		expression += par.value2() ;
		expression += " " ; 			
		break ; 	
    }
	case dbParam::IN: // mutiple args clauses
    {
		// es. IN (v1, v2 ...)
		expression += dbParam::getOperatorName(par.getKey(), par.operation()) ;
		{ 
			for (size_t i=0; i < par.valueIn().size(); i++) {
				if (i > 0) {
					expression += ", " ;
                }
				expression += par.valueIn()[i]->getValue() ;
			}
		}
		expression += ") " ;
		break ; 
    }
	default: // not recognized
    {
		ACS_THROW(InvalidName("Invalid Parameter:" + par.composeTableAndColumn())) ; // PRQA S 3081
	}
    }	
}		

void dbQueryParameters::exportObj(rsResourceSet &s) const 
{

	ACS_COND_THROW(not _qparams.empty(),InvalidValue("Method not implemented")) ; // PRQA S 3081

	rsPushNSpace rpns(s,"dbQueryParameters") ;
	s.setValue("_num",_params.size()) ;
	rsPushNSpace rpns_params(s,"_params") ; // dbQueryParameters._params.dbParam[0].name:value
	for(size_t i=0 ; i < _params.size() ; i++) // export each param
	{
		ostringstream os;
		os<<"dbParam["<<i<<"]";
		rsPushNSpace rpns_params1( s , os.str() ) ;
		_params[i].exportObj(s) ;
	}
}

		
void dbQueryParameters::importObj(rsResourceSet &s) 
{
	ACS_COND_THROW(not _qparams.empty(),InvalidValue("Method not implemented")) ; // PRQA S 3081

	int entries = 0 ;
	
	rsPushNSpace rpns(s,"dbQueryParameters") ;
	s.getValue("_num",entries) ;
	
	rsPushNSpace rpns_params(s,"_params") ;
 	// dbQueryParameters._params.dbParam[0].name:value
	for(int i=0 ; i < entries ; ++i) // each entry
	{
		ostringstream os;
		os<<"dbParam["<<i<<"]";
		rsPushNSpace rpns_params1( s , os.str() ) ;
		dbParam a(_key) ; 
		a.importObj(s) ;
		addParameter(a) ;
	}
}	


void dbQueryParameters::exportXML(XMLOstream &xst) const 
{
	ACS_COND_THROW(not _qparams.empty(),InvalidValue("Method not implemented")) ; // PRQA S 3081

	ostringstream os;
	os << _params.size();
	XMLOstream::Tag txml(xst, "dbQueryParameters") ;
	{
		XMLOstream::Tag txml2(xst, "_num", os.str()) ;
	}

	XMLOstream::Tag txml2(xst, "_params", "", "count=\""+os.str()+"\"") ;
	// <dbQueryParameters>
	//   <_num>2</_num>
	//   <_params count="2">
	//		... p1 ...
	//      ... p2 ... 
	//   </_params>
	// </dbQueryParameters>
	for(size_t i=0 ; i < _params.size() ; i++) // each param
	{
		XMLOstream::Tag txml3(xst, "dbParam" ) ;
		_params[i].exportXML(xst) ;
	}
}


size_t dbQueryParameters::parameterCount() const 
{ return _params.size() + _qparams.size() ; /* Since only one of the two can be filled at the same time */ } // PRQA S 3084
