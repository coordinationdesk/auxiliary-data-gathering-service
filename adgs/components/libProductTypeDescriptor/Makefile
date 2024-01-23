#
#	Copyright 1995-2017, Advanced Computer Systems , Inc.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Advanced Computer Systems, Inc.
#
#	$Author$ 
#
#	@(#) A.C.S. libProductTypeDescriptor Makefile - %M% %I% %E%
#
#	$Log$
#	Revision 2.25  2017/07/10 12:38:20  lucio.pulvirenti
#	ECICMF-108: EarthCAREWithVersProductFileName class added.
#	
#	Revision 2.24  2014/11/06 10:57:26  giucas
#	Added Sentinel5P
#	
#	Revision 2.23  2014/04/23 11:54:14  giucas
#	Sentinel2L0ProductDescriptor.h added
#	
#	Revision 2.22  2014/02/05 15:42:15  giucas
#	Missing Sentinel3L0ProductTypeDescriptor.h added
#	
#	Revision 2.21  2013/12/09 14:01:12  giucas
#	Library completely revisited.
#	
#	Revision 2.20  2013/03/18 14:01:20  giucas
#	Added Sentinel2L0ProductTypeDescriptor
#	
#	Revision 2.19  2011/11/16 14:27:45  giucas
#	Added S3L0ProductFileName
#	
#	Revision 2.18  2011/11/08 08:38:52  giucas
#	Added Sentinel3L0ProductTypeDescriptor.h
#	
#	Revision 2.17  2010/05/19 10:01:52  marpas
#	some fixes to compile in Venus
#	
#	Revision 2.16  2009/11/09 10:53:47  giucas
#	Added SacdL0ProductInfo Sentinel1L0ProductInfo
#	
#	Revision 2.15  2009/11/09 10:10:09  giucas
#	Added
#	
#	Revision 2.14  2009/10/14 13:38:30  giucas
#	Added SacdL0ProductTypeDescriptor
#	
#	Revision 2.13  2009/01/21 11:20:16  giucas
#	Added Radarsat classes
#	
#	Revision 2.12  2008/10/22 16:25:18  giucas
#	Alos classes added
#	
#	Revision 2.11  2008/10/16 08:58:14  nicvac
#	DSODEPENDLIBS updated.
#	
#	Revision 2.10  2008/09/29 13:41:37  giucas
#	Added Oceansat handling
#	
#	Revision 2.9  2008/07/22 13:27:04  giucas
#	Added ErsProductFileName
#	
#	Revision 2.8  2007/10/01 13:40:17  nicvac
#	Jers added.
#	
#	Revision 2.7  2007/07/02 15:18:40  nicvac
#	Mos added.
#	
#	Revision 2.6  2007/03/14 08:36:59  giucas
#	Added AeolusProductFileName
#	
#	Revision 2.5  2006/11/08 10:36:14  giucas
#	Ers and Spot specific L0 classes added
#	
#	Revision 2.4  2006/10/05 12:49:50  giucas
#	Added DummyProductFileName class
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libProductTypeDescriptor

  VERSIONFILE = ProdTypeDescrV.C

   C++SOURCES = ProductTypeDescriptor.C \
		ProdTypeDescriptorFactory.C \
		ProdTypeDescriptorSingleton.C \
   		L0ProductTypeDescriptor.C \
		CryosatL0ProductTypeDescriptor.C \
		EnvisatL0ProductTypeDescriptor.C \
		GoceL0ProductTypeDescriptor.C \
		CosmoL0ProductTypeDescriptor.C \
		EosL0ProductTypeDescriptor.C \
		ErsL0ProductTypeDescriptor.C \
		SpotL0ProductTypeDescriptor.C \
		LandsatL0ProductTypeDescriptor.C \
		JersL0ProductTypeDescriptor.C \
		MosL0ProductTypeDescriptor.C \
		OceansatL0ProductTypeDescriptor.C \
		AlosL0ProductTypeDescriptor.C \
		RadarsatL0ProductTypeDescriptor.C \
		SacdL0ProductTypeDescriptor.C \
		Sentinel1L0ProductTypeDescriptor.C \
		L1ProductTypeDescriptor.C \
		L2ProductTypeDescriptor.C \
		BWSProductTypeDescriptor.C \
		ProductFileNameFactory.C \
		ProductFileName.C \
		CryosatProductFileName.C \
		EnvisatProductFileName.C \
		GoceProductFileName.C \
		CosmoProductFileName.C \
		ErsProductFileName.C \
		DummyProductFileName.C \
		AeolusProductFileName.C \
		L0ProductInfo.C \
		L1ProductInfo.C \
		L2ProductInfo.C \
		BWSProductInfo.C \
		L0Utils.C \
		S3L0ProductFileName.C \
		EarthCAREWithVersProductFileName.C \
		ProdTypeDescrV.C
				

   C++INCLUDE = ProductTypeDescriptor.h \
   		ProdTypeDescriptorFactory.h \
		ProdTypeDescriptorSingleton.h \
		L0ProductTypeDescriptor.h \
		CryosatL0ProductTypeDescriptor.h \
		EnvisatL0ProductTypeDescriptor.h \
		GoceL0ProductTypeDescriptor.h \
		CosmoL0ProductTypeDescriptor.h \
		EosL0ProductTypeDescriptor.h \
		ErsL0ProductTypeDescriptor.h \
		SpotL0ProductTypeDescriptor.h \
		LandsatL0ProductTypeDescriptor.h \
		JersL0ProductTypeDescriptor.h \
		MosL0ProductTypeDescriptor.h \
		OceansatL0ProductTypeDescriptor.h \
		AlosL0ProductTypeDescriptor.h \
		RadarsatL0ProductTypeDescriptor.h \
		SacdL0ProductTypeDescriptor.h \
		Sentinel1L0ProductTypeDescriptor.h \
		L1ProductTypeDescriptor.h \
		L2ProductTypeDescriptor.h \
		BWSProductTypeDescriptor.h \
		ProductFileNameFactory.h \
		ProductFileName.h \
		CryosatProductFileName.h \
		EnvisatProductFileName.h \
		GoceProductFileName.h \
		CosmoProductFileName.h \
		ErsProductFileName.h \
		DummyProductFileName.h \
		AeolusProductFileName.h \
		L0ProductInfo.h \
		L1ProductInfo.h \
		L2ProductInfo.h \
		BWSProductInfo.h \
		L0Utils.h \
		S3L0ProductFileName.h \
		Sentinel2L0ProductTypeDescriptor.h \
		Sentinel3L0ProductTypeDescriptor.h \
		Sentinel5PL0ProductTypeDescriptor.h \
		EarthCAREWithVersProductFileName.h \
		Sentinel6L0ProductTypeDescriptor.h

		
COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
	
include ../../common/commonrules.mk
