#!/usr/bin/python
###############################################################################################
#
#
#	Copyright 1995-2021, Exprivia S.p.A.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia S.p.A.
#
#	$Id$
#
#	$Author$
#
# 	$Log$
#
###############################################################################################

import os
import sys
import subprocess
import commands
import tarfile
import ftplib
import logging

if __name__ == "__main__":

	if len(sys.argv) < 6:
		print "Not enough parameters"
		sys.exit(1)

	filename = ""

	procdir  = sys.argv[1]
	outsite  = sys.argv[2]
	outdir   = sys.argv[3]
	outuser  = sys.argv[4]
	outpwd   = sys.argv[5]

	if len(sys.argv) > 6:
		filename = sys.argv[6]

	pathlen = len(procdir)

	if procdir[pathlen-1] == "/":
		inputdir = procdir[0:pathlen-1]
	else:
		inputdir = procdir

	rootdirname = os.path.dirname(inputdir)
	orderid = os.path.basename(inputdir)

	logging.basicConfig(level=logging.DEBUG)
	logger = logging.getLogger('ExportWD')
	hdlr = logging.FileHandler('/data/ADGS/log/ADGS_ExportWD.log')
	formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
	hdlr.setFormatter(formatter)
	logger.addHandler(hdlr) 

	logger.info("Exporting WD -> "+procdir)

	try:
		os.chdir(rootdirname)
	except OSError, oserr:
		logger.error(rootdirname+" "+oserr.strerror)
		sys.exit(1)
	except :
		logger.error("Error in Chdir: Unknown")
		sys.exit(1)

	if filename == "":
		tarname = orderid + ".tgz"
	else:
		tarname = filename  + ".tgz"

	try:
		mytar = tarfile.open(tarname, "w:gz")
		mytar.add(orderid)
		mytar.close()
	except OSError, oserr:
		logger.error("Error in tar command (OS): "+oserr.strerror)
		sys.exit(1)
	except IOError, ioerr:
		logger.error("Error in tar command (IO): "+ioerr.strerror)
		sys.exit(1)
	except :
		logger.error("Error in tar command: Unknown")
		sys.exit(1)

	try:
		ftp = ftplib.FTP(outsite)
		ftp.login(outuser, outpwd)
		ftp.cwd(outdir)
		ftp.storbinary("STOR " + tarname, open(tarname, "rb"), 1024)
		ftp.quit()
	except ftplib.all_errors, a:
		errStr = str(a)
		logger.error("Error in ftp command: "+errStr.replace("'", "\\'"))
		sys.exit(1)
	except :
		logger.error("Error in ftp command: Unknown")
		sys.exit(1)

	os.remove(tarname)

	logger.info(tarname+" exported to "+outsite+":"+outdir)

	sys.exit(0)


	
