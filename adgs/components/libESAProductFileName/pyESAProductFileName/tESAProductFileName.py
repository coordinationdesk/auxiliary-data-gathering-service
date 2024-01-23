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

from pyESAProductFileName import ESAProductFileName

name = ESAProductFileName().GetName_DEPRECATED("ERS", 
              1,
              "AT1_ATS_0P",
              ESAProductFileName().DateToJD50(1992, 5,31,23,40,20, 0),
              ESAProductFileName().DateToJD50(1992, 6, 1, 1,23,40, 0),
              9,
              4584,
              -1,
              "ESA",
              "../conf/")
print "tESAProductFileName.py: name =", name

fileName, basePath, \
satelliteName, missionNum, fileType, \
validityStartTime, validityStopTime, \
originatingFacility, absoluteOrbitNum, \
safeManifestCrc, fileExtension = \
ESAProductFileName().GetParamsToStringSerials(name, "../conf").split("\\")

print "tESAProductFileName: params =", \
      fileName, basePath, \
      satelliteName, missionNum, fileType, \
      validityStartTime, validityStopTime, \
      originatingFacility, absoluteOrbitNum, \
      safeManifestCrc, fileExtension

namer = ESAProductFileName()
ss=namer.doubleToStringSerial(2.0)
print "tESAProductFileName doubleToStringSerial: double, serial =", 2.0, ss
d = namer.stringSerialToDouble(ss);
print "tESAProductFileName stringSerialToDouble: serial, double =", ss, d
assert d==2.0

def fname_from_file(filename):
    print "fname_from_file", filename
    from pySgiIncludes import pySgi_Data_Transcription_Statistic
    dtStatistics = pySgi_Data_Transcription_Statistic()
    dtStatistics.read("../test/DTStatisticFile.dat")
    print "filename =", filename
    ofn = ESAProductFileName().GetNameFromStatistics_ComputeCRC(dtStatistics, filename)
    return ofn

print "tESAProductFileName.GetNameFromStatistics_ComputeCRC =", fname_from_file("../test/DTUserHeader.dat")
try:
    print "tESAProductFileName.GetNameFromStatistics_ComputeCRC =", fname_from_file("../test/DTStatisticFile.dat")
except RuntimeError, rte:
    print "tESAPRoductFileName.py: correctly raised (and catched) exception", rte
    pass
else:
    raise Exception('tESAPRoductFileName.py', 'method did not raise exception as expected')

assert "ER01_AT1_ATS_0P_19920531T234020_19920601T012340_GAT_4584_E8EA.ESA"==ESAProductFileName().GetNameFromStringSerials_ComputeCRC(satelliteName,
                                                               int(missionNum),
                                                               fileType,
                                                               validityStartTime, validityStopTime,
                                                               int(originatingFacility),
                                                               int(absoluteOrbitNum),
                                                               "../test/DTUserHeader.dat",
                                                               -1,
                                                               fileExtension,
                                                               "/usr/acs/conf/local")
