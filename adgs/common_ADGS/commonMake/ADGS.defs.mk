#
#       Copyright 1995-2010, Advanced Computer Systems , Inc.
#       Via Della Bufalotta, 378 - 00139 Roma - Italy
#       http://www.acsys.it
#
#       All Rights Reserved.
#
#       This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#       the contents of this file may not be disclosed to third parties, copied or
#       duplicated in any form, in whole or in part, without the prior written
#       permission of Advanced Computer Systems, Inc.
#
#       $Prod: $
#
#       $Id$
#
#       $Author$
#
#       $Log$
#

#PDGS specific paths and options definitions
TARGETFILES = $(DSO_LIBRARY)

#ENABLE_DSODEPENDLIBS=on

OTHERFLAGS = -rdynamic -fPIC -DIT_DLLIB -DMITRACE_OFF -DXERCES_HAS_NAME_SPACE -DFTPCURL_USE_OPENSSL -DDISABLE_DAEMONIZE -DPREVENT_ACS_BASE_WARNINGS -DACS_DBCLIENT_PREVENT_DEFAULTKEY -DDEFAULT_LOG_PERMISSIONS=0664 -DDEFAULT_ACS_PATH=\"/data/ADGS\"

#-DDISABLE_MQTT -DDEFAULT_ACS_PATH=\"/data/ADGS\" -DACS_PATHS_EXE_REL 

#override warning coampilation flags
    CXXAUXOPTS = -Wall \
                 -Weffc++ \
                 -Wnon-virtual-dtor \
                 -Wno-non-template-friend \
                 -Wold-style-cast \
                 -Woverloaded-virtual \
                 -Wno-pmf-conversions \
                 -Wsign-promo

# used in commonrules.mk on install commands:

        CPPUNIT_INCLUDE_PATH=/usr/include/cppunit/

        POSTGRESQL_PATH = /usr/pgsql-13

	LIBXML_INCLUDE_PATH = /usr/include/libxml2
        MYSQLCLIENT_INCLUDE_PATH =
        MYSQLCLIENT_LIBRARY_PATH = /usr/lib64/mysql

        MYSQLPP_INCLUDE_PATH =

# This two are symbolic links to actual versions installed

NETCDFINCPATH=
NETCDFLIBPATH=


MISSIONCFIPATH=/opt/cots/EOCFI



INCLUDEPATH = \
                -I. \
                -I  /usr/include/libxml2 \
                $(CUSTOM_INCLUDEPATH) \
                -I$(LIB_INTEGRATION_PATH)/include\
                -I$(PRJROOTTREE)/common/$(PROJECT) \
                -I$(MISSIONCFIPATH)/include \
                -I$(POSTGRESQL_PATH)/include

LIBRARYPATH = \
                -L. \
                $(CUSTOM_LIBPATH) \
                $(CUSTOM_LIBRARYPATH) \
                -L$(LIB_INTEGRATION_PATH)/lib  \
                -L$(MISSIONCFIPATH)/lib/LINUX64 \
                -L$(SHAREDOBJPATH) \
                -L$(POSTGRESQL_PATH)/lib

DSOLIBRARYPATH =

BUILD_PYTHON_WRAPPERS=NO

#CUSTOM_LDOPTS = \
#		-Wl,--copy-dt-needed-entries \
#		-Wl,-rpath,$(XERCES_COTS_PATH)

#MISSIONCFIPATH=$(EXTCOMP_PATH)/extcomponents/Mission_CFI
#MISSIONCFILIBRARYPATH=-L$(MISSIONCFIPATH)/lib/LINUX64_LEGACY
#MISSIONCFILIBRARYPATH=-L$(MISSIONCFIPATH)/lib/LINUX64
#MISSIONCFIINCLUDEPATH=-I$(MISSIONCFIPATH)/include


#GRIBAPIPATH=$(EXTCOMP_PATH)/extcomponents/Grib_API
GRIBAPILIB=-leccodes

#XERCES_COTS_PATH=/usr/local/COTS/ACS/xerces-c-3.1.1-1


#INCLUDEPATH =	\
#		-I. \
#		-isystem /usr/include/boost169 \
#		$(CUSTOM_INCLUDEPATH) \
#		-I$(LIB_INTEGRATION_PATH)/include\
#		-I$(PRJROOTTREE)/common/$(PROJECT) \
#		$(MISSIONCFIINCLUDEPATH) \
#		-I$(GRIBAPIPATH)/include \
#		-I$(LIBXML_INCLUDE_PATH) \
#		$(MCF_NEON_INCLUDE_PATH) \
#		-I$(POSTGRESQL_PATH)/include
#
#LIBRARYPATH =	\
#		-L. \
#		-L/usr/lib64/boost169/ \
#		$(CUSTOM_LIBRARYPATH) \
#		-L$(LIB_INTEGRATION_PATH)/lib  \
#		$(MISSIONCFILIBRARYPATH) \
#		-L$(GRIBAPIPATH)/lib \
#		-L$(SHAREDOBJPATH) \
#		-L$(POSTGRESQL_PATH)/lib \
#		-L$(XERCES_COTS_PATH)


#LDOPTS= \
# 		$(CUSTOM_LDOPTS) \
#		-Wl,-rpath,$(SHAREDOBJPATH) \
#		-Wl,-rpath,/usr/local/lib 


#######################
# ACQ SPECIFIC SECTION
#######################
#MWLIBS= -lDIMessages
#
#TELEMETRY_IO=-lDIFileIO \
#                         -lDICCSDSFileIO\
#                         -lDINetwork \
#                         -lDICCSDSNetwork
#
#
#PROJECT_TIMECONVERTER   = -lexplorer_lib \
##                                                  -lexplorer_data_handling\
#                                                  -lexplorer_file_handling \
#                                                  -lOBTConverter \
#                                                  -lAuxFileParsing \
#                                                  -lAuxFileHandlingDefault\
#                                                  -lAuxFileHandlingBase\
#                                                  -lxml2
#
##PROJECT_TIMECONV_WRAP   = -lExplWrapperDb \
#                                                  -lExplWrapper \
#                                                  -lAuxFileHandlingDefault \
#                                                  -lAuxFileHandlingBase\
#                                                  -lAuxFileParsing\
#                          -lTimeConverter
#
#PROJECT_ORBITPROPAGATOR = -lexplorer_orbit
#
#PROJECT_ORBPROPAG_WRAP = -lExplOrbWrapperDb \
##                                                 -lExplOrbWrapper
#
#
#
###############################
# END OF ACQ SPECIFIC SECTION
###############################

PROJECTFLAG     = -DADGS -DSCHEDULER_CHECKS_DISK_SPACE -DHAS_MISSION_INCLUDES -DINVENTORY_OPTIONAL_SATELLITE
#PROJECTFLAG    = -DPDGS -DSCHEDULER_CHECKS_DISK_SPACE -DHAS_MISSION_INCLUDES -DHAS_APPNAME_DEFINITIONS
PROJECTEXTCOMP  =
#PROJECTEXTCOMP = $(MCFSUBLIBS)
PROJECTDBLIB    = -lDbCryosat
PROJECTAPP      = -lADGSApp \
                $(PROJECTPROP) \
                $(PROJECT_TIMECONV_WRAP) \
                $(PROJECT_TIMECONVERTER) \

PROJECTSTANDALONEAPP      = -lADGSStandaloneApp

ALLOWQTCOMPONENTS=0

# remove test default compilation
TESTCOMPILERULE =
