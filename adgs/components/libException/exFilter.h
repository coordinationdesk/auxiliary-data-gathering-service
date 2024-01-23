// PRQA S 1050 EOF
/*

	Copyright 1995-2018 , Advanced Computer Systems , Inc.
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
	Revision 5.3  2013/11/19 17:52:01  marpas
	using  exostream::getDefaultErrLogger()
	
	Revision 5.2  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.1  2013/06/06 08:32:59  marpas
	minor changes
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.2  2013/04/29 15:39:01  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.1  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.1  2011/03/18 14:04:42  marpas
	minor changes
	
	Revision 3.0  2009/10/08 10:48:09  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.7  2004/09/20 14:16:24  marpas
	exAdaptiveFilter class introduced and tested
	
	Revision 1.6  2004/09/20 12:46:21  marpas
	Adaptative filter introduced
	
	Revision 1.5  2004/04/14 11:06:13  marpas
	filter installation improved
	
	Revision 1.4  2004/04/14 09:05:09  marpas
	many changes
	
	Revision 1.3  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.2  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.1  2002/10/03 15:05:38  marpas
	exFilter and exFilterable added
	

*/

#ifndef _exFilter_H_
#define _exFilter_H_ 

#include <acs_c++config.hpp>
#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)

class exFilterable ;

/*! \class exFilter

	\brief Base class for filters
	
	This class is the base class for all message filters. 
	The user has to reimplement the filterInsertion method to allow(true) or deny(false) 
	the incoming message to be handled to the stream to wich the filter is attached.

	\note A filter can be attached to only one stream. 
	Attaching it to another stream generally will result in detaching the filter from the previosly attaching stream.
*/
class exFilter
{
public:
	explicit exFilter(exostream *s=&exostream::getDefaultOutLogger()) ;
	virtual ~exFilter() ACS_NOEXCEPT ;
	exFilter(const exFilter &) ;
	exFilter &operator=(const exFilter &) ;
	
	/*! the method returns always true
		It needs to be reimplemented in derived classes to
		filter objects.
	*/
	virtual bool filterInsertion(exFilterable &) ;
	void streamDestroying(exostream const *) ACS_NOEXCEPT ;

	void reassignTo(exostream * ) ;
private:
	exostream *_filteredStream ;
} ;

_ACS_END_NAMESPACE

#endif // _exFilter_H_
