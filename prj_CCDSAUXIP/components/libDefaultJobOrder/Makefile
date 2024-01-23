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
#	Authors:
#
#	$Prod:  $
#
#	$Id$
#
#	$Log$
#	Revision 5.0  2013/07/07 18:34:07  marpas
#	adopting libException 5.x standards and coding best practices
#	qa and compilation warnings removed
#	
#	Revision 1.1.1.1  2012/10/17 13:11:09  chicas
#	imported sources
#	
#
#


#                       WARNING
# In case of plugins it is possibly harmful to link static libraries
# and a only those striclty necessary must be included
# The following tests is not-plugins libraries are compiled as shared or not
# And the value of DSODEPENDLIBS depends on this information.
# Note that this test must be performed BEFORE TARGETFILES is overwritten


include ../../common/commondefs.mk


  LIBRARYNAME =	libDefaultJobOrder

  VERSIONFILE =	defaultjoV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		DefaultJobOrder.C \
		DefaultJobOrderFactory.C \
		defaultjoV.C


   C++INCLUDE = \
		DefaultJobOrder.h \
		DefaultJobOrderFactory.h \

DSOSPECIFICDEPS =

TARGETFILES =  $(DSO_LIBRARY)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


#empty rule, do not remove, you can add specific remotion commands
helpspecific:

include ../../common/commonrules.mk

