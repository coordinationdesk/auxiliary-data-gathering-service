# Copyright 1995-2006, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.it
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, Inc.

__revision__='$Id$'

PYWRAPPER_NAME=pyStandaloneApp
PYWRAPPER_DESCRIPTION=libAuxFileTreeHandling wrapper
PYWRAPPER_LIBRARIES=OBTConverter:AuxFileParsing:AuxFileHandlingBase:ACSTimeConverter:ACSTimeConvWrapper:AuxFileTreeHandling:Configuration:DateTime:ESAProductFileName:Exception:File:Resources:TimeConverter:Plugin:StorageManager:xerces-c:DbConfiguration
PYWRAPPER_INCLUDES=../:../../libACSTimeConvWrapper/:../../libACSTimeConverter:../../libOBTConverter

DSODEPENDLIBS =    -lDbBasic  -lException  -lResources  -lXMLResources 

export PYWRAPPER_NAME PYWRAPPER_DESCRIPTION PYWRAPPER_LIBRARIES PYWRAPPER_INCLUDES
include ../../../common/pywrapper.mk
