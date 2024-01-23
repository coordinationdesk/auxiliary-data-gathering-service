// PRQA S 1050 EOF
// 
// 	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
// 	$Prod: A.C.S. PackagerOptimiser_OneItem Class $
// 
// 	$Id$
// 
// 	$Author$
// 


#include <PackagerOptimiser_OneItem.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


//------------------------------------------------------------------------
//
//                           PackagerOptimiser_OneItem   
// 
//------------------------------------------------------------------------
PackagerOptimiser_OneItem::PackagerOptimiser_OneItem(
		const string &mediaType, const int &mediaFormatId, const string &mediaFormat, const int& packingAlgoId, 
		const unsigned long long int& mintr, const unsigned long long int& maxtr,  
		const DateTime& delay, const DateTime& stabilityTime, const TypeReportFormats& reportFormats) :
	PackagerOptimiser(	mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, 
						delay, stabilityTime, reportFormats)
{

	/*
		Comment by ENRCAR::
		In the previous version of the code (PDS_OLD_SCHEMA) the name of the optimiser class was hardwired in the code, called by a LUT.
		In order to prevent to use this optimiser, which is unable to match a product with its reports, the WARNING BELOW was issued at runtime.
		The idea was to ALWAYS use the class PackagerOptimiser_OneItem_PlusRpt, which can be configured either to match 
		a product with the report, or not (it's backward-compatible, it can be configured to behave like this simpler class)
		
		With the new version of the code (! PDS_OLD_SCHEMA) is easier to change the optimiser, just setting the value of the DB column
		t_distribyutionitems.disseminationpackingalgo_id
		
		Therefore, if a report is not needed, this class can be used without the need to receive any warning
	*/

#ifdef PDS_OLD_SCHEMA
	ACS_LOG_WARNING(exDebugSignature << "WARNING: USAGE OF THIS METHOD IS DEPRECATED - " 
        << "PackagerOptimiser_OneItem_PlusRpt SHOULD BE USED, IN ORDER TO PARSE THE CONFIGURATIONFILE" ) ;
#endif
}


//------------------------------------------------------------------------
//
//                          ~PackagerOptimiser_OneItem
//
//------------------------------------------------------------------------
PackagerOptimiser_OneItem::~PackagerOptimiser_OneItem() throw()
{    
  
}


//------------------------------------------------------------------------
//
//                               doOptimise()
//
//------------------------------------------------------------------------
void PackagerOptimiser_OneItem::doOptimise() 
{ 

    TypePriorityList outList ;
    bool quit = false ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"distributionFormat: " << _mediaFormat << " mediaType: " << _mediaType ) ;
 
    
    do // PRQA S 4236
    { 
       	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Running optimiseForNoRule" ) ;
      
        quit=true;
        
        // If an exception is got, will be catched above. So, current media is aborted 
        optimiseForNoRule(_listOfUsers, outList) ;

        if ( not outList.empty() ) 
        {
            quit=false ;
            ACS_LOG_INFO( "ITEM FOUND distributionFormat: " << _mediaFormat << " mediaType: " << _mediaType << " Found " << outList.size() << " items." );
            try
            {
                performTablesUpdate(outList) ;
            }
            catch (exception &e) 
            {
                ACS_LOG_WARNING( "Problem in TablesUpdate for mediaType: " << _mediaType << " Caused by: " << e);
			}
        } // if ( outList.size() ) 
    } while (!quit) ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "mediaType: " << _mediaType << " DONE.") ;

    return ;
}


_ACS_END_NAMESPACE
