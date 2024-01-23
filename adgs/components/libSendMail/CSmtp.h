// PRQA S 1050 EOF
// CSmtp.h: interface for the Smtp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CSMTP_H__
#define __CSMTP_H__


#include <vector>
#include <cstring>
#include <cassert>

#ifdef __linux__
	#include <sys/types.h> // PRQA S 1013 5
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <cerrno>
	#include <cstdio>
	#include <iostream>

	typedef unsigned short WORD;
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct hostent* LPHOSTENT;
	typedef struct servent* LPSERVENT;
	typedef struct in_addr* LPIN_ADDR;
	typedef struct sockaddr* LPSOCKADDR;
#else
	#include <winsock2.h>
	#include <time.h>
	#pragma comment(lib, "ws2_32.lib")
#endif

const char BOUNDARY_TEXT[] = "__MESSAGE__ID__54yg6f6h6y456345"; // PRQA S 2300

enum CSmptXPriority
{
	XPRIORITY_HIGH = 2,
	XPRIORITY_NORMAL = 3,
	XPRIORITY_LOW = 4
};

class ECSmtp: public std::exception // PRQA S 2109
{
public:
	enum CSmtpError
	{
		CSMTP_NO_ERROR = 0,
		WSA_STARTUP = 100, // WSAGetLastError()
		WSA_VER,
		WSA_SEND,
		WSA_RECV,
		WSA_CONNECT,
		WSA_GETHOSTBY_NAME_ADDR,
		WSA_INVALID_SOCKET,
		WSA_HOSTNAME,
		WSA_IOCTLSOCKET,
		WSA_SELECT,
		BAD_IPV4_ADDR,
		UNDEF_MSG_HEADER = 200,
		UNDEF_MAIL_FROM,
		UNDEF_SUBJECT,
		UNDEF_RECIPIENTS,
		UNDEF_LOGIN,
		UNDEF_PASSWORD,
		UNDEF_RECIPIENT_MAIL,
		COMMAND_MAIL_FROM = 300,
		COMMAND_EHLO,
		COMMAND_AUTH_LOGIN,
		COMMAND_DATA,
		COMMAND_QUIT,
		COMMAND_RCPT_TO,
		MSG_BODY_ERROR,
		CONNECTION_CLOSED = 400, // by server
		SERVER_NOT_READY, // remote server
		SERVER_NOT_RESPONDING,
		SELECT_TIMEOUT,
		FILE_NOT_EXIST,
		MSG_TOO_BIG,
		BAD_LOGIN_PASS,
		UNDEF_XYZ_RESPONSE,
		LACK_OF_MEMORY,
		TIME_ERROR,
		RECVBUF_IS_EMPTY,
		SENDBUF_IS_EMPTY,
		OUT_OF_MSG_RANGE,
	};
	explicit ECSmtp(CSmtpError err_) : ErrorCode(err_) {}
	virtual ~ECSmtp() throw() {}
	CSmtpError GetErrorNum(void) const {return ErrorCode;}
	const char * GetErrorText(void) const throw() ;
    virtual const char* what() const throw() { return GetErrorText() ; }
private:
    ECSmtp() ; // not implemented
private:
	CSmtpError ErrorCode;
};

class CSmtp // PRQA S 2109 
{
public:
	CSmtp();
	virtual ~CSmtp() throw();
	void AddRecipient(const char *, const char *name=0);
	void AddBCCRecipient(const char *, const char *name=0);
	void AddCCRecipient(const char *, const char *name=0);    
	void AddAttachment(const char *);   
	void AddMsgLine(const char* );
	void DelRecipients(void);
	void DelBCCRecipients(void);
	void DelCCRecipients(void);
	void DelAttachments(void);
	void DelMsgLines(void);
	void DelMsgLine(unsigned int );
	void ModMsgLine(unsigned int ,const char* );
	size_t GetBCCRecipientCount() const;    
	size_t GetCCRecipientCount() const;
	size_t GetRecipientCount() const;    
	const char* GetLocalHostIP() const;
	const char* GetLocalHostName() const;
	const char* GetMsgLineText(size_t) const;
	size_t GetMsgLines(void) const;
	const char* GetReplyTo() const;
	const char* GetMailFrom() const;
	const char* GetSenderName() const;
	const char* GetSubject() const;
	const char* GetXMailer() const;
	CSmptXPriority GetXPriority() const;
	void Send();
	void SetSubject(const char*);
	void SetSenderName(const char*);
	void SetSenderMail(const char*);
	void SetReplyTo(const char*);
	void SetXMailer(const char*);
	void SetLogin(const char*);
	void SetPassword(const char*);
	void SetXPriority(CSmptXPriority);
	void SetSMTPServer(const char* ,const unsigned short port=0);

	const char* GetTextContentType() const;
	void SetTextContentType(const char *);
private:
    CSmtp(const CSmtp &) ; // not implemented 
    CSmtp& operator= (const CSmtp &) ; // not implemented 
public:
    static const int BUFFER_SIZE=10240 ;	// SendData and RecvData buffers sizes
	static const int SOCKET_ERROR = -1 ;
	static const int INVALID_SOCKET = -1 ;
	static const int MSG_SIZE_IN_MB = 5 ;	// the maximum size of the message with all attachments
    static const int TIME_IN_SEC = 10 ;		// how long client will wait for server response in non-blocking mode
    static const int COUNTER_VALUE = 100 ;	// how many times program will try to receive data

private:	
	bool m_bAuthSet ; /* True if SetLogin called (and v.v.) */
	std::string m_sLocalHostName;
	std::string m_sMailFrom;
	std::string m_sNameFrom;
	std::string m_sSubject;
	std::string m_sTextContentType;
	std::string m_sXMailer;
	std::string m_sReplyTo;
	std::string m_sIPAddr;
	std::string m_sLogin;
	std::string m_sPassword;
	std::string m_sSMTPSrvName;
	unsigned short m_iSMTPSrvPort;
	CSmptXPriority m_iXPriority;
	char *SendBuf;
	char *RecvBuf;
	
	SOCKET hSocket;

	struct Recipient // PRQA S 2173, 2175
	{
        Recipient() : Name(), Mail() {}
		std::string Name; // PRQA S 2100 2
		std::string Mail;
	};

	std::vector<Recipient> Recipients;
	std::vector<Recipient> CCRecipients;
	std::vector<Recipient> BCCRecipients;
	std::vector<std::string> Attachments;
	std::vector<std::string> MsgBody;
 
	void ReceiveData();
	void SendData();
	void FormatHeader(char*);
	int SmtpXYZdigits();
	SOCKET ConnectRemoteServer(const char* szServer, const unsigned short port=0);
	
};


#endif // __CSMTP_H__
