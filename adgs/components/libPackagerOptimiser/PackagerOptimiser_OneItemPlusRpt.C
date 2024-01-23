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
// 	$Prod: A.C.S. PackagerOptimiser_OneItemPlusRpt Class $
// 
// 	$Id$
// 
// 	$Author$
// 
// 	$Log$
// 	Revision 1.4  2014/05/13 16:07:57  lucio.pulvirenti
// 	S2PDGS-760: New debug macros used. Catch(...) removed.
// 	
// 	Revision 1.3  2013/02/07 15:20:55  enrcar
// 	EC:: Updated to handle the new PackagerOptimiser interface
// 	
// 	Revision 1.2  2009/07/13 13:11:56  enrcar
// 	EC:: c++rules
// 	
// 	Revision 1.1  2008/01/31 09:49:53  enrcar
// 	EC:: Imported source
// 	
// 


#include <PackagerOptimiser_OneItemPlusRpt.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)



using namespace std ;


//------------------------------------------------------------------------
//
//                           PackagerOptimiser_OneItemPlusRpt   
// 
//------------------------------------------------------------------------
PackagerOptimiser_OneItemPlusRpt::PackagerOptimiser_OneItemPlusRpt(	
		const string &mediaType, const int &mediaFormatId, const string &mediaFormat, const int& packingAlgoId, 
		const unsigned long long int& mintr, const unsigned long long int& maxtr,  
		const DateTime& delay, const DateTime& stabilityTime, const TypeReportFormats& reportFormats) :
	PackagerOptimiser(	mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, 
                		delay, stabilityTime, reportFormats)
{
}


//------------------------------------------------------------------------
//
//                          ~PackagerOptimiser_OneItemPlusRpt
//
//------------------------------------------------------------------------
PackagerOptimiser_OneItemPlusRpt::~PackagerOptimiser_OneItemPlusRpt() throw()
{    
  
}


//------------------------------------------------------------------------
//
//                               doOptimise()
//
//------------------------------------------------------------------------
void PackagerOptimiser_OneItemPlusRpt::doOptimise() 
{ 

    TypePriorityList outList ;
    bool quit = false ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"distributionFormat: " << _mediaFormat << " mediaType: " << _mediaType);
  
    
    do // PRQA S 4236
    { 
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Running optimiseForNoRule" ) ;
      
        quit=true;
        
        // If an exception is got, will be catched above. So, current media is aborted 
        optimiseForNoRuleCheckReport(_listOfUsers, outList) ;

        if ( not outList.empty() ) 
        {
            quit=false ;
            ACS_LOG_INFO("ITEM FOUND distributionFormat: " << _mediaFormat << " mediaType: " << _mediaType << " Found " << outList.size() << " items." ) ;
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


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"mediaType: " << _mediaType << " DONE." ) ;

    return ;
}

_ACS_END_NAMESPACE
