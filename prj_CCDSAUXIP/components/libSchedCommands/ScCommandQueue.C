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


#include <ScCommandQueue.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ScCommandQueue)

ScCommandQueue::ScCommandQueue() :
	AbstractQueue<ScAbstractCommand>()
{}

ScCommandQueue::ScCommandQueue(const ScCommandQueue &r) : AbstractQueue<ScAbstractCommand>(r)
{}

ScCommandQueue& ScCommandQueue::operator= (const ScCommandQueue &r) 
{
    if (this != &r) {
        AbstractQueue<ScAbstractCommand>::operator=(r) ;
    }
    return *this ;
}

ScCommandQueue::~ScCommandQueue() throw()
{
}

void ScCommandQueue::pushElement (ScAbstractCommand* aCommand)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "inserting command in queue. (  Param = \'" << aCommand->param() << "\'") ;

	AbstractQueue<ScAbstractCommand>::pushElement (aCommand);

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "current queue size is " << c.size()) ;
}



_ACS_END_NAMESPACE

