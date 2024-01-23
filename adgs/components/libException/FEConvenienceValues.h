// PRQA S 1050 EOF
/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 4.1  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.5  2005/03/18 12:58:00  marfav
	SUSPEND added
	
	Revision 1.4  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.3  2003/04/10 10:05:08  antgra
	Now using strings instead of int.
	
	Revision 1.2  2003/02/03 11:46:37  antgra
	Updated values...
	
	Revision 1.1  2003/01/31 15:17:27  marpas
	Convenience event descriptions and LIB_INTEGRATION_PATH variable management

*/

#ifndef _FEConvenienceValues_H_
#define _FEConvenienceValues_H_ 

/*
 * Just convenience defines for common values to be used for FEvent class
 */
// PRQA S 1020 EOF
#define EXFE_ERROR              ("ERROR")
#define EXFE_LOSS_DATA          ("LOSS_DATA")
#define EXFE_IDLE	        	("IDLE")
#define EXFE_WARNING            ("WARNING")
#define EXFE_PRIMARY            ("PRIMARY")
#define EXFE_SECONDARY          ("SECONDARY")
#define EXFE_REPORTING          ("REPORTING")
#define EXFE_REPORT_FINISHED    ("REPORT_FINISHED")
#define EXFE_STARTED            ("STARTED")
#define EXFE_LOCK_ON_DATA       ("LOCK_ON_DATA")
#define EXFE_ENDED              ("ENDED")
#define EXFE_BUSY               ("BUSY")
#define EXFE_ONLINE             ("ONLINE")
#define EXFE_OFFLINE            ("OFFLINE")
#define EXFE_CHECKING_QUEUE     ("CHECKING_QUEUE")
#define EXFE_PROCESSING_ORDER   ("PROCESSING_ORDER")
#define EXFE_RUNNING            ("RUNNING")            
#define EXFE_SUSPENDED          ("SUSPENDED") 

#endif // _FEConvenienceValues_H_
