/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:00  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:27  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.10  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.9  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.8  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.7  2012/11/19 22:51:15  marpas
	compile with PDS_OLD_SCHEMA to use old pds schema.
	dbReceptionRule and related Query class introduced
	
	Revision 2.6  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.5  2009/02/04 15:48:23  marfav
	Addedd doesOverlap method
	
	Revision 2.4  2008/12/17 19:35:22  paoscu
	Query fixed
	
	Revision 2.3  2008/12/17 14:41:29  paoscu
	Overloaded setParametersCLOB
	
	Revision 2.2  2008/12/16 18:48:43  paoscu
	 Implemented dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery
	
	Revision 2.1  2008/12/16 15:59:33  marpas
	 work in progress
	
	Revision 2.1  2008/12/03 18:09:03  paoscu
	dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery Added
	
	Revision 2.1  2008/12/01 20:07:05  paoscu
	Added dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery
	
	

*/

#include <dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>
#include <dbSmartQuery.h>
#include <dbSmartPersistent.h>
#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>

#include <XMLIstringstream.h>


#include <sstream>
#include <list>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery)

dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey)
: dbInventoryOnSiteValCoverOrIntersectQuery(latest , intersect, dbKey), 
_newObjects() 
{

}


dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery(dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery const &r)
: dbInventoryOnSiteValCoverOrIntersectQuery(r),  // these will be override by assignement
_newObjects()
{
}

dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery &dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::operator=(dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryOnSiteValCoverOrIntersectQuery::operator= (r ) ; 
        // leave _newObjects
    }
	return *this ;
}

dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::~dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery() throw() 
{
}


unsigned long dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::doQuery(dbConnection &c, bool incremental, bool enabled)
{
	int result = 0 ;
	
	try
	{
		if( ! incremental)
			clear() ;
		
		clearNewObjects() ;


		short int tdiMask = 61 ; // 111101  =>  32+16+8+4+1

		string parameters=getParameters() ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "common parameters are: " << parameters) ;

		dbSmartQuery bandQuery("t_bands", c.getKey()) ;
		bandQuery.doQuery(c) ;
		
		if( !bandQuery.size() )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No bands found.") ;
			
			return 0 ;
		}
		
		//unsigned storedRecords = 0 ;

		for(unsigned b=0 ; b<bandQuery.size() ; b++)
		{
			unsigned int bandId = bandQuery.at(b)->getId() ;
			
			//Build a new query for the single band
			dbInventoryOnSiteValCoverOrIntersectQuery* singleBandQuery(0) ;
			if(_intersect) {
				singleBandQuery=dynamic_cast<dbInventoryOnSiteValCoverOrIntersectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryOnSiteValIntersectQuery", c.getKey()));
            }
			else {
				singleBandQuery=dynamic_cast<dbInventoryOnSiteValCoverOrIntersectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryOnSiteValCoverQuery", c.getKey()));
            }
			
			if(!singleBandQuery) {
				ACS_THROW(exIllegalValueException( "Failed to create query for single band" ) ) ; // PRQA S 3081
            }

			dbGenQueryScopeGuard queryGuard(singleBandQuery) ;

			singleBandQuery->t0(t0());
			singleBandQuery->t1(t1());
			singleBandQuery->dt0(dt0());
			singleBandQuery->dt1(dt1());
			dbQueryParameters par(c.getKey()) ;
			par.addParameter( dbParam(c.getKey(), "t_inventory","band" , dbParam::EQUAL_TO , db::toSQL(c.getKey(), bandId)) ) ;
			singleBandQuery->addParameter(par) ;
			singleBandQuery->setExtraCondition(parameters) ;

			if(_latest) //order the result
				singleBandQuery->setOrderBy(getOrderByField() + " DESC ") ;
			singleBandQuery->setParametersCLOB( getParametersCLOB() ) ;
			

		
			if( ! singleBandQuery->doQuery( c, false, enabled ) )
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No records found for band "  << bandId) ;
				clearNewObjects() ;
				return 0 ;
			}
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found " << singleBandQuery->size() << " records for band "  << bandId) ;
			
			//Now check the TDIs
			short int matchedBits = 0 ;
			vector<int> usefulRecords ;
						
			for( unsigned int record=0 ; record < singleBandQuery->size() ; record ++ )
			{
				dbInventoryObject *obj = (*singleBandQuery)[record] ;
				short int tdi = obj->getTdi() & tdiMask ;
				short int matchedBitsNew = matchedBits | tdi ;
				if( matchedBitsNew != matchedBits)
					usefulRecords.push_back(record) ;
				matchedBits = matchedBitsNew;
				if( matchedBits == tdiMask) //the subset checked satisfy the mask
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "matched all TDIs for band " << bandId) ;
					// Add all the new records or only the ones checked until now if _latest
					if(_latest)
						for(  unsigned int recordToAdd=0 ; recordToAdd < usefulRecords.size()  ; recordToAdd ++ )
						{
							dbInventoryObject *newObj=dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", c.getKey())) ;
							(*newObj) = (*(*singleBandQuery)[ usefulRecords[recordToAdd] ] );
							_newObjects.push_back(newObj) ;
							result ++ ;
						}
					else
						for(  unsigned int recordToAdd=0 ; recordToAdd < singleBandQuery->size() ; recordToAdd ++ )
						{
							dbInventoryObject *newObj=dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", c.getKey())) ;
							(*newObj) = (*(*singleBandQuery)[recordToAdd] );
							_newObjects.push_back(newObj) ;
							result ++ ;
						}
					break ;
				}
			}
			if( matchedBits != tdiMask) //TDIs not sutisfied for this band
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TDIs not satisfied: present TDIs are 0x" << hex <<  matchedBits) ;
				clearNewObjects() ;
				return 0 ;
			}
		}
		
		//now insert new objects
		for( unsigned int i=0 ; i < _newObjects.size() ; i ++ )
		{
			bool alreadyLoaded(false) ;
			dbInventoryObject *record = _newObjects[i] ;
			for (size_t ne=0; ne<_objects.size();ne++)
			{
 	            dbInventoryObject *oe = dynamic_cast<dbInventoryObject *>(_objects[ne]) ;
 	            if (oe && record->getFileName() == oe->getFileName() && record->getFileVersion() == oe->getFileVersion()) {
 	                alreadyLoaded = true ;
					break ;
                }
 	        }
 	        if (!alreadyLoaded) {
				// move from _newObjects to _objects
				_newObjects[i]=0 ;
				_objects.insert(_objects.begin(), record ) ;
			 }
		}
	
	}
	catch(...)
	{
		clearNewObjects() ;
		throw ;
	}

	return result ;

}







void dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::clearNewObjects()
{

	for (size_t i=0; i < _newObjects.size(); i++)
	{
		delete _newObjects[i] ;
		_newObjects[i] = 0 ;
	}
	_newObjects.clear() ;
}


bool dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery::doesOverlap() const
{
	// The result of this query has not to be filtered with overlap strategies
	return false;
}

_ACS_END_NAMESPACE
