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
	Revision 5.0  2013/07/07 18:27:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.3  2009/06/26 10:49:16  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.2  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.1  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	
	Revision 1.1.1.1  2004/09/14 17:02:38  marfav
	Import libTaskExecution
	
	

*/

#ifndef _TrkHandler_H_
#define _TrkHandler_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <ScLogManager.h>
#include <exStream.h>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)


class TrkHandler : public exHandler // PRQA S 2109, 2153
{
public:
	explicit TrkHandler(const std::string &n, const std::string &, exostream *s=&sclog, bool = true) ;
	virtual ~TrkHandler() throw();
	// virtual bool filterInsertion(exFilterable &) ;
	virtual void notifyFilterable (exFilterable&);

private:
	TrkHandler (); // not implemented 
	TrkHandler (const TrkHandler&);
	TrkHandler& operator= (const TrkHandler&);

private:
	std::ofstream _of;
	std::string _path;
	bool _removeFileOnExit;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TrkHandler) ;
};



_ACS_END_NAMESPACE


#endif // _TrkHandler_H_
