#
#	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libNetcdfWrapper $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.4  2017/06/09 07:49:39  masdal
#	*** empty log message ***
#	
#	Revision 1.2  2011/03/02 11:06:44  marant
#	New files (netcdf4) added
#	
#	Revision 1.1.1.1  2010/02/09 10:41:58  marant
#	Import libNetcdfWrapper
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libNetcdfWrapper

VERSIONFILE = NetcdfWrapperV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = 	Netcdf4DataFile.C \
		Netcdf4Copy.C \
		Netcdf4Utils.C \
		NetcdfWrapperV.C \
		Netcdf4xmlHandler.C \
		Netcdf4xmlConverter.C

C++INCLUDE = 	Netcdf4DataFile.h \
		Netcdf4Utils.h \
		Netcdf4Copy.h \
		Netcdf4xmlHandler.h \
		Netcdf4xmlConverter.h

# in case USE_NETCDIF_STATIC_LIBS is defined (in common_$PROJECT/$PROJECT.defs.mk)
# this library will not declare as dependency and nit try to link the NetCDF library shared ojbects
# it will be up to the application linking phase to include the .a static versions
# as declared by the project defs.mk in the PRJ_LIBS variable
#
ifndef USE_NETCDIF_STATIC_LIBS
DSOSPECIFICDEPS = -lnetcdf_c++4 -lnetcdf 
endif

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

DSODEPENDLIBS = 


