//PRQA S 1050 EOF
// 
// 	Copyright 1995-2021, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
// 	$Prod: A.C.S. PackagerOptimiser Library $
// 
// 	$Id$
// 
// 	$Author$
// 
// 	$Log$
// 	Revision 2.42  2017/03/27 13:37:38  nicvac
// 	APF-273: Dissemination mc messages refactoring.
// 	
// 	Revision 2.41  2017/03/24 17:07:59  nicvac
// 	APF-273: Dissemination MC.
// 	
// 	Revision 2.40  2016/09/08 07:47:22  marfav
// 	Get rid of DGF-FTP configuration namespace
// 	Using now only PacakgerOptimiser nested keys
// 	
// 	Revision 2.39  2016/04/11 13:37:56  marpas
// 	adopting new db interfaces
// 	
// 	Revision 2.38  2016/03/26 21:36:45  marpas
// 	using new db I/F
// 	
// 	Revision 2.37  2016/01/08 15:26:33  francesco.avanzi
// 	Class constructor now fully working.
// 	Useless methods removed.
// 	
// 	Revision 2.36  2015/11/27 10:59:15  francesco.avanzi
// 	minor compiling issue solved
// 	
// 	Revision 2.35  2015/11/27 10:57:17  francesco.avanzi
// 	metric_PACKAGE moved to metric_LINK
// 	
// 	Revision 2.34  2015/11/12 15:45:20  francesco.avanzi
// 	added MC message: now each product id is related to a pkgid
// 	
// 	Revision 2.33  2014/06/25 16:24:12  marpas
// 	coding best practices applied
// 	
// 	Revision 2.32  2014/04/08 16:50:24  marpas
// 	adopting libDateTime 5.13
// 	
// 	Revision 2.31  2014/03/10 15:00:33  francesco.avanzi
// 	better code practices applied
// 	
// 	Revision 2.30  2013/05/14 18:51:13  marpas
// 	no longer master/slave support through dbConnection
// 	
// 	Revision 2.29  2013/04/30 13:34:50  marpas
// 	dbPersistent interface changed
// 	
// 	Revision 2.28  2013/04/17 17:13:23  marpas
// 	new dbPersistent interface
// 	
// 	Revision 2.27  2013/04/16 15:49:26  marpas
// 	getDefaultConnectionPool call fixed
// 	
// 	Revision 2.26  2013/04/16 15:26:22  marpas
// 	fixing getDefaultCOnnectionPool call
// 	
// 	Revision 2.25  2013/03/14 18:01:27  marpas
// 	connectin pool got from Application
// 	
// 	Revision 2.24  2013/02/07 15:18:58  enrcar
// 	EC::
// 	Updated to handle the macro PDS_OLD_SCHEMA and the new schemas. Connections via connection-pool.
// 	
// 	Revision 2.23  2013/01/08 16:30:08  marfav
// 	Added support to format as link to task table id (numeric and not string)
// 	
// 	Revision 2.22  2012/01/16 14:13:23  enrcar
// 	EC:: Modified in order to keep the information of oversized items. This allows the creation of packages in status ERROR
// 	
// 	Revision 2.21  2011/05/04 13:54:21  enrcar
// 	EC:: explicit call to removeTemporaryTable _BEFORE_ calling the calll DTOR (in order to catch exceptions -- since DTOR would remove the temp.table hiding exceptions)
// 	
// 	Revision 2.20  2011/03/14 14:46:04  enrcar
// 	EC:: optimiseForSpace bug found: unable to select any list if the first product exceeded media capacity (this resulted in no package created). Fixed.
// 	Several methods now produce a BOOL in order to check if a proper value is returned
// 	
// 	Revision 2.19  2011/03/03 12:30:08  marpas
// 	ihash_map changed into map
// 	
// 	Revision 2.18  2011/02/08 15:39:11  enrcar
// 	EC:: UPDATE with "IN" parameters replaced with "IN_TABLE" temporary table
// 	
// 	Revision 2.17  2010/06/09 08:21:14  enrcar
// 	EC:: An iterator was erased  in a for cycle and the loop condition was based on the iterator itself. Potentially dangerous. Fixed.
// 	
// 	Revision 2.16  2009/09/28 17:03:38  marpas
// 	dbParam interface obsolecence
// 	
// 	Revision 2.15  2009/07/13 13:13:31  enrcar
// 	EC:: c++rules
// 	
// 	Revision 2.14  2009/06/12 15:08:14  enrcar
// 	EC:: modified in order to handle the cartId
// 	
// 	Revision 2.13  2009/03/24 14:22:04  enrcar
// 	EC:: c++rules
// 	
// 	Revision 2.12  2009/01/20 16:36:39  enrcar
// 	EC:: Added default values in order to avoid compilation warnings
// 	
// 	Revision 2.11  2008/11/12 17:09:57  marpas
// 	eless include removed
// 	
// 	Revision 2.10  2008/10/08 15:23:23  crivig
// 	*** empty log message ***
// 	
// 	Revision 2.9  2008/10/08 15:08:29  crivig
// 	porting to postgres8
// 	
// 	Revision 2.8  2008/04/23 11:59:30  enrcar
// 	EC:: exception handled incorrectly. Fixed
// 	
// 	Revision 2.7  2008/04/22 15:03:15  enrcar
// 	Transactions shortened. Ref. CRY-284
// 	
// 	Revision 2.6  2008/03/06 13:58:52  crivig
// 	EC:: Configuration file is no more loaded into the library
// 	
// 	Revision 2.5  2008/02/06 14:41:30  crivig
// 	EC:: A Warning is emitted if qualifier file is required and is not among items
// 	
// 	Revision 2.4  2008/01/31 09:57:25  enrcar
// 	EC:: getPriorityListFirstItemCheckReport, optimiseForNoRuleCheckReport, checkIfItemIsAReport, getReportIterator : Methods added.
// 	fileVersion, fileType, qualifierFile handled to be stored into ItemDesc's new interface (REPORT HANDLING)
// 	
// 


#include <poV.h>
#include <PackagerOptimiser.h>

#include <dbInventoryObject.h>
#include <dbFileType.h>
#include <dbPersistentFactoryChain.h>
#include <dbQuery.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <DateTime.h>
#
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <Application.h>

#include <dbConv.h>
#include <unistd.h>
#include <dbConnectionPool.h>
#include <dbDistribution.h>  
#include <dbDistributionItem.h> 
#include <dbDistributionItemQuery.h>
#include <dbDistributionItemExtendedQuery.h>
#include <dbDistributionItemExtended.h>

#include <dbGenQueryScopeGuard.h>
#include <dbPersistentScopeGuard.h>
#include <MCFilterables.h>
#include <Filterables.h>
#include <SQLString.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace __gnu_cxx;
using namespace std ;
namespace {
    poV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagerOptimiser);


/*! \class PackagerOptimiser::PackagerOptimiserException

	\brief exception class PackagerOptimiserException inherits from exException

*/
//exDEFINE_EXCEPTION(PackagerOptimiser,PackagerOptimiserException,exException) ;


PackagerOptimiser::ItemIdFilenFilev& PackagerOptimiser::ItemIdFilenFilev::operator=(const ItemIdFilenFilev &t)
{
    if(this != &t)
    {
        this->id_ = t.id_ ;
        this->fn_ = t.fn_ ;
        this->fv_ = t.fv_ ;
    }
    return *this ;
}

//------------------------------------------------------------------------
//
//                           PackagerOptimiser   
// 
//------------------------------------------------------------------------
PackagerOptimiser::PackagerOptimiser(	const string& mediaType, 
										const int& mediaFormatId, 
										const string& mediaFormat, 
										const int& packingAlgoId, 
										const unsigned long long int& mintr, const unsigned long long int& maxtr, 
                                        const DateTime& delay, const DateTime& stabilityTime, 
                                        const TypeReportFormats& reportFormats ) :
    _mediaType(mediaType), _mediaFormatId(mediaFormatId), _mediaFormat(mediaFormat), _packingAlgoId(packingAlgoId), 
	_mintr(mintr), _maxtr(maxtr), _delay(delay), 
    _stabilityTime(stabilityTime), _reportFormats(reportFormats), 
	_listOfUsers(), _listOfTodos(), _rs(), _stop()
{
	  
    _stop=false ;
     
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,">> mediaType: " << _mediaType ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,">> mediaFormatId: " << _mediaFormatId ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,">> mediaFormat: " << _mediaFormat ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,">> packingAlgoId: " << _packingAlgoId ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,">> mintr: " << _mintr ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,">> maxtr: " << _maxtr ) ;
    
    _listOfUsers.clear();
}


//------------------------------------------------------------------------
//
//                          ~PackagerOptimiser
//
//------------------------------------------------------------------------
PackagerOptimiser::~PackagerOptimiser() throw() 
{    
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB ) ; // PRQA S 4631 6
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"_stop status:" << getStopStatus() );
   
    eraseLists(_listOfUsers); 

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"DONE" );
}


//------------------------------------------------------------------------
//
//                               makeMainList()
//
//------------------------------------------------------------------------
void PackagerOptimiser::makeMainList()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    makeList(_listOfUsers);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"DONE." );
}


//------------------------------------------------------------------------
//
//                               eraseMainList()
//
//------------------------------------------------------------------------
void PackagerOptimiser::eraseMainList()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    eraseLists(_listOfUsers);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"DONE." );
}


//------------------------------------------------------------------------
//
//                                 makeList()
//
//------------------------------------------------------------------------
void PackagerOptimiser::makeList(TypeUserList& userList)
{
  
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );

    vector<bool> vecValidity ;
	bool tmpBool = false ; 
    int tmpInt = 0 ;

    
	dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() ) ;
	dbConnection &conn = pw ; // PRQA S 3050
    const string &dbKey = conn.getKey() ;
    // DistributionItem
    dbDistributionItemExtendedQuery* 
		queryDistItems=dynamic_cast<dbDistributionItemExtendedQuery*>(dbQueryFactoryChain::instance()->newObject("dbDistributionItemExtendedQuery", dbKey)); // PRQA S 3081, 4412
    if(!queryDistItems)
	{
         ACS_THROW(PackagerOptimiserException("Error in executing dynamic_cast for dbDistributionItemQuery")); // PRQA S 3081
    }    
    dbGenQueryScopeGuard queryDistItemsGuard(queryDistItems);


    eraseLists(userList) ;

    ACS_COND_THROW( not Application::exists() || (0 == Application::instance()->getDefaultConnectionPool()), exIllegalValueException("No default connection pool found")) ; // PRQA S 3081

	//  Query on T_DistributionItems
	////////////////////////////////////////////////////////////////////////////
	queryDistItems->clear();
	queryDistItems->resetParameters();

    dbQueryParameters DistrItemParameters(dbKey) ;
    DistrItemParameters.addParameter( dbParam(dbKey, "T_DistributionItems","MediaType", dbParam::EQUAL_TO, db::toSQL(dbKey, _mediaType)) ) ;
#ifndef PDS_OLD_SCHEMA
	DistrItemParameters.addParameter( dbParam(dbKey, "T_DistributionItems","Format", dbParam::EQUAL_TO, db::toSQL(dbKey, _mediaFormatId)) ) ;
	DistrItemParameters.addParameter( dbParam(dbKey, "T_DistributionItems","Disseminationpackingalgo_id", dbParam::EQUAL_TO, db::toSQL(dbKey, _packingAlgoId)) ) ;
#else
	DistrItemParameters.addParameter( dbParam(dbKey, "T_DistributionItems","Format", dbParam::EQUAL_TO, db::toSQL(dbKey, _mediaFormat)) ) ;
#endif	
	
	DistrItemParameters.addParameter( dbParam(dbKey, "T_DistributionItems","PackageId", dbParam::ISNULL) ) ;


	queryDistItems->addParameter(DistrItemParameters);
	queryDistItems->doQuery(conn);


	ACS_COND_THROW(0 != _stop, PackagerOptimiserException("Aborted for a stop") ); // PRQA S 3081
	
	
   
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ostringstream f ; 
#ifndef PDS_OLD_SCHEMA
		f << _mediaFormatId << " (" << _mediaFormat << ")" ;
#else
		f << _mediaFormat ;
#endif	
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query on items with UNSET PackageID for medium " << _mediaType 
		<< " format: " << f.str() << " - result size: " << queryDistItems->size());
    ACS_CLASS_END_DEBUG


    TypePriorityList* priorityList = 0;

    size_t di_size = queryDistItems->size() ;
    for (size_t i=0; i<di_size; ++i)
    {  
        ACS_COND_THROW (_stop, PackagerOptimiserException("Aborted for a stop")) ; // PRQA S 3081
    		
        // cast to dbDistributionItemExtended
        dbDistributionItemExtended *queryDistItemsExtended = (*queryDistItems)[i] ;

        // Get required parameters for current items
        dbPersistent::IdType Id = queryDistItemsExtended->getId();
	    string UserId = queryDistItemsExtended->getUserId();
        DateTime DueDate = queryDistItemsExtended->getDueDate();
        DateTime creationDate = queryDistItemsExtended->getCreationDate();
        string fileName = queryDistItemsExtended->getFileName();
        string fileVersion = queryDistItemsExtended->getFileVersion();
        string fileType = queryDistItemsExtended->getFileType();
		bool validity = queryDistItemsExtended->getValidityFlag();
	
        unsigned long long int cursize = ( queryDistItemsExtended->getHeaderSize() + queryDistItemsExtended->getDataSize()); // PRQA S 3084
        string qualifierFile = queryDistItemsExtended->getQualifierFile();
		string cartId = "" ;
		string subReq = "" ;
#ifndef ACS_PACKAGEROPTIMISER_CARTID_UNHANDLED
		tmpInt = queryDistItemsExtended->getCartId(&tmpBool) ;
		if (tmpBool)
		{
			ostringstream o ; o << tmpInt ; cartId = o.str() ; 
		}
		
		tmpInt = queryDistItemsExtended->getSubReq(&tmpBool) ;
		if (tmpBool)
		{
			ostringstream o ; o << tmpInt ; subReq = o.str() ; 
		}
#else
		#warning "Warning. libPackagerOptimiser::PackagerOptimier.C  Compiling without CartId Support"             
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "CARTID SUPPORT UNAVAILABLE" );
#endif

        if (!validity) 
        {
            // Skipped - Not valid
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Item " << Id << " skipped after ValidityFlag check." );
            continue ; 
        }

      
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Item Id: " << Id ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "UserId: !" << UserId << "!" ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "cartId: !" << cartId << "!" ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "subReq: !" << subReq << "!" ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "DueDate (as JD50): " << DueDate.jd50() ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "CreationDate (as JD50): " << creationDate.jd50() ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "fileName: " << fileName ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "fileVersion: " << fileVersion ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "fileType: " << fileType ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "total size: " << cursize ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "qualifierFile: " << qualifierFile ) ;


        ItemPartition itemPartition(UserId, cartId) ;	/* subReq is not used to create a partition */
        TypeUserList::iterator userList_iterator = userList.find(itemPartition);

      
        if (userList_iterator==userList.end())
        {
            // Not found:
            // Create a new multimap
            ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,  "ItemPartition NOT FOUND: a new priority-list is created:" );
                itemPartition.dumpItemPartition() ;
            ACS_CLASS_END_DEBUG
                    
            priorityList = new TypePriorityList ;
            // Associate priority multimap to user 
            
            userList[itemPartition] = priorityList ;  
        } 
        else
        {
            // Found:
            // Retrieve priority multimap associated to user 
            
            ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "ItemPartition FOUND: added to existing priority-list");
                itemPartition.dumpItemPartition() ;
                ItemPartition test = (userList_iterator->first) ;
                test.dumpItemPartition() ;
            ACS_CLASS_END_DEBUG
            priorityList = userList_iterator->second ;
        }
        
		ACS_COND_THROW(_stop, PackagerOptimiserException("Aborted for a stop")); // PRQA S 3081
    
        ItemDesc itemDesc(Id, cursize, creationDate.jd50(), fileName, fileVersion, fileType, qualifierFile, ItemDesc::ITEM_GOOD) ;    /* create ItemDesc object */
    
        /* Store ItemDesc object with DueDate as ordered key */
        priorityList->insert( TypePriorityList::value_type(DueDate.jd50(), itemDesc) );
   
        usleep(10000) ; // PRQA S 4400
    }   // For
    
	
    // DEBUG ONLY: Show list
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
        dumpUserList(userList) ;
    ACS_CLASS_END_DEBUG
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE." );
}


//------------------------------------------------------------------------
//
//                           dumpMainUserList()
//
//------------------------------------------------------------------------
// Dump _listOfUsers Internal User list (Use for debug only)
void PackagerOptimiser::dumpMainUserList()
{        
    ACS_LOG_INFO("******************************************** " ) ;
    dumpUserList(_listOfUsers) ;
    ACS_LOG_INFO("(END DUMP) ********************************** " );
}    


//------------------------------------------------------------------------
//
//                    dumpUserList(TypeUserList userList)
//
//------------------------------------------------------------------------
void PackagerOptimiser::dumpUserList(const TypeUserList &userList)
{        
	// Dump User list (Use for debug only)

    ACS_LOG_INFO(" ******************************************** " ) ;
    
    int c=0; 
    for (TypeUserList::const_iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {
    
        ItemPartition ip = (i->first) ; 
      
        ACS_LOG_INFO("####  User Number: " << (c++) << " with UserID: !" << ip.getUser() << "!  CartId: !" << ip.getCartId() << "!" );
        TypePriorityList* priorityList = i->second ;
        dumpPriorityList(*priorityList) ;
    
    }
        
    ACS_LOG_INFO( "(END DUMP) ********************************** " ) ;
}    


//------------------------------------------------------------------------
//
//                 dumpPriorityList(TypeUserList userList)
//
//------------------------------------------------------------------------
void PackagerOptimiser::dumpPriorityList(const TypePriorityList &priorityList)
{        
// Dump Priority list (Use for debug only)
   
    ACS_LOG_INFO(" ******************************************** " ) ;
    
    unsigned long long int sum = 0;
    
    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++)
    {
        ACS_LOG_INFO("T_DistributionItems_Id: " << (j->second).getItemId() << " Duedate: " << (j->first) <<  "  Size: " << (j->second).getItemSize() );
    }

    evalSumSpace(priorityList, sum);
    ACS_LOG_INFO("Totale Size for this List: " << sum );

    ACS_LOG_INFO("(END DUMP) ********************************** " ) ;
}   

 
//------------------------------------------------------------------------
//
//                    eraseEmptyUsers(TypeUserList& userList)
//
//------------------------------------------------------------------------
void PackagerOptimiser::eraseEmptyUsers(TypeUserList& userList)
{ 
// Check for users with an empty priority list, and clear them

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	for ( TypeUserList::iterator i, j = userList.begin();  (userList.begin()!=userList.end()) && (j!=userList.end()) ; )  // PRQA S 4238, 4107
    {
		i=j ;j++; // Prepare next loop
        
        ItemPartition ip = (i->first) ;
        string user = ip.getUser() ;
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Checking user: " << user );
        
        if ( i->second->empty() )
        {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "REMOVING USER: " << user );

            // Current user has an empty priority list:
            delete (i->second); // remove priority list
            userList.erase(i); // remove user
        }
    } 
    
   
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE" );

}


//------------------------------------------------------------------------
//
//                           eraseUserFromUserList
//
//------------------------------------------------------------------------
bool PackagerOptimiser::eraseUserFromUserList(TypeUserList& userList, ItemPartition& itemPartition) // PRQA S 4020
{ 
// Remove the UserList for a specific users, if exists. Otherwise does nothing.
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
    
    TypeUserList::iterator i = userList.find(itemPartition) ;

    string user = itemPartition.getUser() ;
      
    if (i==userList.end())
    {
        // Not found:
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "User "<< user <<" not yet inserted. Nothing to remove" );

        return false ;
    }   
       
        
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing user: " << user );


    delete (i->second); // remove priority list
    userList.erase(i); // remove user
    
   
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE" );
	
	return true ;
}


//------------------------------------------------------------------------
//
//                    eraseLists(TypeUserList& userList)
//
//------------------------------------------------------------------------
void PackagerOptimiser::eraseLists(TypeUserList& userList)
{        
// Erase User list, and delete[] the priority lists.
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    int c=0; 
    for (TypeUserList::iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {
    
        ItemPartition ip = (i->first) ; 
        string user = ip.getUser() ;
        
    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">>> Cleaning List for User Number: " << (c++) << " with UserID: " << user );

        TypePriorityList* priorityList = i->second ;
        priorityList->clear();
        delete priorityList ;
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">>> Clearing User List" );
   
    userList.clear();	// Clear User List
 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE." );
}    


//------------------------------------------------------------------------
//
//                               evalSmallestDueDate
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalSmallestDueDate(TypeUserList& userList, // IN: userList
                                            long double& date)
{
// All Priority Lists are checked:
// The Smallest date is taken
    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
    date = -1;   // Reset output
    
    bool firstTime = true ; // fist time running

    for (TypeUserList::iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {
        TypePriorityList *priorityList = i->second ;

        if (not priorityList->empty())
        {
            TypePriorityList::iterator j = priorityList->begin() ;

            long double curdate = j->first ;    // Get date
            if (firstTime || (curdate<date)) { date = curdate ; }

            firstTime = false ; // DON'T MOVE FROM THIS POSITION
        }
    }   
        
     
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
    {
        DateTime tmpDate = DateTime::JD50(date) ; // PRQA S 3081
        string tmpDate1 ;  
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. Date is: (" << date << ") " << tmpDate.utcs(tmpDate1) ) ;
    }
    ACS_CLASS_END_DEBUG
	
	return ( firstTime == false ) ;
}


//------------------------------------------------------------------------
//
//                               evalLargestDueDate
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalLargestDueDate( const TypeUserList& userList, // IN: userList
                                            long double& date)
{
// All Priority Lists are checked:
// The Largest date is taken

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
    date = -1;   // Reset output
    
    bool firstTime = true ; // fist time running

    for (TypeUserList::const_iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {
        const TypePriorityList *priorityList = i->second ;

        if (not priorityList->empty())
        {
            TypePriorityList::const_iterator j = priorityList->end() ;
            j--; // Get last element in list ;

            long double curdate = j->first ;    // Get date
            if (firstTime || (curdate>date)) { date = curdate ; }

            firstTime = false ; // DON'T MOVE FROM THIS POSITION
        }
    }   
        
     
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
    {
        DateTime tmpDate = DateTime::JD50(date) ; // PRQA S 3081
        string tmpDate1 ;  
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. Date is: (" << date << ") " << tmpDate.utcs(tmpDate1) ) ;
    }
    ACS_CLASS_END_DEBUG
	
	return ( firstTime == false ) ;
}


//------------------------------------------------------------------------
//
//                        evalLargestCreationDatePrList
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalLargestCreationDatePrList(  const TypePriorityList &priorityList,  // IN: priorityList
                                                        long double& date)                   // OUT: largest creation date
{
//Get the largest CreationDate in a PriorityList
  
    bool firstTime = true ; // fist time running
   
    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++) // PRQA S 4238
    {
        long double curdate = j->second.getItemCreationDate() ;

        if (firstTime || (curdate>date)) {date = curdate ; }
        firstTime = false ;
    }
   
   
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
    {
        DateTime tmpDate = DateTime::JD50(date) ; // PRQA S 3081
        string tmpDate1 ;  
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. date is: (" << date << ") " << tmpDate.utcs(tmpDate1)) ;
    }
    ACS_CLASS_END_DEBUG
	
	return ( firstTime == false ) ;
}


//------------------------------------------------------------------------
//
//                        getPriorityListTillSize
//
//------------------------------------------------------------------------
void PackagerOptimiser::getPriorityListTillSize(TypePriorityList& priorityList, // IN: priorityList
                                                unsigned long long int trsize,  // IN: trashold size
                                                TypePriorityList& outList)  // OUT: priorityList
{
//Remove elements from PriorityList, until the treshold is reached

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold size is:" << trsize );
    
    
    outList.clear(); // Reset output
    unsigned long long int sumsize = 0;

   
    for ( TypePriorityList::iterator i, j = priorityList.begin() ; (priorityList.begin()!=priorityList.end())&&(j!=priorityList.end()); ) // PRQA S 4238, 4107
    {
        i = j; j++; // Prepare next loop
        
        unsigned long long int cursize = (i->second).getItemSize() ;
        
        if ( (sumsize+cursize)<=trsize ) // PRQA S 3084
        {
            // Export (Move) element to output list:
            outList.insert(TypePriorityList::value_type(i->first, i->second));
            // Remove element from input list:
            priorityList.erase(i); 
            sumsize += cursize ; // sumsize is updated. // PRQA S 3084
        }
        // else:
            // Skip this element
            // (Loop is NOT interrupted, since a shorter file could be find yet...)
    } 
    
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE" );
   
}


//------------------------------------------------------------------------
//
//                               getPriorityList
//
//------------------------------------------------------------------------
void PackagerOptimiser::getPriorityList(TypePriorityList& priorityList, // IN: priorityList
                                        TypePriorityList& outList)  // OUT: priorityList
{
//Remove all elements from PriorityList

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
    
    outList.clear(); // Reset output

   
    for ( TypePriorityList::iterator i,j = priorityList.begin() ; (priorityList.begin()!=priorityList.end())&&(j!=priorityList.end()); ) // PRQA S 4238, 4107
    {
        i = j; j++; // Prepare next loop
        
        outList.insert(TypePriorityList::value_type(i->first, i->second));
        // Remove element from input list:
        priorityList.erase(i); 
    }
    
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE." );
   
}


//------------------------------------------------------------------------
//
//                        getPriorityListFirstItem
//
//------------------------------------------------------------------------
void PackagerOptimiser::getPriorityListFirstItem(   TypePriorityList& priorityList, // IN: priorityList
                                                    TypePriorityList& outList)  // OUT: priorityList
{
//Remove the first element (if any) from PriorityList

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
    
    outList.clear(); // Reset output

    if ( not priorityList.empty())
    {
        TypePriorityList::iterator i = priorityList.begin() ;
        outList.insert(TypePriorityList::value_type(i->first, i->second));
        priorityList.erase(i);
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "priorityList is empty." );
    }
   
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE" );
   
}


//------------------------------------------------------------------------
//
//                        getPriorityListFirstItemCheckReport
//
//------------------------------------------------------------------------
void PackagerOptimiser::getPriorityListFirstItemCheckReport(TypePriorityList& priorityList, // IN: priorityList // PRQA S 4020
                                                    		TypePriorityList& outList)  // OUT: priorityList
{
/* 
	Remove the first element (if any) from PriorityList.
	Also, check if there's an associated report and remove it */

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
    
    outList.clear(); // Reset output
   
    if (priorityList.empty())
 	{
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "priorityList is empty.");
    
		return ;	// priorityList empty
	}
   
   	// IF HERE: priorityList not empty

    for (TypePriorityList::iterator i=priorityList.begin(); i!=priorityList.end(); i++) // PRQA S 4238
	{

        ItemDesc item = i->second ;	/* Pick a item */

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Processing Item: " 
                                             << item.getItemId() << " / " 
                                             << item.getItemFileName() << " / " 
                                             << item.getItemFileVersion() );
    
        if ( ! checkIfItemIsAReport(item) )
        {
        	/*  ITEM IS NOT A REPORT. FURTHER CHECKS:  ***********************************/

     		if ( item.getItemQualifierFile() == "" )
            {
                /*  ITEM HAS NO QUALIFIER FILE. WILL BE DISTRIBUTED ***********************************/

                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Item: " 
                                                    << item.getItemId() << " / " 
                                                    << item.getItemFileName() << " / " << item.getItemFileVersion() << " / " 
                                                    << item.getItemFileType() << " has no qualifier file." );

				/* Stand-alone product -- will be distributed */
                outList.insert(TypePriorityList::value_type(i->first, i->second));
                priorityList.erase(i);

                break ;	/* Product found. Quitting "for" */
            }
     		else
            {
                /*  ITEM HAS A QUALIFIER FILE.  ***********************************/

                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Item: " 
                                                    << item.getItemId() << " / " 
                                                    << item.getItemFileName() << " / " << item.getItemFileVersion() << " / " 
                                                    << item.getItemFileType() << " has a qualifier file: !" 
                                                    << item.getItemQualifierFile() << "!" );

				TypePriorityList::iterator repIter = getReportIterator(priorityList, item) ;

				if ( repIter == priorityList.end() )
                {
                    /*  QUALIFIER FILE *NOT FOUND* AMONG DISTRIBUTIONITEMS. PRODUCT IS INVALID. SKIPPING. ***********************************/
    				/*  A Warning will be emitted to MCF */                

                    ACS_LOG_WARNING("Item Id: " << item.getItemId() << " / "
                                                << item.getItemFileName() << " / " 
                                                << item.getItemFileVersion() << " / " 
                                                << item.getItemFileType() << " has a qualifier file: " 
                                                << item.getItemQualifierFile() << " that cannot be found among items. PRODUCT SKIPPED.");

					continue ;	/* Skipping to next */
                }
				else
                {
                    /*  QUALIFIER FILE *FOUND* AMONG DISTRIBUTIONITEMS. DISTRIBUTING BOTH. ***********************************/

                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Item: " << item.getItemFileName() << " / " 
                                                                 << item.getItemFileVersion() << " / " 
                                                                 << item.getItemFileType() << " has a qualifier file: " << item.getItemQualifierFile() 
                                                                  << " FOUND with id: " << (repIter->second).getItemId() );


                    outList.insert(TypePriorityList::value_type(i->first, i->second));
                    outList.insert(TypePriorityList::value_type(repIter->first, repIter->second));
                    priorityList.erase(i);
                    priorityList.erase(repIter);

                    break ;	/* Product found. Quitting "for" */
				}
            }	/* if ( item.getItemQualifierFile() == "" ) */
        }
        else
        {
        	/*  ITEM IS A REPORT - SKIPPED  ***********************************/

            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Item: " << item.getItemId() << " / " 
                                                         << item.getItemFileName() << " / " 
                                                         << item.getItemFileVersion() << " / " 
                                                         << item.getItemFileType() << " IS A REPORT. SKIPPED." );

            ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Dumping Report Item:" );
            ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)    
                item.dumpItemDesc() ;
            ACS_CLASS_END_DEBUG

            continue ;	/* Skipping to next */

        }	/* if ( ! checkIfItemIsAReport(item) ) */

	}	/* for */

    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE" );
   
}


//------------------------------------------------------------------------
//
//                       checkIfItemIsAReport
//
//------------------------------------------------------------------------
bool	/* TRUE: If is a report */
		PackagerOptimiser::checkIfItemIsAReport(const ItemDesc& item)
{
// Check if an ItemDesc item is a report (i.e. its FileType is listed into config.file)
// If DistributionFormat cannot allow report, an exception will be raised
	
    TypeReportFormats::iterator rep ;

    rep = _reportFormats.find(_mediaFormat) ;

	bool found = false ;
    
    if ( rep == _reportFormats.end() )
    {
    	/* Internal error -- this method has been called, but Distribution Format doesn't allow reports. */
		ACS_THROW(PackagerOptimiserException("Internal Error: Format: " + _mediaFormat + " doesn't admit reports!")); // PRQA S 3081
    }
	else
    {
    	const vector <string> &fileTypes = rep->second ;
        size_t ft_size = fileTypes.size() ;
    	for (unsigned int i=0; i<ft_size; ++i)
        {
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Matching: !" << item.getItemFileType() << "! with: !" << fileTypes[i] << "!" );
        
        	if ( fileTypes[i] == item.getItemFileType() )
            {
                found = true ; 
                break ;
            }
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filetype:  Item: " 
                                                << item.getItemId() << " / " 
                                                << item.getItemFileName() << " / " << item.getItemFileVersion() << " / " 
                                                << item.getItemFileType() << " Return value: " << boolalpha << found );

	return found ;
}        


//------------------------------------------------------------------------
//
//                       getReportIterator
//
//------------------------------------------------------------------------
PackagerOptimiser::TypePriorityList::iterator 
		PackagerOptimiser::getReportIterator(TypePriorityList& priorityList, const ItemDesc& item)
{
// Return the iterator of the priorityList element which contains the report of a given item
	
    string itemQualifierFile = item.getItemQualifierFile() ;

	if ( itemQualifierFile == "" )
    {
      	/* Internal error -- this method has been called, but given item has no QualifierFile */
		ostringstream err ;
        err << "Internal Error: Item with id: " << item.getItemId() << " has an EMPTY QualifierFile field." ;
	
		ACS_THROW(PackagerOptimiserException ( err.str())) ; // PRQA S 3081
    }

	TypePriorityList::iterator outIterator = priorityList.end() ;	/* Prepare output value in case cannot find the report */
  
    for (TypePriorityList::iterator i=priorityList.begin(); i!=priorityList.end(); i++) // PRQA S 4238
    {		
        if ( (i->second).getItemFileName() == itemQualifierFile )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Found report " << itemQualifierFile << " with Id: " << (i->second).getItemId() );
               
        	outIterator = i ;
            break ;
        }
	}

    if ( outIterator == priorityList.end() )
    {
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Report " << itemQualifierFile << " Not found." );
    }

	return outIterator ;
}


//------------------------------------------------------------------------
//
//                       evalSumSpaceAfterTreshold
//
//------------------------------------------------------------------------
bool  PackagerOptimiser::evalSumSpaceAfterTreshold( const TypePriorityList &priorityList,  // IN: Priority List
                                                    unsigned long long int trs,               // IN: treshold
                                                    unsigned long long int& sum)              // IN: sum of sizes
{
// Check the SUM-Space soon after (> and not==) the treshold
// It's probably useless.... use evalSumSpaceBeforeTreshold instead 

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs );
   
    sum = 0 ; // Reset output value
   
	bool found = false ;
	
    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++) // PRQA S 4238
    {
    
        unsigned long long int size = (j->second).getItemSize() ;

        sum += size ; // PRQA S 3084
        
        if (sum>trs) { found = true ; break; }
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs << " -- SUM Size is " << sum );
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE.");
	
	return found ;
}


//------------------------------------------------------------------------
//
//                       evalSumSpaceBeforeTreshold
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalSumSpaceBeforeTreshold( const TypePriorityList &priorityList,  // IN: Priority List
                                                    unsigned long long int trs,               // IN: treshold
                                                    unsigned long long int& sum)              // IN: sum of sizes
{
// Check the SUM-Space before (<=) the treshold

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs );
   
    sum = 0 ; // Reset output value

    bool found = false ;
    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++) // PRQA S 4238
    {
        unsigned long long int size = (j->second).getItemSize() ;
        
        if ( (sum+size)<=trs ) { sum += size ; found = true ; } // PRQA S 3084
        // else: do nothing, skip to next element
    }
    
      
   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs << " -- SUM Size is " << sum ) ;
   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE." );
	
	return found ;
}


//------------------------------------------------------------------------
//
//                       evalLargestCreationDateBeforeTreshold
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalLargestCreationDateBeforeTreshold(  const TypePriorityList &priorityList,  // IN: Priority List // PRQA S 4020
                                                                unsigned long long int trs,     // IN: treshold
                                                                long double& date)              		// OUT: largest date
{
// Check the SUM-Space before (<=) the treshold
 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs);
   
    unsigned long long int sum = 0 ; // Reset output value
    bool firstTime = true ;
    long double curdate = 0 ;

    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++) // PRQA S 4238
    {
        unsigned long long int size = (j->second).getItemSize() ;
        curdate = (j->second).getItemCreationDate() ;

        if ( (sum+size)<=trs )  // PRQA S 3084
        {
            sum += size ; // PRQA S 3084

            if ( firstTime || (curdate>date)) { date = curdate ; }
            firstTime = false ;
        }
        // else: do nothing, skip to next element
    }

	if (firstTime) { return false ; }
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
    
	    DateTime tmpDate = DateTime::JD50(date) ;  // PRQA S 3081
        string tmpDate1 ;    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Treshold is: " << trs << " date is (" << date << ") " << tmpDate.utcs(tmpDate1) ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"DONE." ) ;
    
    ACS_CLASS_END_DEBUG
	
	return true ;
}


//------------------------------------------------------------------------
//
//                       evalSumSpaceAndLargestCreationDateBelowTreshold
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalSumSpaceAndLargestCreationDateBelowTreshold(const TypePriorityList& priorityList,  // IN: Priority List // PRQA S 4020
                                                                		unsigned long long int trs,     // IN: treshold
                                                                		unsigned long long int& sum,	// OUT: sum of sizes
                                                                		long double& date)              		// OUT: largest date
{
// Compute BOTH the largest creation date AND the occupation below (<=) the treshold
 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs );
   
    long double curdate = 0 ;
    bool firstTime = true ;

    sum = 0 ;	// Reset output value
    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++)
    {
        unsigned long long int size = (j->second).getItemSize() ;
        curdate = (j->second).getItemCreationDate() ;

        if ( (sum+size)<=trs ) // PRQA S 3084
        {
            sum += size ; // PRQA S 3084

            if ( firstTime || (curdate>date)) { date = curdate ; }
            firstTime = false ;
        }
        // else: do nothing, skip to next element
    }

	if (firstTime) { return false ; }
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
   
        DateTime tmpDate = DateTime::JD50(date) ; // PRQA S 3081
        string tmpDate1 ;    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Treshold is: " << trs << "  largest creation date is (" << date << ") " << tmpDate.utcs(tmpDate1) << "  Occupation is " << sum ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"DONE." ) ;
   
    ACS_CLASS_END_DEBUG
	
	return true ;
}

//------------------------------------------------------------------------
//
//               evalSumSpace(TypePriorityList priorityList)
//
//------------------------------------------------------------------------
bool PackagerOptimiser::evalSumSpace(const TypePriorityList &priorityList, unsigned long long int& sum)
{
// Evaluate SUM-Space in Priority List.
// See also evalSumSpaceBeforeTreshold and evalSumSpaceAfterTreshold

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
   
    sum = 0 ; // Reset output value
	bool found = false ;
	
    for (TypePriorityList::const_iterator j=priorityList.begin(); j!=priorityList.end(); j++) // PRQA S 4238
    {
        dbPersistent::IdType Id = (j->second).getItemId() ; // Get Id
        unsigned long long int size = (j->second).getItemSize() ;  // get Size
        
        sum += size ; // PRQA S 3084
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing Id: " << Id << " Size: " << size << " SUM Size (since now): " << sum );
    	
		found = true ;
	}
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. SUM Size is " << sum );
	
	return found ;
}


//------------------------------------------------------------------------
//
//                          optimiseForTresholdOnDate
//
//------------------------------------------------------------------------
void PackagerOptimiser::optimiseForTresholdOnDate(  TypeUserList& userList, // IN: userList // PRQA S 4020
                                                    unsigned long long int maxtr,     // IN: trashold size
                                                    const DateTime &delay,         // IN: delay 
                                                    TypePriorityList& outList)  // OUT: priorityList
{
// All Priority Lists are checked:
// The chosen list size (if any) must have the largest date *ABOVE* the treshold
 
// EnrCar::
// W8115_OFF
// This method has multiple returns in order to keep its complexity
// as low as possible


   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MAX Tr. is: " << maxtr );
    
  
    outList.clear() ; // Reset output
    
    bool firstTime = true ; // fist time running
    long double chosenDate=0 ;
    unsigned long long int chosenSize=0 ;  /* Useful only for debug */
    TypeUserList::iterator chosenList = userList.end();
    TypePriorityList priorityList ;           

    // The smallest date is taken among users.
    for (TypeUserList::iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {   
        
		// STOPPED			
		ACS_COND_THROW(0 != _stop, PackagerOptimiserException("Aborted for a stop")); // PRQA S 3081
	    

        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        { 
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Dumping userList: " );
            ItemPartition itemPartition = (i->first) ;
            itemPartition.dumpItemPartition() ;
        }
        ACS_CLASS_END_DEBUG

    
        priorityList = *(i->second) ;
        if ( not priorityList.empty() )
        {
            TypePriorityList::iterator j = priorityList.begin() ;

            long double curdate=j->first ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "curdate: " << curdate );
            
            unsigned long long int cursize = 0 ;
			bool found = evalSumSpaceBeforeTreshold(priorityList, maxtr, cursize) ; 


            if ( found && (firstTime || (curdate<chosenDate)) )
            {   // Take the 
                firstTime = false ;
                chosenList = i ;
                chosenDate = curdate ;
                chosenSize = cursize ;
            }

        }
    }
    
    if (firstTime)
    {
        // lists are empty
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List was empty. DONE." );
       
        return ;
    }
 
    DateTime sysdate ;  // Current Date
    sysdate += delay.jd50() ;
    long double sysdate_double = sysdate.jd50() ;

    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
       
        DateTime curDate ;  // Current Date
        long double double_curDate = curDate.jd50() ;
        DateTime tmpDate = DateTime::JD50(chosenDate) ; // PRQA S 3081    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"CURRENT DATE is: (" << double_curDate << ") " << curDate.asISO8601(6) ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Threshold is: (" << sysdate_double << ") " << sysdate.asISO8601(6) );
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Smaller date on list is: (" << chosenDate << ") " << tmpDate.asISO8601(6) ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Data size before Max treshold is: " << chosenSize ) ;
    
    ACS_CLASS_END_DEBUG

    if (sysdate_double<chosenDate)
    {
        // lists are empty
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DATE Treshold doesn't exceed. DONE." );
       
        return ;
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Optimisation will be performed..." );
    }
 
 
    // HERE IF: optimization required.
    
    getPriorityListTillSize(*(chosenList->second), maxtr, outList) ;

    eraseEmptyUsers(userList);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. outList size is: " << outList.size() );
}


//------------------------------------------------------------------------
//
//                              optimiseForSpace
//
//------------------------------------------------------------------------
void PackagerOptimiser::optimiseForSpace(   TypeUserList& userList, // IN: userList // PRQA S 4020
                                            unsigned long long int mintr,  // IN: trashold size
                                            unsigned long long int maxtr,  // IN: trashold size
                                            TypePriorityList& outList)  // OUT: priorityList
{
// All Priority Lists are checked:
// The chosen list size (if any) must be the HIGHTER VALUE between MINTR and MAXTR.
// -OR-
// A value below MINTR if the other files of the list make it exceed MAXTR.
  
// EnrCar::
// W8115_OFF
// This method has multiple returns in order to keep its complexity
// as low as possible


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MIN Tr. is: " << mintr << " MAX Tr. is: " << maxtr );
    
  
    outList.clear() ; // Reset output
    
    bool firstTime = true ; // fist time running
    bool smallList = false ; // if true, there is a list which is almost empty.
	bool listEmpty = true ;	// if TRUE all lists are empty
	bool oversizedItem = false ; // if TRUE at least one oversized item (i.e. even the smallest product doesn't fit in the medium) was found 
    unsigned long long int chosenSize=0 ;
    TypeUserList::iterator chosenList = userList.end() ;
    TypePriorityList priorityList ;           

    for (TypeUserList::iterator i=userList.begin(); (i!=userList.end()) ; i++) // PRQA S 4238
    {
 	    ACS_COND_THROW(0 !=_stop, PackagerOptimiserException("Aborted for a stop")); // PRQA S 3081
		
		if ( not priorityList.empty() )
		{
			listEmpty = false ;
            unsigned long long int cursize = 0 ;
            long double largestCreationDate  = 0 ;
			bool found = evalSumSpaceAndLargestCreationDateBelowTreshold(priorityList, maxtr, cursize, largestCreationDate) ;

			if (found)
			{
            	if (cursize<mintr)
            	{
                	unsigned long long int sum  = 0 ;
                	evalSumSpace(*(i->second), sum) ;
                	if (sum==cursize) { smallList = true ; }    // one list is almost empty
            	}

            	if ( cmpDateToTreshold(largestCreationDate, _stabilityTime) && ( firstTime || (cursize>chosenSize)) )
            	{   // Take the 
                	firstTime = false ;
                	chosenList = i ;
                	chosenSize = cursize ;
            	}
			}	/* if (found) */
			else if (firstTime)
			{
				// IF HERE: No other package was found so far (since firstTime is true) _AND_ this package exceeds medium capacity (since found is false)
				oversizedItem = true ;
				chosenList = i ;
				break ; 	/* A Oversized item was found. The scan must be stopped in order to preserve the value of chosenList */
			}
            else { /* nothing */ }
        } 
         
    }  	/* for (TypeUserList::iterator i=userList.begin(); i!=userList.end(); i++) */
    
    if (listEmpty)
    {
        // lists are empty
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List was empty. DONE." );
       
        return ;
    }
    
	
    if (oversizedItem)
    {
        // even the smallest file EXCEEDS the maximum trashold.
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "oooops, the smallest file EXCEEDS the maximum trashold.");

		/* The first item (even if larger than the threshold) will be moved to the output list */
    	getPriorityListFirstItem(*(chosenList->second), outList) ;

    	for ( TypePriorityList::iterator j = outList.begin() ; (j!=outList.end()); j++) // PRQA S 4238
    	{ j->second.setItemStatus(ItemDesc::ITEM_OVERSIZE) ; }	/* Item status is changed from GOOD to OVERSIZE */
		
    }
    else
	{
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Choosen size is: " << chosenSize );

    	if (chosenSize<mintr)
    	{
        	if (smallList == true) 
        	{
            	// There is ONE (or more) lists with few files! Total size above the minimum.
            	// So, operation is aborted.
            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "oo few files. DONE. MIN Tr. is: " << mintr << " Chosen size was: " << chosenSize );

            	return ;
        	}

        	// HERE IF:
        	// Size is BELOW the minimum trashold, but all lists have BIG remaing files, so maximum trashold would be exceeded.
    	}
		
    	// HERE IF: optimization required.

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Optimisation will be performed..." );


    	getPriorityListTillSize(*(chosenList->second), maxtr, outList) ;
		
	}

	// HERE WHEN: Optimization done.

    eraseEmptyUsers(userList);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. outList size is: " << outList.size() );
}


//------------------------------------------------------------------------
//
//                              optimiseForNoRule
//
//------------------------------------------------------------------------
void PackagerOptimiser::optimiseForNoRule(  TypeUserList& userList,     // IN: userList // PRQA S 4020
                                            TypePriorityList& outList)  // OUT: priorityList
{
// All Users are checked:
// The Priority List with lower duedate is used.

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
  
    outList.clear() ; // Reset output
    
    bool firstTime = true ; // fist time running
    long double chosenDate=0 ;
    TypeUserList::iterator chosenList = userList.end();
    TypePriorityList priorityList ;           

    // The smallest date is taken among users.
    for (TypeUserList::iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {
        
        ACS_COND_THROW(0 != _stop, PackagerOptimiserException("Aborted for a stop")); // PRQA S 3081
	    
        priorityList = *(i->second) ;

        if (not priorityList.empty())
        {   
            TypePriorityList::iterator j = priorityList.begin() ;

            long double curdate=j->first ;
            
            if ( firstTime || (curdate<chosenDate) )
            {   // Take the 
                firstTime = false ;
                chosenList = i ;
                chosenDate = curdate ;
            }
        } 
    }
    
    if (firstTime)
    {
        // lists are empty
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List was empty. DONE." );
       
        return ;
    }

 
    /*
    	WHEN HERE:
        chosenList is the iterator to the TypeUserList item with smaller dueDate
    */    
    
        
    getPriorityListFirstItem(*(chosenList->second), outList) ;

    eraseEmptyUsers(userList);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. outList size is: " << outList.size() );
}


//------------------------------------------------------------------------
//
//                              optimiseForNoRuleCheckReport
//
//------------------------------------------------------------------------
void PackagerOptimiser::optimiseForNoRuleCheckReport(	TypeUserList& userList,     // IN: userList // PRQA S 4020
                                                        TypePriorityList& outList)  // OUT: priorityList
{
// All Users are checked:
// The Priority List with lower duedate is used.
// If a product allow reports, the product-report couple is returned
  
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    
  
    outList.clear() ; // Reset output
    
    bool firstTime = true ; // fist time running
    long double chosenDate=0 ;
    TypeUserList::iterator chosenList = userList.end() ;

    // The smallest date is taken among users.
    for (TypeUserList::iterator i=userList.begin(); i!=userList.end(); i++) // PRQA S 4238
    {
        
		ACS_COND_THROW(0 != _stop, PackagerOptimiserException("Aborted for a stop")); // PRQA S 3081
	      
        TypePriorityList *priorityList = i->second ;

        if (not priorityList->empty())
        {   
            TypePriorityList::iterator j = priorityList->begin() ;

            long double curdate=j->first ;
            
            if ( firstTime || (curdate<chosenDate) )
            {   // Take the 
                firstTime = false ;
                chosenList = i ;
                chosenDate = curdate ;
            }
        } 
    }
    
    if (firstTime)
    {
        // lists are empty
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List was empty. DONE." );
       
        return ;
    }

 
    /*
    	WHEN HERE:
        chosenList is the iterator to the TypeUserList item with smaller dueDate
    */    
    
	if ( _reportFormats.find(_mediaFormat) != _reportFormats.end() )
	{
        /* 
            Current media format FOUND into Configuration File:
            This format handles report.
		*/
		getPriorityListFirstItemCheckReport(*(chosenList->second), outList) ;
	}
	else
    {
		/* 
            Current media format NOT FOUND into Configuration File:
            This format doesn't handle report.

            The same operation performed by rule: optimiseForNoRule
            should be executed.
        */
	    
        /* SAME AS RULE: optimiseForNoRule */
        getPriorityListFirstItem(*(chosenList->second), outList) ;
    }    


    eraseEmptyUsers(userList);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. outList size is: " << outList.size() );
}


//------------------------------------------------------------------------
//
//                              cmpDateToTreshold
//
//------------------------------------------------------------------------
bool                                                // O: true if system_time is "bigger" than date 
        PackagerOptimiser::cmpDateToTreshold(   const DateTime &date,  // IN: date      
                                                const DateTime &delay) // IN: delay 
{
// Given "date" is compared to system_time. If (system_time - date) > treshold, true is returned.
 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    return cmpDateToTreshold(date.jd50(), delay);
}


//------------------------------------------------------------------------
//
//                              cmpDateToTreshold
//
//------------------------------------------------------------------------
bool                                                // O: true if system_time is "bigger" than date 
        PackagerOptimiser::cmpDateToTreshold(   long double givenDate,  // IN: date      
                                                const DateTime &delay) // IN: delay 
{ 
// Given "date" is compared to system_time. If (system_time - date) > treshold, true is returned.
  
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    DateTime sysdate ;  // Current Date
    long double sysdate_double = sysdate.jd50();

    givenDate +=  delay.jd50();


    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
    {   
        DateTime tmpDate = DateTime::JD50(givenDate) ; // PRQA S 3081    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"CURRENT DATE is: (" << sysdate_double << ") " << sysdate.asISO8601(6) ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Threshold date is: (" << givenDate << ") " << tmpDate.asISO8601(6) ); 
    }
    ACS_CLASS_END_DEBUG


    return (sysdate_double>givenDate) ;
}


//------------------------------------------------------------------------
//
//                                 stop()  
// 
//------------------------------------------------------------------------
void PackagerOptimiser::stop()
{    
    // TODO: this message only in debug mode
    ACS_LOG_INFO("stop() method called" );

    _stop=true;
}


//------------------------------------------------------------------------
//
//                           processStatusOfItems
//
//------------------------------------------------------------------------
PackagerOptimiser::PackageStatus PackagerOptimiser::processStatusOfItems(const TypePriorityList& inList) const
{	
	PackageStatus dbStatus = PKG_ELIGIBLE ; 
    for ( TypePriorityList::const_iterator j = inList.begin() ; (j!=inList.end()); ++j)
    {
		if ( (j->second.getItemStatus() == ItemDesc::ITEM_OVERSIZE) || (j->second.getItemStatus() == ItemDesc::ITEM_ERROR) )
		{ 
            dbStatus = PKG_ERROR ; 
            break ; 
        }
    }

	return dbStatus ;
}


//------------------------------------------------------------------------
//
//                           performTablesUpdate
//
//------------------------------------------------------------------------
void PackagerOptimiser::performTablesUpdate(TypePriorityList& inList) // PRQA S 4020
{	
// EnrCar::
// W8115_OFF
// This method has multiple returns in order to keep its complexity
// as low as possible


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Started. Input list size " << inList.size() );

    
    if  (inList.empty())
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List is empty. DONE. " );
   
        return ;
    }

       
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Dump inList" ) ;
        dumpPriorityList(inList);
    ACS_CLASS_END_DEBUG
    
    DateTime curdate ;  // Current Date	
   	ostringstream msg ;

    
	dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() ) ;
	dbConnection &theConnection = pw ; // PRQA S 3050
    // DistributionQueue
    dbDistribution *nDistribution= dynamic_cast<dbDistribution*>(dbPersistentFactoryChain::instance()->newObject("dbDistribution", theConnection.getKey())); // PRQA S 3081, 4412
    ACS_COND_THROW( 0 == nDistribution, PackagerOptimiserException("Error in executing dbDistribution" )); // PRQA S 3081
    
	dbPersistentScopeGuard nDistributionGuard(nDistribution);

   
    ACS_COND_THROW(  not Application::exists() || (0 == Application::instance()->getDefaultConnectionPool() ), exIllegalValueException("No default connection pool found")) ; // PRQA S 3081
    
 
    ///////////////////////////////////////////
    //  Create new dbDistributionQueue record
    ///////////////////////////////////////////
    nDistribution->setStatus( packageStatusToString(PKG_TRANSIENT) ) ;
    nDistribution->setCreationDate(curdate); 
#ifndef PDS_OLD_SCHEMA
	nDistribution->setFormat(_mediaFormatId);
#else
    nDistribution->setFormat(_mediaFormat);
#endif


    ACS_COND_THROW( 0 != _stop, PackagerOptimiserException("Aborted for a stop")); // PRQA S 3081
    dbPersistent::IdType packageId=0 ;
   
	try 
    {			
        packageId = nDistribution->save(theConnection) ;	
	} 
    catch (exception &e) 
    {
        ACS_THROW(PackagerOptimiserException(e, "Error creating a new Package."));
    }

       
    DateTime curDateTime ; string tmpCurDate ;
    ACS_LOG_PRIVINFO("Created new Package: " << packageStatusToString(PKG_TRANSIENT) 
                << " dbDistributionQueue record with packageId: " << packageId );

    vector<ItemIdFilenFilev> itemsId ;
    
    for ( TypePriorityList::const_iterator j = inList.begin() ; (j!=inList.end()); ++j) // PRQA S 4238
    { 
        itemsId.push_back( ItemIdFilenFilev((j->second).getItemId() , (j->second).getItemFileName(), (j->second).getItemFileVersion() )) ; 
    }

	PackageStatus status = processStatusOfItems(inList) ;	// The final status of the package is obtained from the status of the items
    
    _listOfTodos[packageId] = make_pair (status, itemsId) ;

    inList.clear(); // List is cleaned-up (should be not really needed.... depends on optimiser implementation)

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. " );

}


//------------------------------------------------------------------------
//
//                           processToDoList
//
//------------------------------------------------------------------------
bool 											/* TRUE: ToDoList not empty yet -- FALSE: ToDoList empty */
		PackagerOptimiser::processToDoList() // PRQA S 4020
{	

// EnrCar::
// W8115_OFF
// This method has multiple returns in order to keep its complexity
// as low as possible


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Started. Input list size " << _listOfTodos.size() );


	if  (_listOfTodos.empty())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "List is empty. DONE. " );

		return false ;	/* Nothing to do */
	}

    ACS_COND_THROW(not Application::exists() || (Application::instance()->getDefaultConnectionPool() == 0), exIllegalValueException("No default connection pool found")) ; // PRQA S 3081
   
	dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() ) ;
	dbConnection &conn = pw ; // PRQA S 3050
    const string &dbKey = conn.getKey() ;
	for ( PackagerOptimiser::TypeToDoList::iterator i, j = _listOfTodos.begin();  ( (_listOfTodos.begin()!=_listOfTodos.end()) && (j!=_listOfTodos.end()) ); ) // PRQA S 4238, 4107
	{
		i=j ; j++; // Prepare next loop

		dbPersistent::IdType packageId = i->first ;
		pair < PackageStatus, vector<PackagerOptimiser::ItemIdFilenFilev> >& payload = i->second ; 

		if ( not payload.second.empty() && not _stop )
		{

			//////////////////////////////////////////////////
			//  Package will be associated to items
			//////////////////////////////////////////////////
			dbQueryParameters DistrItemParameters(dbKey) ;
			try 
			{	
				dbTemporaryTable ttable(conn) ;	/* Create temporary table (will be removed leaving the scope) */
                
                vector<ItemIdFilenFilev>::iterator it = payload.second.begin();
				vector<dbPersistent::IdType> v ;
                
                for(; it != payload.second.end() ; ++it)
                {
                    v.push_back(it->id_) ;
                } 
                
                string ttable_name = ttable.createTable( v ) ; // create and populate temp.table

				DistrItemParameters.addParameter(dbParam(dbKey, "T_DistributionItems","Id", dbParam::IN_TABLE, ttable_name)) ;


				SQLString sqlString( dbKey, SQLString::UPDATE ) ;
				sqlString.addTableList( dbDistributionItem::getTable() ) ;
				sqlString.addSetList( "packageid", db::toSQL(dbKey, packageId) );
				sqlString.addANDWhereParameter(DistrItemParameters) ;

				dbQuery updateItems(conn,sqlString, "UPD "+dbDistributionItem::getTable()) ;


				updateItems.execForStatus() ;

				DateTime curDateTime ;
				ACS_LOG_PRIVINFO(" Package: " << packageId << " assigned to items") ;
                
                {
                    vector<ItemIdFilenFilev>::const_iterator itt = payload.second.begin();

                    for(; itt !=payload.second.end() ; ++itt )
                    {
			            //APF 273
                        ACS_LOG_MC_INFO( mc::mc_metric (mc::Metric_LINK) 		<<
                        				 mc::mc_param("packageid", packageId ) 	<<
                                         mc::mc_param("filename", itt->fn_) 	<<
                                         mc::mc_param("fileversion", itt->fv_) ) ;
                    }
                }
				
                payload.second.clear() ; // Clear the vector containing the packages to be processed


				if (0 == updateItems.getAffectedRows())
				{
					// The package would be empty (no files in it)!!! 
					// (Probably a mistake in manual file removal)
					// So, will be aborted
					ostringstream err;
					err << "Error creating the Package: " << packageId << " Because all items removed!" ;
					
					ACS_THROW(PackagerOptimiserException(err.str()) ); // PRQA S 3081
				}

				ttable.removeTable() ;

			}
			catch (exception &e) 
			{        
				ostringstream err;
				err << "Can't assign to DB Package: " << packageId;
				PackagerOptimiserException nEx (e, err.str());
                ACS_THROW(nEx);
				ACS_LOG_NOTIFY_EX(nEx) ;
			}
		}
		else if ( not payload.second.empty() && _stop )
		{
			//////////////////////////////////////////////////////////////////////////////////////////
			//  Stop request / Not master and Package not yet associated to items -- Will be removed
			//////////////////////////////////////////////////////////////////////////////////////////

			try 
			{	

				dbQueryParameters distrQueueParameters(dbKey) ;
				distrQueueParameters.addParameter(dbParam(dbKey, "T_DistributionQueue","PackageId", dbParam::EQUAL_TO, db::toSQL(dbKey, i->first))) ;

				SQLString sqlString( dbKey, SQLString::DELETE ) ;
				sqlString.addTableList( "T_DistributionQueue" ) ;
				sqlString.addANDWhereParameter(distrQueueParameters) ;

				dbQuery updateItems(conn,sqlString, "DEL T_DistributionQueue") ;

				updateItems.execForStatus() ;

				DateTime curDateTime ; string tmpCurDate ;

				ACS_LOG_INFO( curDateTime.utcs(tmpCurDate) << " Package removed. Packageid: " << packageId) ;

				_listOfTodos.erase(i) ; /* delete iterator */

			}
			catch (exception &e) 
			{
				ACS_LOG_NOTIFY_EX(e);
                ACS_LOG_ERROR("Error updating package status. PackageId: " << packageId);
				
			}
        
		}
		else if ( payload.second.empty() )
		{
			////////////////////////////////////////////////////////////////////////////////
			//  Package associated to items but still TRANSIENT -- Status will be updated
			////////////////////////////////////////////////////////////////////////////////

			try 
			{	

				dbQueryParameters distrQueueParameters(dbKey);
				distrQueueParameters.addParameter(dbParam(dbKey, "T_DistributionQueue","PackageId", dbParam::EQUAL_TO, db::toSQL(dbKey, i->first))) ;

				PackageStatus status = payload.first ;

				SQLString sqlString( dbKey, SQLString::UPDATE ) ;
				sqlString.addTableList( "T_DistributionQueue" ) ;
				sqlString.addSetList( "status", db::toSQL(dbKey,  packageStatusToString(status) ) );
				sqlString.addANDWhereParameter(distrQueueParameters) ;

				dbQuery updateItems(conn,sqlString, "UPD T_DistributionQueue") ;

				updateItems.execForStatus() ;

				DateTime curDateTime ; string tmpCurDate ;

        
				ACS_LOG_INFO( curDateTime.utcs(tmpCurDate) << " Status updated as \"" << packageStatusToString(status) << "\" on T_DistributionQueue packageid " << i->first ) ;

				if ( status == PKG_ERROR )
				{
					ostringstream o ; 
					o << "Package " << i->first << " was created directly in status ERROR: probably one or more items associated to this package are irregular (e.g. oversize)" ;
					ACS_LOG_TRYCATCHNOTIFY_EX( exCriticalException(o.str()) ) ; // PRQA S 3081
				}

				_listOfTodos.erase(i) ; /* delete iterator */
				
			}
			catch (exception &e) 
			{
				ostringstream err;
				err << "Error updating package status. PackageId: " << packageId ;
				PackagerOptimiserException exc(e, err.str());
				exc.notify() ;
			}

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. " );


		}   /* if ( (i->second).size() ) */
        else { /* nothing */ }
	}	/* for ( i = _listOfTodos.begin() ; (i!=_listOfTodos.end()); i++) */


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE. Status is: " << boolalpha <<  not _listOfTodos.empty() );
    
	return ( _listOfTodos.size() != 0) ;	/* TRUE means that there's still work to do */	
}


//------------------------------------------------------------------------
//
//                           getFormatReportMap
//
//------------------------------------------------------------------------
void PackagerOptimiser::getFormatReportMap(  map< string, vector< string > >  & ftmap)
{

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ftmap.clear () ;	/* clean-up input map */

	rsResourceSet rset(ConfigurationSingleton::instance()->get()) ;

    rsPushNSpace FlatFtpTreeBuilderNS(rset,"PackagerOptimiser") ;	/* From conf-file: PacakgerOptimiser */

    {
        rsPushNSpace ReportFormatsNS(rset,"List_of_ReportFormats") ;
#warning MP use getArray instead
        size_t numReports = rset.getArraySize("ReportFormat"); // PRQA S 4412

   		for (size_t nr=0; nr<numReports; ++nr)
        {

            // set the ReportFormat push name		
            ostringstream nr_nameSpace;
            nr_nameSpace << "ReportFormat[" << nr << "]";
            string nr_pushName = nr_nameSpace.str();

			{
                /* Set into ReportFormat[nr] */
                rsPushNSpace ReportFormatsNS1(rset, nr_pushName) ;	

                string format ;
                rset.getValue("Format", format);		

				rsPushNSpace FileTypesNS(rset,"List_of_Filetypes") ;
#warning MP use getArray instead
                size_t numFiletypes = rset.getArraySize("Filetype"); // PRQA S 4412

    			vector <string> fileTypesV ;

                for (size_t ft=0; ft<numFiletypes; ++ft)
                {
                    // set the Fileype push name		
                    ostringstream ft_nameSpace;
                    ft_nameSpace << "Filetype[" << ft << "]";
                    string ft_pushName = ft_nameSpace.str();

                    {
                        /* Set into Filetype[ft] */
                        rsPushNSpace FileTypesNS1(rset, ft_pushName) ;	

                        string filetype ;
                        rset.getValue("Type", filetype);	

                        fileTypesV.push_back(filetype) ;

                    }	/* Filetype[ft] */ 


            	}	/* for ft */

                /* Store into map */
                ftmap[format] = fileTypesV ;

            }	/* ReportFormat[nr] */


        }	/* for nr */

		
        ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		{
            map< string, vector< string > >::iterator i ;
        
            for (i=ftmap.begin(); i!=ftmap.end(); i++)
            {
            	ACS_LOG_DEBUG("Format: " << i->first );
                
                const vector <string> &ft = i->second ;
            	size_t ft_size = ft.size() ;
                for (size_t j=0; j<ft_size; ++j)
                {
                    ACS_LOG_DEBUG("\tFiletype: " << ft[j] ) ; 
            	}
            }
            
        
        }
        ACS_CLASS_END_DEBUG

    }

    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Finished. " );

}

//------------------------------------------------------------------------
//		ItemPartitionHash::operator()  
//------------------------------------------------------------------------
int PackagerOptimiser::ItemPartitionHash::operator()(ItemPartition &element) const
{ return element.hashValue(); }
 
//------------------------------------------------------------------------
//		eqItemPartition::operator()  
//------------------------------------------------------------------------
bool PackagerOptimiser::eqItemPartition::operator()(const ItemPartition &s1, const ItemPartition &s2) const
{ return (s1==s2); }
		
//------------------------------------------------------------------------
//		eqItemPartition::operator()  
//------------------------------------------------------------------------
bool PackagerOptimiser::lessItemPartition::operator()(const ItemPartition &s1, const ItemPartition &s2) const
{ return (s1<s2); }
		
//------------------------------------------------------------------------
//		getParameter_mediaType()
//------------------------------------------------------------------------
const string &PackagerOptimiser::getParameter_mediaType()  const throw() // PRQA S 4120
{ return _mediaType ; }

//------------------------------------------------------------------------
//		getParameter_mediaFormatId()
//------------------------------------------------------------------------
int PackagerOptimiser::getParameter_mediaFormatId()  const throw() // PRQA S 4120
{ return _mediaFormatId ; }

//------------------------------------------------------------------------
//		getParameter_mediaFormat()
//------------------------------------------------------------------------
const string &PackagerOptimiser::getParameter_mediaFormat() const throw() // PRQA S 4120
{ return _mediaFormat ; }

//------------------------------------------------------------------------
//		getParameter_packingAlgoId()
//------------------------------------------------------------------------
int PackagerOptimiser::getParameter_packingAlgoId() const throw() // PRQA S 4120
{ return _packingAlgoId ; }

//------------------------------------------------------------------------
//		getParameter_mintr()
//------------------------------------------------------------------------
unsigned long long int PackagerOptimiser::getParameter_mintr() const throw()  // PRQA S 4120
{ return _mintr ; }

//------------------------------------------------------------------------
//		getParameter_maxtr()
//------------------------------------------------------------------------
unsigned long long int PackagerOptimiser::getParameter_maxtr() const throw() // PRQA S 4120
{ return _maxtr ; }

//------------------------------------------------------------------------
//		getParameter_delay()
//------------------------------------------------------------------------
const DateTime &PackagerOptimiser::getParameter_delay() const throw() // PRQA S 4120
{ return _delay ; }

//------------------------------------------------------------------------
//		getParameter_stabilityTime()
//------------------------------------------------------------------------
const DateTime &PackagerOptimiser::getParameter_stabilityTime() const throw() // PRQA S 4120
{ return _stabilityTime ; }

//------------------------------------------------------------------------
//		getStopStatus()
//------------------------------------------------------------------------
bool PackagerOptimiser::getStopStatus() const throw()  // PRQA S 4120
{ return _stop; } 



_ACS_END_NAMESPACE
