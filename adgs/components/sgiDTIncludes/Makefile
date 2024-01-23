#
#	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
#	Authors: 
#
#	$Prod: A.C.S. SGI DT includes $
#
#	$Id$
#


include ../../common/commondefs.mk

  LIBRARYNAME =	sgiDTIncludes

  VERSIONFILE = sgiDTV.C

     CSOURCES =	

     CINCLUDE =	\
		mdbm.h \
		./DataTranscription/dtTypes.h \
		./DataTranscription/dtMagic.h \
		acsStations.h \
		acsSatellites.h \
		Odb.h \
		Odb.H \
		./SPOT/Spot.h \
		./SPOT/Spot4.h \
		MODIS_Structures.h \
		Spot_4_Structures.h \
		SPOT_4_Transcription.h \
		DirectIngestionOrder.h \
		./QuickLook/Quick_Look.h \
		SlAT.h \
		station_conf.h \
		state_vectors.h \
		Ingestion.h \
		libql.h \
		smTypes_Structures.h \
		smConstants.h \
		Rm.h \
		libmdpsc.h \
		./LANDSAT/MSS_Structures.h \
		./LANDSAT/Mss_Structures.h \
		./LANDSAT/ETM+_Structures.h \
		./LANDSAT/Tm_Structures.h \
		./LANDSAT/LAND_Structures.h \
		lsMagic.h \
		lsConstants.h \
		lsTypes.h \
		smTypes.h \
		jrTypes.h \
		JErs_Ops_Structures.h \
		opsMagic.h \
		lsCPFTypes.h \
		SPOTInvQLStructures.h \
		mssCPFTypes.h \
		mssConstants.h \
		mssMagic.h


   C++SOURCES = sgiDTV.C \
                fake.C

   C++INCLUDE = 

   POSTCOMPILERULE = createLinks


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

createLinks:
	@echo "Creating symlinks into include..."
	@./test/mkLinks.sh


include ../../common/commonrules.mk
