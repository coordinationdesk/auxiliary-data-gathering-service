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
	
	Revision 1.4  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/24 08:15:39  marfav
	Added notification of messages when a percentage computation is done
	
	Revision 1.1  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	

	


*/

#ifndef _ScLogHandlerBase_H_
#define _ScLogHandlerBase_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <exFilterable.h>


_ACS_BEGIN_NAMESPACE(acs)


// This class is the base class for all Scheduler Log Handlers plugins
// All class derived from this must implement
// - computeTaskCompletionPercentage
// - userFilterableNotification

class ScLogHandlerBase : public exHandler // PRQA S 2109, 2153
{
public:
	explicit ScLogHandlerBase(const std::string &n="ScLogHandlerBase");
	virtual ~ScLogHandlerBase() throw();

	virtual void notifyFilterable(exFilterable &);

	// Percentage completion must be in the range 0 .. 100
	// The method must return true if percentage computation on filterable is succesful
	virtual bool computeTaskCompletionPercentage (exFilterable&, float&) = 0;

	// User filterable notification method
	virtual void userFilterableNotification (exFilterable&) = 0;

private:
	// Declared but not implemented to prevent their use
	ScLogHandlerBase (const ScLogHandlerBase&);
	ScLogHandlerBase& operator= (const ScLogHandlerBase&);

private:
	std::string _processingMsg;
};

_ACS_END_NAMESPACE

#endif // _ScLogHandlerBase_H_
