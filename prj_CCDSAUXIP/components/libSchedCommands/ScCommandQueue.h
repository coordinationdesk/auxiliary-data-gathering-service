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

	$Prod: A.C.S. Scheduler Abstract Commands Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:57:14  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:31:01  marfav
	Import libSchedCommands
	

*/

#ifndef _SCCOMMANDQUEUE_H_
#define _SCCOMMANDQUEUE_H_

#include <exException.h>
#include <exStream.h>

#include <AbstractQueue.h>
#include <ScAbstractCommand.h>


_ACS_BEGIN_NAMESPACE(acs)

class ScCommandQueue : public AbstractQueue<ScAbstractCommand> // PRQA S 2153
{
public:
	ScCommandQueue();
	ScCommandQueue(const ScCommandQueue &);
	ScCommandQueue& operator= (const ScCommandQueue &);
	virtual ~ScCommandQueue() throw() ;

protected:
	virtual void pushElement(ScAbstractCommand*);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ScCommandQueue)
};



_ACS_END_NAMESPACE


#endif //_SCCOMMANDQUEUE_H_
