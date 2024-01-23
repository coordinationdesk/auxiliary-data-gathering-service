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

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2017/02/08 15:36:32  marfav
	CSGACQ-113
	Adding filtering capabilities to MC filterable family
	
	Revision 5.3  2013/11/19 17:52:01  marpas
	using  exostream::getDefaultErrLogger()
	
	Revision 5.2  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.1  2013/06/06 08:28:56  marpas
	coding best practices applied
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.3  2013/04/29 15:39:00  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.2  2012/12/11 17:17:09  marpas
	qa rules
	
	Revision 4.1  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.1  2011/03/18 14:05:17  marpas
	minor changes
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:54  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.2  2005/05/12 14:43:43  marpas
	exFOpAlert class added
	
	Revision 1.1  2004/09/20 14:16:24  marpas
	exAdaptiveFilter class introduced and tested
	
	Revision 1.1  2004/09/20 12:46:21  marpas
	Adaptative filter introduced
	
	

*/

#ifndef _exAdaptiveFilter_H_
#define _exAdaptiveFilter_H_ 

#include <acs_c++config.hpp>
#include <exFilter.h>

_ACS_BEGIN_NAMESPACE(acs)

/*! \class exAdaptiveFilter
 *	
 *	\brief  convenience class to filter messages on their category basis
 *
 *  The class is a basic filter to deal with the 6 categories OpAlert, Debug, Error, Event, Message and Warning.
 *  It can also be used to block or leave pass any other exFilterable not belonging to the main ones.
 *  Default behaviou is to pass the 6 categories and block any other directly derived from exFilterable.	
*/
class exAdaptiveFilter : public exFilter // PRQA S 2153
{
public:
	explicit exAdaptiveFilter(exostream *s=&exostream::getDefaultOutLogger()) ;
	virtual ~exAdaptiveFilter() ACS_NOEXCEPT ;
	exAdaptiveFilter(const exAdaptiveFilter &) ;
	exAdaptiveFilter &operator=(const exAdaptiveFilter &) ;
	
	void allowDebug(bool) ;
	void allowError(bool) ;
	void allowEvent(bool) ;
	void allowMessage(bool) ;
	void allowPrivateInfo(bool) ;
	void allowWarning(bool) ;
	void allowOpAlert(bool) ;
	void allowAnyFilterable(bool) ;
	void allowMCFilterable(bool);
	
	bool allowDebug() const ;
	bool allowError() const ;
	bool allowEvent() const ;
	bool allowMessage() const ;
	bool allowPrivateInfo() const ;
	bool allowWarning() const ;
	bool allowOpAlert() const ;
	bool allowAnyFilterable() const ;
	bool allowMCFilterable() const ;
	
	/*! The method checks the type of the filterable against the 6 big categories OpAlert, Debug, Error, Event Message and Warning 
	 *  and they derived ones returning true or false according with their flag respective state.
	 *  In case the class is not belonging to the 6 above mentioned categories, hence it is directly derived from exFilterable
	 *  it returns true or false according to the AnyFilterable setting.
	*/
	virtual bool filterInsertion(exFilterable &) ;

private:
	bool _allowDebug ;
	bool _allowError ;
	bool _allowEvent ;
	bool _allowMessage ;
	bool _allowPrivateInfo ;
	bool _allowWarning ;
	bool _allowOpAlert ;
	bool _allowAnyFilterable ;
	bool _allowMCFilterable ;
} ;

_ACS_END_NAMESPACE

#endif // _exAdaptiveFilter_H_
