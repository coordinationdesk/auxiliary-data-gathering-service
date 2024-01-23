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
#	$Prod: A.C.S. libSendMail Makefile $
#	
#       $Id$
#
#       $Author$
#
#       $Log$
#       Revision 5.1  2017/05/25 11:16:41  marpas
#       dependencies fixed
#
#       Revision 5.0  2013/06/17 18:25:08  marpas
#       adopting libException 5.x standards
#       coding best practices applied
#       qa qules applied
#
#       Revision 1.3  2010/12/20 15:14:23  marpas
#       smV class renamed into sendmailV
#
#       Revision 1.2  2010/11/26 16:58:51  enrcar
#       EC:: Work in progress
#
#       Revision 1.1.1.1  2010/11/26 13:34:05  enrcar
#       Imported source
#
#


include ../../common/commondefs.mk

  PROJECTFLAG = -D$(shell echo $(PROJECT) | tr a-z A-Z)
  	
  USERFLAG += $(PROJECTFLAG)

  LIBRARYNAME =	libSendMail

  VERSIONFILE = sendmailV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		sendmailV.C \
		base64.C \
		CSmtp.C \
		SendMail.C 


   C++INCLUDE = \
		SendMail.h \
		CSmtp.h \

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS =


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE
include ../../common/commonrules.mk

