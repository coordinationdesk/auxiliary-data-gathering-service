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
	Revision 5.2  2014/07/16 09:04:57  nicvac
	Global mutex use and destroying logic enforced
	
	Revision 5.1  2013/09/27 15:27:31  marpas
	streams have name
	
	Revision 5.0  2013/07/07 18:27:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.3  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.2  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.1  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	
	Revision 1.1.1.1  2004/09/14 17:02:38  marfav
	Import libTaskExecution
	
	

*/


#include <TrkHandler.h>
#include <TrkFilterable.h>

#include <exFilter.h>
#include <exStream.h>
#include <exException.h>
#include <Filterables.h>
#include <File.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(TrkHandler);

TrkHandler::TrkHandler (const string &n, const string &path, exostream *s, bool removeFileOnExit) :
	exHandler (n,s),
	_of(path.c_str()),
	_path(path),
	_removeFileOnExit(removeFileOnExit)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "entering") ;


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "opening the file [" << _path << "] for writing") ;


	if (!_of)
	{
		ACS_THROW (exIOException ("TrkHandler: Unable to open stream for output [" + path + "]", errno)); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting") ;

}

TrkHandler::~TrkHandler() throw()
{
	ThreadSafe::Lock lock (globalMutex());
	destroying();

	// PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "entering") ;

	if (_of) // PRQA S 3050 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "closing the file [" << _path << "]") ;

		_of.close();

		if (_removeFileOnExit)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "deleting the file [" << _path << "]") ;

			File::unlink (_path);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "deletion of the file [" << _path << "] not requested") ;
		}
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "never opened the file [" << _path << "]")
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting") ;
	// PRQA L:L1
}

void TrkHandler::notifyFilterable (exFilterable &f)
{
#warning MF Tracking messages are still mixed in case of multi threaded scheduler execution. Implement a filter please.
	ThreadSafe::Lock lock (globalMutex());
	if (!isDestroying() && _of)
	{
		// Is this filterable a LogMsg ??
		TrkFilterable *msg = dynamic_cast<TrkFilterable*> (&f); // PRQA S 3081
		if (msg)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inserting message [" << msg->message() << "] into stream") ;

			_of << msg->text() << endl;
		}
	}
}


_ACS_END_NAMESPACE
