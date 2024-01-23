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

	$Prod: A.C.S. Standalone Application Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/02/14 14:46:52  marpas
	refactoring
	
	Revision 1.1.1.1  2006/04/20 12:15:46  clanas
	imported source code
	
	

*/


#ifndef _StandaloneQtApp_H_
#define _StandaloneQtApp_H_

#include <StandaloneApp.h>

#include <qapplication.h>


_ACS_BEGIN_NAMESPACE(acs)

class ExceptionPanel ;

class StandaloneQtApp : public StandaloneApp
{
public:
	StandaloneQtApp(std::string const & name, std::string const &subSystem, bool nopatrol=false);
	virtual ~StandaloneQtApp() throw() ;
	QApplication * qtApp();
	int startMainLoop();
	virtual ApplicationDebug::Mode debugMode(int argc, char const * const * argv, char const * const * env, std::string &debugPath) const ;

protected:
	// to implement special calls prior to create a QtApplication
	virtual void preQtApp() ;
	virtual int start(int argc, char const * const * argv, char const * const * env);
	virtual void justBeforeStartEnd() ;

private:
	QApplication *_qtApp;
	ExceptionPanel *_exceptionPanel ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StandaloneQtApp) ;

};


_ACS_END_NAMESPACE

#endif //_StandaloneQtApp_H_

