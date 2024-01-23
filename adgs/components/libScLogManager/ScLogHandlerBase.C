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

	$Prod: A.C.S. Scheduler Log Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/09/27 15:27:30  marpas
	streams have name
	
	Revision 5.0  2013/07/07 18:27:50  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.5  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.4  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.3  2004/09/28 16:33:59  marfav
	Added support for Tracker task percentage messages
	
	Revision 1.2  2004/09/24 08:15:39  marfav
	Added notification of messages when a percentage computation is done
	
	Revision 1.1  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	

*/

#include <ScLogHandlerBase.h>
#include <ScLogManager.h>
#include <ScFilterable.h>
#include <ScOutFilterable.h>
#include <ScErrFilterable.h>
#include <TrkFilterable.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
    const string _defaultPercentageMessage = "updating percentage of task #";
}

// CTOR
ScLogHandlerBase::ScLogHandlerBase (const string &n) :
	exHandler (n, &sclog),
    _processingMsg(_defaultPercentageMessage)
{
#warning use messages format from scheduler configuration space
}

ScLogHandlerBase::~ScLogHandlerBase() throw()
{
	Lock lock (globalMutex());
	destroying();
}

		
void ScLogHandlerBase::notifyFilterable(exFilterable &element)
{
	Lock lock (globalMutex());
	if (!isDestroying())
	{

		ScFilterable *f = dynamic_cast<ScFilterable*> (&element); // PRQA S 3081
		
		// Is it a filterable message of the Scheduler messages family?
		if (f)
		{
			TrkFilterable *t = dynamic_cast<TrkFilterable*> (&element); // PRQA S 3081
			float pct = 0 ;
			if (!t)
			{
				bool validPercentage = false;
				try
				{
					validPercentage = computeTaskCompletionPercentage(element, pct); 
				}
				catch (...)
				{
					// Ooops... there will be no task percentage computation!!
					// Continue notifying messages to handlers
				}
				if (validPercentage)
				{
					TrkFilterable trk;
					trk << "ScLogHandlerBase: " << _processingMsg << f->getOriginatorTaskName() << "[" << pct << "]";
					// Re-issue the message using TRK format specification
					sclog << trk;
					// Lock the stream of the original filterable message
					// to avoid messages accumulation
					element.text();
				}
			}
			// Let the handler to have notification for any message
			try
			{
				userFilterableNotification (element);
			}
			catch (...)
			{
				// Do not let user specific code to throw exceptions
			}
		
		}
	}
}




_ACS_END_NAMESPACE
