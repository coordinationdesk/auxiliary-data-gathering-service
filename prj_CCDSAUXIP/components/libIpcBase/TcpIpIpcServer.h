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
	Revision 5.9  2015/11/19 16:48:25  marpas
	qa warning fixed
	
	Revision 5.8  2015/11/19 15:51:57  marpas
	coding best practice applied
	
	Revision 5.7  2015/11/19 10:43:25  marpas
	using macros to throw exceptions
	
	Revision 5.6  2014/03/14 10:10:30  marfav
	Avoid the copy of the string when unmarshalling
	
	Revision 5.5  2013/10/07 18:23:40  marpas
	qa warnings & coding best practices
	
	Revision 5.4  2013/10/07 08:20:01  marpas
	coding best practices
	qa warnings
	
	Revision 5.3  2013/10/03 15:06:16  marpas
	typo error
	
	Revision 5.2  2013/10/03 15:03:27  marpas
	fixing typo
	
	Revision 5.1  2013/07/26 09:28:55  marfav
	Exit latency (select timeout) lowered to 5 milliseconds
	
	Revision 5.0  2013/07/07 18:26:23  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	
	Revision 1.11  2012/06/05 13:28:41  marfav
	compiler warnings removed
	
	Revision 1.10  2012/06/05 10:30:27  marfav
	Robustness improved when closing file descriptors
	and when using inet structures (memset used) - S2GPP-476
	
	Revision 1.9  2011/03/01 16:23:33  marpas
	GCC 4.4.x support
	
	Revision 1.8  2011/01/24 15:53:27  marpas
	Mutex.h does not longer exixts
	
	Revision 1.7  2011/01/24 12:59:39  marpas
	exDECLARE_EXCEPTION libException(3.8) is now inlined and can be used inside templates
	
	Revision 1.6  2010/09/13 16:30:36  marpas
	linException 3.4 interface adopted
	
	Revision 1.5  2009/06/26 10:43:41  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.4  2009/03/11 18:44:36  marpas
	new exceptions copy ctor added
	
	Revision 1.3  2007/04/11 14:40:22  clanas
	inserted an std scope
	
	Revision 1.2  2007/03/26 14:12:57  nicvac
	Optional max allowed message length handling.
	
	Revision 1.1.1.1  2007/02/19 11:05:17  marant
	Import libIpcBase
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/10/13 08:43:41  marfav
	inline max method definition fixed
	
	Revision 1.10  2005/10/12 15:21:23  marpas
	GCC 3.4.3 complains fixed
	
	Revision 1.9  2005/06/29 15:03:17  marfav
	Attributes protected instead of private
	
	Revision 1.8  2005/05/20 16:16:16  marfav
	Standard input stream check added
	
	Revision 1.7  2005/01/17 09:51:33  clanas
	insertd std scope resolutor into includes to compile with ICC
	
	Revision 1.6  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.5  2004/07/06 13:18:56  marfav
	Using stop instead of exit when joining the thread
	
	Revision 1.4  2004/06/21 10:02:22  marfav
	Closing all accepted connections while stopping
	
	Revision 1.3  2004/05/05 08:19:12  marfav
	Align to new Thread::nanosleep interface
	
	Revision 1.2  2004/04/13 13:55:44  marfav
	Fixed include dependencies
	
	Revision 1.1.1.1  2004/04/07 13:33:14  marfav
	Import libTlIpc
	


*/



#ifndef _TCPIPIPC_SERVER_H_
#define _TCPIPIPC_SERVER_H_

#include <acs_c++config.hpp>

#include <Thread.h>
#include <Filterables.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstring>
	
#include <sys/types.h> // PRQA S 1013 6
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/times.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 *\brief Virtual class used to accept TCP/IP connections for receive messages.
 *
 * Each instance of the class starts a Thread listening on a configurable
 * TCP port for incoming connections.
 * Each class inheriting from TcpIpIpcServer must reimplement unmarshal()
 * that is in charge of unpacking messages.
 */

template <class T>
class TcpIpIpcServer : protected Thread // PRQA S 2109, 2194
{
public: 
	using Thread::runException ;

public:

	/**
	 * class TcpUnmarshalException declaration 
	 * An TcpUnmarshalException is thrown each time the UDP packet received is not valid.
	 */	
	exDECLARE_EXCEPTION(TcpUnmarshalException, exException) ; // PRQA S 2502


	explicit TcpIpIpcServer(bool tcp = true, bool std_in = false, unsigned long max_msg_size=0) :
        Thread(),
	    _listenTcpIp(tcp),
	    _listenStdin(std_in),
	    _messagesTaken(false),
        _messageList(),
        _mutex(),
	    _mainFd(-1),
	    _port(),
        _connectedFd(),
        _message(),
	    _maxMsgSize(max_msg_size)
    {
	    _connectedFd.clear();

	    if (_listenStdin) {
		    _connectedFd.push_back(0); // add descriptor of standard input stream
        }
    }

	virtual ~TcpIpIpcServer() throw()
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start") ;

	    try {
		    stopListening();
	    }
	    catch(std::exception &x) {
		    ACS_LOG_NOTIFY_EX(x) ;
	    }
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "End") ;
    }

	/**
	 * \brief Sets the port to use for accept incoming connections
	 * \param port the fisrt value.
	 */	
	void setPort(unsigned short port) 
    {
	    acs::Lock guard(_mutex);
	    _port=port;
    }
	unsigned short getPort() const throw() { return _port; }

	/**
	 * \brief Set listening channels and start listening thread
	 *
	 * If in error occurs during the channels initialization an exIOException will be thrown
	 */
	void startListening()
    {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start") ;

	    if(! running() )
	    {
		    if (_listenTcpIp)
		    {
			    setChannel();
		    }
		    _messageList.clear();
		    start();
	    }//if(! running() )
    }

	bool isListening() const { return running(); }
	void stopListening()
    {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start") ;
	    if(running())
	    {
		    this->stop(); // Join the thread

		    if (_listenTcpIp)
		    {
			    if (_mainFd > 0)
			    {
				    close(_mainFd);
				    _mainFd=-1;
			    }
		    }

		    // Close all accepted connections
            size_t cfd_size = _connectedFd.size() ;
		    for (size_t i=0; i<cfd_size; ++i)
		    {
			    if (_connectedFd[i] > 0)
			    {
				    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Closing incoming connection [" << _connectedFd[i] << "]") ;
				    close (_connectedFd[i]);
			    }
		    }
		    _connectedFd.clear();
	    }
	    else
	    {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The IPC server was not running") ;
	    }

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "End") ;
    }
    
	/**
	 * Fill std::vector v with new income messages.
	 */
	void getMessages(std::vector<T> &callerList)
    {
	    acs::Lock guard(_mutex); //automaticaly acquire the lock
	    callerList = _messageList;
	    _messageList.clear();
    }

	bool popMessage(T& element)
    {
	    bool result = false ;
	    {
		    acs::Lock guard(_mutex); //automaticaly acquire the lock
		    result = (_messageList.size() != 0);
		    if (result)
		    {
			    element = _messageList.front();
			    _messageList.erase (_messageList.begin());
		    }
	    }
	    // release the lock
	    return result;
    }


	/**
	 * \return income messages number.
	 */
	size_t messagesNumber() const
    {
	    acs::Lock guard(_mutex); //automaticaly acquire the lock
	    return _messageList.size();
    }

protected:

	/**
	 * This function decides whether a message should be stored or discarded.
	 * Default method always returns true.
	 */
	virtual bool filterMessage (const T & message) { return true; } // PRQA S 2131


	/**
	 * This method is in charge of unpacking messages.
	 * For every message in incoming buffer unmarshal must call putMessage(const T &message).
	 */
	virtual void unmarshal(const std::string&) =0 ;

	/**
	 * Used by unmarshal(const string &, unsigned int).
	 * It calls filterMessage(const T & message) and then store the message.
	 */
	void putMessage(const T &message)
    {
       if( filterMessage(message) )
       {
	       //Now comes the critical section
	       {
		       acs::Lock guard(_mutex); //automaticaly acquire the lock
   		       _messageList.push_back(message);
	       }
	       _messagesTaken=true;
       }
    }

	/**
	 * This method is executed by listening thread at the end of every loop.
	 * The loop ends when a messages packet is received or a timer expires (the timer duration is about two seconds) .
	 * Custom action shouldn't take too much time.
	 */
	virtual void customAction() {}


	/**
	  *  Accept new incoming connections and open a socket descriptor for it
	  */
	virtual void acceptNewConnection()
    {
    	struct sockaddr_in client;
	    socklen_t sin_size;

	    sin_size = sizeof (struct sockaddr_in);
	    int newFd = accept (_mainFd, reinterpret_cast <struct sockaddr*> (&client), &sin_size) ;
	    ACS_COND_THROW(newFd == -1,
            exIOException("Error in accepting new connection.", errno )); // PRQA S 3081
	    
        _connectedFd.push_back (newFd);
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Added new connection to : " << inet_ntoa(client.sin_addr) << "(fd " << newFd << ")") ;
    }


	/**
	  *  Read the input packet from a socket descriptor, compose the message and call the unmarshall method
	  */
	virtual void manageConnection (int i)
    {

	    int fd = _connectedFd[i];
	    char buf [TCP_BUFFER_SIZE+1]; 
	    int count = read(fd, buf, TCP_BUFFER_SIZE);

	    if (count == -1)
	    {
		    // there was an error... close this connecton an go ahead
		    close (fd);
		    _connectedFd.erase (_connectedFd.begin() + i);
		    if (fd != 0)
		    {
			    // This is a read from a socket
			    ACS_THROW(exIOException("Error in reading from a socket descriptor.", errno )) ; // PRQA S 3081
		    }
		    else
		    {
			    // The standard input is closed... end of file reached
			    // is not necessary to raise an exception
			    // just disabling the standard input channel
			    _listenStdin = false;	
		    }
	    }
	    else if (count == 0)
	    {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "connection closed by foreign host.") ;
		    close (fd);
		    _connectedFd.erase (_connectedFd.begin() + i);
	    }
	    else
	    {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "read " << count << " chars on socket descriptor  [" << fd << "]") ;

		    buf[count] = '\0';
		    std::replace(buf, &buf[count], '\r', ' '); 	// protect against winzoz maniacs!

		    // remove trailing carriage return
		    if (buf[count-1] == '\n')
		    {
			    buf[count-1] = '\0';
			    _message[fd].append(buf);
			    // A line was read

			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Line read from socket : [" << _message[fd] << "]") ;
			    // make a swap between an empty string and the _message[fd]
			    // this is to avoid a strange effect on the string copy that does not make
			    // a deep copy but shares the internal pointer
			    std::string theMessage ("");
			    theMessage.swap (_message[fd]);
			    // _message[fd]=""; - useless. already done during the swap
			    unmarshal (theMessage);
		    }
		    else
		    {
			    _message[fd].append(buf);

			    if (_maxMsgSize)
			    {
			        if (_message[fd].size() >= _maxMsgSize)
			        {
					    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "max incomig msg size (" << _maxMsgSize
							           << ") exceeded.  Server forcing connection closing.") ;
				        close (fd);
				        _connectedFd.erase (_connectedFd.begin() + i);
			        }
			    }
		    }
	    }
    }

private:
	// declared but not implemented to prevent their use.
	TcpIpIpcServer(const TcpIpIpcServer &);
	TcpIpIpcServer& operator=(const TcpIpIpcServer &);

private:
	virtual void run();
	void setChannel();
public:
    static const int TCP_BUFFER_SIZE  = 1500 ;
    static const int BACKLOG = 10 ;

	static const unsigned long _sleepTime = 100000000 ; // nanoseconds (1/10 sec)
	static const unsigned long _selectTimeout = 100000 ; // microseconds (1/10 sec)

private:
	bool _listenTcpIp;
	bool _listenStdin;
	bool _messagesTaken;
	std::vector<T> _messageList;
	acs::Mutex _mutex;
	int _mainFd;
	unsigned short _port;
	std::vector<int> _connectedFd;
	std::map<int,std::string> _message;
	unsigned long _maxMsgSize;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(TcpIpIpcServer) ;
};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(TcpIpIpcServer,T) ;



// Deactivate old sytle cast warning because of
// FD_SET, FD_ISSET, AF_INET, INADDR_ANY
// macros using it
// PRQA S 1040 2
#pragma GCC diagnostic ignored "-Wold-style-cast"

template <class T> inline
void TcpIpIpcServer<T>::run()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start") ;

	struct timeval tv;
	fd_set rfds;
	int retval;

	while (true)
	{
		try
		{

			if(requestedCancel()) { //cancel was requested
				return;
            }

			if ((!_listenTcpIp) && (!_listenStdin)) { // listening to nothing... the stdin stream was closed by the parents
				return;
            }

			tv.tv_sec = 0;
			tv.tv_usec = _selectTimeout;

			FD_ZERO (&rfds);
			if (_listenTcpIp)
			{
				// do not listen to _mainFd if no Tcp/Ip channel was set
				FD_SET (_mainFd, &rfds);
			}

			// _mainFd will be set to -1 if no tcp/ip connection is enabled
			int maxFd = _mainFd;
			for (unsigned int i=0; i<_connectedFd.size(); i++)
			{
				FD_SET (_connectedFd[i], &rfds);
				maxFd = std::max(maxFd, _connectedFd[i]);
			}

			retval = select (maxFd+1, &rfds, 0, 0, &tv);
			if (retval == 0)
			{
				// a select timeout...
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "select timeout") ;
			}
			else if (retval < 0)
			{
				ACS_THROW(exIOException("Error in selecting socket descriptors.", errno )) ; // PRQA S 3081
			}
			else
			{
				for (unsigned int i=0; i<_connectedFd.size(); i++)
				{
					if (FD_ISSET (_connectedFd[i], &rfds))
					{
						manageConnection (i);
					}
				}
				if (_listenTcpIp && FD_ISSET (_mainFd, &rfds)) {
					acceptNewConnection();
                }
			}
			customAction() ; //execute custom action
		}
		catch(std::exception &e)
		{
			ACS_LOG_NOTIFY_EX(e);
			Thread::nanosleep(_sleepTime);
		}
	}  // while (true)
}

template <class T> inline
void TcpIpIpcServer<T>::setChannel() // PRQA S 2133, 2134
{
	struct sockaddr_in server; // PRQA S 4102

	// clear the structure
	memset(&server, 0, sizeof(struct sockaddr_in));

	if ((_mainFd=socket(AF_INET,SOCK_STREAM | SOCK_CLOEXEC,0)) == -1)
	{
		_mainFd=-1;
		ACS_THROW(exIOException("Cannot create the socket." , errno )) ; // PRQA S 3081
	}

	int flag=1;
//	if (setsockopt(_mainFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&flag, sizeof(flag)) == -1)
//	if (setsockopt(_mainFd, SOL_SOCKET, SO_REUSEADDR, static_cast <const void *>(&flag), sizeof(flag)) == -1)
	if (setsockopt(_mainFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1)
	{
		// setsockopt failed. Close the descriptor and throw.
		close (_mainFd);
		_mainFd=-1;
		ACS_THROW(exIOException("Cannot set the socket parameters." , errno )); // PRQA S 3081
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(_port);
	server.sin_addr.s_addr = INADDR_ANY; // PRQA S 3080, 3081

	if (0 != bind(_mainFd,reinterpret_cast<struct sockaddr*>(&server), sizeof(struct sockaddr))) // PRQA S 3030, 3081
	{
		// bind failed. Close the descriptor and throw.
		close (_mainFd);
		_mainFd=-1;
		ACS_THROW(exIOException("Cannot bind the socket." , errno )) ; // PRQA S 3081
	}

	if (listen(_mainFd,BACKLOG) == -1)
	{
		// listn failed. Close the descriptor and throw.
		close (_mainFd);
		_mainFd=-1;
		ACS_THROW(exIOException("Cannot start listening the socket." , errno )) ; // PRQA S 3081
	}
}
// PRQA S 1040 EOF
#pragma GCC diagnostic warning "-Wold-style-cast"                

_ACS_END_NAMESPACE

#endif   //_TCPIPIPC_SERVER_H_
