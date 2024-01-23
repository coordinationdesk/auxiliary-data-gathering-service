#
#	Copyright 1995-2012, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libExporterBase$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.3  2017/01/11 17:08:08  nicvac
#	ExporterBase class removed
#	
#	Revision 5.2  2016/03/18 17:56:52  marpas
#	missing dependency added
#	
#	Revision 5.1  2016/03/18 17:34:14  marpas
#	adapted to c2evo dev
#	
#	Revision 5.0  2013/06/07 15:51:57  marpas
#	adopting libException 5.x standards
#	
#	Revision 1.14  2013/05/27 11:31:03  marpas
#	useless variables removed
#	DSOs dependencies updated (added libUserNotification)
#	
#	Revision 1.13  2013/05/23 13:46:38  nicvac
#	S2PDGS-308: Notification Refactoring.
#	
#	Revision 1.12  2013/03/15 10:58:58  nicvac
#	S2PDGS-186
#	ADC-83
#	WaitingAcknowledge Step implemented
#	
#	Revision 1.11  2013/03/14 12:32:41  marpas
#	removed ConnectionPool.C ConnectionPool.h
#	
#	Revision 1.10  2013/02/21 16:15:15  nicvac
#	First version of complete implementation.
#	
#	Revision 1.9  2013/02/15 14:25:34  nicvac
#	dsodepend upddated
#	
#	Revision 1.8  2013/02/13 13:48:54  nicvac
#	Implementation.
#	
#	Revision 1.7  2013/01/25 13:53:38  marpas
#	using pattern::Singleton from libException
#	
#	Revision 1.6  2012/12/06 18:34:35  nicvac
#	Removed dependency from libDataConsumer.
#	
#	Revision 1.5  2012/11/27 14:42:55  nicvac
#	depend libs
#	
#	Revision 1.4  2012/11/23 17:31:06  nicvac
#	ExporterServiceResolver added.
#	
#	Revision 1.3  2012/11/20 18:05:51  nicvac
#	DataPackage and Distribution serialization.
#	
#	Revision 1.2  2012/11/09 13:31:21  nicvac
#	Exported revisited to allow its subclassing.
#	
#	Revision 1.1.1.1  2012/10/15 14:07:46  nicvac
#	Import libExporterBase
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libExporterBase

VERSIONFILE = ExporterBaseV.C

C++SOURCES = \
	DistributionSerializable.C \
	ExporterServiceResolver.C \
	PackageEmailNotification.C \
	ExporterBaseV.C \


C++INCLUDE = \
	DistributionSerializable.h \
	ExporterServiceResolver.h \
	PackageEmailNotification.h \
	ExporterCommon.h \



DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

DSOSPECIFICDEPS =

