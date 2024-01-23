// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2018/09/13 10:14:09  marpas
	ying to decouple instances of formatter from global ctor and dtor
	
	Revision 5.3  2018/09/13 09:48:45  marpas
	atexit handler removed
	
	Revision 5.2  2018/09/12 10:05:39  marpas
	exception's stack trace management revived into streams.
	tested
	
	Revision 5.1  2013/11/15 13:19:30  marpas
	introducing the isFilterable concept into an exostream.
	It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.6  2013/05/14 17:58:44  marpas
	exSystemException added
	
	Revision 4.5  2013/04/29 15:39:01  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.4  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.2  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.1  2012/05/30 16:20:51  marpas
	S3PDGS-741 - exception's format depends on exMsgFmt
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.5  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.4  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.3  2008/03/14 13:45:05  integrator
	removed virtual inheritance from exMsgFmtFactory
	
	Revision 2.2  2008/02/15 16:58:54  marpas
	each stream can have its own formatter setFormatter method added
	exception::what() has no longer the stack if enabled
	exception::stack() returns the stack (if enabled)
	exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
	Filterables now hase details, they can be set via exFilterable::detail(...) method
	Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
	exception inserted into a Filterable will fill its detail with their stack (if any)
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.8  2004/04/28 10:11:22  marpas
	Lock policy improved
	ThreadSafe::SignalAction implemented
	
	Revision 1.7  2004/04/06 10:35:51  marpas
	cleanup
	
	Revision 1.6  2004/04/06 09:55:18  marpas
	new message architecture implemented: more freedom in formatting messages
	new class exFDebug added
	
	Revision 1.5  2004/03/24 11:21:54  marpas
	time method implemented in the factory to allow new factories to change the time format
	
	Revision 1.4  2004/03/24 11:11:10  marpas
	separator added
	
	Revision 1.3  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.2  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.1  2002/11/21 17:57:03  marpas
	- exostream modified for exFilterableManagement
	- exFEvent type added
	- exMsgFmtFactory added to provide tag and other values to the exFMessage
	class
	

*/

#ifndef _exMsgFmtFactory_H_
#define _exMsgFmtFactory_H_ 

#include <acs_c++config.hpp>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class exFilterable ;


class exMsgFmt { // PRQA S 2109
public:
	
	exMsgFmt(const exMsgFmt &) = delete ;
	exMsgFmt &operator=(const exMsgFmt &) = delete ;
	exMsgFmt() = default ;
	virtual ~exMsgFmt() = default ;
	/** convenience ctor for inheritance without need to call 
	  * the sequence emitPid(bool) ... emitTime(bool) ...
	  */
	exMsgFmt(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) ;
	
	virtual bool emitPid() const ;
	virtual void emitPid(bool) ;
	virtual bool emitTime() const ;
	virtual void emitTime(bool) ;
	virtual std::string startTag() const ;
	virtual void startTag(const std::string &s) ;
	virtual std::string endTag() const ;
	virtual void endTag(const std::string &s) ;
	virtual std::string separator() const ;
	virtual void separator(const std::string &s) ;
	virtual std::string time() const ; // PRQA S 2502
	virtual std::string text(const exFilterable &) const ;
	virtual std::string text(const std::exception &, bool stack) const ;

	virtual void appName(const std::string &) ;
	virtual const std::string &appName()const ;

private:
	std::string _appName = "" ;

	bool _emitPid = false ;
	bool _emitTime = false ;
	std::string _startTag = "" ;
	std::string _endTag = "" ;
	std::string _separator = " " ;
} ;


/*! \class exMsgFmtFactory

	\brief base class for messages formatters

	This class is useful to create standard message formatters.	
*/
class exMsgFmtFactory : public exMsgFmt { // PRQA S 2109,2153
public:
	// declared but not implemented
	exMsgFmtFactory(const exMsgFmtFactory &) = delete ;
	exMsgFmtFactory &operator=(const exMsgFmtFactory &) = delete ;

	static exMsgFmt *instance() ;
	
	exMsgFmtFactory() ;
	~exMsgFmtFactory() noexcept override ;
	
protected:
	/** convenience ctor for inheritance without need to call 
	  * the sequence emitPid(bool) ... emitTime(bool) ...
	  */
	exMsgFmtFactory(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) ;
} ;


_ACS_END_NAMESPACE

#endif // _exMsgFmtFactory_H_
