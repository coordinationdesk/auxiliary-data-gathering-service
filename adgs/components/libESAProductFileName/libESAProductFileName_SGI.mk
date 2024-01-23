#
#	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
#	$Prod: libESAProductFileName SGI Makefile $
#
#	$Id$
#
#   $Log$
#   Revision 1.5  2006/06/27 14:57:39  davcas
#   DC: include paths changed
#
#   Revision 1.4  2006/05/24 14:27:15  enrcar
#   include of pwd was in wrong position -- fixed
#
#   Revision 1.3  2006/05/24 13:55:35  enrcar
#   Include path added (statistics are now handled)
#
#   Revision 1.2  2006/05/18 13:11:24  enrcar
#   SGI: unable to execute install target. Fixed.
#
#   Revision 1.1.1.1  2006/05/17 09:22:06  enrcar
#   imported source
#
#


        SHELL =	/bin/ksh

  LIBRARYNAME =	libESAProductFileName

     CSOURCES =	libESAProductFileName.c

   C++SOURCES = 

      INCLUDE = ESAProductFileName.h

  VERSIONFILE = 

      INCPATH = -I. -I$(SAR_INC) -I$(SAR_INC)/ISR -I$(ACS_INCLUDE) -I$(USR_ACS_INCLUDE) -I$(SAR_INC)/DataTranscription 



# Set here dependencies of this library to other ones
   USERLIBS.H =  
   
       CFLAGS = -g -o32 -fullwarn $(INCPATH)
     CPPFLAGS = $(CFLAGS)
      ARFLAGS = -rv
      LDFLAGS = -shared -delay_load -quickstart_load

      OBJECTS =	$(CSOURCES:.c=.o) $(C++SOURCES:.c++=.o) 
   AR_LIBRARY =	$(LIBRARYNAME).a
  DSO_LIBRARY =	$(LIBRARYNAME).so
  TARGETFILES = $(AR_LIBRARY) 
   SCCS_FILES = $(CSOURCES) $(C++SOURCES) $(INCLUDE) 
   MAIL_DLIST = $(ACS_INSTALL_PATH)/distrib.list/$(LIBRARYNAME).dl
  DEPEND_LIST = $(ACS_INSTALL_PATH)/src/depend/$(LIBRARYNAME).dp

all:: mklink compile
	
clobber:: clean all

mklink::
	@rm -f libESAProductFileName.c ;\
	ln -s libESAProductFileName.C  libESAProductFileName.c

check_install_path::
	@if [ "$(ACS_INSTALL_PATH)" = "" ] ;\
	then \
		echo Env var ACS_INSTALL_PATH is not defined;\
		exit 1; \
	fi
	
install:: check_install_path
	@make $(ACS_INSTALL_PATH)/lib/$(AR_LIBRARY)
	@make $(ACS_INSTALL_PATH)/include/$(INCLUDE)
#	@make $(ACS_INSTALL_PATH)/man
	
$(ACS_INSTALL_PATH)/lib/$(DSO_LIBRARY): $(DSO_LIBRARY)
	@echo Copying \"$?\" in \"$@\"
	cp $? $@

$(ACS_INSTALL_PATH)/lib/$(AR_LIBRARY): $(AR_LIBRARY)
	@echo Copying \"$?\" in \"$@\"
	cp $? $@

$(ACS_INSTALL_PATH)/include/$(INCLUDE): $(INCLUDE)
	@echo Removing \(if any\) old \"$@\"
	rm -f $@
	@echo Copying \"$?\" in \"$@\"
	cp $? $@

$(ACS_INSTALL_PATH)/man::
	@echo Creating and installing man structure
	@cd man; make install

# this target compile either the shared and the archive versions of the library
compile::   $(TARGETFILES)


$(AR_LIBRARY): $(OBJECTS)
	ar $(ARFLAGS) $@ $(OBJECTS)
	@echo The \"$@\" library has been succesfully built
	@echo 

$(DSO_LIBRARY): $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o $@ 
	@echo The \"$@\" library has been succesfully built
	@echo 

.c.o: $(INCLUDE)
	@echo -- C-Compiling module $*
	cc $(CFLAGS) -c $*.c
	@echo 

.c++.o: $(INCLUDE) 
	@echo -- C-Compiling module $*
	CC $(CPPFLAGS) -c $*.c++
	@echo 

clean:
	@echo Cleaning $(LIBRARYNAME) environment
	@for VAR in $(OBJECTS); do rm -f $$VAR; echo '\t'removed $$VAR;  done
	@for VAR in $(TARGETFILES); do rm -f $$VAR; echo '\t'removed $$VAR;  done
	@echo 

	
dependencies::
	@if [ -r $(DEPEND_LIST) ] ;\
	then \
	    echo Processing $(DEPEND_LIST) dependecies list;\
	    for l in "`grep -v '#' $(DEPEND_LIST)`" ; \
	    do \
		if [ $$l ] ;\
		then \
		    if  [ -d $(ACS_INSTALL_PATH)/src/$$l ] ;\
		    then \
			echo $(LIBRARYNAME): Compiling \"$$l\" as $(LIBRARYNAME) dependency;\
			cd $(ACS_INSTALL_PATH)/src/$$l ; make clobber install ; \
		    else \
			echo $(LIBRARYNAME): the directory $(ACS_INSTALL_PATH)/src/$$l does not exists ; \
		    fi ; \
		fi ; \
	    done ; \
	fi 
	
	
	
