// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Standalone Application Library $

	$Id$

	$Author$

	$Log$
	Revision 1.6  2012/12/11 15:57:58  marpas
	qa rules
	
	Revision 1.5  2012/02/14 14:46:52  marpas
	refactoring
	
	Revision 1.4  2008/06/04 15:50:30  enrcar
	EC:: fileOperation01Factory intantiated
	
	Revision 1.3  2006/04/21 13:40:01  clanas
	removed Mcf handler
	
	Revision 1.2  2006/04/20 15:17:43  clanas
	defalt MCF set to false
	
	Revision 1.1.1.1  2006/04/20 12:15:46  clanas
	imported source code
	
	
	

*/


#ifndef _StandaloneApp_H_
#define _StandaloneApp_H_

#include <Application.h>
#include <string>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class exLogStream ;
class exostream ;

class exAdaptiveFilter ;

class StandaloneApp : public Application // PRQA S 2109
{

public:
	StandaloneApp(std::string const & name, std::string const &subSystem, bool nomcf=true);
	virtual ~StandaloneApp() throw() ;

protected:

	/**
	 * inherited from Application
	 */	
	virtual void postDaemonize();
	virtual void preDaemonize();

	bool allowMcf() const ;

	virtual int start(int argc, char const * const * argv, char const * const * env);
	virtual void initPlugins(int argc, char const * const * argv) ;
	virtual void justBeforeStartEnd() ;
	exAdaptiveFilter *getAdaptiveFilter() ;


private:
	// reimplemented from Application
	virtual void beforeRestart();

	// declared but not implemented to prevent their use
	StandaloneApp();
	StandaloneApp(const StandaloneApp & );
	StandaloneApp &operator=(const StandaloneApp & );
private:
	exAdaptiveFilter *_adaptiveFilter ;

	bool _allowMcf ;		

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StandaloneApp) ;

};


_ACS_END_NAMESPACE

#endif //_StandaloneApp_H_

