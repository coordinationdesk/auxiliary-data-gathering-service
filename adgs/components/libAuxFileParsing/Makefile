#
#	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libAuxFileParsing $
#
#	$Log$
#	Revision 2.19  2014/03/19 19:32:04  marpas
#	useless .C removed
#	interface fixed
#	
#	Revision 2.18  2014/03/19 19:14:55  marpas
#	coding best practices application in progress (not ended)
#	warning fixing (qa & g++)
#	
#	Revision 2.17  2013/03/28 15:18:41  lucio.pulvirenti
#	OrbitDataMatcher Class added.
#	
#	Revision 2.16  2013/03/27 17:40:14  lucio.pulvirenti
#	OrbitDataMatcher class removed.
#	
#	Revision 2.15  2013/03/27 17:35:05  lucio.pulvirenti
#	First issue.
#	
#	Revision 2.14  2011/09/22 08:22:06  giucas
#	Added S1OBMemConfFileParser
#	
#	Revision 2.13  2011/09/22 07:33:43  giucas
#	Added S1OBMemConfParams
#	
#	Revision 2.12  2009/07/23 13:31:32  marant
#	MosTimeCorrParams added
#	
#	Revision 2.11  2009/02/25 14:27:57  giucas
#	Added RadarsatPOFileParser
#	
#	Revision 2.10  2008/12/16 13:51:11  giucas
#	Removed AlosTL3FileParser
#	
#	Revision 2.9  2008/12/12 15:22:47  lucio.pulvirenti
#	AlosTL3FileParser class added
#	
#	Revision 2.8  2007/12/03 10:16:49  giucas
#	L5DefinitiveFileParser added
#	
#	Revision 2.7  2007/07/25 17:46:42  nicvac
#	stable and tested.
#	
#	Revision 2.6  2006/12/12 09:44:04  marant
#	ErsTCFileParser.[hC] added
#	
#	Revision 2.5  2006/10/18 16:38:08  marpas
#	new interface released to get rid of involuntary dependencies
#	
#	Revision 2.4  2006/09/27 10:11:12  crivig
#	added new parser for Spot State vector
#	
#	Revision 2.3  2006/08/23 14:50:52  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.2  2006/07/10 12:15:55  giucas
#	Added ACSSVFileParser
#	
#	Revision 2.1  2006/05/10 07:27:46  giucas
#	STDMFileParser class added
#	
#	Revision 2.0  2006/02/28 09:32:51  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.9  2005/11/16 18:23:28  giucas
#	Added EnvisatOSFileParser
#	
#	Revision 1.8  2005/11/16 10:33:19  giucas
#	Added EnvisatPOFileParser
#	
#	Revision 1.7  2005/10/26 08:08:19  giucas
#	Added AuxFileParserSingleton
#	
#	Revision 1.6  2005/09/16 11:51:20  seralb
#	Added ErsOrbitDataFileParser
#	
#	Revision 1.5  2005/09/12 14:44:08  seralb
#	Added Spot satellite
#	
#	Revision 1.4  2005/09/08 13:04:48  paoscu
#	INCLUDEPATH removed
#	
#	Revision 1.3  2005/08/31 07:23:31  clanas
#	Removed xercex include
#	
#	Revision 1.2  2005/07/01 14:10:36  seralb
#	Added ERS satellite
#	
#	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
#	Imported libAuxFileParsing
#	
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libAuxFileParsing

  VERSIONFILE = auxFileParsV.C

   C++SOURCES = \
		AuxFileParserFactory.C \
		AuxFileParserSingleton.C \
		PredictedOrbitData.C \
		CryosatPOFileParser.C \
		EnvisatPOFileParser.C \
		AOCSacsFileParser.C \
		CFGOTimeCorrParams.C \
		USCTimeCorrParams.C \
		OBTUTCTimeCorrParams.C \
		ErsTimeCorrParams.C \
		GoceOBTFileParser.C \
		EnvisatOBTFileParser.C \
		ErsOBTFileParser.C \
		ErsTCFileParser.C \
		ErsOrbitDataFileParser.C \
		OrbitScenario.C \
		CryosatOSFileParser.C \
		EnvisatOSFileParser.C \
		ACSSVFileParser.C \
		MosTCFileParser.C \
		L5DefinitiveFileParser.C \
		RadarsatPOFileParser.C \
		MosTimeCorrParams.C \
		S1OBMemConfParams.C \
		S1OBMemConfFileParser.C \
		OrbitDataMatcher.C \
		auxFileParsV.C



   C++INCLUDE = AuxFileParser.h \
		AuxFileParserFactory.h \
		AuxFileParserSingleton.h \
		PredictedOrbitFileParser.h \
		PredictedOrbitData.h \
		CryosatPOFileParser.h \
		EnvisatPOFileParser.h \
		AOCSacsFileParser.h \
		CFGOTimeCorrParams.h \
		USCTimeCorrParams.h \
		OBTUTCTimeCorrParams.h \
		ErsTimeCorrParams.h \
		ErsOrbitDataFileParser.h \
		OBTFileParser.h \
		GoceOBTFileParser.h \
		EnvisatOBTFileParser.h \
		ErsOBTFileParser.h \
		ErsTCFileParser.h \
		OrbitScenarioFileParser.h \
		OrbitScenario.h \
		CryosatOSFileParser.h \
		EnvisatOSFileParser.h \
		OrbitDataFileParser.h \
		STDMFileParser.h \
		ACSSVFileParser.h \
		MosTCFileParser.h \
		L5DefinitiveFileParser.h \
		RadarsatPOFileParser.h \
		MosTimeCorrParams.h \
		S1OBMemConfParams.h \
		S1OBMemConfFileParser.h \
		OrbitDataMatcher.h


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
	
include ../../common/commonrules.mk
