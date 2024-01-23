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

#include <ImporterTableName.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ImporterTableName)



ImporterTableName::ImporterTableName():
_tableNames()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	_tableNames[ t_receptionrules ] = "t_receptionrules";
}



const string& ImporterTableName::get( const ImporterTableNames& tableName ) const {

	return _tableNames.at( tableName );
}

void ImporterTableName::set( const ImporterTableNames& tableName, const std::string& dbImporterTableName ) {

	_tableNames[ tableName ] = dbImporterTableName;
}


_ACS_END_NAMESPACE

