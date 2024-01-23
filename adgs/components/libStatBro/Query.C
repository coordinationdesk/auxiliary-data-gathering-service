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

	$Prod: A.C.S. StatBro Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/10/20 08:39:18  marpas
	S1PDGS-31828
	Query class looks for countRecords parameters in .qry statement. In the parameter is present and set to false, the SHOW statement will not have its records counted in the final showResult class. This is to support the need to not generate empty reports, still having some show statements returning some records e.g. to provide headers, or just labels
	
	Revision 5.2  2016/05/10 14:48:00  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.1  2013/07/02 11:58:53  marpas
	minor changes for statistics
	
	Revision 5.0  2013/07/02 11:22:11  marpas
	adopting libException 5.x standards
	coding best practices
	qa warnings
	
	Revision 2.23  2013/04/12 12:20:07  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.22  2013/03/28 13:47:55  marpas
	statistical message improved to allow integrators to check report generation efficiency
	
	Revision 2.21  2013/03/26 18:00:17  marpas
	version is here now
	
	Revision 2.20  2013/03/26 17:59:05  marpas
	heavy refactoring, breaking comaptibility with Qt applications and StatisticalReportGenerator in favour f StatBroEngine
	
	Revision 2.19  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.18  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.17  2012/03/08 13:43:12  marpas
	messages improved
	
	Revision 2.16  2012/03/08 13:28:01  marpas
	robustness and messages improved
	
	Revision 2.15  2012/02/13 17:04:33  marpas
	removing compiler warnings
	
	Revision 2.14  2010/09/09 13:55:51  marpas
	bug fixed in validitystop -
	
	Revision 2.13  2010/04/26 10:26:03  marpas
	preparation for ORACLE integration. (need new libDbGeo ORACLE aware)
	
	Revision 2.12  2009/04/08 09:51:43  marpas
	getSystem method added
	
	Revision 2.11  2009/02/24 14:50:09  marpas
	Porting to Postgres enforced
	
	Revision 2.10  2008/12/10 18:36:16  marpas
	mproved
	
	Revision 2.9  2008/11/18 16:25:58  marpas
	interface changed for StatBroEngine
	
	Revision 2.8  2008/11/17 16:31:44  marpas
	Informix and Postgres management added
	
	Revision 2.7  2008/11/14 15:34:58  marpas
	getSatellite method added
	
	Revision 2.6  2008/10/08 15:52:49  crivig
	porting to postgres8
	
	Revision 2.5  2007/06/11 15:07:28  marpas
	msg fixed
	
	Revision 2.4  2006/06/23 08:38:55  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.3  2006/06/20 16:53:30  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.2  2006/06/16 09:59:55  marpas
	subtitle string has parameters substitution.
	
	Revision 2.1  2006/04/12 17:51:34  paoscu
	More verbosity added to exceptions.
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.28  2005/10/20 11:52:49  marpas
	DBEXPRESS compatibility added
	
	Revision 1.27  2005/10/10 15:46:01  paoscu
	DbExpress compatibility (partial) added
	
	Revision 1.26  2005/06/08 10:38:53  marpas
	exceptions catching fixed
	
	Revision 1.25  2005/04/08 06:48:07  marpas
	parameter delimiter can be changed using "paramDelimiter" parameter in the qry file.
	
	typing error in replaceParameters method name fixed
	
	Revision 1.24  2005/02/09 11:18:29  marpas
	isInteractive method added, thus the query can discriminate whether the query MUST ask parameters to the user or rely on pre-empted ones.
	
	Revision 1.23  2005/01/14 18:09:58  marpas
	loadfile function reset parameters and validity
	
	Revision 1.22  2004/10/25 09:22:21  marpas
	another patch for ITEssential::Release() Informix bug
	
	Revision 1.21  2004/10/15 09:59:36  marpas
	exceptions messages improved
	
	Revision 1.20  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.19  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.18  2004/05/07 16:11:33  marpas
	Fixed header support implemented
	
	Revision 1.17  2004/01/23 15:17:54  paoscu
	Time functionalities enhanced
	Header management.
	
	Revision 1.16  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.15  2003/10/23 18:14:40  paoscu
	New style for header.
	
	Revision 1.14  2003/10/23 17:18:51  paoscu
	Now it's possible to avoid parameters replacement.
	
	Revision 1.13  2003/10/23 16:22:41  paoscu
	Parameters management improved.
	
	Revision 1.12  2003/10/16 16:29:29  paoscu
	If HtmlTitle is empty then setOutTitle is not used.
	
	Revision 1.11  2003/10/07 16:40:08  paoscu
	setParameter and readParameter methods added
	Parameters can be specified in different ways inside query file
	sql command files are now parsed by a ParamParser
	
	Revision 1.10  2003/09/26 10:24:40  paoscu
	getStyleSheet method added.
	unsetenv GL_DATETIME.
	Sql query are now handled by a dbMultiLineQuery
	The query can be passed by an external sql file
	
	Revision 1.9  2003/08/19 16:08:41  paoscu
	Exceptions handled in validityStart and validityStop methods.
	
	Revision 1.8  2003/08/19 14:27:10  paoscu
	validityStart and validityStop methods added.
	
	Revision 1.7  2003/07/11 21:07:53  marpas
	exException::clone implemented
	
	Revision 1.6  2003/04/17 14:09:15  marpas
	GCC3.2
	
	Revision 1.5  2003/03/11 13:43:23  marpas
	some string::lenght changed in string::empty
	
	Revision 1.4  2003/03/06 11:04:10  marpas
	commented out lines just removed
	
	Revision 1.3  2003/02/26 13:47:33  marpas
	stable
	
	Revision 1.2  2003/02/20 18:15:17  marpas
	StringUtils::equalsNoCase fixed for null strings
	
	Revision 1.1.1.1  2003/02/14 10:16:00  marpas
	Import libStatBro
	
	

*/

#include <Query.h>
#include <ShowResult.h>
#include <rsPushNSpace.h>
#include <ParamParser.h>
#include <StringUtils.h>

#include <dbConnection.h>
#include <dbMultiLineQuery.h>
#include <Filterables.h>
#include <dbSet.h>
#include <dbRow.h>
#include <File.h>
#include <sbV.h>

#include <sstream>
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
namespace { // unnamed
    sbV version ;
}
ACS_CLASS_DEFINE_DEBUG_LEVEL(Query)

Query::Query(const string &filename, const string &stat) :
	_filename(filename),
    _mergeFile(),
    _rs(),
    _parameters(),
    _validityStart(),
    _validityStop(),
	_validityTimeEvaluated(false),
    statString_(stat)
{
	loadFile() ;
}

Query::Query (const Query &p) :
	_filename(p._filename),
    _mergeFile(p._mergeFile),
    _rs(p._rs),
    _parameters(p._parameters),
    _validityStart(p._validityStart),
    _validityStop(p._validityStop),
	_validityTimeEvaluated(p._validityTimeEvaluated),
    statString_(p.statString_)
{
}


Query::~Query() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
    // PRQA L:L1
}


string Query::tr(Query::FieldType t)
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	string strtype;
	switch (t)
	{
	case Query::Q_STRING:
        {
		    strtype = "String";
		    break;
        }
	case Query::Q_INTEGER:
        {
		    strtype = "Integer";
		    break;
        }
	case Query::Q_FLOAT:
        {
		    strtype = "Float";
		    break;	
        }
	case Query::Q_DOUBLE:
        {
		    strtype = "Double";
		    break;
        }
	case Query::Q_DATE:
        {
		    strtype = "Date";
		    break;
        }
	}

	return strtype;
}


string Query::tr(Query::CommandType t)
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	string strtype ;
	switch (t)
	{
	case Query::Q_EXEC:	
        {
			strtype = "EXECUTE" ;
		    break;
        }
	case Query::Q_SHOW:
		{
            strtype = "SHOW" ;
	        break;
	    }
    }

	return strtype ;
}


Query::FieldType Query::string2FldType(const string &str) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "str: " << str) ;

	if (StringUtils::equalsNoCase(str,"String")) {
		return Query::Q_STRING;
    }
	else if (StringUtils::equalsNoCase(str,"Integer")) {
		return Query::Q_INTEGER;
    }
	else if (StringUtils::equalsNoCase(str,"Float")) {
		return Query::Q_FLOAT;
    }
	else if (StringUtils::equalsNoCase(str,"Double")) {
		return Query::Q_DOUBLE;
    }
	else if (StringUtils::equalsNoCase(str,"Date")) {
		return Query::Q_DATE;
    }
	else
	{	
		ACS_THROW(exIllegalValueException(str + " is not a valid field type")) ; // PRQA S 3081
	}
}


Query::CommandType Query::string2CmdType(const string & str) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "str: " << str) ;

	if (StringUtils::equalsNoCase(str,"EXECUTE")) {
		return Query::Q_EXEC;
    }
	else if (StringUtils::equalsNoCase(str,"SHOW")) {
		return Query::Q_SHOW;
    }
	else
	{	
		ACS_THROW (exIllegalValueException(str + " is not a valid command type")) ; // PRQA S 3081
	}
}


void Query::run(ShowResult &showRes, dbConnection &conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	unsetenv("GL_DATETIME");
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "warning unsetenv GL_DATETIME") ;

	showRes.clear() ;
    showRes.setDateFormat(getDateFormat()) ;

	try{
		loadFile() ;
		if (!_mergeFile.empty()) {
			mergeFile(_mergeFile) ;
        }
	}
	catch(std::exception &e){
		ACS_THROW( ErrorDuringQuery(e, "Error while loading file") ) ; // PRQA S 3081
	}
	
	//Init ShowResult
	showRes.init(_rs);
	
	size_t numStm = _rs.getArraySize("Sqlstm") ; // PRQA S 4412

	vector<string> nameStm ;

	try{
		evaluateTimes(conn) ;
	}
	catch(std::exception &e){
		ACS_THROW( ErrorDuringQuery(e, "Error in evaluateTimes") ) ;
	}


	string title ;
	try
	{
		_rs.getValue("HtmlTitle",title) ;
		showRes.setOutTitle(title) ;
	}
	catch (rsResourceSet::NotFoundException &) {}


	bool showHeader = true ; // default
	try
	{
		_rs.getValue("showHeader", showHeader) ;
	}
	catch(exception &) {}

	if(showHeader)
	{
		showRes.insertHeader(_validityStart , _validityStop);
	}

	_rs.getArray("Sqlstm",nameStm,numStm) ;


	int showQueries = 0 ;
	for (size_t i = 0; i < numStm; ++i)
	{
		{
			rsPushNSpace nsp(_rs, nameStm[i]) ;
			string type ;
			_rs.getValue("type", type) ;
			if (string2CmdType(type) == Query::Q_SHOW) {
				showQueries++ ;
            }
		}
	}

	showRes.setNumShowQueries(showQueries) ;

	for (size_t i = 0; i < numStm; ++i)
	{
        bool countRecords = true ;

		string cmd ;
		string sqlFile ;
		string type ;
		{
			{
				rsPushNSpace nsp(_rs, nameStm[i]) ;

				_rs.getValue("type", type) ;

                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "type: \"" << type << "\"") ;
                try {
				    _rs.getValue("countRecords", countRecords) ;
                    
                } catch(exception &) {}
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "countRecords: \"" << boolalpha << countRecords << "\"") ;
                
				string cmdName = "command." + SQLDialectFactory::dialectName(conn.getKey()) ;
				try {
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cmdName: \"" << cmdName << "\"") ;
					_rs.getValue(cmdName, cmd) ;
				}
				catch (rsResourceSet::NotFoundException &) {
					try {
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cmdName: \"" << cmdName << "\"") ;
						_rs.getValue("command", cmd) ;
					}
					catch (rsResourceSet::NotFoundException &) {
					}
				}

				if(cmd.empty()) {
					string fileName = "file."+ SQLDialectFactory::dialectName(conn.getKey()) ;
					try {
						_rs.getValue(fileName, sqlFile) ;
					}
					catch (rsResourceSet::NotFoundException &x) {
						try {
							_rs.getValue("file", sqlFile) ;
						}
						catch (rsResourceSet::NotFoundException &x1) {
							ostringstream os;
							os<<"Cannot find fields \"command\" or \"file\" in query n. "<<i;
							ACS_THROW(FileNotValid(x1,os.str())) ; // PRQA S 3081  
						}
					}
				}
			}
			try {
				string subtitle ;
				{
					rsPushNSpace nsp(_rs, nameStm[i]) ;
					_rs.getValue("subtitle", subtitle) ;
				}
				replaceParameters(subtitle,nameStm[i], conn) ;
				showRes.setSubTitle(subtitle) ;
			}
			catch (rsResourceSet::NotFoundException &) {
				// do nothing: no subtitle
				showRes.setSubTitle("") ;
			} 					
			{
				rsPushNSpace nsp(_rs, nameStm[i]) ;

				try {
					string queryClass ;
					_rs.getValue("class", queryClass) ;
					showRes.setQueryClass(queryClass) ;
				}
				catch (rsResourceSet::NotFoundException &) {
					showRes.setQueryClass("") ;
				} 					
			}

		}

		if(cmd.empty()) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executeFile on statement #" << i) ;
			executeFile(sqlFile, string2CmdType(type), showRes, conn, nameStm[i], i, countRecords) ; // PRQA S 3000, 3010
        }
		else {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executeCommand on statement #" << i) ;
			executeCommand(cmd, string2CmdType(type), showRes, conn, nameStm[i], i, countRecords) ; // PRQA S 3000, 3010
        }
	} //for 

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "showable res was filled with " 
                            << showRes.getShowRecords() << " records from " 
                            << showRes.getNumShowQueries() << " queries") ;
	showRes.showResult(getName()) ;
}


void Query::setParameter(string const&  paramName , string const & value, bool inmap)
{
	if (inmap) { _parameters[paramName]=value ; }
    else { _rs.setValue(paramName, value) ; }
}


string Query::readParameter(string const& paramName, string const & nameSpace, dbConnection &conn) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "paramName = \"" << paramName << "\""
			<< " namespace = \"" << nameSpace << "\"") ;

	//First attempt: Check if the value is inserted from outside
	if((_parameters.find(paramName) != _parameters.end() ) ) {
		return _parameters[paramName] ;
    }


	string paramNamespace = "Param." + paramName ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying with paramNamespace = \"" << paramNamespace << "\"") ;

	try
	{
		//Second attempt: search at local level with dialect name
		rsPushNSpace nsp1(_rs, nameSpace) ;
		rsPushNSpace nsp2(_rs, paramNamespace) ;
		string value ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying with \"" << nameSpace << "." << paramNamespace << ".value." << SQLDialectFactory::dialectName(conn.getKey()) << "\"") ;
		_rs.getValue("value."+SQLDialectFactory::dialectName(conn.getKey()), value) ;
		
		return value ;
	}
	catch (rsResourceSet::NotFoundException &) {}

	try
	{
		//Third attempt: search at local level without dialect name
		rsPushNSpace nsp1(_rs, nameSpace) ;
		rsPushNSpace nsp2(_rs, paramNamespace) ;
		string value ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying with \"" << nameSpace << "." << paramNamespace << ".value\"") ;
		_rs.getValue("value", value) ;
		
		return value ;
	}
	catch (rsResourceSet::NotFoundException &) {}
    
	try
	{
		//Fourth attempt: search at global level
		rsPushNSpace nsp2(_rs, paramNamespace) ;
		string value ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying with \"" << paramNamespace << ".value." << SQLDialectFactory::dialectName(conn.getKey()) << "\"") ;
		_rs.getValue("value."+SQLDialectFactory::dialectName(conn.getKey()), value) ;
		
		return value ;
	}
	catch (rsResourceSet::NotFoundException &) {}

	try
	{
		//Fifth attempt: search at global level
		rsPushNSpace nsp2(_rs, paramNamespace) ;
		string value ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying with \"" << paramNamespace << ".value\"") ;
		_rs.getValue("value", value) ;
		
		return value ;
	}
	catch (rsResourceSet::NotFoundException &) {}
	//Nothing found till now: ask
	//First at local level with dialect
	try
	{
		rsPushNSpace nsp1(_rs, nameSpace) ;
		rsPushNSpace nsp2(_rs, paramNamespace) ;
		string value ;
		value = getParameters(paramName, nameSpace) ;
		//Store it for next time
		_rs.setValue("value", value) ;
		return value ; 
	}
	catch (ParameterNotRetrieved &) {}

	//At last search at global level
	string value;
	rsPushNSpace nsp2(_rs, paramNamespace) ;
	value = getParameters(paramName , "") ;

	//Store it for next time
	_rs.setValue("value", value) ; 

	return value ;

}



DateTime Query::validityStart(dbConnection &c) 
{
	 evaluateTimes(c) ;
	 return _validityStart;
}


DateTime Query::validityStop(dbConnection &c) 
{	
	evaluateTimes(c) ;
	return _validityStop;
}


string Query::getName() const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


	string val ;
	_rs.getValue("Name",val) ;
	return val ;
}


string Query::getDescr() const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


	string val ;
	_rs.getValue("Descr",val) ;
	return val ;
}


string Query::getHelp() const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


	string val ;
	_rs.getValue("Help",val) ;
	return val ;
}


string Query::getStyleSheet() const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


	string val ;
	try{
		_rs.getValue("StyleSheet",val) ;
	}
	catch(exception &){}
	return val ;
}


string Query::getFileType() const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;


	string val ;
	try{
		_rs.getValue("FileType",val) ;
	}
	catch(exception &){}
	return val ;
}

string Query::getSatellite() const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	string val ;
	try{
		_rs.getValue("Satellite",val) ;
	}
	catch(exception &e)
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(ParameterNotRetrieved(e,"Cannot find parameter Satellite in query and parameters file")) ;
		ACS_LOG_WARNING("No Satellite reading filename " << _filename << " " << (!_mergeFile.empty() ? "merged with " + _mergeFile : string("") )) ; // PRQA S 3081, 3380, 3385
	}
	return val ;
}

string Query::getSystem() const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	string val ;
	try{
		_rs.getValue("System",val) ;
	}
	catch(exception &e)
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(ParameterNotRetrieved(e,"Cannot find parameter System in query and parameters file")) ;
		ACS_LOG_WARNING("No System reading filename " << _filename << " " << (!_mergeFile.empty() ? "merged with " + _mergeFile : string("") )) ; // PRQA S 3081, 3380, 3385
	}
	return val ;
}

string Query::getFormat() const  // PRQA S 4214
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	string val ;
	try{
		_rs.getValue("Format",val) ;
	}
	catch(exception &e)
	{
		ACS_LOG_WARNING("No Format reading filename " << _filename << " " << (!_mergeFile.empty() ? "merged with " + _mergeFile : string("") )) ; // PRQA S 3081, 3380, 3385
	}
	return val ;
}

string Query::getDateFormat() const // PRQA S 4020, 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	string val ;
	try{
		_rs.getValue("XmlDateFormat",val) ;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning \"" << val << "\"") ;
        return val ;
	}
	catch(exception &e)
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(ParameterNotRetrieved(e,"Cannot find parameter XmlDateFormat in query and parameters file")) ;
		ACS_LOG_WARNING("No XmlDateFormat reading filename " << _filename << " " << (!_mergeFile.empty() ? "merged with " + _mergeFile : string("") )) ; // PRQA S 3081, 3380, 3385
	    ostringstream os ;
        os << ShowResult::DateWUtc ;
        ACS_LOG_WARNING("Setting XmlDateFormat to: \"" << os.str() << "\"") ;
        return os.str() ;
    }
	
}

void Query::setDateFormat(const string &df)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	_rs.setValue("XmlDateFormat",df) ;
}

bool Query::isInteractive() const 
{
	return false ;
}

void Query::loadFile()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	_rs.clear() ; 
	
	if (isInteractive()) { // an interactive query: must ask parameters to the user
		_parameters.clear() ;
		_validityTimeEvaluated = false ;
	}
	
	ifstream file ; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading file: " << _filename) ;

	file.open(_filename.c_str()) ;
	if (not file.good())
	{
		ACS_THROW(FileNotValid("Cannot open file: " + _filename, errno)) ; // PRQA S 3081
	}
	else
	{
		try
		{
			string	val ;
			file >> _rs ;
			_rs.getValue("Name",val) ;
			_rs.getValue("Descr",val) ;
			_rs.getValue("Help",val) ;
		}
		catch (exception &x)
		{
			ACS_THROW(FileNotValid(x,"Invalid file format: "+_filename)) ;
		}
	}
}

void Query::mergeFile(const std::string &f) 
{
	_mergeFile = f ;
	ifstream file(f.c_str()) ;
	ACS_COND_THROW(not file.good(),FileNotValid("Cannot open file: " + f, errno)) ;
	try
	{
		_rs.mergeMode(rsResourceSet::ModeChange) ;
		file >> _rs ;
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			try {
				ofstream rsout("query.rs") ;
				rsout << _rs  ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "resSet in query.rs") ;
			}
			catch(exception &e) {
				ACS_LOG_NOTIFY_EX(e) ;
			}
		ACS_CLASS_END_DEBUG
		
	}
	catch (exception &x)
	{
		ACS_THROW(FileNotValid(x,"Invalid file format: "+f)) ;
	}
}

void Query::executeCommand(
	string 				&cmd,
	Query::CommandType	cmdType,
	ShowResult 			&showRes,
	dbConnection 		&conn, 
	string const 		 &nameSpace,
    int                 stmtnum, 
    bool                addToShow 
)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	replaceParameters(cmd , nameSpace, conn) ;
	
	dbMultiLineQuery query(statString_+nameSpace, stmtnum, conn, cmd) ;	

	switch (cmdType)
	{
	case Query::Q_EXEC:
        {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "EXEC: " << cmd) ;

			if (!query.execForStatus())
			{
				ACS_THROW(ErrorDuringQuery( "Error executing query <" + cmd + ">")) ; // PRQA S 3081
			}

			break ;
        }
	case Query::Q_SHOW:
        {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SHOW: " << cmd) ;

			dbSet set ; 

            query.execForSet(set) ; 

			parse(showRes, nameSpace) ;

            if (addToShow) { showRes.addShowRecords(set.size()) ; }
            showRes.addResult(set) ; 

			break ;
        }
	}
}


void Query::executeFile(
	string 				&sqlFile,
	Query::CommandType	cmdType,
	ShowResult 			&showRes,
	dbConnection 		&conn, 
	string const 		 &nameSpace,
    int                 stmtnum, 
    bool                addToShow 
)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "statement #" << stmtnum
        << " nameSpace: \"" << nameSpace << "\" sqlFile: " << sqlFile) ;
	string absoluteSqlPath;
	string command;
	ACS_COND_THROW(sqlFile.empty(),FileNotValid("Sql query filename is empty")) ; // PRQA S 3081
	
	if(sqlFile[0] != '/') //relative path
	{
		ACS_COND_THROW(sqlFile.empty(),FileNotValid("Query filename is empty")) ; // PRQA S 3081
		absoluteSqlPath=File::getDirPath(_filename) + "/" + sqlFile ;
	}
	else {
		absoluteSqlPath = sqlFile ;
    }
	
	ifstream inFile(absoluteSqlPath.c_str());
	ACS_COND_THROW(!inFile,exIOException("Error in opening  file \"" + absoluteSqlPath +"\".",errno)) ; // PRQA S 3081

	// get length of file:
	inFile.seekg (0, ios::end);
	off_t length = inFile.tellg(); // PRQA S 3050
	inFile.seekg (0, ios::beg);
    
	char *buffer = new char [length+1]; // PRQA S 3000
	inFile.read (buffer,length);
	buffer[length]='\0' ;
	inFile.close();

	string sqlCommand(buffer) ;
	delete [] buffer ;

	replaceParameters(sqlCommand , nameSpace, conn) ;
	
	istringstream commandStream(sqlCommand) ;
	
	dbMultiLineQuery query(statString_+nameSpace, stmtnum, conn, commandStream) ;	

	switch (cmdType)
	{
	case Query::Q_EXEC:
        {
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "EXEC: sql file: \"" << absoluteSqlPath << "\"") ;

		    if (!query.execForStatus())
		    {
			    ACS_THROW(ErrorDuringQuery("Error executing file \"" + absoluteSqlPath + "\"")) ; // PRQA S 3081
		    }

		    break ;
        }
	case Query::Q_SHOW:
        {
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SHOW: sql file: \"" << absoluteSqlPath << "\"") ;

		    dbSet set ; 

		    query.execForSet(set) ; 

		    parse(showRes, nameSpace) ;
            if (addToShow) { showRes.addShowRecords(set.size()) ; }
		    showRes.addResult(set) ;

		    break ;
        }
	}
}


void Query::replaceParameters(string & comm , string const & nameSpace, dbConnection &conn) // PRQA S 4020
{
	bool replace=true ; // if not found - default
	char delimiter = 0 ;


	{ // under the same namespace

		rsPushNSpace nsp1(_rs, nameSpace) ;
		try
		{
			_rs.getValue("replaceParameters", replace) ;
		}
		catch(exception &) { }

		try
		{
			_rs.getValue("paramDelimiter", delimiter) ;
		}
		catch(exception &) { }
	}
	
	if(!replace) {
		return;
    }


	bool done = false ;
	while (!done)
	{
		try
		{
			ParamParser pp ;

			if (delimiter != 0) {
				pp.delimiter(delimiter) ;
            }

			string param = pp.getVariable(comm) ;

			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "about to obtain value for parameter \"" << param << "\"") ;

			string value = readParameter(param , nameSpace, conn) ;

			comm = pp.subVariable(comm, param, value) ;
		}
		catch (ParamParser::NoParamFound &)
		{
			done = true ;
		}
	}
}



void Query::parse(ShowResult &showRes, string const & nameSpace)
{
	showRes.parse(_rs, nameSpace) ;
}


void Query::evaluateTimes(dbConnection &c)
{
	if(!_validityTimeEvaluated)
	{
		evaluateStartTime(c) ;
		evaluateStopTime(c) ;
	}
	
	_validityTimeEvaluated=true ;
	
}

void Query::evaluateStartTime(dbConnection &c) 
{
	string sqlTime;
	try {
		_rs.getValue("ValidityStart."+ SQLDialectFactory::dialectName(c.getKey()), sqlTime) ;
	}
	catch(rsResourceSet::NotFoundException &) {
		try{
			_rs.getValue("ValidityStart",sqlTime) ;
		}
		catch(exception&) { }
	}
	if(!sqlTime.empty()) {
		_validityStart = evaluateTime(sqlTime, c, "start");
    }
}


void Query::evaluateStopTime(dbConnection &c) 
{	
	string sqlTime;
	try {
		_rs.getValue("ValidityStop."+ SQLDialectFactory::dialectName(c.getKey()), sqlTime) ;
	}
	catch(rsResourceSet::NotFoundException &) {
		try{
			_rs.getValue("ValidityStop",sqlTime) ;
		}
		catch(exception &) { }
	}
	
	if(!sqlTime.empty()) {
		_validityStop = evaluateTime(sqlTime,c, "stop");
    }
}





DateTime Query::evaluateTime(string const & sqlTime, dbConnection &conn, const std::string &stat)
{
	DateTime res;
	
	string cmd=sqlTime;
	replaceParameters(cmd , "", conn) ;

	string queryString ;
	if (SQLDialectFactory::dialectName(conn.getKey()) == "Informix9") { 
	    queryString="SELECT FIRST 1 " + cmd + " AS Date FROM systables";
    }
	else if (SQLDialectFactory::dialectName(conn.getKey()) == "Postgres8") {
	    queryString="SELECT " + cmd + " AS Date " ;
    }
	else {
        ACS_THROW(ErrorDuringQuery(SQLDialectFactory::dialectName(conn.getKey()) + " unsupported dialect")) ; // PRQA S 3081
    }

	try
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "queryString is : \"" << queryString <<"\".") ;

		dbMultiLineQuery query("TM_" + statString_ + stat, 0, conn, queryString) ;	
		dbSet set ; 
        query.execForSet(set) ; 
        ACS_COND_THROW(set.empty(), ErrorDuringQuery("empty set")) ;

		dbRow queryResult(conn.getKey());
        set.getNextRow(queryResult) ; // first and only 
		if(queryResult.columns() != 0) { // PRQA S 3000
			queryResult.column(0,res);
        }
		else
		{
			ACS_THROW(ErrorDuringQuery("Empty result.")) ; // PRQA S 3081
		}
	}
	catch(exception &e)
	{
		ACS_THROW(ErrorDuringQuery(e, "Error while evaluating sql time string. queryString is : \"" + queryString + "\".")) ; // PRQA S 3081
	}
	
	return res;

}

_ACS_END_NAMESPACE
