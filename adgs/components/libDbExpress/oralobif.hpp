/*

	Copyright 2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. LOB interface for Oracle $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2010/02/19 17:40:01  enrcar
	EC:: Oracle LOB interface added
	
	Revision 2.0  2010/02/19 11:18:57  marpas
	ORACLE dependency from now on
	
	Revision 1.1  2010/02/19 11:16:49  marpas
	oralobif.hpp header file added to be used in conjunction with ORACLE driver and
	related dialect for LOB operations.
	

*/

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif


#ifndef _oralobif_H_
#define _oralobif_H_ 

#include <sstream>
#include <string.h>

class oracleLobIf {

    public:

        enum LOBType { BLOB=0, CLOB=1, UNDEF_TYPE=127 } ;
        enum LOBOperation { DOWNLOAD=0, UPLOAD=1, UNDEF_OPER=127 } ; 
        oracleLobIf() ;
        ~oracleLobIf() ;

  	    std::string getColumn() ;
  	    std::string getFileName() ;
        LOBType getLOBType() ;
        LOBOperation getLOBOperation() ;
	    std::string getTableName() ;
	    std::string getWhereCondition() ;
        const char* getColumnC(char* b, size_t n) ;
        const char* getFileNameC(char* b, size_t n) ;
        const char* getTableNameC(char* b, size_t n) ;
        const char* getWhereConditionC(char* b, size_t n) ;

  	    void setColumn(const std::string&) ;    
  	    void setFileName(const std::string&) ;    
        void setLOBType(const LOBType&) ;
        void setLOBOperation(const LOBOperation &) ;   
	    void setTableName(const std::string&) ;    
	    void setWhereCondition(const std::string&) ;

		char* dump(char*, size_t) ;

    private:
	    // not implemented
	    oracleLobIf(const oracleLobIf &) ;
	    oracleLobIf& operator=(const oracleLobIf &) ;

	    std::string _column, _fileName, _tableName, _whereCondition ;
 		LOBType _LOBType ;
        LOBOperation _LOBOperation ;

} ;	// class oracleLobIf


inline oracleLobIf::oracleLobIf() :
    _column(""), _fileName(""), _tableName(""), _whereCondition(""), _LOBType(UNDEF_TYPE), _LOBOperation(UNDEF_OPER)  
{ /* DO NOTHING */ }
inline oracleLobIf::~oracleLobIf() {}


inline void oracleLobIf::setColumn(const std::string& column) { _column = column ; }   
inline void oracleLobIf::setFileName(const std::string& fileName) { _fileName = fileName; } 
inline void oracleLobIf::setLOBType(const oracleLobIf::LOBType& LOBType) { _LOBType = LOBType; }
inline void oracleLobIf::setLOBOperation(const oracleLobIf::LOBOperation & LOBOperation) { _LOBOperation = LOBOperation; } 
inline void oracleLobIf::setTableName(const std::string& tableName) { _tableName = tableName; }    
inline void oracleLobIf::setWhereCondition(const std::string& whereCondition) { _whereCondition = whereCondition; }


inline std::string oracleLobIf::getColumn() { return _column; }
inline std::string oracleLobIf::getFileName() { return _fileName; }
inline oracleLobIf::LOBType oracleLobIf::getLOBType() { return _LOBType; }              
inline oracleLobIf::LOBOperation oracleLobIf::getLOBOperation() { return _LOBOperation; }
inline std::string oracleLobIf::getTableName() { return _tableName; } 
inline std::string oracleLobIf::getWhereCondition() { return _whereCondition; }   

inline const char* oracleLobIf::getColumnC(char* b, size_t n) { memset(b, 0, n); return strncpy(b, _column.c_str(), (n-1)); }
inline const char* oracleLobIf::getFileNameC(char* b, size_t n) { memset(b, 0, n); return strncpy(b, _fileName.c_str(), (n-1)); }
inline const char* oracleLobIf::getTableNameC(char* b, size_t n) { memset(b, 0, n); return strncpy(b, _tableName.c_str(), (n-1)); }
inline const char* oracleLobIf::getWhereConditionC(char* b, size_t n) { memset(b, 0, n); return strncpy(b, _whereCondition.c_str(), (n-1)); } 

inline char* oracleLobIf::dump(char* buffer, size_t n)
{
	std::string LOBOperation="UNDEFINED", LOBType="UNDEFINED" ;
    switch (_LOBOperation) { case DOWNLOAD: LOBOperation="DOWNLOAD"; break ; case UPLOAD: LOBOperation="UPLOAD"; break ; default: break ; }
    switch (_LOBType) { case BLOB: LOBType="BLOB"; break ; case CLOB: LOBType="CLOB"; break ; default: break ; }

	std::ostringstream msg ; msg << "oracleLobIf::dump()  ptr: " << (void*)(this) 
        << "\n\tLOBOperation: " << LOBOperation << "\n\tLOBType: " << LOBType
    	<< "\n\tTableName: !" << _tableName << "!\n\tColumn: !" << _column << "!\n\tFilename: !" << _fileName 
        << "!\n\tWhereCondition: !" << _whereCondition << "!\n" ;

	memset (buffer, 0, n) ; if (n>1) strncpy(buffer, (msg.str()).c_str(), (n-1)) ;
	return buffer ;
}


#endif /* _oralobif_H_ */
