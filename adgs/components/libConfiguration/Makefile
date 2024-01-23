#
#	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Makefile for libConfiguration $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.6  2013/02/28 13:21:09  marpas
#	no libMessage dependency
#	
#	Revision 2.5  2009/07/06 15:32:05  danalt
#	added lProc DSODEPENDLIBS
#	
#	Revision 2.4  2009/07/01 16:38:59  marpas
#	robustness improved: the ConfigurationSingleton is allocated statically and no longer dynamically when calling istance()
#	The above method alwasy returns the statically allocated object.
#	Interface has no changed
#	
#	Revision 2.3  2008/12/15 18:43:52  marpas
#	SystemMode implementation in progress
#	
#	Revision 2.2  2008/11/13 12:38:26  marpas
#	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
#	
#	Revision 2.1  2006/08/23 14:50:07  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:37:08  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.7  2005/09/08 14:17:13  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.6  2004/06/04 08:22:40  marfav
#	Added ConfigurablePluginLoader class
#	
#	Revision 1.5  2004/05/25 10:25:02  marpas
#	ConfigurablePlugin class added
#	documentation created
#	
#	Revision 1.4  2003/01/31 14:46:21  marpas
#	bug fixed and LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.3  2002/11/12 13:58:35  marpas
#	ConfigurationSingleton class added w/ test
#	
#	Revision 1.2  2002/10/10 12:09:09  marpas
#	Makefile fixed.
#	
#	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
#	libConfiguration import
#		
#



  LIBRARYNAME =	libConfiguration

  VERSIONFILE = cfv.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		ConfigMerger.C \
		ResourceSpace.C \
		ResourceSpaceFactory.C \
		ConfigurationSingleton.C \
		ConfigurablePlugin.C \
		ConfigurablePluginLoader.C \
		SystemModeFactory.C \
		SystemMode.C \
		cfv.C

   C++INCLUDE = \
		ConfigMerger.h \
		ResourceSpace.h \
		ConfigurationSingleton.h \
		ConfigurablePlugin.h \
		ResourceSpaceFactory.h \
		ConfigurablePluginLoader.h \
		SystemModeFactory.h \
		SystemMode.h \



include ../../common/commondefs.mk

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

