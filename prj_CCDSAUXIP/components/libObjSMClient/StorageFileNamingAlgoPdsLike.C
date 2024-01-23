// PRQA S 1050 EOF
/*
	Copyright 2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	
*/ 

#include <StorageFileNamingAlgoPdsLike.h>

#include <dbPersistentOperatorIF.h>	
#include <DateTime.h>		
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(StorageFileNamingAlgoPdsLike)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StorageFileNamingAlgoPdsLike::StorageFileNamingAlgoPdsLike(const dbPersistent *o):
								StorageFileNamingAlgo(o) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);			
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StorageFileNamingAlgoPdsLike::~StorageFileNamingAlgoPdsLike() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void StorageFileNamingAlgoPdsLike::buildName( dbPersistentOperatorIF *oper, string & filename, string & originalname )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);		// PRQA S 4631
	ACS_COND_THROW( oper == 0, StorageFileNamingAlgoException( "dbPersistentOperatorIF object null pointer!") );
	
	originalname = oper->getFileName(*getObject()) ;
	{
		ostringstream os;
		os << originalname ;
		string fileversion = oper->getFileVersion(*getObject()) ;
		if (not fileversion.empty()) {
			os << "_V" << fileversion ;
		}
		DateTime::UTCD utcd = DateTime();
		os << "_" << setfill('0') << setw(2) << utcd.hour << setw(2) << utcd.min << setw(2) << utcd.sec << setw(3) << utcd.millisec(); // PRQA S 4400
		filename = os.str();
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename on storage=\"" << filename << "\" original name=\"" << originalname << '\"' );
}

_ACS_END_NAMESPACE
