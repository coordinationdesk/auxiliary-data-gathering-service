#
#	Copyright 2013-2021, Exprivia SpA.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Prod: $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.17  2017/07/28 09:21:55  lucio.pulvirenti
#	ECICMF-108: ECShape2InstIdFilesPreInventory class added.
#	
#	Revision 1.16  2017/07/14 15:06:32  lucio.pulvirenti
#	ECICMF-108: ECDlinkFrameFilesPreInventory added.
#	
#	Revision 1.15  2016/05/17 15:19:54  massimo.ricci
#	Snavons preinventory added
#	
#	Revision 1.14  2016/04/18 19:23:02  francesco.avanzi
#	S3ExelisFilenamePreInventory added
#	
#	Revision 1.13  2016/04/12 14:43:14  lucio.pulvirenti
#	S3MPM-695: USItoS3MPMPreInventory class added.
#	
#	Revision 1.12  2016/04/04 15:29:28  francesco.avanzi
#	introducing new preInventory S3ExtFilenamePreInventory.C
#	
#	Revision 1.11  2016/01/12 14:01:38  francesco.avanzi
#	 FilenamePreInventory added
#	
#	Revision 1.10  2015/08/04 16:02:54  francesco.avanzi
#	file modified, still needs to be tested
#	
#	Revision 1.9  2015/06/30 10:40:46  lucio.pulvirenti
#	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: AlsatSensorModelAuxPreInventory class developed.
#	
#	Revision 1.8  2015/06/18 15:47:19  lucio.pulvirenti
#	S3MPM-528: S3MPMReportsStdLikePreInventory class added.
#	
#	Revision 1.7  2015/06/09 15:49:04  lucio.pulvirenti
#	S3MPM-504: S3PMRequestProductPreInventory class added.
#	
#	Revision 1.6  2015/06/05 13:54:13  lucio.pulvirenti
#	S3MPM-504: EOFlikePreInventory class added.
#	
#	Revision 1.5  2014/09/30 15:46:27  lucio.pulvirenti
#	S3SCCDBAuxPreInventory class added.
#	
#	Revision 1.4  2014/09/29 14:36:30  lucio.pulvirenti
#	S3OpticalProductsPreInventory class added.
#	
#	Revision 1.3  2014/03/12 11:19:08  lucio.pulvirenti
#	JersSAROPSPreInventory and ESAStandardPreInventory classes added.
#	
#	Revision 1.2  2014/03/10 16:49:07  lucio.pulvirenti
#	Work in progress.
#	
#	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
#	Import libFilenamePreInventory
#	
#	Revision 1.2  2013/09/16 17:11:25  lucio.pulvirenti
#	depend lib removed.
#	
#	Revision 1.1.1.1  2013/09/12 15:44:18  lucio.pulvirenti
#	Import libFilenamePreInventory
#	
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libFilenamePreInventory

VERSIONFILE = FilenamePreInventoryV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = FilenamePreInventory.C \
	FilenamePreInventoryFactory.C \
	FilenamePreInventoryV.C \
	S3PDGSPreInventory.C \
	GHRSSTL4PreInventory.C \
	JersSAROPSPreInventory.C \
	ESAStandardPreInventory.C \
	S3OpticalProductsPreInventory.C \
	S3SCCDBAuxPreInventory.C \
	EOFlikePreInventory.C \
	S3PMRequestProductPreInventory.C \
	S3MPMReportsStdLikePreInventory.C \
	AlsatFilenamePreInventory.C \
	SpaceNavPreInventory.C \
	SnavonsPreInventory.C \
	S3ExtFilenamePreInventory.C \
	USItoS3MPMPreInventory.C \
	S3ExelisFilenamePreInventory.C \
	S3DimsaxFilenamePreInventory.C \
	S3GPDFilenamePreInventory.C \
	S6MOG2DFilenamePreInventory.C \
    

C++INCLUDE = FilenamePreInventory.h \
	FilenamePreInventoryFactory.h \
	S3PDGSPreInventory.h \
	GHRSSTL4PreInventory.h \
	JersSAROPSPreInventory.h \
	ESAStandardPreInventory.h \
	S3OpticalProductsPreInventory.h \
	S3SCCDBAuxPreInventory.h \
	EOFlikePreInventory.h \
	S3PMRequestProductPreInventory.h \
	S3MPMReportsStdLikePreInventory.h \
	AlsatFilenamePreInventory.h \
	SpaceNavPreInventory.h \
	SnavonsPreInventory.h \
	S3ExtFilenamePreInventory.h \
	S3ExelisFilenamePreInventory.h \
 	S3DimsaxFilenamePreInventory.h \
	S3GPDFilenamePreInventory.h \
	S6MOG2DFilenamePreInventory.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

DSODEPENDLIBS = \




