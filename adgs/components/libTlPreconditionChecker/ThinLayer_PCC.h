// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.6  2015/11/20 11:57:18  marpas
	coding best practices application in progress
	
	Revision 1.5  2015/06/16 13:09:25  damdec
	Compilation warnings solved.
	
	Revision 1.4  2013/02/21 13:43:56  marfav
	Compiler warnings removed
	
	Revision 1.3  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.2  2008/11/07 15:40:53  marfav
	Added _deltaT in parameters
	
	Revision 1.1  2008/04/29 12:45:40  marfav
	Added support for PreconditionCheckFactoryChain
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#ifndef _THINLAYER_PCC_H_
#define _THINLAYER_PCC_H_


#include <PCC_Base.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class ThinLayer_PCC : public PCC_Base // PRQA S 2109, 2153
{


public:

    using PCC_Base::exitVisit;
    using PCC_Base::enterVisit;
    using PCC_Base::visit;

	exDECLARE_EXCEPTION (ThinLayer_PCC_Exception, exException) ; // PRQA S 2131, 2502
    
	explicit ThinLayer_PCC (StringKey factoryKey);
	virtual ~ThinLayer_PCC () throw() ;

	virtual bool doPreconditionCheck (TaskTable& tt, tlOrder anOrder, double _deltaT = 0, dbConnection* = 0);
	virtual void getVisitedTaskTable(TaskTable&);
	virtual void getSymbolTable(SymTable&);

	virtual bool exitVisit (TaskTable&);
	virtual bool exitVisit (Task&);
	virtual bool enterVisit (InputDescriptor&);
	virtual bool visit (InputDescriptor&);
	virtual bool exitVisit (InputDescriptor&);
	virtual bool visit (InputAlternative&);
    

protected:
	virtual void reset();
	virtual bool checkTaskTable(TaskTable&);

private:
    ThinLayer_PCC (); // not implmented
    ThinLayer_PCC (const ThinLayer_PCC&); // not implmented
    ThinLayer_PCC& operator= (const ThinLayer_PCC&); // not implmented


private:
	PCC_Params _myParameters;
	TaskTable *_myTaskTable; // we use a pointer to detect calls to getVisitedTaskTable before a call to visit(TaskTable)

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ThinLayer_PCC)
};
	
_ACS_END_NAMESPACE
		       
#endif //_THINLAYER_PCC_H_
