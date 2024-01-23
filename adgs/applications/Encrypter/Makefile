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
#	Authors: 
#
#	$Prod$
#
#	$Name$
#
#	$Id$
#
# 	$Log$
# 	Revision 1.3  2017/03/20 09:22:00  clanas
# 	modified to use new dependency mechanism
# 	
# 	Revision 1.2  2012/02/22 13:44:09  micmaz
# 	updated makefile.
# 	
# 	Revision 1.1.1.1  2011/06/23 13:43:16  micmaz
# 	Encrypter utility
# 	
# 	
# 	
#

include ../../common/commondefs.mk


  USERFLAG = $(PROJECTFLAG)

 LIBRARYNAME = Encrypter


  VERSIONFILE = EncrypterVersion.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
   		Encrypter.C  EncrypterVersion.C


   C++INCLUDE = \

	
#prevent remotion of objects, target and versioning
      TARGETFILES = Encrypter
      VERSIONRULE =
    CLEANTESTRULE =


$(TARGETFILES): $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS) $(DEPLIBS) $(PRJLIBS) 
        	

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk

