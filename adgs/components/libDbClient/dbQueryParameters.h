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
	Revision 5.4  2015/04/22 08:14:47  enrcar
	Interface slightly modified to avoid regressions
	
	Revision 5.3  2015/04/21 15:05:33  enrcar
	dbQueryParamter now recursive
	
	Revision 5.2  2013/07/15 16:19:13  marpas
	deprecating methods having default db key string
	
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.9  2013/05/29 10:53:08  marpas
	i/f rationalization
	
	Revision 4.8  2013/03/29 19:47:21  marpas
	getting rid of Html related functions and libraries
	
	Revision 4.7  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.6  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.5  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:52  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.2  2009/06/19 17:47:57  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.1  2008/11/04 13:42:10  marpas
	make up and useless stuff deletion
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2006/01/17 18:20:38  marpas
	Following methods added:
		ParamOperator getParamOperator() const ;
	void setParamOperator(ParamOperator) ;
	
	Revision 1.3  2006/01/13 18:05:35  paoscu
	Query parameters implementation completely changed.
	
	Revision 1.2  2005/07/13 09:26:41  enrcar
	updated to dbClient version 1.13
	
	Revision 1.13  2005/07/12 16:16:11  marpas
	introducing dbParam::ISNOTNULL
	dbParam::verifyDbValues flag to avoid checking into dbvalues when composing complex where clauses
	
	Revision 1.12  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.11  2004/07/20 10:15:11  marpas
	IN added to dbParam
	some minor changes and fixes
	
	Revision 1.10  2003/09/03 13:55:59  paoscu
	Export methods are now const.
	
	Revision 1.9  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.8  2003/05/16 18:49:22  paoscu
	Commented a not implemented method.
	
	Revision 1.7  2003/05/02 19:01:57  paoscu
	getParameter now is const
	
	Revision 1.6  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.5  2003/02/20 18:12:12  paoscu
	Easter is closer so ...
	
	Revision 1.4  2003/02/20 17:42:25  paoscu
	numerical check now works (integral/floating not literal)
	
	Revision 1.3  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.2  2002/11/29 17:53:38  danalt
	added exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#ifndef _dbQueryParameters_H_
#define _dbQueryParameters_H_ 

#include <dbParam.h>
#include <dbException.h>
#include <iostream>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbQueryParameters
//
//
//

class dbQueryParameters  // PRQA S 2109
{
public:

	/*! class InvalidName declaration */
	exDECLARE_EXCEPTION(InvalidName,dbException) ;  // PRQA S 2131, 2153, 2502

	/*! class InvalidValue declaration */
	exDECLARE_EXCEPTION(InvalidValue,dbException) ; // PRQA S 2131, 2153, 2502

    enum ParamOperator {
        AND, // default
        OR
    } ;

	__attribute__((deprecated)) dbQueryParameters() ;	
	explicit dbQueryParameters(const std::string &key);	

	dbQueryParameters(const dbQueryParameters &);				
	virtual ~dbQueryParameters() throw() ;
	
	dbQueryParameters &operator=(const dbQueryParameters &) ; 	

	void addParameter(const dbParam &) ; 
		
	void addQueryParameter(const dbQueryParameters &, dbConnection *conn=0) ; 

	
	// note resets all parameters and the param operator too ( default AND) 
	void resetParameters() ; 


	
	ParamOperator getParamOperator() const ; 
	// will build an expression with the ParamOperator between each parameter
	void setParamOperator(ParamOperator) ; 
	
	
	size_t parameterCount() const ; 				// Number of parameters for both _params and _qparams
	bool has(const std::string & tableName , const std::string & fieldName) const ;
	void deleteParameter(const std::string & tableName , const std::string & fieldName) ; 		// Remove all parameters with the specified table and field
	const dbParam &getParameter(size_t) const;		 		// Return the 'n' parameter
	const dbQueryParameters &getQueryParameter(size_t) const;		 		// Return the 'n' parameter
		
	std::string getSqlString() const ; 				// get the WHERE part of the query
	
	friend std::istream &operator>>(std::istream &, dbQueryParameters &) ;		// read from a stream // PRQA S 2107
	friend std::ostream &operator<<(std::ostream &, const dbQueryParameters &) ;	// write to a stream // PRQA S 2107
	friend exostream &operator<<(exostream &, const dbQueryParameters &) ;	// write to a stream // PRQA S 2107, 2072

	// inherited from dbExportable
	void exportObj(rsResourceSet &s) const ;        // export the class to resourceSet
	void importObj(rsResourceSet &s)  ;         // import the class from a resourceSet
	void exportXML(XMLOstream &xst) const  ;        // export the class to XML stream
	
	
	
	static bool isNumeric(const std::string &s, bool floating=true) ; 
private:
	static void translate(const dbParam &par, std::string &description, std::string &expression) ; 

private:
	std::vector<dbParam> _params ; 
	std::vector<dbQueryParameters> _qparams ; 
	ParamOperator _paramOperator ;
	std::string _key ; 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbQueryParameters) ;

} ; 

std::istream &operator>>(std::istream &, dbQueryParameters &) ;		 // read from a stream
std::ostream &operator<<(std::ostream &, const dbQueryParameters &) ;  // write to a stream
exostream &operator<<(exostream &, const dbQueryParameters &) ;  // write to a stream

_ACS_END_NAMESPACE

#endif // _dbQueryParameters_H_
