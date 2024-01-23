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
	Revision 1.4  2008/04/22 11:00:26  marfav
	Aligned to new visitable task table elements
	
	Revision 1.3  2004/12/03 17:57:14  marfav
	Header fixed
	
	Revision 1.2  2004/09/14 16:48:22  marfav
	Using TaskPoolLauncher instead of TaskPool
	
	Revision 1.1.1.1  2004/04/07 13:31:01  marfav
	Import libSchedCommands
	

*/

#ifndef _SCABSTRACTCOMMAND_H_
#define _SCABSTRACTCOMMAND_H_

#include <acs_c++config.hpp>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class ExecutorBase;
	
class ScAbstractCommand
{
public:
	ScAbstractCommand();
	ScAbstractCommand(const ScAbstractCommand &);
	ScAbstractCommand& operator= (const ScAbstractCommand &);
	virtual ~ScAbstractCommand() throw() ;

	void param (const std::string &);
	const std::string &param() const throw() ;
	void value (const std::string & );
	const std::string &value() const throw() ;

	virtual void execute (ExecutorBase&) {} // PRQA S 2131 2
	virtual std::string getSignature () const {return "Abstract";}

private: //attributes
	std::string _param;
	std::string _value;
};



_ACS_END_NAMESPACE


#endif // _SCABSTRACTCOMMAND_H_
