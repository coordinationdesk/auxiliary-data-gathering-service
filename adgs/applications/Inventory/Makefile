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
#	$Author$ 
#
#	@(#) A.C.S. Inventory Makefile - %M% %I% %E%
#
#	$Log$
#	Revision 5.1  2017/03/20 10:14:47  clanas
#	updated to use the new dependency mechanism
#	
#	Revision 5.0  2013/06/20 07:51:40  marpas
#	implementing SENT-37 about messaging
#	rationalization, streams redirection and chaining fixed
#	adopting libException 5.x standards
#	coding best practices applied
#	qa warnings removed
#	compilation warnings removed
#	
#	Revision 2.11  2013/03/29 19:56:04  marpas
#	removing useless libraries
#	
#	Revision 2.10  2013/03/08 17:31:05  marpas
#	libDbBasic no longer used
#	
#	Revision 2.9  2013/02/28 13:55:57  marpas
#	no libMessage dependency
#	
#	Revision 2.8  2009/01/21 16:58:01  daniela.bencivenni
#	Removed LibQueryPref
#	
#	Revision 2.7  2008/12/04 18:18:30  ivafam
#	libResultManager removed
#	
#	Revision 2.6  2008/09/23 14:38:22  enrcar
#	Bug fixed
#	
#	Revision 2.5  2008/08/27 14:25:30  davcas
#	Added compile instruction of InventoryDaemon
#	
#	Revision 2.4  2008/07/23 13:10:02  giucas
#	libESAProductFileName added
#	
#	Revision 2.3  2008/06/06 10:27:22  giapas
#	libACSStations added.
#	
#	Revision 2.2  2006/09/11 15:00:27  giapas
#	ADM specific section added.
#	
#	Revision 2.1  2006/06/30 09:02:21  marfav
#	Fixed dependencies
#	
#	Revision 2.0  2006/02/28 11:52:50  paoscu
#	Exception 2.1 I/F adopted
#	
#	Revision 1.32  2005/12/16 13:27:52  giapas
#	Added Gocecmf project.
#	
#	Revision 1.31  2005/11/14 18:22:12  giapas
#	Section for CompactStation added.
#	
#	Revision 1.30  2005/11/04 10:13:36  danalt
#	added libssl
#	
#	Revision 1.29  2005/11/03 17:09:56  paoscu
#	-lACS_SSL and -lCRC added.
#	
#	Revision 1.28  2005/10/14 15:08:16  ivafam
#	ARFClient and HSM libraries removed
#	
#	Revision 1.27  2005/07/22 13:22:10  marfav
#	Modified CMAPI library name
#	
#	Revision 1.26  2005/07/19 08:49:55  giucas
#	Removed installation of header file
#	
#	Revision 1.25  2005/07/14 11:10:11  ivafam
#	Added project GoceLTA
#	
#	Revision 1.24  2005/06/15 13:21:20  integrator
#	Added libStorageManager
#	
#	Revision 1.23  2005/04/22 14:38:17  seralb
#	Cfi removed
#	
#	Revision 1.22  2005/02/18 10:54:57  ivafam
#	Added GTS project
#	
#	Revision 1.21  2005/01/11 14:51:32  clanas
#	removed ACE lib
#	
#	Revision 1.20  2004/11/16 10:15:15  marfav
#	Added libRegExp
#	
#	Revision 1.19  2004/10/19 14:45:37  seralb
#	Upgrade
#	
#	Revision 1.18  2004/10/11 09:21:36  marfav
#	dbCosmoExtended library added
#	
#	Revision 1.17  2004/10/05 08:00:15  marpas
#	fixed
#	
#	Revision 1.16  2004/09/29 09:19:12  giucas
#	Updated for new MCF lib
#	

include ../../common/commondefs.mk

EXTCOMPLIBS = \
        -lxerces-c \
        $(INFORMIXLIBSMTHREAD)

  USERFLAG = $(PROJECTFLAG)


  LIBRARYNAME = Inventory
	

   C++SOURCES = InventoryApp.C \
   		InventoryDaemon.C \
   		InventoryV.C
   
   C++INCLUDE = 
   
   VERSIONFILE = InventoryV.C
   				

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = Inventory InventoryDaemon
      INST_EXECUTABLE = $(TARGETFILES)
      CLEANTESTRULE=

#$(TARGETFILES): $(OBJECTS) $(LIB_INTEGRATION_PATH)/lib/*
#	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@ $(OBJECTS) $(LIB)


Inventory:  $(OBJECTS) InventoryApp.o
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  InventoryApp.o InventoryV.o  $(DEPLIBS) $(PRJLIBS)

InventoryDaemon:  $(OBJECTS) InventoryDaemon.o 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  InventoryDaemon.o InventoryV.o $(DEPLIBS) $(PRJLIBS) 

include ../../common/commonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:



