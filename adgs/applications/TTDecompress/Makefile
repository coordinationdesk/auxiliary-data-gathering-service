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
#	$Prod: $
#
#	$Id$
#
#	$ $
#
#	$Log$
#	Revision 1.4  2013/02/04 15:09:25  chicas
#	 Configurable Decompress app from conf space
#	
#	Revision 1.3  2013/02/04 11:09:30  chicas
#	Rewrite joborder with extracted filetypes/filenames
#	
#	Revision 1.2  2013/01/24 08:54:39  chicas
#	added script copy
#	
#	Revision 1.1.1.1  2013/01/23 16:33:10  chicas
#	Imported sources
#	
#	
#	
#        
#

include ../../common/commondefs.mk

   USERFLAG = $(PROJECTFLAG)

   LIBRARYNAME = TTDecompress

   VERSIONFILE = TTDecompressV.C

   CUSTOM_INCLUDEPATH =

   CUSTOM_LIBPATH =

   CSOURCES =	

   CINCLUDE =	

   C++SOURCES = \
	TTDecompress.C \
	TTDecompressV.C


   C++INCLUDE = 



EXES =  \
	TTDecompress


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS); \
cp ./Scripts/Decompress.sh $(EXE_INTEGRATION_PATH)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=

# DO NOT DELETE

include ../../common/commonrules.mk

