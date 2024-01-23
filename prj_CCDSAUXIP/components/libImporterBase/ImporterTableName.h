/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libImporterBase$
        
*/

#ifndef _ImporterTableName_H_
#define _ImporterTableName_H_

#include <exException.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Table names may change in app specialization
**/

class ImporterTableName : public Singleton <ImporterTableName> { 
public:
	//Table names enum
	typedef enum {
		t_receptionrules=0,
		ImporterTableNamesSize
	} ImporterTableNames;

	/** return the table name */
	const std::string& get( const ImporterTableNames& tableName ) const;

	/** Used by specializations during init phase to set the right table names */
	void               set( const ImporterTableNames& tableName, const std::string& dbImporterTableName );


private:
	/** store the table names */
	std::map< ImporterTableNames, std::string > _tableNames;

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	ImporterTableName();

	ACS_DECLARE_NO_COPY_CLASS(ImporterTableName);
	friend ImporterTableName* Singleton<ImporterTableName>::instance(); //SINGLETON

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ImporterTableName)

};

_ACS_END_NAMESPACE

#endif //_ImporterTableName_H_

