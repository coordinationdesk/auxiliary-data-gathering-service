// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	Revision 5.1  2013/06/05 15:52:30  marpas
	Filterables.C and Filterables.h introduced to collect all standard filterables.
	all header files are kept and they simply include the Filterables.h one, while their respective .C files have been removed
	
	Revision 5.0  2013/06/04 15:35:01  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.2  2013/04/29 15:39:01  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.1  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:21:59  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:54  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.8  2004/04/06 10:35:51  marpas
	cleanup
	
	Revision 1.7  2004/04/06 09:55:18  marpas
	new message architecture implemented: more freedom in formatting messages
	new class exFDebug added
	
	Revision 1.6  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.5  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.4  2003/03/11 17:31:54  marpas
	slightly improved, exFEvent has a new constructor
	
	Revision 1.3  2003/02/27 14:26:35  marpas
	New constructor gets value as integer
	
	Revision 1.2  2003/01/31 15:17:27  marpas
	Convenience event descriptions and LIB_INTEGRATION_PATH variable management
	
	Revision 1.1  2002/11/21 17:57:03  marpas
	- exostream modified for exFilterableManagement
	- exFEvent type added
	- exMsgFmtFactory added to provide tag and other values to the exFMessage
	class

*/

#ifndef _exFEvent_H_
#define _exFEvent_H_ 

#include <Filterables.h>
#include <FEConvenienceValues.h>
#warning deprecated include - use Filterables.h instead

#endif // _exFEvent_H_
