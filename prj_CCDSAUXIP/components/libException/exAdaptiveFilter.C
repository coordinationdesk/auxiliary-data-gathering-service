/*

	Copyright 1995-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/02/08 15:36:32  marfav
	CSGACQ-113
	Adding filtering capabilities to MC filterable family
	
	Revision 5.2  2013/06/18 12:25:19  marpas
	some include files are now deprecated
	
	Revision 5.1  2013/06/06 08:28:52  marpas
	coding best practices applied
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.5  2013/04/29 15:39:00  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.4  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/22 13:41:46  marpas
	pr qa instrumentation
	
	Revision 4.2  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.1  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.1  2011/03/18 14:05:17  marpas
	minor changes
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:54  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.2  2005/05/12 14:43:43  marpas
	exFOpAlert class added
	
	Revision 1.1  2004/09/20 14:16:24  marpas
	exAdaptiveFilter class introduced and tested
	
	Revision 1.1  2004/09/20 12:46:21  marpas
	Adaptative filter introduced
	
	

*/

#include <exAdaptiveFilter.h>
#include <Filterables.h>
#include <MCFilterables.h>

_ACS_BEGIN_NAMESPACE(acs)





exAdaptiveFilter::exAdaptiveFilter(exostream *s) : 
	exFilter(s)
{
}

bool exAdaptiveFilter::filterInsertion(exFilterable &f) // PRQA S 4020 
{
// returns the boolean corresponding to the base class

	// MC Parameters must be checked firts because have multiple inheritance
	if (dynamic_cast<mc::MCScopedParams *>(&f)) { // PRQA S 3081
		return _allowMCFilterable ;
    }
	if (dynamic_cast<exFDebug *>(&f)) { // PRQA S 3081
		return _allowDebug ;
    }
	if (dynamic_cast<exFOpAlert *>(&f)) { // PRQA S 3081
		return _allowOpAlert ;
    }
	if (dynamic_cast<exFError *>(&f)) { // PRQA S 3081
		return _allowError ;
    }
	if (dynamic_cast<exFEvent *>(&f)) { // PRQA S 3081
		return _allowEvent ;
    }
	if (dynamic_cast<exFWarning *>(&f)) { // PRQA S 3081
		return _allowWarning ;
    }
	if (dynamic_cast<exFPrivateInfo *>(&f)) { // PRQA S 3081
		return _allowPrivateInfo ;
    }
	if (dynamic_cast<exFMessage *>(&f)) { // PRQA S 3081
		return _allowMessage ;
    }

// no specific class, must be other	
	return _allowAnyFilterable ;	
}

void exAdaptiveFilter::allowDebug(bool v) // PRQA S 4121
{
	_allowDebug = v ;
}

void exAdaptiveFilter::allowError(bool v) // PRQA S 4121
{
	_allowError = v ;
}

void exAdaptiveFilter::allowEvent(bool v) // PRQA S 4121
{
	_allowEvent = v ;
}

void exAdaptiveFilter::allowMessage(bool v) // PRQA S 4121
{
	_allowMessage = v ;
}

void exAdaptiveFilter::allowPrivateInfo(bool v) // PRQA S 4121
{
	_allowPrivateInfo = v ;
}

void exAdaptiveFilter::allowWarning(bool v) // PRQA S 4121
{
	_allowWarning = v ;
}

void exAdaptiveFilter::allowOpAlert(bool v) // PRQA S 4121
{
	_allowOpAlert = v ;
}

void exAdaptiveFilter::allowAnyFilterable(bool v) // PRQA S 4121
{
	_allowAnyFilterable = v ;
}

void exAdaptiveFilter::allowMCFilterable(bool v) // PRQA S 4121
{
	_allowMCFilterable = v ;
}

bool exAdaptiveFilter::allowDebug() const  // PRQA S 4120
{
	return _allowDebug ;
}

bool exAdaptiveFilter::allowError() const // PRQA S 4120
{
	return _allowError ;
}

bool exAdaptiveFilter::allowEvent() const // PRQA S 4120
{
	return _allowEvent ;
}

bool exAdaptiveFilter::allowMessage() const // PRQA S 4120
{
	return _allowMessage ;
}

bool exAdaptiveFilter::allowPrivateInfo() const // PRQA S 4120
{
	return _allowPrivateInfo ;
}

bool exAdaptiveFilter::allowWarning() const // PRQA S 4120
{
	return _allowWarning ;
}

bool exAdaptiveFilter::allowOpAlert() const // PRQA S 4120
{
	return _allowOpAlert ;
}

bool exAdaptiveFilter::allowAnyFilterable() const // PRQA S 4120
{
	return _allowAnyFilterable ;
}

bool exAdaptiveFilter::allowMCFilterable() const // PRQA S 4120
{
	return _allowMCFilterable ;
}
	

_ACS_END_NAMESPACE
