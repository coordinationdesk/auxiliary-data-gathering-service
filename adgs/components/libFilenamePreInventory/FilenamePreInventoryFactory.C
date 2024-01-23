// PRQA S 1050 EOF
/*
	Copyright 2013-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.21  2017/07/28 09:23:18  lucio.pulvirenti
	ECICMF-108: ECShape2InstIdFilesPreInventoryFactory added.
	
	Revision 1.20  2017/07/14 14:45:58  lucio.pulvirenti
	ECICMF-108: ECDlinkFrameFilesPreInventoryFactory added. Old debug macros replaced.
	
	Revision 1.19  2016/05/17 15:10:24  massimo.ricci
	(1) Snavons preinventory added for ORC of SpaceNav project
	
	Revision 1.18  2016/04/19 09:16:29  francesco.avanzi
	factory corrected
	
	Revision 1.17  2016/04/18 19:26:46  francesco.avanzi
	S3ExelisFilenamePreInventory added
	
	Revision 1.16  2016/04/15 08:43:18  lucio.pulvirenti
	S3MPM-695: USItoS3MPMPreInventoryFactory class added.
	
	Revision 1.15  2016/04/04 15:29:28  francesco.avanzi
	introducing new preInventory S3ExtFilenamePreInventory.C
	
	Revision 1.14  2016/01/12 16:28:13  francesco.avanzi
	 comments modified
	
	Revision 1.13  2016/01/12 14:01:23  francesco.avanzi
	module modified to enhance FilenamePreInventory
	
	Revision 1.12  2015/08/04 16:02:54  francesco.avanzi
	file modified, still needs to be tested
	
	Revision 1.11  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: AlsatFilenamePreInventory class developed.
	
	Revision 1.10  2015/06/18 15:46:27  lucio.pulvirenti
	S3MPM-528: S3MPMReportsStdLikePreInventoryFactory added.
	
	Revision 1.9  2015/06/09 15:50:11  lucio.pulvirenti
	S3MPM-504: S3PMRequestProductPreInventoryFactory class added.
	
	Revision 1.8  2015/06/05 13:55:23  lucio.pulvirenti
	S3MPM-504: EOFlikePreInventoryFactory added.
	
	Revision 1.7  2014/09/30 15:47:59  lucio.pulvirenti
	S3SCCDBAuxPreInventoryFactory class added.
	
	Revision 1.6  2014/09/29 14:37:43  lucio.pulvirenti
	S3OpticalProductsPreInventoryFactory added.
	
	Revision 1.5  2014/03/12 11:16:52  lucio.pulvirenti
	JersSAROPSPreInventoryFactory and ESAStandardPreInventoryFactory added as per jira issue PDGSEVO-35.
	
	Revision 1.4  2014/03/11 17:02:14  lucio.pulvirenti
	Work in progress.
	
	Revision 1.3  2014/03/10 16:49:07  lucio.pulvirenti
	Work in progress.
	
	Revision 1.2  2013/09/20 11:16:23  lucio.pulvirenti
	Debug define added.
	
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
	
	Revision 1.1.1.1  2013/09/12 15:44:18  lucio.pulvirenti
	Import libFilenamePreInventory
	
	
        
*/ 

#include <FilenamePreInventoryFactory.h>
#include <PreInventoryFactoryChain.h>

#include <S3PDGSPreInventory.h>
#include <GHRSSTL4PreInventory.h>
#include <JersSAROPSPreInventory.h>
#include <ESAStandardPreInventory.h>
#include <S3OpticalProductsPreInventory.h>
#include <S3SCCDBAuxPreInventory.h>
#include <EOFlikePreInventory.h>
#include <S3PMRequestProductPreInventory.h>
#include <S3MPMReportsStdLikePreInventory.h>
#include <AlsatFilenamePreInventory.h>
#include <SpaceNavPreInventory.h>
#include <SnavonsPreInventory.h>
#include <S3ExtFilenamePreInventory.h>
#include <S3ExelisFilenamePreInventory.h>
#include <S3DimsaxFilenamePreInventory.h>
#include <USItoS3MPMPreInventory.h>
#include <S3GPDFilenamePreInventory.h>
#include <S6MOG2DFilenamePreInventory.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3PDGSPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(GHRSSTL4PreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(JersSAROPSPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(ESAStandardPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3OpticalProductsPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3SCCDBAuxPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFlikePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3PMRequestProductPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3MPMReportsStdLikePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(AlsatFilenamePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(SpaceNavPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(SnavonsPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(USItoS3MPMPreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3ExtFilenamePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3ExelisFilenamePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3DimsaxFilenamePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S3GPDFilenamePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(S6MOG2DFilenamePreInventoryFactory)


const string S3PDGSPreInventoryFactory::_S3PDGSPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3PDGSPreInventoryFactory";
const string GHRSSTL4PreInventoryFactory::_GHRSSTL4PreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.GHRSSTL4PreInventoryFactory";
const string JersSAROPSPreInventoryFactory::_JersSAROPSPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.JersSAROPSPreInventoryFactory";
const string ESAStandardPreInventoryFactory::_ESAStandardPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.ESAStandardPreInventoryFactory";
const string S3OpticalProductsPreInventoryFactory::_S3OpticalProductsPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3OpticalProductsPreInventoryFactory";
const string S3SCCDBAuxPreInventoryFactory::_S3SCCDBAuxPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3SCCDBAuxPreInventoryFactory";
const string EOFlikePreInventoryFactory::_EOFlikePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.EOFlikePreInventoryFactory";
const string S3PMRequestProductPreInventoryFactory::_S3PMRequestProductPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3PMRequestProductPreInventoryFactory";
const string S3MPMReportsStdLikePreInventoryFactory::_S3MPMReportsStdLikePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3MPMReportsStdLikePreInventoryFactory";
const string AlsatFilenamePreInventoryFactory::_AlsatFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.AlsatFilenamePreInventoryFactory";
const string SpaceNavPreInventoryFactory::_SpaceNavPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.SpaceNavPreInventoryFactory";
const string SnavonsPreInventoryFactory::_SnavonsPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.SnavonsPreInventoryFactory";
const string S3ExtFilenamePreInventoryFactory::_S3ExtFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3ExtFilenamePreInventoryFactory";
const string S3ExelisFilenamePreInventoryFactory::_S3ExelisFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3ExelisFilenamePreInventoryFactory";
const string S3DimsaxFilenamePreInventoryFactory::_S3DimsaxFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3DimsaxFilenamePreInventoryFactory";
const string USItoS3MPMPreInventoryFactory::_USItoS3MPMPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.USItoS3MPMPreInventoryFactory";
const string S3GPDFilenamePreInventoryFactory::_S3GPDFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S3GPDFilenamePreInventoryFactory";
const string S6MOG2DFilenamePreInventoryFactory::_S6MOG2DFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.S6MOG2DFilenamePreInventoryFactory";


// **********************************************************************
// ************** S3PDGS PreInventory FACTORY ***************************
// **********************************************************************


S3PDGSPreInventoryFactory::S3PDGSPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3PDGSPreInventoryConfigKey, &S3PDGSPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3PDGSPreInventoryFactory::~S3PDGSPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3PDGSPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3PDGSPreInventory *object = new S3PDGSPreInventory;
	object -> setConfigurationRootTag(_S3PDGSPreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ************** GHRSST L4  PreInventory FACTORY ***********************
// **********************************************************************


GHRSSTL4PreInventoryFactory::GHRSSTL4PreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_GHRSSTL4PreInventoryConfigKey, &GHRSSTL4PreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

GHRSSTL4PreInventoryFactory::~GHRSSTL4PreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* GHRSSTL4PreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	GHRSSTL4PreInventory *object = new GHRSSTL4PreInventory;
	object -> setConfigurationRootTag(_GHRSSTL4PreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ************** Jers SAR/OPS  PreInventory FACTORY ***********************
// **********************************************************************


JersSAROPSPreInventoryFactory::JersSAROPSPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_JersSAROPSPreInventoryConfigKey, &JersSAROPSPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

JersSAROPSPreInventoryFactory::~JersSAROPSPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* JersSAROPSPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	JersSAROPSPreInventory *object = new JersSAROPSPreInventory;
	object -> setConfigurationRootTag(_JersSAROPSPreInventoryConfigKey);
	return object;
	
}


// **********************************************************************
// ************** ESA Standard  PreInventory FACTORY ***********************
// **********************************************************************


ESAStandardPreInventoryFactory::ESAStandardPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_ESAStandardPreInventoryConfigKey, &ESAStandardPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

ESAStandardPreInventoryFactory::~ESAStandardPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* ESAStandardPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	ESAStandardPreInventory *object = new ESAStandardPreInventory;
	object -> setConfigurationRootTag(_ESAStandardPreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ************** S3 Optical products PreInventory FACTORY ***********************
// **********************************************************************


S3OpticalProductsPreInventoryFactory::S3OpticalProductsPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3OpticalProductsPreInventoryConfigKey, &S3OpticalProductsPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3OpticalProductsPreInventoryFactory::~S3OpticalProductsPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3OpticalProductsPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3OpticalProductsPreInventory *object = new S3OpticalProductsPreInventory;
	object -> setConfigurationRootTag(_S3OpticalProductsPreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ************ S3 SCCDB Aux PreInventory FACTORY ***********************
// **********************************************************************


S3SCCDBAuxPreInventoryFactory::S3SCCDBAuxPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3SCCDBAuxPreInventoryConfigKey, &S3SCCDBAuxPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3SCCDBAuxPreInventoryFactory::~S3SCCDBAuxPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3SCCDBAuxPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3SCCDBAuxPreInventory *object = new S3SCCDBAuxPreInventory;
	object -> setConfigurationRootTag(_S3SCCDBAuxPreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// **************** EOF like PreInventory FACTORY ***********************
// **********************************************************************


EOFlikePreInventoryFactory::EOFlikePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_EOFlikePreInventoryConfigKey, &EOFlikePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

EOFlikePreInventoryFactory::~EOFlikePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* EOFlikePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	EOFlikePreInventory *object = new EOFlikePreInventory;
	object -> setConfigurationRootTag(_EOFlikePreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ************ Request Products PreInventory FACTORY *******************
// **********************************************************************


S3PMRequestProductPreInventoryFactory::S3PMRequestProductPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3PMRequestProductPreInventoryConfigKey, &S3PMRequestProductPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3PMRequestProductPreInventoryFactory::~S3PMRequestProductPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3PMRequestProductPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3PMRequestProductPreInventory *object = new S3PMRequestProductPreInventory;
	object -> setConfigurationRootTag(_S3PMRequestProductPreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ************ Request Products PreInventory FACTORY *******************
// **********************************************************************


S3MPMReportsStdLikePreInventoryFactory::S3MPMReportsStdLikePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3MPMReportsStdLikePreInventoryConfigKey, &S3MPMReportsStdLikePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3MPMReportsStdLikePreInventoryFactory::~S3MPMReportsStdLikePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3MPMReportsStdLikePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3MPMReportsStdLikePreInventory *object = new S3MPMReportsStdLikePreInventory;
	object -> setConfigurationRootTag(_S3MPMReportsStdLikePreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ******* Alsat 1B Sensor Model Aux Data file PreInventory FACTORY *****
// **********************************************************************


AlsatFilenamePreInventoryFactory::AlsatFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_AlsatFilenamePreInventoryConfigKey, &AlsatFilenamePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

AlsatFilenamePreInventoryFactory::~AlsatFilenamePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* AlsatFilenamePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	AlsatFilenamePreInventory *object = new AlsatFilenamePreInventory;
	object -> setConfigurationRootTag(_AlsatFilenamePreInventoryConfigKey);
	return object;
	
}

// *********************************************
// ******* SpaceNav PreInventory FACTORY *******
// *********************************************


SpaceNavPreInventoryFactory::SpaceNavPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_SpaceNavPreInventoryConfigKey, &SpaceNavPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

SpaceNavPreInventoryFactory::~SpaceNavPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* SpaceNavPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	SpaceNavPreInventory *object = new SpaceNavPreInventory;
	object -> setConfigurationRootTag(_SpaceNavPreInventoryConfigKey);
	return object;
	
}

// ********************************************
// ******* Snavons PreInventory FACTORY *******
// ********************************************


SnavonsPreInventoryFactory::SnavonsPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_SnavonsPreInventoryConfigKey, &SnavonsPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

SnavonsPreInventoryFactory::~SnavonsPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* SnavonsPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	SnavonsPreInventory *object = new SnavonsPreInventory;
	object -> setConfigurationRootTag(_SnavonsPreInventoryConfigKey);
	return object;
	
}

// **************************************************
// ******* S3ExtFilename PreInventory FACTORY *******
// **************************************************


S3ExtFilenamePreInventoryFactory::S3ExtFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3ExtFilenamePreInventoryConfigKey, &S3ExtFilenamePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3ExtFilenamePreInventoryFactory::~S3ExtFilenamePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3ExtFilenamePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3ExtFilenamePreInventory *object = new S3ExtFilenamePreInventory;
	object -> setConfigurationRootTag(_S3ExtFilenamePreInventoryConfigKey);
	return object;
	
}
// *********************************************
// ******* S3Exelis PreInventory FACTORY *******
// *********************************************


S3ExelisFilenamePreInventoryFactory::S3ExelisFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3ExelisFilenamePreInventoryConfigKey, &S3ExelisFilenamePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3ExelisFilenamePreInventoryFactory::~S3ExelisFilenamePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3ExelisFilenamePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3ExelisFilenamePreInventory *object = new S3ExelisFilenamePreInventory ;
	object -> setConfigurationRootTag(_S3ExelisFilenamePreInventoryConfigKey);
	return object;
	
}


// **********************************************
// ******* S3 DimSax PreInventory FACTORY *******
// **********************************************


S3DimsaxFilenamePreInventoryFactory::S3DimsaxFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3DimsaxFilenamePreInventoryConfigKey, &S3DimsaxFilenamePreInventoryFactory::buildObject)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3DimsaxFilenamePreInventoryFactory::~S3DimsaxFilenamePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}

PreInventoryBase* S3DimsaxFilenamePreInventoryFactory::buildObject()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

	S3DimsaxFilenamePreInventory *object = new S3DimsaxFilenamePreInventory ;
	object -> setConfigurationRootTag(_S3DimsaxFilenamePreInventoryConfigKey);
	return object;

}



// **********************************************************************
// ************ USI to MPMF PAR & Subscriptions: FACTORY ****************
// **********************************************************************


USItoS3MPMPreInventoryFactory::USItoS3MPMPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_USItoS3MPMPreInventoryConfigKey, &USItoS3MPMPreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

USItoS3MPMPreInventoryFactory::~USItoS3MPMPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* USItoS3MPMPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	USItoS3MPMPreInventory *object = new USItoS3MPMPreInventory;
	object -> setConfigurationRootTag(_USItoS3MPMPreInventoryConfigKey);
	return object;
	
}


// **********************************************************************
// ***************** S3 GPD PreInventory FACTORY ************************
// **********************************************************************


S3GPDFilenamePreInventoryFactory::S3GPDFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S3GPDFilenamePreInventoryConfigKey, &S3GPDFilenamePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3GPDFilenamePreInventoryFactory::~S3GPDFilenamePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S3GPDFilenamePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S3GPDFilenamePreInventory *object = new S3GPDFilenamePreInventory;
	object -> setConfigurationRootTag(_S3GPDFilenamePreInventoryConfigKey);
	return object;
	
}

// **********************************************************************
// ***************** S6 MOG2D PreInventory FACTORY ************************
// **********************************************************************


S6MOG2DFilenamePreInventoryFactory::S6MOG2DFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_S6MOG2DFilenamePreInventoryConfigKey, &S6MOG2DFilenamePreInventoryFactory::buildObject) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S6MOG2DFilenamePreInventoryFactory::~S6MOG2DFilenamePreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
}



PreInventoryBase* S6MOG2DFilenamePreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);
	
	S6MOG2DFilenamePreInventory *object = new S6MOG2DFilenamePreInventory;
	object -> setConfigurationRootTag(_S6MOG2DFilenamePreInventoryConfigKey);
	return object;
	
}




// *************************************************************
// ***************** EMBEDDED FACTORY **************************
// *************************************************************
namespace { // unnamed
    S3PDGSPreInventoryFactory EmbeddedS3PDGSPreInventoryFactory;
    GHRSSTL4PreInventoryFactory EmbeddedGHRSSTL4PreInventoryFactory;
	JersSAROPSPreInventoryFactory EmbeddedJersSAROPSPreInventoryFactory;
	ESAStandardPreInventoryFactory EmbeddedESAStandardPreInventoryFactory;
	S3OpticalProductsPreInventoryFactory EmbeddedS3OpticalProductsPreInventoryFactory;
	S3SCCDBAuxPreInventoryFactory EmbeddedS3SCCDBAuxPreInventoryFactory;
	EOFlikePreInventoryFactory EmbeddedEOFlikePreInventoryFactory;
	S3PMRequestProductPreInventoryFactory EmbeddedS3PMRequestProductPreInventoryFactory;
	S3MPMReportsStdLikePreInventoryFactory EmbeddedS3MPMReportsStdLikePreInventoryFactory;
	AlsatFilenamePreInventoryFactory EmbeddedAlsatFilenamePreInventoryFactory;
	SpaceNavPreInventoryFactory EmbeddedSpaceNavPreInventoryFactory;
	SnavonsPreInventoryFactory EmbeddedSnavonsPreInventoryFactory;
	S3ExtFilenamePreInventoryFactory EmbeddedS3ExtFilenamePreInventoryFactory;
	S3ExelisFilenamePreInventoryFactory EmbeddedS3ExelisFilenamePreInventoryFactory;
	S3DimsaxFilenamePreInventoryFactory EmbeddedS3DimsaxFilenamePreInventoryFactory;
	USItoS3MPMPreInventoryFactory EmbeddedUSItoS3MPMPreInventoryFactory;
	S3GPDFilenamePreInventoryFactory EmbeddedS3GPDFilenamePreInventoryFactory;
	S6MOG2DFilenamePreInventoryFactory EmbeddedS6MOG2DFilenamePreInventoryFactory;

}
_ACS_END_NAMESPACE

