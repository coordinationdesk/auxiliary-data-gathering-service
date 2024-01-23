 /*
     Copyright 1995-2021, Exprivia SpA - DADF
     Via Della Bufalotta, 378 - 00139 Roma - Italy
     http://www.exprivia.it

     All Rights Reserved.

     This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
     the contents of this file may not be disclosed to third parties, copied or
     duplicated in any form, in whole or in part, without the prior written
     permission of Exprivia, SpA
*/
#include <MQTTDbBroadcastListenerBase.h>
#include <dbConnectionPool.h>
#include <ConfigurationSingleton.h>

using namespace std;
using namespace acs;
ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTDbBroadcastListenerBase) ;

MQTTDbEventType::MQTTDbEventType() :
				_events(), 
				_tableName()
{
}
MQTTDbEventType::MQTTDbEventType(const std::string & table, const std::string & event): 
				_events(), 
				_tableName(table)
{
	_events.push_back(event);
}
MQTTDbEventType::MQTTDbEventType (const MQTTDbEventType& other): 
				_events(other._events), 
				_tableName(other._tableName)
				{}
				
MQTTDbEventType& MQTTDbEventType::operator= (const MQTTDbEventType & other)
{
	if (this != &other) 
	{
		_events = other._events;
		_tableName = other._tableName;
	}
	return *this;
}
MQTTDbEventType::MQTTDbEventType(MQTTDbEventType&& other): 
	_events(std::move(other._events)),
	_tableName(std::move(other._tableName))
{
}
MQTTDbEventType& MQTTDbEventType::operator=(MQTTDbEventType&& other)
{
	if (this != &other) 
	{
		_events = std::move(other._events);
		_tableName = std::move(other._tableName);
	}
	return *this;
}
const std::vector <std::string> & MQTTDbEventType::getEvents() const
{
	return _events;
}
const std::string & MQTTDbEventType::getTableName() const 
{
	return _tableName;
}
void MQTTDbEventType::setEvents(const std::vector <std::string> & events)
{
	_events = events;
}
void MQTTDbEventType::addEvent(const std::string & event)
{
	_events.push_back(event);
}
void MQTTDbEventType::setTableName(const std::string & name)
{
	_tableName = name;
}


MQTTDbBroadcastListenerBase::MQTTDbBroadcastListenerBase(dbConnectionPool& cp) 
			: MQTTDbMultiSubscriber(/*HB_Enabled:*/false, cp, "127.0.0.1", 1884, cb_emit_signal, AppStopController()) 
{
    	rsResourceSet &conf = ConfigurationSingleton::instance()->get() ;
	string host = "";
	if (!conf.getValue("MQTTDbBroadcast.MQTTBrokerHost", host, false))
	{
		ACS_LOG_WARNING("MQTTDbBroadcast.MQTTBrokerHost configuration value not found. MQTTDbBroadcastListenerBase is connecting to the local host");
	}
	else
	{
		setHost(host);
	}
	int port = 0;
	if (!conf.getValue("MQTTDbBroadcast.MQTTBrokerPort", port, false))
	{
		ACS_LOG_WARNING("MQTTDbBroadcast.MQTTBrokerPort configuration value not found. MQTTDbBroadcastListenerBase is connecting to the default port 1884.");
	}
	else
	{
		setPort(port);
	}
}
void MQTTDbBroadcastListenerBase::startListening()			 
{ 
	if (!Thread::running())
	{
		Thread::start();
	}
	enable_messages (true) ; 
} 
void MQTTDbBroadcastListenerBase::stopListening()
{ 
	enable_messages (false) ; 
} 
void MQTTDbBroadcastListenerBase::sendSignal(const bool b)
{ 
	enable_callback(b) ; 
}
bool MQTTDbBroadcastListenerBase::isListening()
{ 
	return messages_enabled() ; 
}
bool MQTTDbBroadcastListenerBase::filterMessage(const MQTTMessageHandler & m)
{
	return true;
}

void MQTTDbBroadcastListenerBase::setDbEventInterests(const std::vector<MQTTDbEventType> & dbEvents)
{
	// the subscriptions should be cleaned before adding new...
	for (size_t k = 0; k < dbEvents.size(); ++k)
	{
		const std::vector <std::string> &tmpEvents = dbEvents[k].getEvents();
		string tableName = dbEvents[k].getTableName();
		// subscription for each event of interest
		for_each(tmpEvents.begin(), tmpEvents.end(), [&tableName, this](const string & event){ subscribe(true, tableName, event); });
	}
}

void MQTTDbBroadcastListenerBase::cb_emit_signal(bool b, size_t n, pthread_t id) 
{ 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "==== CALLBACK RECEIVED: " << (b?"T":"F") << "," << n); 
	pthread_kill(id, SIGUSR2);
}
