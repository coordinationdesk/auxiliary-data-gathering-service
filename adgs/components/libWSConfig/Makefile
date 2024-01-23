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
#	Authors: 
#
#	$Prod: A.C.S. WS Configuration Library Makefile $
#	$Id$
#
#   $Log$
#   Revision 2.2  2009/03/04 21:19:17  marpas
#   CODECHECK parsing order: no regressions
#
#   Revision 2.1  2006/08/23 14:51:30  paoscu
#   Shared libraries dependencies added
#
#   Revision 2.0  2006/02/28 10:10:54  marpas
#   Exception 2.1 I/F adopted
#
#   Revision 1.9  2005/09/07 11:27:16  nicvac
#   INCLUDEPATH definition removed.
#
#   Revision 1.8  2005/08/31 07:59:00  clanas
#   Removed acs and xercex include
#
#   Revision 1.7  2005/08/03 10:25:51  nicvac
#   Obsolete reference to ACE removed.
#
#   Revision 1.6  2005/06/28 08:31:42  marfav
#   Multiple WSConfig schemas supported
#   OrderType translation supported by libTlOrder manager
#
#   Revision 1.4  2004/12/23 17:35:11  marfav
#   WSConfigMessage added
#
#   Revision 1.3  2003/08/20 09:55:06  envisat
#   Changed commondefs and commonrules includes paths from /raid0/cryosat to ../../
#
#   Revision 1.2  2003/02/05 13:36:22  fracar
#   fixed specific cleanup
#
#   Revision 1.1.1.1  2003/01/31 16:03:27  marfav
#   Import libWSConfig
#
#
#
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libWSConfig

  VERSIONFILE =	tlwscV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		WSConfigMessage.C \
		WSConfig.C \
		tlwscV.C \


   C++INCLUDE = \
		WSConfig.h \
		WSConfigMessage.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:


include ../../common/commonrules.mk
