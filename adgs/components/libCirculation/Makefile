#
#	Copyright 1995-2011, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Circulation Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 7.2  2018/06/14 14:42:24  damdec
#	PDSEV-24: implementation finalized.
#	
#	Revision 7.1  2017/03/28 16:14:26  marfav
#	Added support to config installation
#	
#	Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
#	Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
#	
#	Revision 6.2  2016/08/31 09:23:58  damdec
#	CirculationCallback linked
#	
#	Revision 6.1  2016/08/31 08:59:25  damdec
#	PDSEV-24: new CirculationCallbackSingleton and AsyncCallbackNotifier classes added.
#	
#	Revision 6.0  2016/08/05 10:21:05  marfav
#	Creating a major release due to db schema change involved
#	
#	Revision 5.5  2015/02/19 14:56:52  francesco.avanzi
#	getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
#	
#	Revision 5.4  2014/03/10 15:48:45  marfav
#	Removed ACS_TABLES_FUSION_318 dependency and dead code
#	
#	Revision 5.3  2013/08/07 14:24:34  lucio.pulvirenti
#	Repository class added.
#	PDS_SCHEMA_318 flag added to CUSTOMFLAGS: if new PDS schema after S2PDGS-318 is used, set PDS_SCHEMA_318 = -DACS_TABLES_FUSION_318
#	in the project common defs.
#	
#	Revision 5.2  2013/07/23 17:12:42  marpas
#	TransferProtocol removed
#	
#	Revision 5.1  2013/07/12 09:06:55  marpas
#	porting circulation business intelligence into the library
#	
#	Revision 5.0  2013/06/07 15:49:01  marpas
#	adopting libException 5.x standards
#	
#	Revision 1.38  2012/04/04 15:31:36  micmaz
#	work in progress
#	
#	Revision 1.37  2012/04/03 10:11:27  micmaz
#	work in progress
#	
#	Revision 1.36  2012/03/27 10:54:16  micmaz
#	work in progress
#	
#	Revision 1.35  2012/03/09 17:42:49  micmaz
#	added media functionality
#	
#	Revision 1.34  2012/02/28 17:50:21  micmaz
#	removed wrong headers.
#	
#	Revision 1.33  2012/02/28 17:20:15  micmaz
#	refactory Data circulator
#	
#	Revision 1.32  2012/02/16 16:00:12  micmaz
#	IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
#	http://jira.acsys.it/browse/S3PDGS-518
#	
#	Revision 1.31  2012/02/13 16:30:02  micmaz
#	added warnings detection
#	
#	Revision 1.30  2012/02/07 10:46:54  marpas
#	warnings no longer forced
#	
#	Revision 1.29  2012/02/06 14:57:51  marpas
#	User class removed
#	
#	Revision 1.28  2012/02/06 12:35:45  marpas
#	iwarnings added
#	
#	Revision 1.27  2011/10/28 13:13:24  micmaz
#	IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
#	http://jira.acsys.it/browse/S1PDGS-2274
#	
#	Revision 1.26  2011/09/13 16:30:43  micmaz
#	RESOLVED - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
#	http://jira.acsys.it/browse/S1PDGS-1605
#	IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
#	http://jira.acsys.it/browse/S1PDGS-1781
#	
#	Revision 1.24  2011/09/08 16:31:31  micmaz
#	IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
#	http://jira.acsys.it/browse/S1PDGS-1605
#	
#	Revision 1.23  2011/09/05 16:16:57  micmaz
#	IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
#	http://jira.acsys.it/browse/S1PDGS-1605
#	
#	Revision 1.22  2011/08/10 10:37:37  marpas
#	useless files removed
#	
#	Revision 1.21  2011/07/15 17:10:20  micmaz
#	work in progress
#	
#	Revision 1.20  2011/07/11 13:34:32  micmaz
#	work in progress
#	
#	Revision 1.19  2011/07/06 14:50:41  micmaz
#	*** empty log message ***
#	
#	Revision 1.18  2011/07/05 14:05:47  micmaz
#	work in progress
#	
#	Revision 1.17  2011/07/01 16:41:20  micmaz
#	changed the temporary file name strategy.
#	
#	Revision 1.16  2011/06/28 17:27:20  micmaz
#	wrong makefile
#	
#	Revision 1.15  2011/06/28 17:04:59  micmaz
#	Added the application status in order to verify when a policy must be reprocessed
#	
#	Revision 1.14  2011/06/23 15:49:20  micmaz
#	work in progress
#	
#	Revision 1.13  2011/06/23 10:33:22  micmaz
#	work in progress
#	
#	Revision 1.12  2011/06/20 16:24:57  micmaz
#	work in progress
#	
#	Revision 1.11  2011/06/16 18:26:55  micmaz
#	work in progress
#	
#	Revision 1.10  2011/06/16 12:49:45  micmaz
#	work in progress..
#	
#	Revision 1.9  2011/06/08 17:41:48  micmaz
#	work in progress
#	
#	Revision 1.8  2011/06/01 16:38:10  micmaztime
#	removed crypto from libCirculation and added to libFtpCurl
#	
#	Revision 1.7  2011/05/26 13:00:56  micmaz
#	added encryption utility.
#	
#	Revision 1.6  2011/04/11 12:25:16  micmaz
#	work in progress
#	
#	Revision 1.5  2011/04/08 17:01:15  micmaz
#	work in progress
#	
#	Revision 1.4  2011/04/05 17:15:36  micmaz
#	work in progress...
#	
#	Revision 1.3  2011/03/31 13:45:47  micmaz
#	work in progress
#	
#	Revision 1.2  2011/03/29 17:07:19  micmaz
#	work in progress
#	
#	Revision 1.1.1.1  2011/03/29 15:09:09  micmaz
#	“Start”
#	
#	
#
#

include ../../common/commondefs.mk

  CUSTOMFLAGS:=$(PDS_SCHEMA_318) $(CUSTOMFLAGS)
  LIBRARYNAME =	libCirculation

  CONF_FILES = libCirculation.so.xml


  VERSIONFILE = CirculationVersion.C

     CSOURCES =	

     CINCLUDE =	

C++SOURCES = \
	Circulation.C  \
    	CirculationCallbackSingleton.C \
	CirculationConsumer.C \
	CirculationDBService.C  \
	CirculationMode.C         \
	CirculationPolicy.C         \
	CirculationRegister.C     \
	CirculationRemoveQueue.C    \
	CirculationRemoveService.C    \
	CirculationVersion.C \
	ConsumableCirculation.C \
	ImplicitCirculationConfigurer.C \
	ImplicitCirculationService.C \
	KnownHost.C \
	KnownHostLimits.C \
	MediaChecker.C \
	MediaSigner.C \
	MediaUpdater.C \
	SecurityZone.C \
	SecurityZoneRelay.C \
	Repository.C \
    	AllowedCiphersSingleton.C \
	AsyncCallbackNotifier.C \
#	FileType.C \

C++INCLUDE = \
    	CirculationCallbackSingleton.h \
	CirculationConsumer.h \
	CirculationDBService.h \
	Circulation.h \
	CirculationItem.h \
	CirculationMode.h \
	CirculationPolicy.h \
	CirculationRegister.h \
	CirculationRemoveQueue.h \
	CirculationRemoveService.h \
	CirculationVersion.h \
	ConsumableCirculation.h \
	ImplicitCirculationConfigurer.h \
	ImplicitCirculationService.h \
	KnownHost.h \
	KnownHostLimits.h \
	MediaChecker.h \
	MediaSigner.h \
	MediaUpdater.h \
	SecurityZone.h \
	Repository.h \
	SecurityZoneRelay.h \
    	AllowedCiphersSingleton.h \
	AsyncCallbackNotifier.h \
#	FileType.h \
 
COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk



