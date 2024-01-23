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
#	$Prod: A.C.S. ThinLayer Order Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.2  2009/03/04 21:04:02  marpas
#	CODECHECK parsing fixed
#	
#	Revision 2.1  2006/08/23 14:50:44  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:06:01  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.6  2005/06/28 08:26:31  marfav
#	Added OrderTypeMgr class
#	
#	Revision 1.5  2005/03/09 09:26:08  marfav
#	TlOrderQueue added
#	
#	Revision 1.4  2003/08/20 09:52:28  envisat
#	Changed commondefs and commonrules includes paths from /raid0/cryosat to ../../
#	
#	Revision 1.3  2003/02/05 13:33:08  fracar
#	got rid of ^M
#	
#	Revision 1.2  2003/01/31 13:57:50  marfav
#	handling different integration and test environments
#	
#	Revision 1.1.1.1  2003/01/30 15:17:06  danalt
#	Import libTlOrder
#	
#
#
#


include ../../common/commondefs.mk

  LIBRARYNAME =	libTlOrder

  VERSIONFILE = tloV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		TlOrderQueueSingleton.C \
		tlOrder.C \
		TlOrderQueue.C \
		OrderTypeMgr.C \
		tloV.C \


   C++INCLUDE = \
		tlOrder.h \
		TlOrderQueue.h \
		TlOrderQueueSingleton.h \
		OrderTypeMgr.h


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk
