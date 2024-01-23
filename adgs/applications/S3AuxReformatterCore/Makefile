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
#	$Prod: Sentinel3 Auxiliary Reformatter application $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.3  2013/06/12 10:53:06  marfav
#	Work in progress
#	
#	Revision 1.2  2013/06/11 13:44:53  marfav
#	Configuration updated
#	
#	Revision 1.1.1.1  2013/06/11 13:33:29  marfav
#	Importing S3AuxReformatterCore
#	
#	
#	

include ../../common/commondefs.mk

INSTALLSPECIFICRULE = myInstallRule

  USERFLAG    = $(PROJECTFLAG)

  LIBRARYNAME = S3AuxReformatterCore
	
  VERSIONFILE = S3AuxRefCoreV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = S3AuxReformatter.C \
   		S3AuxRefCoreV.C
		
   UIC_SOURCES= 
	
   MOC_SOURCES = 

   C++INCLUDE = 


		
LIB =	$(PROJECTSTANDALONEAPP) \
	-lOcadaJobOrder \
	-lS3AuxFormatter \
	-lSignatureAlgo \
	-lFileInventory \
	-lApp \
	-lSignal \
	-lException \
	-lm 

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = S3AuxReformatterCore
      CLEANTESTRULE =
	

$(TARGETFILES):  $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@ $(OBJECTS) $(LIB)


CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=

ROOT=$(shell echo `pwd`)
INSTROOT=$(ROOT)/test/instroot
CONFROOT=$(ROOT)/test/confroot
BINDIR=$(INSTROOT)/bin 
CONFDIR=$(CONFROOT)/conf

#include linuxcommonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -rf $(INSTROOT)
	@rm -rf $(CONFROOT)

#empty rule, do not remove, you can add specific help strings
helpspecific:

#specific installation rule


myInstallRule:
	@rm -rf $(INSTROOT)
	@rm -rf $(CONFROOT)
	@mkdir -p $(BINDIR)
	@mkdir -p $(CONFDIR)
	@ln -sf $(ROOT)/$(TARGETFILES) $(BINDIR) 
	@cp -r $(ROOT)/conf/xml/* $(CONFDIR)
	@rm -rf `find $(CONFDIR) -name CVS`
	@export replace=`echo $(INSTROOT) | sed 's/\//\\\\\//g'`; for name in `find $(CONFDIR) -name "*.xml"`; do sed -i "s/{home}/$$replace/g" $$name; done;
	@export replace=`echo $(CONFROOT) | sed 's/\//\\\\\//g'`; for name in `find $(CONFDIR) -name "*.xml"`; do sed -i "s/{homeconf}/$$replace/g" $$name; done;

include ../../common/commonrules.mk

# DO NOT DELETE
