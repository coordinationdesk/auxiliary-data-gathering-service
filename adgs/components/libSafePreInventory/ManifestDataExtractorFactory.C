// PRQA S 1050 EOF
/*

	Copyright 2013-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/11/13 17:19:01  lucio.pulvirenti
	Filterables include added and exDebug removed.
	
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	

*/


#include <ManifestDataExtractorFactory.h>

#include <ManifestDataExtractorSingleton.h>
#include <S3AuxManifestDataExtractor.h>
#include <S3ProdManifestDataExtractor.h>
#include <S1ProdManifestDataExtractor.h>
#include <S1AuxManifestDataExtractor.h>

#include <Filterables.h>

using namespace acs;
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ManifestDataExtractorFactory);

ManifestDataExtractorFactory::ManifestDataExtractorFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");

	ManifestDataExtractorSingleton::instance()->registerFactory("S3A_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->registerFactory("S3B_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->registerFactory("S3C_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->registerFactory("S3D_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->registerFactory("S3__AUX",&buildS3AuxManifestDataExtractor ) ;	
	
	ManifestDataExtractorSingleton::instance()->registerFactory("S3A_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S3B_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S3C_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S3D_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S3__PROD",&buildS3ProdManifestDataExtractor ) ;

	ManifestDataExtractorSingleton::instance()->registerFactory("S1A_PROD",&buildS1ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S1B_PROD",&buildS1ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S1__PROD",&buildS1ProdManifestDataExtractor ) ;

	ManifestDataExtractorSingleton::instance()->registerFactory("S1A_AUX",&buildS1AuxManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S1B_AUX",&buildS1AuxManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->registerFactory("S1__AUX",&buildS1AuxManifestDataExtractor ) ;


	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");
}


ManifestDataExtractorFactory::~ManifestDataExtractorFactory()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
			
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3A_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3B_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3C_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3D_AUX",&buildS3AuxManifestDataExtractor ) ;	
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3__AUX",&buildS3AuxManifestDataExtractor ) ;	

	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3A_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3B_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3C_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3D_PROD",&buildS3ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S3__PROD",&buildS3ProdManifestDataExtractor ) ;

	ManifestDataExtractorSingleton::instance()->unregisterFactory("S1A_PROD",&buildS1ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S1B_PROD",&buildS1ProdManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S1__PROD",&buildS1ProdManifestDataExtractor ) ;
	
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S1A_AUX",&buildS1AuxManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S1B_AUX",&buildS1AuxManifestDataExtractor ) ;
	ManifestDataExtractorSingleton::instance()->unregisterFactory("S1__AUX",&buildS1AuxManifestDataExtractor ) ;

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "End");

}

// *************** buildS3AuxManifestDataExtractor ***************************
ManifestDataExtractor* ManifestDataExtractorFactory::buildS3AuxManifestDataExtractor()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
	
	return new S3AuxManifestDataExtractor();
}

// *************** buildS3ProdManifestDataExtractor ***************************
ManifestDataExtractor* ManifestDataExtractorFactory::buildS3ProdManifestDataExtractor()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");

	return new S3ProdManifestDataExtractor();
}

// *************** buildS1ProdManifestDataExtractor ***************************
ManifestDataExtractor* ManifestDataExtractorFactory::buildS1ProdManifestDataExtractor()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");

	return new S1ProdManifestDataExtractor();
}

// *************** buildS1AuxManifestDataExtractor ***************************
ManifestDataExtractor* ManifestDataExtractorFactory::buildS1AuxManifestDataExtractor()
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");

	return new S1AuxManifestDataExtractor();
}
