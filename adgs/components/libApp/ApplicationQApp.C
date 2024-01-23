// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.2  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:59  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/10/14 09:59:56  marpas
	ApplicationQApp class added
	
*/


#include <ApplicationQApp.h>
#include <Application.h>
#include <qtimer.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ApplicationQApp::ApplicationQApp(Application &a,int & argc, char ** argv) :
	QApplication(argc, argv), 
	_application(a),
	_timer(0)
{

}

ApplicationQApp::~ApplicationQApp() ACS_NOEXCEPT
{
    delete _timer ;
} 


void ApplicationQApp::updateDebug() 
{
	_application.updateDebug() ;
}

_ACS_END_NAMESPACE

