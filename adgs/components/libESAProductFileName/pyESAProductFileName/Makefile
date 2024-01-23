# Copyright 1995-2006, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.it
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, Inc.

include ../../../common/commondefs.mk

  LIBRARYNAME =	pyESAProductFileName

  VERSIONFILE = pyepfnV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = pyESAProductFileName.C\
   				pyepfnV.C
				

CUSTOM_INCLUDEPATH= -I/usr/include/python2.4 -I..
CUSTOMFLAGS= -DNDEBUG -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4
#for insure++ && gcc 2.x
#ALLRULE = precompile compile 
DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 
DSODEPENDLIBS = -lESAProductFileName -lResources -lSgiIncludes -lCRC -lException 

#WRAPPERS = pyDateTime

#empty rule, do not remove, you can add specific help strings
helpspecific:


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


include ../../../common/commonrules.mk

# DO NOT DELETE
