// PRQA S 1050 EOF
/*
   Copyright 1995-2021, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA. 

   libMQTT class
*/ 


#ifndef _MQTTWorker_H_
#define _MQTTWorker_H_

#include <StopController.h>

#include <acs_c++pers.hpp>
#include <Singleton.hpp>
#include <ThreadSafe.h>
#include <StringUtils.h>
#include <string>
#include <vector>

// forward declaration of libmosquitto stuff
struct mosquitto;
struct mosquitto_message;

_ACS_BEGIN_NAMESPACE(acs) 
		
class MQTTWorker
{
public:

	// Enum to track the connection status with a state machine
	enum ConnStatus
	{
		ConnStatusIDLE,
		ConnStatusCONNECTING,
		ConnStatusCONNECTED,
		ConnStatusERROR
	};

	static std::string connStatusToString (const ConnStatus& s)
	{
		std::string retval;
		
		switch (s)
		{
			case ConnStatusIDLE:
				retval = "ConnStatusIDLE";
			break;
			case ConnStatusCONNECTING:
				retval = "ConnStatusCONNECTING";
			break;
			case ConnStatusCONNECTED:
				retval = "ConnStatusCONNECTED";
			break;
			case ConnStatusERROR:
				retval = "ConnStatusERROR";
			break;
			default:
				retval = "ConnStatusUNKNOWN";
			break;
		}
		
		return retval;
	}

	typedef enum {
		/*
		 * QoS 0:
		 * The minimal QoS level is zero. 
		 * This service level guarantees a best-effort delivery.
		 * There is no guarantee of delivery.
		 * The recipient does not acknowledge receipt of the message and the message is not stored and re-transmitted by the sender.
		 * QoS level 0 is often called “fire and forget” and provides the same guarantee as the underlying TCP protocol.
		 */
		QOS_0 = 0,

		/*
		 * QoS 1:
		 * QoS level 1 guarantees that a message is delivered at least one time to the receiver.
		 */
		QOS_1 = 1,
		
		/*
		 * QoS 2:
		 * QoS 2 is the highest level of service in MQTT.
		 * This level guarantees that each message is received exactly once by the intended recipients.
		 * QoS 2 is the safest and slowest quality of service level.
		 * The guarantee is provided by at least two request/response flows (a four-part handshake) between the sender and the receiver.
		 */
		QOS_2 = 2
	} MQTT_QOS;
	
	/** Useful QOS enumerative conversion functions */
	
	static MQTT_QOS stringToMQTT_QOS(const std::string& qosstr) {
		MQTT_QOS qos = QOS_0;
		if (StringUtils::equalsNoCase(qosstr, "QOS_0")) {
			qos = QOS_0;
		}
		else if (StringUtils::equalsNoCase(qosstr, "QOS_1")) {
			qos = QOS_1;
		}
		else if (StringUtils::equalsNoCase(qosstr, "QOS_2")) {
			qos = QOS_2;
		}
		else {
			ACS_THROW(exIllegalValueException("Unknown MQTT QOS string \"" + qosstr + "\" given as input."));
		}
		
		return qos;
	}

	static std::string MQTT_QOSToString(MQTT_QOS qos) {
		std::string qosstr;
		switch (qos) {
		case QOS_0:
			qosstr = "QOS_0";
			break;
		case QOS_1:
			qosstr = "QOS_1";
			break;
		case QOS_2:
			qosstr = "QOS_2";
			break;
		default:
			// Never reached, just for completeness
			std::ostringstream ss;
			ss << "Unknown MQTT QoS value " << static_cast<unsigned int>(qos) << " given as input";
			ACS_THROW(exIllegalValueException(ss.str()));
		}
		
		return qosstr;
	}

	static MQTT_QOS intToMQTT_QOS(int qosval) {
		if (qosval < 0) {
			std::ostringstream ss; 
			ss << "QoS negative values " << qosval << " are not allowed.";
			ACS_THROW(exIllegalValueException(ss.str()));
		}
		
		return static_cast<MQTT_QOS>(qosval);
	}

	static int MQTT_QOSToInt(MQTT_QOS qos) {
		return static_cast<int>(qos);
	}
	
	// Inner struct configuration 
	struct Configuration
	{
		std::string clientId;
		bool clean_session;
		std::string host;
		int port;
		int keepalive;
		std::string username;
		std::string password;
		
		Configuration () :
			clientId(""),
			clean_session(true),
			host("127.0.0.1"),
			port(1883),
			keepalive(60),
			username(""),
			password("")
		{}
		
		Configuration (const Configuration& c) :
			clientId(c.clientId),
			clean_session(c.clean_session),
			host(c.host),
			port(c.port),
			keepalive(c.keepalive),
			username(c.username),
			password(c.password)

		{}
		
		Configuration& operator= (const Configuration& c)
		{
			if ( this != &c )
			{
				clientId = c.clientId;
				clean_session = c.clean_session;
				host = c.host;
				port = c.port;
				keepalive = c.keepalive;
				username = c.username;
				password = c.password;
			}
			return *this;
		}
	};
	

	// Base exception for all the MQTTWorker related exception
	exDECLARE_EXCEPTION (MQTTException,exException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION (ConnectionError, MQTTException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION (ConnectionLookupError, ConnectionError) ; // Error Getting Address Information, i.e. DNS Error
	exDECLARE_EXCEPTION (PreconditionFailure, MQTTException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION (BadTopic, MQTTException) ; // PRQA S 2131, 2153, 2502


public:
	MQTTWorker(const std::string& name, const struct Configuration& config, StopController const& stopController = AppStopController()) ;
	virtual ~MQTTWorker() ACS_NOEXCEPT;

	// Return the worker name as set at ctor time
	const std::string& getName() const;

	// Get actual configuration
	const Configuration& getConfiguration() const;

	// callback prototypes
	void connectCallback (int rc);
	void disconnectCallback (int rc);
	void publishCallback (int mid);
	void messageCallback (const struct mosquitto_message *msg);
	void subscribeCallback (int mid, int qos_count, const int * granted_qos);
	void unsubscribeCallback (int mid);
	void logCallback (int level, const char *str);

	// Return info on the actual connection status
	bool isConnected() const;
	bool isLoopActive() const;
	bool isPendingConnect() const;

	// Connection tracking helpers
	const ConnStatus& getConnTrackStatus() const;
	const std::string& getConnTrackMessage() const;

/**
 * This has been extracted from the mosquitto headers and can change
 * verify it after updating to another version of mosquitto library
 * 
 * be aware that payload can be NULL (and payloadlen can be ZERO)
 * 
	struct mosquitto_message{
			int mid;
			char *topic;
			void *payload;
			int payloadlen;
			int qos;
			bool retain;
	};
 */ 

protected:

	// Terminate the connection and stop the mosquitto event loop
	// It is based on stopEventLoop and disconnect calls and
	// MUST be used by any derived class in their desctructors to prevent
	// callback notifications after the destruction of the classes  
	// implementing the notification handlers
	// Multiple call to this function shall be harmless
	void shutDownMQTTLayer();

	// Start thread managing the broker connection
	// It will automatically try to re-connect in case of failures
	bool connect(unsigned long timeoutMs = 5000);
	
	// Terminate connection if any
	bool disconnect(unsigned long timeoutMs = 5000);
	
	// Control the threaded loop inner object
	void startEventLoop();
	void stopEventLoop(bool force=false);
	
	// Helpers for attributes
	void setLoopActive (bool b);
	void setPendingConnect (bool b);
	void setConnected (bool b);

	// User defined implementation of callbacks
	// Shall be reimplemented by derived classes
	virtual void onConnect (int rc);
	virtual void onDisconnect (int rc);
	virtual void onPublish (int mid);
	virtual void onMessage (const struct mosquitto_message *msg);
	virtual void onSubscribe (int mid, const std::vector<MQTT_QOS> & grantedQOS);
	virtual void onUnsubscribe (int mid);
	virtual void onLog (int level, const std::string& message);

	// Internal implementation of callbacks
	// Used to create Publisher / Subscriber base classes from which others can derive
	virtual void internal_onConnect (int rc);
	virtual void internal_onDisconnect (int rc);
	virtual void internal_onPublish (int mid);
	virtual void internal_onMessage (const struct mosquitto_message *msg);
	virtual void internal_onSubscribe (int mid, const std::vector<MQTT_QOS> & grantedQOS);
	virtual void internal_onUnsubscribe (int mid);
	virtual void internal_onLog (int level, const std::string& message);

	// Used to implement thread safety also on derived classes
	const Mutex& getMutex() const; 
	
	// Access the internal instance
	struct mosquitto* getClient() const;
	
	// Manage connection tracking transactions
	void setConnTrackStatus(const ConnStatus& status);
	void setConnTrackMessage(int rc, const std::string& msg);


private:
	// Cannot copy me
	MQTTWorker(const MQTTWorker&);
	MQTTWorker& operator= (const MQTTWorker&);

private:
	// this will throw in case _mqttinstance is null
	void checkMQTTInstance() const ;
	
	void createMQTTInstance();
	void destroyMQTTInstance() ACS_NOEXCEPT;
	void refreshMQTTInstance();

private:
	std::string _name;
	struct Configuration _config;
	struct mosquitto* _mqttinstance;
	bool _connected;
	bool _loopActive;
	// Trace if a connection request is pending or not
	bool _pendingConnect;

	// Connection tracking status
	ConnStatus _connTrackStatus;
	
	// Error description string in case of connection issues
	std::string _connTrackMessage;

	// Attribute mutex, can be seen from outside
	Mutex _mutex;
	// Protect operations on the event loop to avoid concurrent start and stop from separate threads
	Mutex _eventLoopMutex;
	
	StopController const & _stopController ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTWorker) ;

};

_ACS_END_NAMESPACE

#endif // _MQTTWorker_H_
