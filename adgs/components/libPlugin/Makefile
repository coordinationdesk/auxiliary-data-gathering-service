#
#	Copyright 1995-2001, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Plugin Library $
#	$Id$
#
#   $Log $
#

include ../../common/commondefs.mk


#must be an absolute path for coverage 
CUSTOM_INCLUDEPATH = -I$(shell pwd)/..
#must be an absolute path for coverage 
CUSTOM_LIBRARYPATH = -L$(shell pwd)/..
#must be an absolute path to avoid linking the installed one
CUSTOM_LDOPTS = -Wl,-rpath,$(shell pwd)/..


  LIBRARYNAME =	libPlugin

  VERSIONFILE = plV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = Plugin.C PluginLoader.C PluginSet.C DirPluginSet.C plV.C

   C++INCLUDE = Plugin.h PluginLoader.h PluginSet.h DirPluginSet.h

include ../../common/commonrules.mk


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific remotion commands
helpspecific:
