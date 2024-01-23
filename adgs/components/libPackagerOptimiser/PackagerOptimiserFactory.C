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
// 	$Prod: A.C.S. PackagerOptimiser Factory Class $
// 
// 	$Id$
// 
// 	$Author$
// 	
//  $Log$
//  Revision 2.4  2014/05/13 16:02:09  lucio.pulvirenti
//  S2PDGS-760: New debug macros used.
//
//  Revision 2.3  2013/02/07 15:08:10  enrcar
//  EC:: Updated to handle the new interface of PackagerOptimiser
//
//


#include <PackagerOptimiserFactory.h>
#include <PackagerOptimiserSingleton.h>

#include <PackagerOptimiser_DateSize.h>
#include <PackagerOptimiser_OneItem.h>
#include <PackagerOptimiser_OneItemPlusRpt.h>


using namespace std;
using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagerOptimiserFactory);


// *********************  PackagerOptimiserFactory  *********************
PackagerOptimiserFactory::PackagerOptimiserFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START.");

	/* Register all the available factories */
	registerPackagerOptimiserFactoryMethod( "PackagerOptimiser_DateSize" , &buildPackagerOptimiser_DateSize );
	registerPackagerOptimiserFactoryMethod( "PackagerOptimiser_OneItem" , &buildPackagerOptimiser_OneItem );    
	registerPackagerOptimiserFactoryMethod( "PackagerOptimiser_OneItemPlusRpt" , &buildPackagerOptimiser_OneItemPlusRpt );  
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END.");

}


// *********************  ~PackagerOptimiserFactory  *********************
PackagerOptimiserFactory::~PackagerOptimiserFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START."); // PRQA S 4631 L1
	
	/* Unregister all the available factories */
	unregisterPackagerOptimiserFactoryMethod("PackagerOptimiser_DateSize" , &buildPackagerOptimiser_DateSize );
	unregisterPackagerOptimiserFactoryMethod("PackagerOptimiser_OneItem" , &buildPackagerOptimiser_OneItem );   
	unregisterPackagerOptimiserFactoryMethod("PackagerOptimiser_OneItemPlusRpt" , &buildPackagerOptimiser_OneItemPlusRpt ); 
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END.");
    
    // PRQA L:L1

}
										

// *********************  buildPackagerOptimiser_DateSize  *********************
PackagerOptimiser* PackagerOptimiserFactory::buildPackagerOptimiser_DateSize(
						const string& mediaType, const int& mediaFormatId, const string& mediaFormat, 
						const int& packingAlgoId, const unsigned long long int& mintr, 
                        const unsigned long long int& maxtr, const DateTime& delay, const DateTime& stabilityTime,
                        const PackagerOptimiser::TypeReportFormats& reportFormats)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	/* Return a pointer to PackagerOptimiser_DateSize */	
	return new PackagerOptimiser_DateSize(
    				mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, delay, stabilityTime, reportFormats) ;
}


// *********************  buildPackagerOptimiser_OneItem  *********************
PackagerOptimiser* PackagerOptimiserFactory::buildPackagerOptimiser_OneItem(
						const string& mediaType, const int& mediaFormatId, const string& mediaFormat, 
						const int& packingAlgoId, const unsigned long long int& mintr, 
                        const unsigned long long int& maxtr, const DateTime& delay, const DateTime& stabilityTime,
                        const PackagerOptimiser::TypeReportFormats& reportFormats)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	/* Return a pointer to PackagerOptimiser_OneItem */	
	return new PackagerOptimiser_OneItem(
    				mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, delay, stabilityTime, reportFormats) ;

}


// *********************  buildPackagerOptimiser_OneItemPlusRpt  *********************
PackagerOptimiser* PackagerOptimiserFactory::buildPackagerOptimiser_OneItemPlusRpt(
						const string& mediaType, const int& mediaFormatId, const string& mediaFormat, 
						const int& packingAlgoId, const unsigned long long int& mintr, 
                        const unsigned long long int& maxtr, const DateTime& delay, const DateTime& stabilityTime,
                        const PackagerOptimiser::TypeReportFormats& reportFormats)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	/* Return a pointer to PackagerOptimiser_OneItemPlusRpt */	
	return new PackagerOptimiser_OneItemPlusRpt(
    				mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, delay, stabilityTime, reportFormats) ;
}

