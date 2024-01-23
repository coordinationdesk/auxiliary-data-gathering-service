#
#       Copyright 1995-2021, Exprivia SPA
#       Via Della Bufalotta, 378 - 00139 Roma - Italy
#
#       All Rights Reserved.
#

#PDGS specific paths and options definitions
TARGETFILES = $(DSO_LIBRARY)

MAKEBROWSER=/usr/bin/firefox

#-DFTPCURL_USE_GNUTLS -> when libcurl is built with gcrypt insted of libcrypto/libssh2

DEFAULT_LOCK_PATH_DEFINE?=-DDEFAULT_ACS_LOCK_PATH=\"/tmp\"

OTHERFLAGS = -rdynamic -fPIC -DDISABLE_DAEMONIZE \
 -DXERCES_HAS_NAME_SPACE -DFTPCURL_USE_OPENSSL \
 -DACS_PATHS_EXE_REL -DPREVENT_ACS_BASE_WARNINGS \
 -DDEFAULT_LOG_PERMISSIONS=0664 -DDEFAULT_ACS_PID_PERMISSIONS=0664 \
 -DUSE_LIBPDSDB $(DEFAULT_LOCK_PATH_DEFINE)


#override warning coampilation flags
    CXXAUXOPTS = --std=c++17 -Wall \
                 -Weffc++ \
                 -Wnon-virtual-dtor \
                 -Wno-non-template-friend \
                 -Wold-style-cast \
                 -Woverloaded-virtual \
                 -Wno-pmf-conversions \
                 -Wsign-promo \
                 -fdiagnostics-show-option

# used in commonrules.mk on install commands:


	CPPUNIT_INCLUDE_PATH=/usr/include/cppunit/

	POSTGRESQL_PATH = /usr/pgsql-13

MISSIONCFIPATH=/opt/cots/EOCFI
MISSIONCFILIBRARYPATH=-L$(MISSIONCFIPATH)/lib/LINUX64
MISSIONCFIINCLUDEPATH=-I$(MISSIONCFIPATH)/include
AWS_INCLUDE_PATH = -I/usr/include/aws
AWS_LIBRARY_PATH = /usr/lib64/aws


INCLUDEPATH = \
		-I. \
		$(CUSTOM_INCLUDEPATH) \
		-I$(LIB_INTEGRATION_PATH)/include\
		-isystem /usr/include/boost169 \
		-I/usr/include/libxml2 \
		-I$(PRJROOTTREE)/common/$(PROJECT) \
		$(MISSIONCFIINCLUDEPATH) \
		$(MYSQLPP_INCLUDE_PATH) \
		$(MYSQLCLIENT_INCLUDE_PATH) \
		$(MCF_NEON_INCLUDE_PATH) \
		-I$(POSTGRESQL_PATH)/include \
		$(QTINCLUDEPATH)

LIBRARYPATH = \
		-L. \
		$(CUSTOM_LIBRARYPATH) \
		-L/usr/lib64/boost169 \
		-L$(LIB_INTEGRATION_PATH)/lib  \
		-L$(SHAREDOBJPATH) \
		$(MISSIONCFILIBRARYPATH) \
		-L$(POSTGRESQL_PATH)/lib \
		-L$(AWS_LIBRARY_PATH)


DSOLIBRARYPATH =\
		$(CUSTOM_LDOPTS) \
		-Wl,--copy-dt-needed-entries \
		-Wl,-rpath,$(SHAREDOBJPATH) \
		-Wl,-rpath,/usr/lib64/boost169

#LDOPTS= \
#		$(CUSTOM_LDOPTS) \
#		-Wl,--copy-dt-needed-entries \
#		-Wl,-rpath,$(SHAREDOBJPATH) \
#		-Wl,-rpath,/usr/lib64/boost169
BUILD_PYTHON_WRAPPERS=NO


PROJECTFLAG     = -DLTA -DSCHEDULER_CHECKS_DISK_SPACE -DHAS_MISSION_INCLUDES -DINVENTORY_OPTIONAL_SATELLITE
PROJECTEXTCOMP  = $(MCFSUBLIBS)
PROJECTDBLIB    = -lPdsDb
PROJECTAPP      = -lSRVApp
PROJECTSTANDALONEAPP      = -lLTAStandaloneApp

PRJLIBS=  -lxerces-c \
	      -lm \
	      -ldl \
	      -lcrypt \
	      -lpthread



ALLOWQTCOMPONENTS=0

# remove test default compilation
TESTCOMPILERULE =
DEPENDTESTRULE =
