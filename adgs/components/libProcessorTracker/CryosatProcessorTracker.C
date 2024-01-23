// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processor Tracker Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2014/04/14 14:29:15  marpas
	warnings and useless headers fixed
	
	Revision 2.2  2010/08/03 14:32:58  nicvac
	Task Table plugins management
	
	Revision 2.1  2009/06/26 10:47:27  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.0  2006/02/28 10:18:10  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2004/12/03 17:50:50  marfav
	Header fixed
	
	Revision 1.6  2004/11/15 16:50:50  marfav
	CryosatProcessorTracker included in ProcessorTracker
	CryosatProcessorTracker prepared for deletion
	
	Revision 1.5  2004/09/28 16:33:30  marfav
	Addedd support for task percentage
	
	Revision 1.4  2004/09/01 09:17:56  marfav
	Added support for exit code of tracked processes
	
	Revision 1.3  2004/05/28 15:08:18  marfav
	Support for processing start time added
	
	Revision 1.2  2004/05/25 16:26:05  marfav
	Added a ctor
	now CryosatProcessorTracker can track a scheduler log
	using orderId and processingRootPath
	
	Revision 1.1.1.1  2004/04/15 12:19:58  marfav
	Import libProcessorTracker
	
	

*/

#include <CryosatProcessorTracker.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


CryosatProcessorTracker::CryosatProcessorTracker (const scOrder& wo) :
		ProcessorTracker (wo)
{
}

CryosatProcessorTracker::CryosatProcessorTracker(int orderId, const std::string &rootPath) :
		ProcessorTracker (orderId, rootPath)
{
}

CryosatProcessorTracker::~CryosatProcessorTracker() throw() 
{}




_ACS_END_NAMESPACE
