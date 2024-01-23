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
	Revision 5.1  2013/07/02 11:58:53  marpas
	minor changes for statistics
	
	Revision 5.0  2013/07/02 11:22:11  marpas
	adopting libException 5.x standards
	coding best practices
	qa warnings
	
	Revision 2.9  2013/04/12 12:20:07  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.8  2013/03/28 13:47:55  marpas
	statistical message improved to allow integrators to check report generation efficiency
	
	Revision 2.7  2013/03/26 17:59:05  marpas
	heavy refactoring, breaking comaptibility with Qt applications and StatisticalReportGenerator in favour f StatBroEngine
	
	Revision 2.6  2013/03/26 00:51:26  marpas
	coding best practices
	new base libraries interface adopted
	qa rules applied
	work in progress
	
	Revision 2.5  2012/03/15 11:05:26  marpas
	dealing with alternative DateFormat
	
	Revision 2.4  2012/03/08 13:28:01  marpas
	robustness and messages improved
	
	Revision 2.3  2009/04/08 09:51:43  marpas
	getSystem method added
	
	Revision 2.2  2008/11/18 16:25:58  marpas
	interface changed for StatBroEngine
	
	Revision 2.1  2008/11/14 15:34:58  marpas
	getSatellite method added
	
	Revision 2.0  2006/02/28 10:18:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.18  2005/10/20 11:52:49  marpas
	DBEXPRESS compatibility added
	
	Revision 1.17  2005/04/08 06:48:07  marpas
	parameter delimiter can be changed using "paramDelimiter" parameter in the qry file.
	
	typing error in replaceParameters method name fixed
	
	Revision 1.16  2005/02/09 11:18:29  marpas
	isInteractive method added, thus the query can discriminate whether the query MUST ask parameters to the user or rely on pre-empted ones.
	
	Revision 1.15  2004/10/25 09:22:21  marpas
	another patch for ITEssential::Release() Informix bug
	
	Revision 1.14  2004/10/07 01:34:17  marpas
	new multiset query supported
	
	Revision 1.13  2004/10/01 16:02:40  marpas
	Node attributes for Xml generation take into account
	They Must be separated by "|" sign
	
	Revision 1.12  2004/05/07 16:11:33  marpas
	Fixed header support implemented
	
	Revision 1.11  2004/01/23 15:17:54  paoscu
	Time functionalities enhanced
	Header management.
	
	Revision 1.10  2003/10/24 16:19:31  paoscu
	User labels option added.
	
	Revision 1.9  2003/10/23 16:22:41  paoscu
	Parameters management improved.
	
	Revision 1.8  2003/10/07 16:40:08  paoscu
	setParameter and readParameter methods added
	Parameters can be specified in different ways inside query file
	sql command files are now parsed by a ParamParser
	
	Revision 1.7  2003/09/26 10:24:40  paoscu
	getStyleSheet method added.
	unsetenv GL_DATETIME.
	Sql query are now handled by a dbMultiLineQuery
	The query can be passed by an external sql file
	
	Revision 1.6  2003/08/19 16:08:41  paoscu
	Exceptions handled in validityStart and validityStop methods.
	
	Revision 1.5  2003/08/19 14:27:10  paoscu
	validityStart and validityStop methods added.
	
	Revision 1.4  2003/07/11 21:07:53  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 15:51:13  marpas
	using namespace std no longer found into acs includes
	
	Revision 1.2  2003/03/06 11:04:10  marpas
	commented out lines just removed
	
	Revision 1.1.1.1  2003/02/14 10:16:00  marpas
	Import libStatBro
	
	

*/

#ifndef _Query_H_
#define _Query_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <rsResourceSet.h>
#include <DateTime.h>

#include <map>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbMultiQuery ;
class ShowResult ;

class Query // PRQA S 2109
{
public:
	enum FieldType {
		Q_STRING,
		Q_INTEGER,
		Q_FLOAT,
		Q_DOUBLE,
		Q_DATE
	} ;

	enum CommandType {
		Q_EXEC,
		Q_SHOW
	} ;

public:
	/*! class FileNotValid declaration */
	exDECLARE_EXCEPTION(FileNotValid,exException) ; // PRQA S 2131, 2502

	/*! class AbortOperation declaration */
	exDECLARE_EXCEPTION(AbortOperation,exException) ;   // PRQA S 2131, 2502

	/*! class NotImplementedMethod declaration */
	exDECLARE_EXCEPTION(NotImplementedMethod,exException) ;// PRQA S 2131, 2502

	/*! class ErrorDuringQuery declaration */
	exDECLARE_EXCEPTION(ErrorDuringQuery,exException) ;// PRQA S 2131, 2502

	/*! class ParameterNotRetrieved declaration */
	exDECLARE_EXCEPTION(ParameterNotRetrieved,exException) ;// PRQA S 2131, 2502

public:
    explicit Query(const std::string &, const std::string &stat);
	Query (const Query &) ;
    virtual ~Query() throw() ;

	static std::string tr(Query::FieldType);
	static Query::FieldType string2FldType(const std::string &);
	static std::string tr(Query::CommandType);
	static Query::CommandType string2CmdType(const std::string &); 

	virtual void run(ShowResult &, dbConnection &) ;
	virtual void setParameter(std::string const&  paramName , std::string const & value, bool inmap=true) ;
	virtual std::string readParameter(std::string const&  paramName , std::string const & nameSpace, dbConnection &) ;
	virtual std::string getParameters(std::string const &, std::string const & nameSpace) = 0;

	virtual DateTime validityStart(dbConnection &)  ;
	virtual DateTime validityStop(dbConnection &)  ;

	std::string getName() const ;
	std::string getDescr() const ;
	std::string getHelp() const ;
	std::string getStyleSheet() const ;
	std::string getFileType() const ;
	std::string getSatellite() const ;
	std::string getSystem() const ;
	std::string getFormat() const ;
	std::string getDateFormat() const ;
	void setDateFormat(const std::string &) ;
	
	// default returns false, interactive queries should reimplement this method.
	virtual bool isInteractive() const ;
	void loadFile();
	void mergeFile(const std::string &) ;
private:
	void executeCommand(std::string &, 
						Query::CommandType, 
                        ShowResult &, 
                        dbConnection &, 
                        std::string const & nameSpace,
                        int stmtnum, bool addToShow=true) ;
	void executeFile(std::string &, 
					 Query::CommandType, 
                     ShowResult &, 
                     dbConnection &, 
                     std::string const & nameSpace,
                     int stmtnum, bool addToShow=true) ;
                     
	void replaceParameters(std::string & comm, std::string const & nameSpace, dbConnection &) ;
	void parse(ShowResult &, std::string const & nameSpace) ;
	

	void evaluateTimes(dbConnection &) ;
	void evaluateStartTime(dbConnection &) ;
	void evaluateStopTime(dbConnection &) ;
	
	//If sqlTime evaluation returns an error an ErrorDuringQuery is raised
	DateTime evaluateTime(std::string const & sqlTime, dbConnection &, const std::string &stat);
		
private:
	// functions declared but not implemented
	Query() ;
	Query &operator=(const Query &) ;

private:

	std::string _filename ;
	std::string _mergeFile ;
	rsResourceSet _rs ;
	std::map<std::string , std::string> _parameters ;

	DateTime _validityStart ;
    DateTime _validityStop ;
	bool _validityTimeEvaluated ;
	std::string statString_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Query) ;
	
};

_ACS_END_NAMESPACE

#endif // _Query_H_
