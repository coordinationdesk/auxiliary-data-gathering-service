#
#	Copyright 2012, Advanced Computer Systems , Inc.
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
#	$Prod:  $
#	$Id$
#
#   $Log$
#   Revision 5.0  2013/06/19 08:41:07  chicas
#   *** empty log message ***
#
#   Revision 1.4  2013/06/12 10:35:33  nicvac
#   S2PDGS-298: Notifications on success.
#
#   Revision 1.3  2012/12/11 15:05:18  chicas
#   Configurable Root Namespace; added sendMail in case of failure; message improved
#
#   Revision 1.2  2012/11/29 17:15:46  chicas
#   work in progress
#
#   Revision 1.1.1.1  2012/11/26 14:45:46  chicas
#   Imported sources
#
#
#



  PROJECTFLAG = -D$(shell echo $(PROJECT) | tr a-z A-Z)
  	
  USERFLAG += $(PROJECTFLAG)

  LIBRARYNAME =	libTaskTableProcess

  VERSIONFILE = lTTProcV.C



   C++SOURCES = \
	TTProcessEmailNotification.C \
	TaskTableProcessConfig.C \
	TaskTableProcess.C \
	lTTProcV.C

   C++INCLUDE = \
	TaskTableProcessConfig.h \
	TaskTableProcess.h


include ../../common/commondefs.mk




DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

include ../../common/commonrules.mk

DSOSPECIFICDEPS =

cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:




# DO NOT DELETE

