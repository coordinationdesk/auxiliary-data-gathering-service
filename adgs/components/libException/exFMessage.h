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
	
	Revision 4.1  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:21:59  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.5  2009/06/26 10:41:54  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.4  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.3  2008/02/15 16:58:54  marpas
	each stream can have its own formatter setFormatter method added
	exception::what() has no longer the stack if enabled
	exception::stack() returns the stack (if enabled)
	exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
	Filterables now hase details, they can be set via exFilterable::detail(...) method
	Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
	exception inserted into a Filterable will fill its detail with their stack (if any)
	
	Revision 2.2  2006/10/31 20:41:52  marpas
	defaultNotify implementation almost done
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.14  2004/09/27 09:23:06  marpas
	exFilterable I/F changed
	
	Revision 1.13  2004/04/06 10:35:51  marpas
	cleanup
	
	Revision 1.12  2004/04/06 09:55:18  marpas
	new message architecture implemented: more freedom in formatting messages
	new class exFDebug added
	
	Revision 1.11  2004/03/24 11:21:54  marpas
	time method implemented in the factory to allow new factories to change the time format
	
	Revision 1.10  2004/03/24 11:11:10  marpas
	separator added
	
	Revision 1.9  2003/07/16 16:40:11  marpas
	notify static method improved
	
	Revision 1.8  2003/07/16 16:33:04  marpas
	notify method added
	
	Revision 1.7  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.6  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.5  2002/12/06 16:11:44  marpas
	Manipulators fix for gcc 2.96
	
	Revision 1.4  2002/11/26 10:31:18  marpas
	Thread safe implementation improved.
	
	Revision 1.3  2002/11/22 13:59:26  marpas
	Thread-Safe implementation
	
	Revision 1.2  2002/11/21 17:57:03  marpas
	- exostream modified for exFilterableManagement
	- exFEvent type added
	- exMsgFmtFactory added to provide tag and other values to the exFMessage
	class
	
	Revision 1.1  2002/10/09 14:17:08  marpas
	ass exFMessage added (test too)
	
*/

#ifndef _exFMessage_H_
#define _exFMessage_H_ 

#include <Filterables.h>
#warning deprecated include - use Filterables.h instead

#endif // _exFMessage_H_
