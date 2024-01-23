// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$
	
	$Log$
	Revision 2.20  2017/11/23 13:28:08  giucas
	Added Csg key as copy of Cosmo.
	
	Revision 2.19  2015/11/30 09:33:27  giucas
	Updated S5P section
	
	Revision 2.18  2014/11/06 10:57:26  giucas
	Added Sentinel5P
	
	Revision 2.17  2014/03/26 09:57:37  marpas
	coding best practices applied
	warnings fixed (qa & g++)
	
	Revision 2.16  2014/02/05 15:42:25  giucas
	Added include Filterables
	
	Revision 2.15  2013/12/05 10:02:11  giucas
	Call to static singleton factory methods directly
	
	Revision 2.14  2011/10/28 07:09:19  giucas
	Factory updated for S2 and S3
	
	Revision 2.13  2011/10/07 08:22:38  giucas
	Factory registration updated
	
	Revision 2.12  2011/05/04 14:11:26  giucas
	Updated for sentinel missions
	
	Revision 2.11  2009/11/19 16:26:43  giucas
	New OS2 keys added.
	
	Revision 2.10  2009/11/09 10:13:49  giucas
	Added registration for Sentinel1
	
	Revision 2.9  2009/10/14 13:39:13  giucas
	Added SAC-D handling.
	
	Revision 2.8  2009/01/21 11:19:45  giucas
	Radarsat handling added
	
	Revision 2.7  2008/10/22 16:22:16  giucas
	Added Alos handling
	
	Revision 2.6  2008/09/29 13:41:37  giucas
	Added Oceansat handling
	
	Revision 2.5  2007/10/01 13:40:17  nicvac
	Jers added.
	
	Revision 2.4  2007/07/02 15:18:41  nicvac
	Mos added.
	
	Revision 2.3  2006/11/08 10:36:14  giucas
	Ers and Spot specific L0 classes added
	
	Revision 2.2  2006/10/05 12:37:34  giucas
	Added Landsat classes
	
	Revision 2.1  2006/10/05 12:10:36  giucas
	Added EOS object regidstrations
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.9  2004/08/06 11:50:00  giucas
	Added builder method for CosmoL0ProductTypeDescriptor
	
	Revision 1.8  2004/08/02 16:01:09  giucas
	Modified to register specific L0 implementations satellite dependent
	


*/

#include <ProdTypeDescriptorFactory.h>
#include <ProdTypeDescriptorSingleton.h>

#include <CryosatL0ProductTypeDescriptor.h>
#include <EnvisatL0ProductTypeDescriptor.h>
#include <GoceL0ProductTypeDescriptor.h>
#include <CosmoL0ProductTypeDescriptor.h>
#include <EosL0ProductTypeDescriptor.h>
#include <LandsatL0ProductTypeDescriptor.h>
#include <JersL0ProductTypeDescriptor.h>
#include <MosL0ProductTypeDescriptor.h>
#include <ErsL0ProductTypeDescriptor.h>
#include <SpotL0ProductTypeDescriptor.h>
#include <OceansatL0ProductTypeDescriptor.h>
#include <AlosL0ProductTypeDescriptor.h>
#include <RadarsatL0ProductTypeDescriptor.h>
#include <SacdL0ProductTypeDescriptor.h>
#include <Sentinel1L0ProductTypeDescriptor.h>

#include <L1ProductTypeDescriptor.h>
#include <L2ProductTypeDescriptor.h>
#include <BWSProductTypeDescriptor.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProdTypeDescriptorFactory)

ProdTypeDescriptorFactory::ProdTypeDescriptorFactory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		

	//--------------------------------------------------------------------------------------------------------------------
	//LEVEL0 Product Type Descriptors
	ProdTypeDescriptorSingleton::instance()->registerFactory("CryosatL0ProductTypeDescriptor" , &buildCryosatL0ProductTypeDescriptor );
	ProdTypeDescriptorSingleton::instance()->registerFactory("EnvisatL0ProductTypeDescriptor" , &buildEnvisatL0ProductTypeDescriptor );
	ProdTypeDescriptorSingleton::instance()->registerFactory("GoceL0ProductTypeDescriptor"    , &buildGoceL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("CosmoL0ProductTypeDescriptor"   , &buildCosmoL0ProductTypeDescriptor   );
	ProdTypeDescriptorSingleton::instance()->registerFactory("CsgL0ProductTypeDescriptor"   , &buildCosmoL0ProductTypeDescriptor   );
	ProdTypeDescriptorSingleton::instance()->registerFactory("EosL0ProductTypeDescriptor"     , &buildEosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->registerFactory("EosamL0ProductTypeDescriptor"   , &buildEosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->registerFactory("EospmL0ProductTypeDescriptor"   , &buildEosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->registerFactory("LandsatL0ProductTypeDescriptor" , &buildLandsatL0ProductTypeDescriptor );
    ProdTypeDescriptorSingleton::instance()->registerFactory("JersL0ProductTypeDescriptor"    , &buildJersL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->registerFactory("MosL0ProductTypeDescriptor"     , &buildMosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->registerFactory("ErsL0ProductTypeDescriptor" 	 , &buildErsL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->registerFactory("SpotL0ProductTypeDescriptor" 	 , &buildSpotL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("OceansatL0ProductTypeDescriptor" 	 , &buildOceansatL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Oceansat2L0ProductTypeDescriptor" 	 , &buildOceansatL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("AlosL0ProductTypeDescriptor" 	 , &buildAlosL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("RadarsatL0ProductTypeDescriptor" 	 , &buildRadarsatL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("SacdL0ProductTypeDescriptor" 	 , &buildSacdL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sac-dL0ProductTypeDescriptor" 	 , &buildSacdL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel1dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-1dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S1dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );

	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel2dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-2dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S2dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );

	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel3dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-3dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S3dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );

	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel5PL0ProductTypeDescriptor" 	, &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-5PL0ProductTypeDescriptor" 	, &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S5PL0ProductTypeDescriptor" 	 		, &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel5pL0ProductTypeDescriptor" 	, &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-5pL0ProductTypeDescriptor" 	, &buildSentinel1L0ProductTypeDescriptor    );

	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel6L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel6AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel6BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-6AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-6BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel6aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel6bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-6aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("Sentinel-6bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S6AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S6BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S6aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->registerFactory("S6bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );

	//--------------------------------------------------------------------------------------------------------------------
	ProdTypeDescriptorSingleton::instance()->registerFactory("L1ProductTypeDescriptor" , &buildL1ProductTypeDescriptor );	
	ProdTypeDescriptorSingleton::instance()->registerFactory("L2ProductTypeDescriptor" , &buildL2ProductTypeDescriptor );	
	ProdTypeDescriptorSingleton::instance()->registerFactory("BWSProductTypeDescriptor" , &buildBWSProductTypeDescriptor );	
	
}


ProdTypeDescriptorFactory::~ProdTypeDescriptorFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		

	//--------------------------------------------------------------------------------------------------------------------
	//LEVEL0 Product Type Descriptors
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("CryosatL0ProductTypeDescriptor" , &buildCryosatL0ProductTypeDescriptor );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("EnvisatL0ProductTypeDescriptor" , &buildEnvisatL0ProductTypeDescriptor );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("GoceL0ProductTypeDescriptor"    , &buildGoceL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("CosmoL0ProductTypeDescriptor"   , &buildCosmoL0ProductTypeDescriptor   );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("CsgL0ProductTypeDescriptor"   , &buildCosmoL0ProductTypeDescriptor   );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("EosL0ProductTypeDescriptor"     , &buildEosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("EosamL0ProductTypeDescriptor"   , &buildEosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("EospmL0ProductTypeDescriptor"   , &buildEosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("LandsatL0ProductTypeDescriptor" , &buildLandsatL0ProductTypeDescriptor );
    ProdTypeDescriptorSingleton::instance()->unregisterFactory("JersL0ProductTypeDescriptor"    , &buildJersL0ProductTypeDescriptor    );
    ProdTypeDescriptorSingleton::instance()->unregisterFactory("MosL0ProductTypeDescriptor"     , &buildMosL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("ErsL0ProductTypeDescriptor" 	   , &buildErsL0ProductTypeDescriptor     );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("SpotL0ProductTypeDescriptor"    , &buildSpotL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("OceansatL0ProductTypeDescriptor" , &buildOceansatL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Oceansat2L0ProductTypeDescriptor" 	 , &buildOceansatL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("AlosL0ProductTypeDescriptor" 	, &buildAlosL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("RadarsatL0ProductTypeDescriptor" , &buildRadarsatL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("SacdL0ProductTypeDescriptor" 	 , &buildSacdL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sac-dL0ProductTypeDescriptor" 	 , &buildSacdL0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel1dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-1dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S1dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel2dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-2dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S2dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel3dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-3dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3CL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3DL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3cL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S3dL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel6L0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel6AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel6BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-6AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-6BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel6aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel6bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-6aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("Sentinel-6bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S6AL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S6BL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S6aL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("S6bL0ProductTypeDescriptor" 	 , &buildSentinel1L0ProductTypeDescriptor    );

	//--------------------------------------------------------------------------------------------------------------------
	
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("L1ProductTypeDescriptor" , &buildL1ProductTypeDescriptor );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("L2ProductTypeDescriptor" , &buildL2ProductTypeDescriptor );
	ProdTypeDescriptorSingleton::instance()->unregisterFactory("BWSProductTypeDescriptor" , &buildBWSProductTypeDescriptor );
    // PRQA L:L1
}

// *************** buildCryosatL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildCryosatL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new CryosatL0ProductTypeDescriptor;
}

// *************** buildEnvisatL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildEnvisatL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new EnvisatL0ProductTypeDescriptor;
}

// *************** buildGoceL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildGoceL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new GoceL0ProductTypeDescriptor;
}

// *************** buildCosmoL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildCosmoL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new CosmoL0ProductTypeDescriptor;
}

// *************** buildEosL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildEosL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new EosL0ProductTypeDescriptor;
}

// *************** buildLandsatL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildLandsatL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new LandsatL0ProductTypeDescriptor;
}

// *************** buildJersL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildJersL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new JersL0ProductTypeDescriptor;
}


// *************** buildMosL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildMosL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new MosL0ProductTypeDescriptor;
}


// *************** buildErsL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildErsL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new ErsL0ProductTypeDescriptor;
}

// *************** buildSpotL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildSpotL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new SpotL0ProductTypeDescriptor;
}

// *************** buildOceansatL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildOceansatL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new OceansatL0ProductTypeDescriptor;
}

// *************** buildAlosL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildAlosL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new AlosL0ProductTypeDescriptor;
}

// *************** buildRadarsatL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildRadarsatL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new RadarsatL0ProductTypeDescriptor;
}

// *************** buildSacdL0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildSacdL0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new SacdL0ProductTypeDescriptor;
}

// *************** buildSentinel1L0ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildSentinel1L0ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new Sentinel1L0ProductTypeDescriptor;
}

///////////////////////////////////////////////////////////////////////////////////////////
// L1 Builders
///////////////////////////////////////////////////////////////////////////////////////////


// *************** buildL1ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildL1ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new L1ProductTypeDescriptor;
}

// *************** buildL2ProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildL2ProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new L2ProductTypeDescriptor;
}

// *************** buildBWSProductTypeDescriptor ***************************
ProductTypeDescriptor* ProdTypeDescriptorFactory::buildBWSProductTypeDescriptor()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;		
	return new BWSProductTypeDescriptor;
}

_ACS_END_NAMESPACE
