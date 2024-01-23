// PRQA S 1050 EOF
/*

	Copyright 2002-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.21  2014/05/15 15:39:03  marpas
	Sentinel-1ProductFileName honored
	
	Revision 2.20  2014/03/26 09:57:37  marpas
	coding best practices applied
	warnings fixed (qa & g++)
	
	Revision 2.19  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.18  2012/11/13 13:40:05  giucas
	Factory updated
	
	Revision 2.17  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.16  2011/10/28 06:58:09  giucas
	Factory updated
	
	Revision 2.15  2011/05/04 14:11:26  giucas
	Updated for sentinel missions
	
	Revision 2.14  2009/11/19 16:26:43  giucas
	New OS2 keys added.
	
	Revision 2.13  2009/11/09 10:40:28  giucas
	Added Sentinel1 handling
	
	Revision 2.12  2009/10/14 13:39:13  giucas
	Added SAC-D handling.
	
	Revision 2.11  2009/01/21 11:19:45  giucas
	Radarsat handling added
	
	Revision 2.10  2008/10/22 16:25:07  giucas
	Added Alos handling
	
	Revision 2.9  2008/10/16 13:51:34  serfol
	getting the resource values, now the tag name doesn't contain the satellite name
	
	Revision 2.8  2008/09/29 13:41:38  giucas
	Added Oceansat handling
	
	Revision 2.7  2008/08/22 11:40:27  giucas
	unregistration for ERS fixed
	
	Revision 2.6  2008/07/22 13:26:52  giucas
	Modified Ers factory object
	
	Revision 2.5  2007/10/01 13:40:17  nicvac
	Jers added.
	
	Revision 2.4  2007/07/02 15:18:41  nicvac
	Mos added.
	
	Revision 2.3  2007/03/14 09:03:10  giucas
	AeolusProductFileName added
	
	Revision 2.2  2006/11/08 10:36:14  giucas
	Ers and Spot specific L0 classes added
	
	Revision 2.1  2006/10/05 12:54:21  giucas
	Added Eos and Landsat as Dummy
	
	Revision 2.0  2006/02/28 09:31:27  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.3  2004/08/06 14:12:35  giucas
	Added builder method for Cosmo ProductFileName
	
	Revision 1.2  2004/08/02 16:01:56  giucas
	Added handling of GOCE
	
	Revision 1.1  2003/09/09 13:13:38  giucas
	First release
	


*/

#include <ProductFileNameFactory.h>
#include <CryosatProductFileName.h>
#include <EnvisatProductFileName.h>
#include <GoceProductFileName.h>
#include <CosmoProductFileName.h>
#include <DummyProductFileName.h>
#include <AeolusProductFileName.h>
#include <ErsProductFileName.h>
#include <EarthCAREWithVersProductFileName.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ProductFileNameFactory::ProductFileNameFactory() :
	_map()
{
	registerFactory("CryosatProductFileName" , &buildCryosatProductFileName );
	registerFactory("EnvisatProductFileName" , &buildEnvisatProductFileName );
	registerFactory("EnvisatL1ProductFileName" , &buildEnvisatProductFileName );
	registerFactory("GoceProductFileName" 	 , &buildGoceProductFileName    );	
	registerFactory("CosmoProductFileName" 	 , &buildCosmoProductFileName   );
	registerFactory("EosProductFileName" 	 , &buildDummyProductFileName 	);
	registerFactory("EosamProductFileName" 	 , &buildDummyProductFileName   );
	registerFactory("EospmProductFileName" 	 , &buildDummyProductFileName   );	
	registerFactory("LandsatProductFileName" , &buildDummyProductFileName   );
    registerFactory("JersProductFileName"    , &buildDummyProductFileName   );
    registerFactory("MosProductFileName"     , &buildDummyProductFileName   );
	registerFactory("ErsProductFileName" 	 , &buildErsProductFileName   );
	registerFactory("ErsL1ProductFileName" 	 , &buildEnvisatProductFileName );
	registerFactory("SpotProductFileName" 	 , &buildDummyProductFileName   );
	registerFactory("AeolusProductFileName"  , &buildAeolusProductFileName  );
	registerFactory("OceansatProductFileName", &buildDummyProductFileName   );
	registerFactory("Oceansat2ProductFileName", &buildDummyProductFileName   );
	registerFactory("AlosProductFileName"	 , &buildDummyProductFileName   );
	registerFactory("RadarsatProductFileName", &buildDummyProductFileName   );
	registerFactory("SacdProductFileName"    , &buildDummyProductFileName   );
	registerFactory("Sac-dProductFileName"   , &buildDummyProductFileName   );
	registerFactory("Sentinel1ProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-1ProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel1AProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel1BProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-1AProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-1BProductFileName" , &buildDummyProductFileName   );
	registerFactory("S1AProductFileName" , &buildDummyProductFileName   );
	registerFactory("S1BProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel1CProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel1DProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-1CProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-1DProductFileName" , &buildDummyProductFileName   );
	registerFactory("S1CProductFileName" , &buildDummyProductFileName   );
	registerFactory("S1DProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel2ProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel2AProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel2BProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-2AProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-2BProductFileName" , &buildDummyProductFileName   );
	registerFactory("S2AProductFileName" , &buildDummyProductFileName   );
	registerFactory("S2BProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel2CProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel2DProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-2CProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-2DProductFileName" , &buildDummyProductFileName   );
	registerFactory("S2CProductFileName" , &buildDummyProductFileName   );
	registerFactory("S2DProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel3ProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel3AProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel3BProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-3AProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-3BProductFileName" , &buildDummyProductFileName   );
	registerFactory("S3AProductFileName" , &buildDummyProductFileName   );
	registerFactory("S3BProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel3CProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel3DProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-3CProductFileName" , &buildDummyProductFileName   );
	registerFactory("Sentinel-3DProductFileName" , &buildDummyProductFileName   );
	registerFactory("S3CProductFileName" , &buildDummyProductFileName   );
	registerFactory("S3DProductFileName" , &buildDummyProductFileName   );
	registerFactory("ECProductFileName" , &buildDummyProductFileName   );
	registerFactory("ECVersProductFileName" , &buildEarthCAREWithVersProductFileName   );
}

//factory destructor
ProductFileNameFactory::~ProductFileNameFactory()
{
    // PRQA S 4631 L1
	unregisterFactory("CryosatProductFileName" , &buildCryosatProductFileName );
	unregisterFactory("EnvisatProductFileName" , &buildEnvisatProductFileName );
	unregisterFactory("EnvisatL1ProductFileName" , &buildEnvisatProductFileName );
	unregisterFactory("GoceProductFileName"    , &buildGoceProductFileName    );
	unregisterFactory("CosmoProductFileName"   , &buildCosmoProductFileName   );
	unregisterFactory("EosProductFileName" 	   , &buildDummyProductFileName   );
	unregisterFactory("EosamProductFileName"   , &buildDummyProductFileName   );
	unregisterFactory("EospmProductFileName"   , &buildDummyProductFileName   );	
	unregisterFactory("LandsatProductFileName" , &buildDummyProductFileName   );
    unregisterFactory("JersProductFileName"    , &buildDummyProductFileName   );
    unregisterFactory("MosProductFileName"     , &buildDummyProductFileName   );
	unregisterFactory("ErsProductFileName" 	   , &buildErsProductFileName   );
	unregisterFactory("ErsL1ProductFileName"   , &buildEnvisatProductFileName );
	unregisterFactory("SpotProductFileName"    , &buildDummyProductFileName   );
	unregisterFactory("AeolusProductFileName"  , &buildAeolusProductFileName  );
	unregisterFactory("OceansatProductFileName", &buildDummyProductFileName   );
	unregisterFactory("Oceansat2ProductFileName", &buildDummyProductFileName   );
	unregisterFactory("AlosProductFileName"	   , &buildDummyProductFileName   );
	unregisterFactory("RadarsatProductFileName"	, &buildDummyProductFileName   );
	unregisterFactory("SacdProductFileName"    , &buildDummyProductFileName   );
	unregisterFactory("Sac-dProductFileName"   , &buildDummyProductFileName   );
	unregisterFactory("Sentinel1ProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-1ProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel1AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel1BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-1AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-1BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S1AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S1BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel1CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel1DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-1CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-1DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S1CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S1DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel2ProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel2AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel2BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-2AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-2BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S2AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S2BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel2CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel2DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-2CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-2DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S2CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S2DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel3ProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel3AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel3BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-3AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-3BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S3AProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S3BProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel3CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel3DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-3CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("Sentinel-3DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S3CProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("S3DProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("ECProductFileName" , &buildDummyProductFileName   );
	unregisterFactory("ECVersProductFileName" , &buildEarthCAREWithVersProductFileName   );
    // PRQA L:L1
}

///////////////////////
// builder methods
///////////////////////

ProductFileName* ProductFileNameFactory::buildCryosatProductFileName()
{
	return new CryosatProductFileName;
}

ProductFileName* ProductFileNameFactory::buildEnvisatProductFileName()
{
	return new EnvisatProductFileName;
}

ProductFileName* ProductFileNameFactory::buildGoceProductFileName()
{
	return new GoceProductFileName;
}

ProductFileName* ProductFileNameFactory::buildCosmoProductFileName()
{
	return new CosmoProductFileName;
}

ProductFileName* ProductFileNameFactory::buildDummyProductFileName()
{
	return new DummyProductFileName;
}

ProductFileName* ProductFileNameFactory::buildAeolusProductFileName()
{
	return new AeolusProductFileName;
}
ProductFileName* ProductFileNameFactory::buildErsProductFileName()
{
	return new ErsProductFileName;
}

ProductFileName* ProductFileNameFactory::buildEarthCAREWithVersProductFileName()
{
	return new EarthCAREWithVersProductFileName;
}

///////////////////////
// public methods
///////////////////////

ProductFileName* ProductFileNameFactory::newObject(string const & mkey) const
{
	string key = mkey + "ProductFileName";
	map<string, ProductFileName* (*)()>::const_iterator it = _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound("key \""+key+"\" is unknown.")) ; // PRQA S 3081
		
	}
	return ( *(it->second ) )();
}


///////////////////////
// private methods
///////////////////////

void ProductFileNameFactory::registerFactory(string const & key, ProductFileName* (*builder)() )
{
	map<string, ProductFileName* (*)()>::iterator it = _map.find(key);
	//register key only if not present in map
	if(it == _map.end() )
	{
		_map.insert( pair<string, ProductFileName* (*)() >(key,builder) );
	}	
}

void ProductFileNameFactory::unregisterFactory(string const & key, ProductFileName* (*builder)() ) // PRQA S 4020
{
	//see if the key is already registered in _map
	map<string, ProductFileName* (*)()>::iterator it = _map.find(key);
	if(it == _map.end() )
	{
		return ; //the builder is not present
	}
	_map.erase(it);
}


_ACS_END_NAMESPACE

