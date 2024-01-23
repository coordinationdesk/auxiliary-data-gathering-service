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
	
	Revision 4.3  2013/04/29 15:39:01  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.2  2012/12/11 15:52:44  marpas
	qa rules
	
	Revision 4.1  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:21:59  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.2  2005/09/02 07:16:56  marpas
	exFOpEventAlert ctor slightly modified, now the second string (value) is defaulted to the empty string just to simplify code using it.
	
	Revision 1.1  2005/09/01 11:54:53  marpas
	new exFOpEventAlert class added
	

*/

#ifndef _exFOpEventAlert_H_
#define _exFOpEventAlert_H_ 

#include <Filterables.h>
#include <FEConvenienceValues.h>
#warning deprecated include - use Filterables.h instead

#endif // _exFOpEventAlert_H_
