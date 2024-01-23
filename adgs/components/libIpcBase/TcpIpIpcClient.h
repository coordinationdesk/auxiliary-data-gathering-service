// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer IPC Library $

	$Id$

	$Author$

	$Log$
	Revision 5.7  2015/11/19 10:43:25  marpas
	using macros to throw exceptions
	
	Revision 5.6  2013/11/28 11:30:27  marpas
	qa warning fixed
	
	Revision 5.5  2013/10/22 10:51:27  marpas
	qa warning
	
	Revision 5.4  2013/10/14 15:40:25  marfav
	Forcing \n at the end of any message to be sent
	
	Revision 5.3  2013/10/08 08:41:32  marpas
	compilations warnings fixed
	
	Revision 5.2  2013/10/07 18:23:40  marpas
	qa warnings & coding best practices
	
	Revision 5.1  2013/10/02 18:47:04  marpas
	fixing error message
	
	Revision 5.0  2013/07/07 18:26:23  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	
	Revision 1.5  2012/06/05 13:28:41  marfav
	compiler warnings removed
	
	Revision 1.4  2012/06/05 10:30:27  marfav
	Robustness improved when closing file descriptors
	and when using inet structures (memset used) - S2GPP-476
	
	Revision 1.3  2011/03/01 16:23:33  marpas
	GCC 4.4.x support
	
	Revision 1.2  2011/01/24 15:53:27  marpas
	Mutex.h does not longer exixts
	
	Revision 1.1.1.1  2007/02/19 11:05:17  marant
	Import libIpcBase
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/10/12 15:21:23  marpas
	GCC 3.4.3 complains fixed
	
	Revision 1.3  2005/01/17 09:51:33  clanas
	insertd std scope resolutor into includes to compile with ICC
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:13  marfav
	Import libTlIpc
	


*/


#ifndef _TCPIPIPCCLIENT_H
#define _TCPIPIPCCLIENT_H

#include <acs_c++config.hpp>
#include <SigPipeGuard.h>
#include <Network.h>

#include <cstdio>
#include <sys/types.h> // PRQA S 1013 3
#include <sys/socket.h>
#include <netinet/in.h>
#if (__GNUC__  > 3)  ||  (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#include <arpa/inet.h>  // PRQA S 1013
#endif
#include <netdb.h>
#include <unistd.h>
#include <csignal>

#include <ThreadSafe.h>
#include <exException.h>
#include <Filterables.h>

#include <string>
#include <iostream>
#include <cstring>


_ACS_BEGIN_NAMESPACE(acs)


template <class T>
class TcpIpIpcClient // PRQA S 2109
{
public:
    exDECLARE_EXCEPTION(TcpMarshalException, exException) ;   // PRQA S 2502
public:
	explicit TcpIpIpcClient(int = -1);
	virtual ~TcpIpIpcClient() throw() ;

	void setServerByHostName(const std::string&);
	void setServerByIpAddress (const std::string&);
	void setServerPort (unsigned short);

	void sendMessage (const T&);

protected:
	// Derived classes must implement the marshal method
	// and call the sendToServer method passing the marshalled string
	// marshal must not use internal attributes of the base class
	virtual void marshal (const T&) = 0;
	virtual void sendToServer (const std::string&);


private:
	// obtain a socket and assign it to a file descriptor
	void setChannell();
    TcpIpIpcClient(const TcpIpIpcClient &) ; // not implemented 
    TcpIpIpcClient& operator= (const TcpIpIpcClient &) ; // not implemented 
public:
    static const unsigned short defaultClientPort = 3550 ;
private:
	int _mainFd;
	int _userFd;
	bool _channelSet;
	sockaddr_in _serverAddress;
	acs::ThreadSafe _ipcClientMutex;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TcpIpIpcClient) ;
};


ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(TcpIpIpcClient,T) ;


template<class T>
TcpIpIpcClient<T>::TcpIpIpcClient (int userFd) :
			_mainFd(-1),
			_userFd (userFd),
			_channelSet(false),
            _serverAddress(),
            _ipcClientMutex()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start") ;


	// set some "dummy" values
	memset (&_serverAddress, 0, sizeof (struct sockaddr_in));
	_serverAddress.sin_family = AF_INET;
	setServerByHostName ("localhost");
	setServerPort (defaultClientPort);
}

template<class T>
TcpIpIpcClient<T>::~TcpIpIpcClient () throw() 
{
	if (_channelSet) {
		close(_mainFd);
    }
}


#pragma GCC diagnostic ignored "-Wold-style-cast"
template<class T> void
TcpIpIpcClient<T>::setServerPort (unsigned short thePort)
{
	acs::ThreadSafe::Lock guard(_ipcClientMutex); // acquire the lock
	_serverAddress.sin_port = htons (thePort);
	// release the lock
}
#pragma GCC diagnostic warning "-Wold-style-cast"


template<class T> void
TcpIpIpcClient<T>::setServerByHostName (const std::string &hostName)
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start") ;
	
	std::vector<std::string> ipaddresses = net::Network::getIPAddresses(hostName,_serverAddress.sin_family);
	ACS_COND_THROW( ipaddresses.empty(), exIOException( "Cannot resolve hostname " + hostName));	
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Setting " << ipaddresses[0] << " as " << hostName) ;
    setServerByIpAddress(ipaddresses[0]) ;
}


template<class T> void
TcpIpIpcClient<T>::setServerByIpAddress (const std::string &ipAddress)
{
	acs::ThreadSafe::Lock guard(_ipcClientMutex); // acquire the lock
	if(!inet_aton(ipAddress.c_str() , &(_serverAddress.sin_addr)))
	{
		std::ostringstream os;
		os << "Cannot resolve IP address " << ipAddress;
		ACS_THROW (exIOException( os.str() , errno )); // PRQA S 3081
	}
	// release the lock
}



template<class T> void
TcpIpIpcClient<T>::setChannell () // PRQA S 4020
{
	// PRIVATE - no need to lock it !
	if (_channelSet) {
		return;
    }

	int fd = socket (AF_INET, SOCK_STREAM, 0);
	ACS_COND_THROW (fd == -1,
        exIOException( "Cannot obtain a socket.", errno )); // PRQA S 3081

    _mainFd = fd;
	_channelSet = true;
}


template<class T> void
TcpIpIpcClient<T>::sendMessage (const T& message)
{
	// DO NOT LOCK
	// the lock will be acquired by sendToServer method
	marshal (message);
}

template<class T> void
TcpIpIpcClient<T>::sendToServer (const std::string& aMessage)
{
	acs::ThreadSafe::Lock guard(_ipcClientMutex); // acquire the lock

	std::string theMessage (aMessage);

	// Check that a \n is placed at the end of the message
	if (theMessage[theMessage.size()-1] != '\n') // PRQA S 3084
	{
		// Add it if not present
		theMessage += "\n";
	}

	// Prepare the channel only if user has not selected a descriptor
	if (_userFd == -1)
	{
		setChannell();
		if ( connect (_mainFd, reinterpret_cast<struct sockaddr *>(&_serverAddress), sizeof(struct sockaddr)) == -1) // PRQA S 3030, 3081
		{
			ACS_THROW (exIOException( "Cannot connect to server.", errno )); // PRQA S 3081
		}
	}

	// send the message to the selected descriptor
	if (_userFd == -1)
	{
		ssize_t retval = send (_mainFd, theMessage.c_str(), strlen (theMessage.c_str()), MSG_NOSIGNAL);
		ACS_COND_THROW (retval != ssize_t(theMessage.length()), // PRQA S 3081
		    exIOException( "Error while sending data.", errno ));
	}
	else
	{

#ifndef DONT_USE_SIGPIPE_GUARD
		// Register the Sig_Ignore handler for SIGPIPE only in this scope
		SigPipeGuard _theGuard;
#else //DONT_USE_SIGPIPE_GUARD

		struct sigaction oldact;
		struct sigaction ignoreact;
		ignoreact.sa_handler = SIG_IGN;

		int result;

		// protect against SIGPIPE
		do
		{
			// retry in case of EINTR
			result = sigaction(SIGPIPE, &ignoreact, &oldact);
		}
		while ((result != 0) && (errno == EINTR));
		ACS_COND_THROW (result != 0,
            exIOException( "Error while disablig SIGPIPE signal.", errno )); // PRQA S 3081
#endif //DONT_USE_SIGPIPE_GUARD

		// write the string.... (add then flush)
		ssize_t retval = write (_userFd, theMessage.c_str(), strlen (theMessage.c_str()));
		ACS_COND_THROW (retval != ssize_t(theMessage.length()), // PRQA S 3081
		    exIOException( "Error while sending data.", errno ));
		fdatasync (_userFd);

#ifdef DONT_USE_SIGPIPE_GUARD
		// restore SIGPIPE
		do
		{
			// retry in case of EINTR
			result = sigaction(SIGPIPE, &oldact, NULL);
		}
		while ((result != 0) && (errno == EINTR));
		ACS_COND_THROW (result != 0,
            exIOException( "Error while restoring SIGPIPE handler.", errno )); // PRQA S 3081
#endif //DONT_USE_SIGPIPE_GUARD


	}

	// close the TCP/IP descriptor (if any)
	if (_userFd == -1)
	{
		close (_mainFd);
		// arm setChannel()
		_channelSet = false;
	}
	// release the lock
}

_ACS_END_NAMESPACE


#endif //_TCPIPIPCCLIENT_H

