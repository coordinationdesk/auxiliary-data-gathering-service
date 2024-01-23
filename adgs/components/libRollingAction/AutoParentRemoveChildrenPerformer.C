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

	$Prod: A.C.S. libRollingAction$

	$Id$

	$Author$

	$Log$
	Revision 2.9  2018/01/30 18:50:11  marpas
	trying to optimise dbConnection use
	qa warnings partially removed
	coding best practice application in progress
	
	Revision 2.8  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.7  2016/04/18 15:25:17  marpas
	some NameIdAssociators adhere to the standards now - they need a db key
	
	Revision 2.6  2016/01/21 17:05:31  nicvac
	S2PDGS-1394: Report configurable on item id
	
	Revision 2.5  2014/12/03 14:48:36  nicvac
	S2PDGS-897
	
	Revision 2.4  2014/12/02 14:49:07  nicvac
	S2PDGS-897
	
	Revision 2.3  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.2  2014/11/26 17:14:50  nicvac
	S2PDGS-897
	
	Revision 2.1  2014/11/26 11:18:39  nicvac
	AutoParentRemoveChildrenPerformer added
	
        
*/ 

#include <AutoParentRemoveChildrenPerformer.h>

#include <RollingPoliciesActions.h>
#include <RollingAction.h>

#include <SMInterface.h>
#include <MigrationReport.h>
#include <dbSmartPersistent.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
ACS_CLASS_DEFINE_DEBUG_LEVEL(AutoParentRemoveChildrenPerformer)



AutoParentRemoveChildrenPerformer::AutoParentRemoveChildrenPerformer() : 
    ActionPerformer()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

AutoParentRemoveChildrenPerformer::~AutoParentRemoveChildrenPerformer() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void AutoParentRemoveChildrenPerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	RollingReportAutoSession rollingReportAutoSession( rollingReport );

	string currentItem;
	RollingReport::InfoRecord currentItemInfo;

	bool offLineStatusDetected(false);

	bool parentIsOnAsynchSM(false);
	vector<dbPersistent::IdType> asyncSMids;
	try {
		parentIsOnAsynchSM = this->isOnAsynchSM( conn, actionToProcess.getObject(), asyncSMids );
		offLineStatusDetected = true;
	}  catch (exception& ex) {
		//Notify the DB contains invalidated items that should be removed.
		errorOccurred = true;
		ACS_LOG_WARNING("Cannot detect Off-line status for Inventory item id ["<< actionToProcess.getObject()<<"]. Will retry on next loop. Problem occurred: "<<ex.what());
	}

	if ( offLineStatusDetected ) {

		bool invalidatedParent(false);
		//... invalidate parent: remove the physical file from the specified storage and set the validity flag to FALSE. And ...
		try  {
			if ( this->invalidateItem(conn, actionToProcess.getObject(), actionToProcess.getStorage(), currentItem, currentItemInfo) ) {
				invalidatedParent = true;
				rollingReport.record(currentItem, RollingReport::invalidated, currentItemInfo);
			}
		} catch(exception& ex) {
			errorOccurred = true;
			ACS_LOG_WARNING("Problem occurred rolling Inventory Id ["<< actionToProcess.getObject()<<"] "<<currentItem<<". Will retry on next loop. Problem occurred: "<<ex.what());
		}

		if ( invalidatedParent ) {

			//... remove the children (both physical file from the specified storage and entry from inventory). And ...
			try {

				//Get the migration report to check if the children has been actually migrated
				// (LTA can skip a children according to filtering rules)
				MigrationReport migrationReport;
				for ( vector<dbPersistent::IdType>::iterator it=asyncSMids.begin(); // PRQA S 4238
						(it != asyncSMids.end()) && migrationReport.isEmpty();
						++it) {
					dbPersistent::IdType pkgid = getPackageFromInv(conn, actionToProcess.getObject(), *it );
					getReportFromPkg(conn, pkgid, migrationReport);
				}

				vector<dbPersistent::IdType> childrenIds;
				this->getChildrenIds(conn, actionToProcess.getObject(), childrenIds );

				for (vector<dbPersistent::IdType>::iterator it=childrenIds.begin(); it!=childrenIds.end(); ++it) { // PRQA S 4238
					string childItem;
					RollingReport::InfoRecord childItemInfo;
					if ( this->removeItem( conn, *it, childItem, childItemInfo ) ) {

						//Detect the status of the removed children.
						// - parentIsOnAsynchSM && migrationReport.isEmpty() ==> migration report previously removed
						//	 by me (items rolled on the past, restored from asynch, here rolling them again)
						//   ==> can assume all (restored) children are (still) offline.
						// - parentIsOnAsynchSM && migrationReport.isEmpty() ==> first time I'm rolling
						//   ==> check if the children has been migrated or skipped by MigrationSever
						bool childOffLine = (
								parentIsOnAsynchSM  &&
								( migrationReport.isEmpty()	|| StringUtils::equalsNoCase( migrationReport.getAction(childItem), "MIGRATED" )) // PRQA S 4412
							);

						rollingReport.record( childItem,
								( childOffLine ? RollingReport::offline : RollingReport::removed ), // PRQA S 3380
								childItemInfo);
					}

				}

				//I'm the only consumer of the Migration reports
				for ( vector<dbPersistent::IdType>::iterator it=asyncSMids.begin(); // PRQA S 4238
						it != asyncSMids.end(); ++it) {
					dbPersistent::IdType pkgid = getPackageFromInv(conn, actionToProcess.getObject(), *it );
					getReportFromPkg(conn, pkgid, migrationReport);
					if ( ! migrationReport.isEmpty() ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Deleting Migration report for pkgid ["<<pkgid<<"] storageid ["<<actionToProcess.getStorage()<<"]");
						deleteClobFromPkg(conn, pkgid, actionToProcess.getStorage());
					}
				}


			} catch(exception& ex) {
				//Notify the DB contains orphans. A specific and controlled Process/Procedure is in charge of clean orphans.
				ACS_LOG_WARNING("Problem occurred removing Children of Removed Parent Inventory item id ["<< actionToProcess.getObject()<<"]. DB contains Orphan items. Problem occurred: "<<ex.what());
			}

			//... remove or re-validate the inventory entry Parent depending on its Migration status

			if ( parentIsOnAsynchSM ) {
				// Validate again the invalidated Parent.
				// But why do you invalidate and then validate again parent?
				// - Can't you just remove the children? No, because system level issues could be raised (e.g.: a valid parent with partial children during deletion)
				try {
					string x; RollingReport::InfoRecord y; //not interesting here
					this->invEntrySetValid(conn, actionToProcess.getObject(), true, x, y );
					rollingReport.record( currentItem, RollingReport::offline, currentItemInfo  );

				}  catch (exception& ex) {
					//Notify the DB contains OFF-LINE invalidated items that should be validated.
					ACS_LOG_WARNING("Problem occurred setting validity=true for Off-line Inventory item id ["<< actionToProcess.getObject()<<"]. The Off-line item is invalidated (removed from storage manager and invalidated in inventory). It should be set with validity=true. Problem occurred: "<<ex.what());
				}
				ACS_LOG_INFO("Off-line set Inventory Id ["<<actionToProcess.getObject()<<"] "<<currentItem<<"  and removed children (if any)" );

			} else {
				// Remove or validate again the invalidated Parent.
				// But why do you invalidate and then remove parent?
				// - Can't you just remove the parent first and then the children? No, because the delete on cascade, no children will be detected
				// - Can't you remove the children first and then the parent? No, because system level issues could be raised (e.g.: a valid parent with partial children)
				try {
					string x; RollingReport::InfoRecord y; //not interesting here
					this->invEntryRemove( conn, actionToProcess.getObject(), x, y);
					rollingReport.record( currentItem, RollingReport::removed, currentItemInfo );
				} catch (exception& ex) {
					//Notify the DB contains invalidated items that should be removed.
					ACS_LOG_WARNING("Problem occurred removing Inventory item id ["<< actionToProcess.getObject()<<"]. Anyway the item is invalidated (removed from storage manager and invalidated in inventory). Problem occurred: "<<ex.what());
				}
				ACS_LOG_INFO("Removed Inventory Id ["<<actionToProcess.getObject()<<"] "<<currentItem<<"  and removed children (if any)" );
			}

		}

	}


}


void AutoParentRemoveChildrenPerformer::getReportFromPkg (dbConnection &conn , dbPersistent::IdType pkgid, MigrationReport& rpt)
{
    rpt.deserialize (getClobFromPkg (conn, pkgid));
}


string AutoParentRemoveChildrenPerformer::getClobFromPkg (dbConnection &conn, dbPersistent::IdType pkgid)
{
    string result("");
    string dbKey = conn.getKey();
    SQLString sql (dbKey, SQLString::SELECT);

    sql.addTableList ("t_inventorysm");
    sql.addSelectList ("t_inventorysm.id");

    // Build Where clause (inventory id)
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t_inventorysm","packageid" , dbParam::EQUAL_TO , db::toSQL (conn, pkgid)));
        sql.addANDWhereParameter(pp) ;
    }

    // Build Where clause (storage manager id)
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t_inventorysm","migration_report" , dbParam::ISNOTNULL));
        sql.addANDWhereParameter(pp) ;
    }

    dbSet set; 
    dbRow row(dbKey);
    dbQuery query( conn, sql, "GET_INVSM_REP" );

    query.execForSet (set);

    // This will throw in case of multiple reports associated with the same package id
    if (set.size() > 1)
    {
        ostringstream os;
        os << "Too many migration reports associated to the requested package [" << pkgid << "]";
        ACS_THROW (exIllegalValueException (os.str())); // PRQA S 3081
    }

    dbPersistent::IdType invSmId;

    if (set.getNextRow (row))
    {
        row.column(0, invSmId);

        // The migration_report has to be handled as clob

		dbSmartPersistent tInvsm("t_inventorysm", conn);

		if ( tInvsm.load( conn, invSmId ) && tInvsm.isSet ( "migration_report", conn) ) 
		{

			// download the blob data field
			string dwnName = File::tempname("/tmp/SMInterface_getClobFromPkg_", ".xml"); //Temp filename
			FileGuard dwnNameGuard(dwnName) ; //Erase file at scope exit
			tInvsm.downloadFile("migration_report", dwnName, conn) ;
			{
				std::ifstream ifs( dwnName.c_str() );
				std::stringstream buffer;
				buffer << ifs.rdbuf();
				result = buffer.str();
			}
		}
	}

    return result;
}



dbPersistent::IdType AutoParentRemoveChildrenPerformer::getPackageFromInv (dbConnection &conn, dbPersistent::IdType invid, dbPersistent::IdType smid)
{
    dbPersistent::IdType result=-1;
    string dbKey = conn.getKey();
    SQLString sql (dbKey, SQLString::SELECT);
    sql.addTableList ("t_inventorysm");
    sql.addSelectList ("t_inventorysm.packageid");

    // Build Where clause (inventory id)
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t_inventorysm","inv_id" , dbParam::EQUAL_TO , db::toSQL (conn, invid)));
        sql.addANDWhereParameter(pp) ;
    }

    // Build Where clause (storage manager id)
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t_inventorysm","storagemanid" , dbParam::EQUAL_TO , db::toSQL (conn, smid)));
        sql.addANDWhereParameter(pp) ;
    }

    dbSet set; 
    dbRow row(dbKey);
    dbQuery query( conn, sql, "INVSMPKG_FROM_INVID" );

    query.execForSet (set);

    // This will throw in case of multiple packages associated with the same inventory on the same smid
    if (set.size() > 1)
    {
        ostringstream os;
        os << "Too many packages associated to the requested inventory item [" << invid << "] on the sm [" << smid << "]";
        ACS_THROW (exIllegalValueException (os.str())); // PRQA S 3081
    }

    if (set.getNextRow (row))
    {
        dbPersistent::IdType pkgid = 0 ;
        bool isNull = false;
        row.column(0, pkgid, &isNull);
        // If NULL do not change the value of the return variable (-1)
        if (!isNull)
        {
            result = pkgid;
        }
    }

    return result;    
}


void AutoParentRemoveChildrenPerformer::deleteClobFromPkg (dbConnection &conn, dbPersistent::IdType pkgid, dbPersistent::IdType smid ) 
{
    const string &dbKey = conn.getKey() ;
    SQLString sqlRel( dbKey, SQLString::UPDATE ) ;
    sqlRel.addTableList("t_inventorysm");
    sqlRel.addSetList("migration_report", db::toSQL (dbKey, dbConv::NullValue()) ) ;
    // Build Where clause
    {
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, "t_inventorysm","packageid" , dbParam::EQUAL_TO , db::toSQL(dbKey, pkgid) )) ; 
	    sqlRel.addORWhereParameter(pp) ;
    }

    ostringstream queryName ; queryName << "DEL_RPT_SMID" ;
    if (smid > 0) {
        queryName << '_' << smid ;
    }
	dbQuery queryREL(conn, sqlRel, queryName.str()) ;	
	if (!queryREL.execForStatus())
	{
		   dbQuery::QueryExecuteException e("Cannot delete clob from t_inventorysm: Command : "
		    								+ sqlRel.getSQL() + " Error : " + queryREL.errorString()) ;
		   ACS_THROW (e) ;
	}
}


_ACS_END_NAMESPACE

