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

from pyDateTime import DateTime, JD50

todayj = JD50(-1)
j = JD50(1.0)
d = DateTime(j)
today = DateTime(todayj)
print d, dir(d)
try:
    d.setDefaultFractionDigits(8)
except RuntimeError, re:
    print re
    pass

print "getSqlString() \"" ,  d.getSqlString() ,  "\""
print "getSqlString(3) \"" ,  d.getSqlString(3) ,  "\""
print "getSqlString(6) \"" ,  d.getSqlString(6) ,  "\""
print "jd50()", d.jd50()
print "today getSqlString() \"" ,  today.getSqlString() ,  "\""
print "today getSqlString(3) \"" ,  today.getSqlString(3) ,  "\""
print "today getSqlString(6) \"" ,  today.getSqlString(6) ,  "\""
print "jd50()", today.jd50()


d2 = DateTime(j)
print d2.utcd().year, d2.utcd().microsec()
