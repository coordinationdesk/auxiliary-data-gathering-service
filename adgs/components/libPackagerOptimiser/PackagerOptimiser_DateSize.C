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
// 	$Prod: A.C.S. PackagerOptimiser_DateSize Class $
// 
// 	$Id$
// 
// 	$Author$
// 


#include <PackagerOptimiser_DateSize.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


//------------------------------------------------------------------------
//
//                           PackagerOptimiser_DateSize   
// 
//------------------------------------------------------------------------
PackagerOptimiser_DateSize::PackagerOptimiser_DateSize(	
		const string &mediaType, const int &mediaFormatId, const string &mediaFormat, const int& packingAlgoId, 
		const unsigned long long int& mintr, const unsigned long long int& maxtr,  
		const DateTime& delay, const DateTime& stabilityTime, const TypeReportFormats& reportFormats) :
	PackagerOptimiser(	mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, 
                		delay, stabilityTime, reportFormats)
{
}


//------------------------------------------------------------------------
//
//                          ~PackagerOptimiser_DateSize
//
//------------------------------------------------------------------------
PackagerOptimiser_DateSize::~PackagerOptimiser_DateSize() throw()
{    
  
}


//------------------------------------------------------------------------
//
//                             doOptimise         
//
//------------------------------------------------------------------------
void PackagerOptimiser_DateSize::doOptimise()
{ 
    TypePriorityList outList ;
    bool quit = false ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "distributionFormat: " << _mediaFormat << " mediaType: " 
		<< _mediaType << " mintr: " << _mintr << " maxtr: " << _maxtr );
 
    
    do // PRQA S 4236
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running optimiseForTresholdOnDate" );
      
        quit=true;         
        // If an exception is got, will be catched above. So, current media is aborted 
        optimiseForTresholdOnDate(_listOfUsers, _maxtr, _delay, outList) ;
        
        
        if ( not outList.empty() ) 
        {
            quit=false ;
            ACS_LOG_INFO("DATE TRESHOLD. distributionFormat: " << _mediaFormat << " mediaType: " << _mediaType << " Found " << outList.size() << " items." ) ;
            try
            {
                performTablesUpdate(outList) ;
            }
            catch (exception &e) 
            {
                ACS_LOG_WARNING("PackagerOptimiser_DateSize Problem in TablesUpdate for mediaType: " << _mediaType << " Caused by: " << e );
			}
        } // if ( outList.size() ) 
        
    } while (!quit) ;

   
    do // PRQA S 4236
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running optimiseForSpace" );
      
        quit=true;
        // If an exception is got, will be catched above. So, current media is aborted 
        optimiseForSpace(_listOfUsers, _mintr, _maxtr, outList) ;

        if ( not outList.empty() ) 
        {
            quit=false ;
            ACS_LOG_INFO("SPACE TRESHOLD.  distributionFormat: " << _mediaFormat << " mediaType: " << _mediaType << " Found " << outList.size() << " items.") ;
            try
            {
                performTablesUpdate(outList) ;
            }
            catch (exception &e) 
            {
                ACS_LOG_WARNING("Problem in TablesUpdate for mediaType: " << _mediaType << " Caused by: " << e );
			}
        } // if ( outList.size() ) 
    } while (!quit) ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "mediaType: " << _mediaType << " DONE.");

    return ;  
}


_ACS_END_NAMESPACE
