 /*
     Copyright 1995-2021, Exprivia SpA - DADF
     Via Della Bufalotta, 378 - 00139 Roma - Italy
     http://www.exprivia.com

     All Rights Reserved.

     This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
     the contents of this file may not be disclosed to third parties, copied or
     duplicated in any form, in whole or in part, without the prior written
     permission of Exprivia, SpA
*/

#ifndef _MQTTDBBROADCASTLISTENERBASE_H
#define _MQTTDBBROADCASTLISTENERBASE_H

#include <MQTTDbMultiSubscriber.h>
#include <exException.h>
#include <signal.h>

_ACS_BEGIN_NAMESPACE(acs)
class dbConnectionPool;

/* 
	Utility class used to save information on the Db broadcast events of interest
*/
class MQTTDbEventType
{
public:
	MQTTDbEventType(const std::string & table, const std::string & event);
	MQTTDbEventType();
	virtual ~MQTTDbEventType() ACS_NOEXCEPT {};

	MQTTDbEventType (const MQTTDbEventType&);
	MQTTDbEventType& operator = (const MQTTDbEventType&);
	MQTTDbEventType(MQTTDbEventType&& other); 
	MQTTDbEventType& operator=(MQTTDbEventType&& other);

	const std::vector <std::string> & getEvents() const;
	const std::string & getTableName() const ;
	void addEvent(const std::string & event);
	void setEvents(const std::vector <std::string> & events);
	void setTableName(const std::string & name);
	
private:
	std::vector <std::string> 	_events;	// vector of events associated to the _tableName
	std::string 			_tableName;
	
};


class MQTTDbBroadcastListenerBase : public MQTTDbMultiSubscriber
{
	public:
		MQTTDbBroadcastListenerBase(dbConnectionPool& cp) ;
		virtual ~MQTTDbBroadcastListenerBase() ACS_NOEXCEPT {}
		
		MQTTDbBroadcastListenerBase() = delete;
		MQTTDbBroadcastListenerBase (const MQTTDbBroadcastListenerBase&) = delete;
		MQTTDbBroadcastListenerBase& operator = (const MQTTDbBroadcastListenerBase&) = delete;
		MQTTDbBroadcastListenerBase(MQTTDbBroadcastListenerBase&& other) = delete; 
		MQTTDbBroadcastListenerBase& operator=(MQTTDbBroadcastListenerBase&& other) = delete;

		void startListening();
		void stopListening();
		void sendSignal(const bool b);
		bool isListening();
void reloadConfiguration() {} // FIXME TO BE IMPLEMENTED
		// Subscribe the Db broadcast events of interest
		void setDbEventInterests(const std::vector<MQTTDbEventType> & dbEvents);
		// filterMessage shound be re-implemented in the specialized classes
		virtual bool filterMessage(const MQTTMessageHandler & m);

	public:
		static void cb_emit_signal(bool b, size_t n, pthread_t id) ;
	private:
		//void init() ;

	private:
		ACS_CLASS_DECLARE_DEBUG_LEVEL (MQTTDbBroadcastListenerBase)
};
_ACS_END_NAMESPACE

#endif
