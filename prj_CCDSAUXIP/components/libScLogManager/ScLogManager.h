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
	Revision 5.4  2015/08/28 10:16:13  matteo.airoldi
	Message notification on syslog can be switched off
	
	Revision 5.3  2014/07/16 09:04:57  nicvac
	Global mutex use and destroying logic enforced
	
	Revision 5.2  2014/01/31 15:36:33  marfav
	S2PDGS-603 Processor messages can be filtered by configuration
	
	Revision 5.1  2013/09/26 19:34:36  marpas
	adopting libException Rev_5_16 interfaces
	test improved
	still doubles messages
	
	Revision 5.0  2013/07/07 18:27:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.4  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	
	Revision 1.1.1.1  2004/09/14 16:57:30  marfav
	Import libScLogManager
	


*/

#ifndef _ScLogManager_H_
#define _ScLogManager_H_

#include <acs_c++config.hpp>
#include <exStream.h>
#include <exFilter.h>


_ACS_BEGIN_NAMESPACE(acs)

class ScDefaultHandler : public exHandler // PRQA S 2109, 2153
{
public:
	ScDefaultHandler();
	virtual ~ScDefaultHandler() throw() ;

	virtual void notifyFilterable(exFilterable &);

    void notifyStdStreams (bool);
    bool notifyStdStreams() const;

	void notifyOnSyslog (bool);
	bool notifyOnSyslog () const;

	void setSyslogTaskMessageLimit (size_t);
	size_t getSyslogTaskMessageLimit () const;

	size_t getSyslogSkippedMessageCount() const;
	size_t getSyslogSkippedMessageCount(const std::string& ) const;

private:
	// declared but not implemented to prevent their use
	ScDefaultHandler (const ScDefaultHandler&);
	ScDefaultHandler& operator= (const ScDefaultHandler&);

private:
    bool _notifyStdStreams;
	bool _notifyOnSyslog;
	std::map <std::string, size_t> _issuedMessages;
	std::map <std::string, size_t> _skippedMessages;
	size_t _overallSkippedMessages;
	size_t _messageLimit;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ScDefaultHandler) ;
};



class ScLogManager : public exostream // PRQA S 2109, 2153
{
public:
	ScLogManager();
	virtual ~ScLogManager() throw() ;

    void notifyStdStreams (bool);
    bool notifyStdStreams() const;

	void notifyOnSyslog (bool);
	bool notifyOnSyslog () const;

	void setSyslogTaskMessageLimit (size_t);
	size_t getSyslogTaskMessageLimit () const;

	size_t getSyslogSkippedMessageCount() const;
	size_t getSyslogSkippedMessageCount(const std::string& ) const;

private:
	ScLogManager (const ScLogManager&);
	ScLogManager& operator= (const ScLogManager&);


private:
	ScDefaultHandler _defaultHandler;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ScLogManager) ;
};


// Default log stream for the scheduler
extern ScLogManager sclog;



_ACS_END_NAMESPACE



#endif //_ScLogManager_H_
