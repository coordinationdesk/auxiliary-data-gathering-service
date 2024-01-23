// PRQA S 1050 EOF
////////////////////////////////////////////////////////////////////////////////
// Original class CFastSmtp written by 
// christopher w. backen <immortal@cox.net>
// More details at: http://www.codeproject.com/KB/IP/zsmtp.aspx
// 
// Modifications introduced by Jakub Piwowarczyk:
// 1. name of the class and functions
// 2. new functions added: SendData,ReceiveData and more
// 3. authentication added
// 4. attachments added
// 5 .comments added
// 6. DELAY_IN_MS removed (no delay during sending the message)
// 7. non-blocking mode
// More details at: http://www.codeproject.com/KB/mcpp/CSmtp.aspx
////////////////////////////////////////////////////////////////////////////////

#include <CSmtp.h>
#include <libgen.h>
#include <base64.h>
#include <unistd.h>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////
//        NAME: CSmtp
// DESCRIPTION: Constructor of CSmtp class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority, m_iSMTPSrvPort, RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
CSmtp::CSmtp() :
	m_bAuthSet(false),
	m_sLocalHostName(),
	m_sMailFrom(),
	m_sNameFrom(),
	m_sSubject(),
	m_sTextContentType("text/plain; charset=US-ASCII"),
	m_sXMailer(),
	m_sReplyTo(),
	m_sIPAddr(),
	m_sLogin(),
	m_sPassword(),
	m_sSMTPSrvName(),
	m_iSMTPSrvPort(0),
	m_iXPriority(XPRIORITY_NORMAL),
	SendBuf(0),
	RecvBuf(0),
	hSocket(),
    Recipients(),
    CCRecipients(),
    BCCRecipients(),
	Attachments(),
	MsgBody()
 
{
/*
	m_iXPriority = XPRIORITY_NORMAL;
	m_iSMTPSrvPort = 0;
    m_bAuthSet = false ;

	m_sTextContentType = "text/plain; charset=US-ASCII";
*/
#ifndef __linux__
	// Initialize WinSock
	WSADATA wsaData;
	WORD wVer = MAKEWORD(2,2);    
	if (WSAStartup(wVer,&wsaData) != NO_ERROR) {
		throw ECSmtp(ECSmtp::WSA_STARTUP);    // PRQA S 3081
    }
	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup();
		throw ECSmtp(ECSmtp::WSA_VER);   // PRQA S 3081
	}
#endif
	
	if((RecvBuf = new char[BUFFER_SIZE]) == 0) {
		throw ECSmtp(ECSmtp::LACK_OF_MEMORY); // PRQA S 3081
    }
	
	if((SendBuf = new char[BUFFER_SIZE]) == 0) {
		delete [] RecvBuf ;
        throw ECSmtp(ECSmtp::LACK_OF_MEMORY); // PRQA S 3081
    }
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: CSmtp
// DESCRIPTION: Destructor of CSmtp class.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf, SendBuf
// MODIFIES GL: RecvBuf, SendBuf
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
CSmtp::~CSmtp() throw()
{
	delete[] SendBuf;
	delete[] RecvBuf;

#ifndef __linux__
	WSACleanup();
#endif
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddAttachment
// DESCRIPTION: New attachment is added.
//   ARGUMENTS: const char *Path - name of attachment added
// USES GLOBAL: Attachments
// MODIFIES GL: Attachments
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::AddAttachment(const char *Path)
{
	assert(Path != 0); // PRQA S 3081, 3380, 3385
	Attachments.insert(Attachments.end(),Path);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddRecipient
// DESCRIPTION: New recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the recipient
//              const char *name - name of the recipient
// USES GLOBAL: Recipients
// MODIFIES GL: Recipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::AddRecipient(const char *email, const char *name)
{	
	if(!email) {
		throw ECSmtp(ECSmtp::UNDEF_RECIPIENT_MAIL); // PRQA S 3081
    }

	Recipient recipient;
	recipient.Mail.insert(0,email);
	if (name!=0) {
        recipient.Name.insert(0,name); 
    }
    else {
        recipient.Name.insert(0,"");
    }

	Recipients.insert(Recipients.end(), recipient);   
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddCCRecipient
// DESCRIPTION: New cc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the cc-recipient
//              const char *name - name of the ccc-recipient
// USES GLOBAL: CCRecipients
// MODIFIES GL: CCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::AddCCRecipient(const char *email, const char *name)
{	
	if(!email) {
		throw ECSmtp(ECSmtp::UNDEF_RECIPIENT_MAIL); // PRQA S 3081
    }

	Recipient recipient;
	recipient.Mail.insert(0,email);
	if (name!=0) {
        recipient.Name.insert(0,name) ;
    }
    else {
        recipient.Name.insert(0,"");
    }

	CCRecipients.insert(CCRecipients.end(), recipient);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddBCCRecipient
// DESCRIPTION: New bcc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the bcc-recipient
//              const char *name - name of the bccc-recipient
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::AddBCCRecipient(const char *email, const char *name)
{	
	if(!email) {
		throw ECSmtp(ECSmtp::UNDEF_RECIPIENT_MAIL); // PRQA S 3081
    }
    
	Recipient recipient;
	recipient.Mail.insert(0,email);
	if (name!=0) {
        recipient.Name.insert(0,name) ;
    }
    else {
        recipient.Name.insert(0,"");
    }

	BCCRecipients.insert(BCCRecipients.end(), recipient);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddMsgLine
// DESCRIPTION: Adds new line in a message.
//   ARGUMENTS: const char *Text - text of the new line
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::AddMsgLine(const char* Text)
{
	MsgBody.insert(MsgBody.end(),Text);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelMsgLine
// DESCRIPTION: Deletes specified line in text message.. .
//   ARGUMENTS: unsigned int Line - line to be delete
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::DelMsgLine(unsigned int Line)
{
	if(Line > MsgBody.size()) {
		throw ECSmtp(ECSmtp::OUT_OF_MSG_RANGE); // PRQA S 3081
    }
	MsgBody.erase(MsgBody.begin()+Line); // PRQA S 3000
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelRecipients
// DESCRIPTION: Deletes all recipients. .
//   ARGUMENTS: void
// USES GLOBAL: Recipients
// MODIFIES GL: Recipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::DelRecipients()
{
	Recipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelBCCRecipients
// DESCRIPTION: Deletes all BCC recipients. .
//   ARGUMENTS: void
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::DelBCCRecipients()
{
	BCCRecipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelCCRecipients
// DESCRIPTION: Deletes all CC recipients. .
//   ARGUMENTS: void
// USES GLOBAL: CCRecipients
// MODIFIES GL: CCRecipients
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::DelCCRecipients()
{
	CCRecipients.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelMsgLines
// DESCRIPTION: Deletes message text.
//   ARGUMENTS: void
// USES GLOBAL: MsgBody
// MODIFIES GL: MsgBody
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::DelMsgLines()
{
	MsgBody.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: DelAttachments
// DESCRIPTION: Deletes all recipients. .
//   ARGUMENTS: void
// USES GLOBAL: Attchments
// MODIFIES GL: Attachments
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::DelAttachments()
{
	Attachments.clear();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddBCCRecipient
// DESCRIPTION: New bcc-recipient data is added i.e.: email and name. .
//   ARGUMENTS: const char *email - mail of the bcc-recipient
//              const char *name - name of the bccc-recipient
// USES GLOBAL: BCCRecipients
// MODIFIES GL: BCCRecipients, m_oError
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::ModMsgLine(unsigned int Line,const char* Text)
{
	if(Text)
	{
		if(Line > MsgBody.size()) {
			throw ECSmtp(ECSmtp::OUT_OF_MSG_RANGE); // PRQA S 3081
        }
		MsgBody.at(Line) = Text ;
	}
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Send
// DESCRIPTION: Sending the mail. .
//   ARGUMENTS: none
// USES GLOBAL: m_sSMTPSrvName, m_iSMTPSrvPort, SendBuf, RecvBuf, m_sLogin,
//              m_sPassword, m_sMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_sMsgBody, Attachments, 
// MODIFIES GL: SendBuf 
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::Send()
{
    size_t rcpt_count = 0  ; 
	char *FileBuf = 0 ;
    char *FileName = 0;
	FILE* hFile = 0;
	unsigned long FileSize  = 0;
    unsigned long TotalSize = 0; 
	bool bAccepted = false ;

	// ***** CONNECTING TO SMTP SERVER *****

	// connecting to remote host:
	if( (hSocket = ConnectRemoteServer(m_sSMTPSrvName.c_str(), m_iSMTPSrvPort)) == INVALID_SOCKET ) { 
		throw ECSmtp(ECSmtp::WSA_INVALID_SOCKET);  // PRQA S 3081
    }

	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
		   case 220:    // PRQA S 4402
               {
				   bAccepted = true;
				   break;
               }
		   default:
               {
				   throw ECSmtp(ECSmtp::SERVER_NOT_READY);  // PRQA S 3081
               }
		}
	}while(!bAccepted);

	// EHLO <SP> <domain> <CRLF>
	sprintf(SendBuf,"EHLO %s\r\n",((GetLocalHostName()!=0) && (GetLocalHostName()[0] != 0)) ? m_sLocalHostName.c_str() : "domain"); // PRQA S 3380
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
		case 250:    // PRQA S 4402
            {
				bAccepted = true;
				break;
            }
		default:
            {
				throw ECSmtp(ECSmtp::COMMAND_EHLO);  // PRQA S 3081
            }
		}
	}while(!bAccepted);
	
    // ***** AUTHENTICATING, IF REQUIRED *****

    if (m_bAuthSet)
    {

	    // AUTH <SP> LOGIN <CRLF>
	    strcpy(SendBuf,"AUTH LOGIN\r\n");
	    SendData();
	    bAccepted = false;
	    do
	    {
		    ReceiveData();
		    switch(SmtpXYZdigits())
		    {
			case 250:    // PRQA S 4402
                {
				    break;
                }
			case 334:    // PRQA S 4402
                {
				    bAccepted = true;
				    break;
                }
			default:
                {
				    throw ECSmtp(ECSmtp::COMMAND_AUTH_LOGIN);  // PRQA S 3081
                }
		    }
	    }while(!bAccepted);

	    // send login:
	    if(m_sLogin.empty()) {
		    throw ECSmtp(ECSmtp::UNDEF_LOGIN);  // PRQA S 3081
        }
	    std::string encoded_login = base64_encode(reinterpret_cast<const unsigned char*>(m_sLogin.c_str()),m_sLogin.size()); // PRQA S 3010, 3030, 3081
	    sprintf(SendBuf,"%s\r\n",encoded_login.c_str());
	    SendData();
	    bAccepted = false;
	    do
	    {
		    ReceiveData();
		    switch(SmtpXYZdigits())
		    {
			case 334:    // PRQA S 4402
                {
				    bAccepted = true;
				    break;
			    }
            default:
                {
				    throw ECSmtp(ECSmtp::UNDEF_XYZ_RESPONSE);  // PRQA S 3081
                }
		    }
	    }while(!bAccepted);

	    // send password:
	    if(m_sPassword.empty()) {
		    throw ECSmtp(ECSmtp::UNDEF_PASSWORD);  // PRQA S 3081
        }
	    std::string encoded_password = base64_encode(reinterpret_cast<const unsigned char*>(m_sPassword.c_str()),m_sPassword.size()); // PRQA S 3010, 3030, 3081
	    sprintf(SendBuf,"%s\r\n",encoded_password.c_str());
	    SendData();
	    bAccepted = false;
	    do
	    {
		    ReceiveData();
		    switch(SmtpXYZdigits())
		    {
			case 235:    // PRQA S 4402
                {
				    bAccepted = true;
				    break;
                }
			case 334:    // PRQA S 4402
                {
				    break;
                }
		    case 535:    // PRQA S 4402
                {
				    throw ECSmtp(ECSmtp::BAD_LOGIN_PASS); // PRQA S 3081
                }
			default:
                {
				    throw ECSmtp(ECSmtp::UNDEF_XYZ_RESPONSE); // PRQA S 3081
                }
		    }
	    }while(!bAccepted);
	} // END if (m_bAuthSet)


	// ***** SENDING E-MAIL *****
	
	// MAIL <SP> FROM:<reverse-path> <CRLF>
	if(m_sMailFrom.empty()) {
		throw ECSmtp(ECSmtp::UNDEF_MAIL_FROM);  // PRQA S 3081
    }
	sprintf(SendBuf,"MAIL FROM:<%s>\r\n",m_sMailFrom.c_str());
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
	    case 250:    // PRQA S 4402
            {
				bAccepted = true;
				break;
            }
        default:
            {
				throw ECSmtp(ECSmtp::COMMAND_MAIL_FROM);  // PRQA S 3081
            }
		}
	}while(!bAccepted);

	// RCPT <SP> TO:<forward-path> <CRLF>
	if( 0 == (rcpt_count = Recipients.size())) { 
		throw ECSmtp(ECSmtp::UNDEF_RECIPIENTS);  // PRQA S 3081
    }
    size_t recsize = Recipients.size() ;
    
	for(size_t i=0;i<recsize;++i)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(Recipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
            case 250:    // PRQA S 4402
                {
					bAccepted = true;
					break;
                }
            default:
                {
					rcpt_count--;
                }
			}
		}while(!bAccepted);
	}
	if(rcpt_count <= 0) {
		throw ECSmtp(ECSmtp::COMMAND_RCPT_TO);  // PRQA S 3081
    }

    size_t ccsize = CCRecipients.size() ;
	for(size_t i=0;i<ccsize;++i)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(CCRecipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
			case 250:    // PRQA S 4402
                {
					bAccepted = true;
					break;
                }
		    default:
                {
					; // not necessary to throw
                }
			}
		}while(!bAccepted);
	}

    size_t bccsize = BCCRecipients.size() ;
	for(size_t i=0;i<bccsize;++i)
	{
		sprintf(SendBuf,"RCPT TO:<%s>\r\n",(BCCRecipients.at(i).Mail).c_str());
		SendData();
		bAccepted = false;
		do
		{
			ReceiveData();
			switch(SmtpXYZdigits())
			{
			case 250:    // PRQA S 4402
                {
					bAccepted = true;
					break;
                }
				default:
                {
					; // not necessary to throw
                }
			}
		}while(!bAccepted);
	}
	
	// DATA <CRLF>
	strcpy(SendBuf,"DATA\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
        case 354:    // PRQA S 4402
            {
				bAccepted = true;
				break;
			}
        case 250:    // PRQA S 4402
            {
				break;
			}
        default:
            {
				throw ECSmtp(ECSmtp::COMMAND_DATA);  // PRQA S 3081
		    }
        }
	}while(!bAccepted);
	
	// send header(s)
	FormatHeader(SendBuf);
	SendData();

	// send text message
	if(0 != GetMsgLines())
	{
		for(size_t i=0;i<GetMsgLines();i++)
		{
			sprintf(SendBuf,"%s\r\n",GetMsgLineText(i)); 
			SendData();
		}
	}
	else
	{
		sprintf(SendBuf,"%s\r\n"," ");
		SendData();
	}

	// next goes attachments (if they are)
	if((FileBuf = new char[55]) == 0) { // PRQA S 4403
		throw ECSmtp(ECSmtp::LACK_OF_MEMORY);  // PRQA S 3081
    }
	if((FileName = new char[4096]) == 0) { // PRQA S 4403
		throw ECSmtp(ECSmtp::LACK_OF_MEMORY); // PRQA S 3081
    }
	TotalSize = 0;
    size_t attsize = Attachments.size() ;
	for(size_t FileId=0;FileId<attsize;++FileId)
	{
		strcpy(FileName,Attachments[FileId].c_str());

		sprintf(SendBuf,"\r\n--%s\r\n",BOUNDARY_TEXT);
		strcat(SendBuf,"Content-Type: application/octet-stream name=\"");
		strcat(SendBuf,basename(FileName));
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: base64\r\n");
		strcat(SendBuf,"Content-Disposition: attachment; filename=\"");
		strcat(SendBuf,basename(FileName));
		strcat(SendBuf,"\"\r\n");
		strcat(SendBuf,"\r\n");

		SendData();

		// opening the file:
		hFile = fopen(FileName,"rb");
		if(hFile == 0) {
			throw ECSmtp(ECSmtp::FILE_NOT_EXIST);  // PRQA S 3081
		}
		// checking file size:
		FileSize = 0;
		while(0 == feof(hFile)) {
			FileSize += fread(FileBuf,sizeof(char),54,hFile); // PRQA S 3084, 4400
        }
		TotalSize += FileSize; // PRQA S 3084

		// sending the file:
		if(TotalSize/1024 > MSG_SIZE_IN_MB*1024) { // PRQA S 3084, 4400
			throw ECSmtp(ECSmtp::MSG_TOO_BIG);  // PRQA S 3081
        }
		else
		{
			fseek (hFile,0,SEEK_SET);

			size_t MsgPart = 0;
			for(size_t i=0;i<FileSize/54+1;i++) // PRQA S 3084, 4400
			{
    			size_t res = fread(FileBuf,sizeof(char),54,hFile); // PRQA S 4400
				if (0 != MsgPart) {
                    strcat(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str()) ; // PRQA S 3010, 3030, 3081
                }
                else {
                    strcpy(SendBuf,base64_encode(reinterpret_cast<const unsigned char*>(FileBuf),res).c_str()); // PRQA S 3010, 3030, 3081
                }
				strcat(SendBuf,"\r\n");
				MsgPart += res + 2; // PRQA S 3084, 4400
				if(MsgPart >= BUFFER_SIZE/2) // PRQA S 4400
				{ // sending part of the message
					MsgPart = 0;
					SendData(); // FileBuf, FileName, fclose(hFile);
				}
			}
			if(0 != MsgPart)
			{
				SendData(); // FileBuf, FileName, fclose(hFile);
			}
		}
		fclose(hFile);
	}
	delete[] FileBuf;
	delete[] FileName;
	
	// sending last message block (if there is one or more attachments)
	if(not Attachments.empty())
	{
		sprintf(SendBuf,"\r\n--%s--\r\n",BOUNDARY_TEXT);
		SendData();
	}
	
	// <CRLF> . <CRLF>
	strcpy(SendBuf,"\r\n.\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
        case 250:    // PRQA S 4402
            {
				bAccepted = true;
				break;
            }
        default:
            {
				throw ECSmtp(ECSmtp::MSG_BODY_ERROR);  // PRQA S 3081
            }
		}
	}while(!bAccepted);

	// ***** CLOSING CONNECTION *****
	
	// QUIT <CRLF>
	strcpy(SendBuf,"QUIT\r\n");
	SendData();
	bAccepted = false;
	do
	{
		ReceiveData();
		switch(SmtpXYZdigits())
		{
        case 221:    // PRQA S 4402
            {
				bAccepted = true;
				break;
			}
        default:
            {
				throw ECSmtp(ECSmtp::COMMAND_QUIT);   // PRQA S 3081
		    }
        }
	}while(!bAccepted);

#ifdef __linux__
	close(hSocket);
#else
	closesocket(hSocket);
#endif
	hSocket = 0 ;
}

// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"
////////////////////////////////////////////////////////////////////////////////
//        NAME: ConnectRemoteServer
// DESCRIPTION: Connecting to the service running on the remote server. 
//   ARGUMENTS: const char *server - service name
//              const unsigned short port - service port
// USES GLOBAL: m_pcSMTPSrvName, m_iSMTPSrvPort, SendBuf, RecvBuf, m_pcLogin,
//              m_pcPassword, m_pcMailFrom, Recipients, CCRecipients,
//              BCCRecipients, m_pcMsgBody, Attachments, 
// MODIFIES GL: m_oError 
//     RETURNS: socket of the remote service
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
SOCKET CSmtp::ConnectRemoteServer(const char *szServer,const unsigned short port) // PRQA S 4020
{
	unsigned short nPort = 0;
	LPSERVENT lpServEnt = 0 ;
	SOCKADDR_IN sockAddr; // PRQA S 4102
	unsigned long ul = 1;
	fd_set fdwrite ;  // PRQA S 4102 3
    fd_set fdexcept; 
	timeval timeout;
	int res = 0;

	timeout.tv_sec = TIME_IN_SEC;
	timeout.tv_usec = 0;

	SOCKET myhSocket = INVALID_SOCKET;

	if((myhSocket = socket(PF_INET, SOCK_STREAM,0)) == INVALID_SOCKET) {
		throw ECSmtp(ECSmtp::WSA_INVALID_SOCKET);    // PRQA S 3081
    }

	if(port != 0) { // PRQA S 3000
		nPort = htons(port);
    }
	else
	{
		lpServEnt = getservbyname("mail", 0);
		if (lpServEnt == 0) {
			nPort = htons(25); // PRQA S 4400
        }
		else {
			nPort = lpServEnt->s_port; // PRQA S 3000, 3010
        }
	}
			
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = nPort;
	if((sockAddr.sin_addr.s_addr = inet_addr(szServer)) == INADDR_NONE) // PRQA S 3080, 3081
	{
		LPHOSTENT host = gethostbyname(szServer);
		if (host) {
			memcpy(&sockAddr.sin_addr,host->h_addr_list[0],host->h_length); // PRQA S 3000
        }
		else
		{
#ifdef __linux__
			close(myhSocket);
#else
			closesocket(myhSocket);
#endif
			throw ECSmtp(ECSmtp::WSA_GETHOSTBY_NAME_ADDR);    // PRQA S 3081
		}				
	}

	// start non-blocking mode for socket:
#ifdef __linux__
	if(ioctl(myhSocket,FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR) // PRQA S 3080, 3081
#else
	if(ioctlsocket(myhSocket,FIONBIO, (unsigned long*)&ul) == SOCKET_ERROR)
#endif
	{
#ifdef __linux__
		close(myhSocket);
#else
		closesocket(myhSocket);
#endif
		throw ECSmtp(ECSmtp::WSA_IOCTLSOCKET);   // PRQA S 3081
	}

	if(connect(myhSocket,(LPSOCKADDR)&sockAddr,sizeof(sockAddr)) == SOCKET_ERROR)  // PRQA S 3030, 3080, 3081
	{
#ifdef __linux__
		if(errno != EINPROGRESS)
#else
		if(WSAGetLastError() != WSAEWOULDBLOCK)
#endif
		{
#ifdef __linux__
			close(myhSocket);
#else
			closesocket(myhSocket);
#endif
			throw ECSmtp(ECSmtp::WSA_CONNECT);   // PRQA S 3081
		}
	}
	else {
		return myhSocket;
    }

	while(true)
	{
		FD_ZERO(&fdwrite); // PRQA S 4092, 4101, 4107 2
		FD_ZERO(&fdexcept);

		FD_SET(myhSocket,&fdwrite); // PRQA S 3003, 3080, 3081 2
		FD_SET(myhSocket,&fdexcept);

		if((res = select(myhSocket+1,0,&fdwrite,&fdexcept,&timeout)) == SOCKET_ERROR)
		{
#ifdef __linux__
			close(myhSocket);
#else
			closesocket(myhSocket);
#endif
			throw ECSmtp(ECSmtp::WSA_SELECT);   // PRQA S 3081
		}

		if( 0 == res)
		{
#ifdef __linux__
			close(myhSocket);
#else
			closesocket(myhSocket);
#endif
			throw ECSmtp(ECSmtp::SELECT_TIMEOUT);   // PRQA S 3081
		}
		if((0 != res) && FD_ISSET(myhSocket,&fdwrite)) {
			break;
        }
		if((0 != res) && FD_ISSET(myhSocket,&fdexcept))
		{
#ifdef __linux__
			close(myhSocket);
#else
			closesocket(myhSocket);
#endif
			throw ECSmtp(ECSmtp::WSA_SELECT);   // PRQA S 3081
		}
	} // while

	FD_CLR(myhSocket,&fdwrite); // PRQA S 3003, 3080, 3081 2
	FD_CLR(myhSocket,&fdexcept);

	return myhSocket;
}

// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"

////////////////////////////////////////////////////////////////////////////////
//        NAME: SmtpXYZdigits
// DESCRIPTION: Converts three letters from RecvBuf to the number.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: none
//     RETURNS: integer number
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
int CSmtp::SmtpXYZdigits() // PRQA S 4020, 4211
{
	assert(RecvBuf); // PRQA S 3081, 3380, 3385
	if(RecvBuf == 0) {
		return 0;
    }
	return (RecvBuf[0]-'0')*100 + (RecvBuf[1]-'0')*10 + RecvBuf[2]-'0'; // PRQA S 4400
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: FormatHeader
// DESCRIPTION: Prepares a header of the message.
//   ARGUMENTS: char* header - formated header string
// USES GLOBAL: Recipients, CCRecipients, BCCRecipients
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::FormatHeader(char* header)
{
	char month[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; // PRQA S 4403
	std::string to;
	std::string cc;
	std::string bcc;
	time_t rawtime = 0 ;
	struct tm* timeinfo = 0;
    char tz[6] ; // PRQA S 4403
    

	// date/time check
	if(time(&rawtime) > 0) {
		timeinfo = localtime(&rawtime);
    }
	else {
		throw ECSmtp(ECSmtp::TIME_ERROR);   // PRQA S 3081
    }

	// check for at least one recipient
	if(not Recipients.empty())
	{
        size_t recsize = Recipients.size() ;
		for (size_t i=0;i<recsize;++i)
		{
			if(i > 0) {
				to.append(",");
            }
			to += Recipients[i].Name;
			to.append("<");
			to += Recipients[i].Mail;
			to.append(">");
		}
	}
	else {
		throw ECSmtp(ECSmtp::UNDEF_RECIPIENTS);    // PRQA S 3081
    }

	if(not CCRecipients.empty())
	{
        size_t ccsize = CCRecipients.size() ;
		for (size_t i=0;i<ccsize;++i)
		{
			if(i > 0) {
				cc. append(",");
            }
			cc += CCRecipients[i].Name;
			cc.append("<");
			cc += CCRecipients[i].Mail;
			cc.append(">");
		}
	}

	if(not BCCRecipients.empty())
	{
        size_t bccsize = BCCRecipients.size() ;
		for (size_t i=0;i<bccsize;++i)
		{
			if(i > 0) {
				bcc.append(",");
            }
			bcc += BCCRecipients[i].Name;
			bcc.append("<");
			bcc += BCCRecipients[i].Mail;
			bcc.append(">");
		}
	}

    sprintf(tz, "%c%02d%02d",
                ((timeinfo->tm_gmtoff>=0)? '+': '-'), // PRQA S 3380
                (abs(timeinfo->tm_gmtoff)/3600)%23,   // PRQA S 4400 2
                ((abs(timeinfo->tm_gmtoff)/60)%60) ) ;

	// Date: <SP> <dd> <SP> <mon> <SP> <yy> <SP> <hh> ":" <mm> ":" <ss> <SP> <zone> <CRLF>
	sprintf(header,"Date: %d %s %d %02d:%02d:%02d %s\r\n",	timeinfo->tm_mday,
															month[timeinfo->tm_mon],
															timeinfo->tm_year+1900, // PRQA S 4400
															timeinfo->tm_hour,
															timeinfo->tm_min,
															timeinfo->tm_sec, 
															tz); 
	
	// From: <SP> <sender>  <SP> "<" <sender-email> ">" <CRLF>
	if(m_sMailFrom.empty()) {
		throw ECSmtp(ECSmtp::UNDEF_MAIL_FROM);    // PRQA S 3081
    }
	strcat(header,"From: ");
	if(not m_sNameFrom.empty()) {
		strcat(header, m_sNameFrom.c_str());
    }
	strcat(header," <");
	if(not m_sMailFrom.empty()) {
		strcat(header,m_sMailFrom.c_str());
    }
	else {
		strcat(header,"mail@domain.com");
    }
	strcat(header, ">\r\n");

	// X-Mailer: <SP> <xmailer-app> <CRLF>
	if(not m_sXMailer.empty())
	{
		strcat(header,"X-Mailer: ");
		strcat(header, m_sXMailer.c_str());
		strcat(header, "\r\n");
	}

	// Reply-To: <SP> <reverse-path> <CRLF>
	if(not m_sReplyTo.empty())
	{
		strcat(header, "Reply-To: ");
		strcat(header, m_sReplyTo.c_str());
		strcat(header, "\r\n");
	}

	// X-Priority: <SP> <number> <CRLF>
	switch(m_iXPriority)
	{
    case XPRIORITY_HIGH:
        {
			strcat(header,"X-Priority: 2 (High)\r\n");
			break;
        }
    case XPRIORITY_NORMAL:
        {
			strcat(header,"X-Priority: 3 (Normal)\r\n");
			break;
        }
    case XPRIORITY_LOW:
        {
			strcat(header,"X-Priority: 4 (Low)\r\n");
			break;
        }
    default:
        {
			strcat(header,"X-Priority: 3 (Normal)\r\n");
        }
	}

	// To: <SP> <remote-user-mail> <CRLF>
	strcat(header,"To: ");
	strcat(header, to.c_str());
	strcat(header, "\r\n");

	// Cc: <SP> <remote-user-mail> <CRLF>
	if(not CCRecipients.empty())
	{
		strcat(header,"Cc: ");
		strcat(header, cc.c_str());
		strcat(header, "\r\n");
	}


//  keep bcc recipients list private (i.e. don't add to header)
//	if(BCCRecipients.size())
//	{
//		strcat(header,"Bcc: ");
//		strcat(header, bcc.c_str());
//		strcat(header, "\r\n");
//	}

	// Subject: <SP> <subject-text> <CRLF>
	if(m_sSubject.empty()) {
		strcat(header, "Subject:  ");
    }
	else
	{
	  strcat(header, "Subject: ");
	  strcat(header, m_sSubject.c_str());
	}
	strcat(header, "\r\n");
	
	// MIME-Version: <SP> 1.0 <CRLF>
	strcat(header,"MIME-Version: 1.0\r\n");
	if(Attachments.empty())
	{ // no attachments
		strcat(header,"Content-type: ");
		strcat(header, m_sTextContentType.c_str());
		strcat(header, "\r\n");

		strcat(header,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");
	}
	else
	{ // there is one or more attachments
		strcat(header,"Content-Type: multipart/mixed; boundary=\"");
		strcat(header,BOUNDARY_TEXT);
		strcat(header,"\"\r\n");
		strcat(header,"\r\n");
		// first goes text message
		strcat(SendBuf,"--");
		strcat(SendBuf,BOUNDARY_TEXT);
		strcat(SendBuf,"\r\n");
		strcat(SendBuf,"Content-type: ");
		strcat(SendBuf, m_sTextContentType.c_str());
		strcat(SendBuf, "\r\n");
		strcat(SendBuf,"Content-Transfer-Encoding: 7bit\r\n");
		strcat(SendBuf,"\r\n");
	}

	// done
}


// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"
////////////////////////////////////////////////////////////////////////////////
//        NAME: ReceiveData
// DESCRIPTION: Receives a row terminated '\n'.
//   ARGUMENTS: none
// USES GLOBAL: RecvBuf
// MODIFIES GL: RecvBuf
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-07
////////////////////////////////////////////////////////////////////////////////
void CSmtp::ReceiveData() // PRQA S 4211
{
	int res = 0 ; 
	fd_set fdread; // PRQA S 4102 2
	timeval time ;

	time.tv_sec = TIME_IN_SEC;
	time.tv_usec = 0;

	if(RecvBuf == 0) {
		throw ECSmtp(ECSmtp::RECVBUF_IS_EMPTY);    // PRQA S 3081
    }

    int i = 0;
	while(1) // PRQA S 4091
	{
		FD_ZERO(&fdread); // PRQA S 4092, 4101, 4107

		FD_SET(hSocket,&fdread); // PRQA S 3003, 3080, 3081

		if((res = select(hSocket+1, &fdread, 0, 0, &time)) == SOCKET_ERROR)
		{
			FD_CLR(hSocket,&fdread); // PRQA S 3003, 3080, 3081
			throw ECSmtp(ECSmtp::WSA_SELECT);    // PRQA S 3081
		}

		if(0 == res)
		{
			//timeout
			FD_CLR(hSocket,&fdread); // PRQA S 3003, 3080, 3081
			throw ECSmtp(ECSmtp::SERVER_NOT_RESPONDING);   // PRQA S 3081
		}

		if((0 != res) && FD_ISSET(hSocket,&fdread))
		{
			if(i >= BUFFER_SIZE)
			{
				FD_CLR(hSocket,&fdread); // PRQA S 3003, 3080, 3081
				throw ECSmtp(ECSmtp::LACK_OF_MEMORY);   // PRQA S 3081
			}
			if(recv(hSocket,&RecvBuf[i++],1,0) == SOCKET_ERROR)
			{
				FD_CLR(hSocket,&fdread); // PRQA S 3003, 3080, 3081
				throw ECSmtp(ECSmtp::WSA_RECV);   // PRQA S 3081
			}
			if(RecvBuf[i-1]=='\n') 
			{
				// EC:: When here, the first line (from the beginning to LF) of the message was extracted.
				// The message might be multi-line.
				
				if (i >=4 ) // PRQA S 4400
				{
					/* 
						THIS PORTION OF CODE ALLOWS TO HANDLE (REMOVE FROM SOCKET BUFFER AND COPY TO LOCAL BUFFER) MULTI-LINE RECEIVED BUFFER
						
						All the socket return buffers are in the form
						NNN[' '|'-']mmmmmmmmm \n
						Where:
						  NNN: 3 digit return code: 250, 351 ...
						  [' '|'-'] : can be space (last line of the msg) or dash (multi-line msg, not last line)
						  mmmmmmmmm : message
						  \n : LF
						  
						Example of a single line received buffer:
						  400 This is a single line received message \n
						Example of a multi-line buffer:
						  250-This is the first line \n
						  250-of a very long message \n
						  250 that finishes in this line. \n
						  
						So, if the returned message has >=4 chars, and the 4rd is "-" (instead of " ") the 
						message is multi line. All the further lines will be rejected, and the first line will be returned.  
						To reject the message, it will be copied in the buffer, but after the \0 inserted after the first row.
						Therefore, it will be unreachable and therefore discarded.
					*/	  
					if (RecvBuf[3] == '-') {  // PRQA S 4400
						while ( (i<(BUFFER_SIZE-1)) && (recv(hSocket,&RecvBuf[i++],1,0) >= 1)) {}	/* Multi line message: moved from the soocket buffer and added to the local one */ // PRQA S 3230
					}
				}
				RecvBuf[i] = '\0';	/* Terminate the local buffer */
			
				break;
			}
		}
	}	// END: while
	
	FD_CLR(hSocket,&fdread); // PRQA S 3003, 3080, 3081
}

// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"


// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"

////////////////////////////////////////////////////////////////////////////////
//        NAME: SendData
// DESCRIPTION: Sends data from SendBuf buffer.
//   ARGUMENTS: none
// USES GLOBAL: SendBuf
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SendData() // PRQA S 4211
{
	int idx = 0 ;
    int res = 0 ;
    size_t nLeft = strlen(SendBuf);
	fd_set fdwrite; // PRQA S 4102 2 
	timeval time; 

	time.tv_sec = TIME_IN_SEC;
	time.tv_usec = 0;

	if(SendBuf == 0) {
		throw ECSmtp(ECSmtp::SENDBUF_IS_EMPTY);   // PRQA S 3081
    }

	while(1) // PRQA S 4091
	{
		FD_ZERO(&fdwrite); // PRQA S 4092, 4101, 4107

		FD_SET(hSocket,&fdwrite); // PRQA S 3003, 3080, 3081

		if((res = select(hSocket+1,0,&fdwrite,0,&time)) == SOCKET_ERROR)
		{
			FD_CLR(hSocket,&fdwrite); // PRQA S 3003, 3080, 3081
			throw ECSmtp(ECSmtp::WSA_SELECT);   // PRQA S 3081
		}

		if( 0 == res)
		{
			//timeout
			FD_CLR(hSocket,&fdwrite); // PRQA S 3003, 3080, 3081
			throw ECSmtp(ECSmtp::SERVER_NOT_RESPONDING);   // PRQA S 3081
		}

		if((0 != res) && FD_ISSET(hSocket,&fdwrite))
		{
			if(nLeft > 0)
			{
				if((res = send(hSocket,&SendBuf[idx],nLeft,0)) == SOCKET_ERROR) // PRQA S 3010
				{
					FD_CLR(hSocket,&fdwrite); // PRQA S 3003, 3080, 3081
					throw ECSmtp(ECSmtp::WSA_SEND);   // PRQA S 3081
				}
				if(0 == res) {
					break;
                }
				nLeft -= res; // PRQA S 3000, 3084
				idx += res;
			}
			else {
				break;
            }
		}
	}

	FD_CLR(hSocket,&fdwrite); // PRQA S 3003, 3080, 3081
}

// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetLocalHostName
// DESCRIPTION: Returns local host name. 
//   ARGUMENTS: none
// USES GLOBAL: m_pcLocalHostName
// MODIFIES GL: m_oError, m_pcLocalHostName 
//     RETURNS: socket of the remote service
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char* CSmtp::GetLocalHostName() const
{
	char str[255] ; // PRQA S 4403

	if(gethostname(str,255) == SOCKET_ERROR) // PRQA S 4400
	{
		throw ECSmtp(ECSmtp::WSA_HOSTNAME);   // PRQA S 3081
	}
#warning MP who initializes m_sLocalHostName, what about str ?
	return m_sLocalHostName.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetRecipientCount
// DESCRIPTION: Returns the number of recipents.
//   ARGUMENTS: none
// USES GLOBAL: Recipients
// MODIFIES GL: none 
//     RETURNS: number of recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
size_t CSmtp::GetRecipientCount() const
{
	return Recipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetBCCRecipientCount
// DESCRIPTION: Returns the number of bcc-recipents. 
//   ARGUMENTS: none
// USES GLOBAL: BCCRecipients
// MODIFIES GL: none 
//     RETURNS: number of bcc-recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
size_t CSmtp::GetBCCRecipientCount() const
{
	return BCCRecipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetCCRecipientCount
// DESCRIPTION: Returns the number of cc-recipents.
//   ARGUMENTS: none
// USES GLOBAL: CCRecipients
// MODIFIES GL: none 
//     RETURNS: number of cc-recipents
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
size_t CSmtp::GetCCRecipientCount() const
{
	return CCRecipients.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetReplyTo
// DESCRIPTION: Returns m_pcReplyTo string.
//   ARGUMENTS: none
// USES GLOBAL: m_sReplyTo
// MODIFIES GL: none 
//     RETURNS: m_sReplyTo string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char* CSmtp::GetReplyTo() const
{
	return m_sReplyTo.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetMailFrom
// DESCRIPTION: Returns m_pcMailFrom string.
//   ARGUMENTS: none
// USES GLOBAL: m_sMailFrom
// MODIFIES GL: none 
//     RETURNS: m_sMailFrom string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char* CSmtp::GetMailFrom() const
{
	return m_sMailFrom.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetSenderName
// DESCRIPTION: Returns m_pcNameFrom string.
//   ARGUMENTS: none
// USES GLOBAL: m_sNameFrom
// MODIFIES GL: none 
//     RETURNS: m_sNameFrom string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char* CSmtp::GetSenderName() const
{
	return m_sNameFrom.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetSubject
// DESCRIPTION: Returns m_pcSubject string.
//   ARGUMENTS: none
// USES GLOBAL: m_sSubject
// MODIFIES GL: none 
//     RETURNS: m_sSubject string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char* CSmtp::GetSubject() const
{
	return m_sSubject.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetXMailer
// DESCRIPTION: Returns m_pcXMailer string.
//   ARGUMENTS: none
// USES GLOBAL: m_pcXMailer
// MODIFIES GL: none 
//     RETURNS: m_pcXMailer string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char* CSmtp::GetXMailer() const
{
	return m_sXMailer.c_str();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetXPriority
// DESCRIPTION: Returns m_iXPriority string.
//   ARGUMENTS: none
// USES GLOBAL: m_iXPriority
// MODIFIES GL: none 
//     RETURNS: CSmptXPriority m_pcXMailer
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
CSmptXPriority CSmtp::GetXPriority() const // PRQA S 4120
{
	return m_iXPriority;
}

const char* CSmtp::GetMsgLineText(size_t Line) const
{
	if(Line > MsgBody.size()) {
		throw ECSmtp(ECSmtp::OUT_OF_MSG_RANGE);    // PRQA S 3081
    }
	return MsgBody.at(Line).c_str();
}

size_t CSmtp::GetMsgLines() const
{
	return MsgBody.size();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetXPriority
// DESCRIPTION: Setting priority of the message.
//   ARGUMENTS: CSmptXPriority priority - priority of the message (	XPRIORITY_HIGH,
//              XPRIORITY_NORMAL, XPRIORITY_LOW)
// USES GLOBAL: none
// MODIFIES GL: m_iXPriority 
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetXPriority(CSmptXPriority priority) // PRQA S 4121
{
	m_iXPriority = priority;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetReplyTo
// DESCRIPTION: Setting the return address.
//   ARGUMENTS: const char *ReplyTo - return address
// USES GLOBAL: m_sReplyTo
// MODIFIES GL: m_sReplyTo
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetReplyTo(const char *ReplyTo)
{
	//m_sReplyTo.erase();
	m_sReplyTo.insert(0,ReplyTo);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSenderMail
// DESCRIPTION: Setting sender's mail.
//   ARGUMENTS: const char *EMail - sender's e-mail
// USES GLOBAL: m_sMailFrom
// MODIFIES GL: m_sMailFrom
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetSenderMail(const char *EMail)
{
	m_sMailFrom.erase();
	m_sMailFrom.insert(0,EMail);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSenderName
// DESCRIPTION: Setting sender's name.
//   ARGUMENTS: const char *Name - sender's name
// USES GLOBAL: m_sNameFrom
// MODIFIES GL: m_sNameFrom
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetSenderName(const char *Name)
{
	m_sNameFrom.erase();
	m_sNameFrom.insert(0,Name);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSubject
// DESCRIPTION: Setting subject of the message.
//   ARGUMENTS: const char *Subject - subject of the message
// USES GLOBAL: m_sSubject
// MODIFIES GL: m_sSubject
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetSubject(const char *Subject)
{
	m_sSubject.erase();
	m_sSubject.insert(0,Subject);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSubject
// DESCRIPTION: Setting the name of program which is sending the mail.
//   ARGUMENTS: const char *XMailer - programe name
// USES GLOBAL: m_sXMailer
// MODIFIES GL: m_sXMailer
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetXMailer(const char *XMailer)
{
	m_sXMailer.erase();
	m_sXMailer.insert(0,XMailer);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetLogin
// DESCRIPTION: Setting the login of SMTP account's owner.
//   ARGUMENTS: const char *Login - login of SMTP account's owner
// USES GLOBAL: m_sLogin
// MODIFIES GL: m_sLogin
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetLogin(const char *Login)
{
	m_bAuthSet = true ;
	m_sLogin.erase();
	m_sLogin.insert(0,Login);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetPassword
// DESCRIPTION: Setting the password of SMTP account's owner.
//   ARGUMENTS: const char *Password - password of SMTP account's owner
// USES GLOBAL: m_sPassword
// MODIFIES GL: m_sPassword
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JP 2010-07-08
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetPassword(const char *Password)
{
	m_sPassword.erase();
	m_sPassword.insert(0,Password);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: SetSMTPServer
// DESCRIPTION: Setting the SMTP service name and port.
//   ARGUMENTS: const char* SrvName - SMTP service name
//              const unsigned short SrvPort - SMTO service port
// USES GLOBAL: m_sSMTPSrvName
// MODIFIES GL: m_sSMTPSrvName 
//     RETURNS: none
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
//							JO 2010-0708
////////////////////////////////////////////////////////////////////////////////
void CSmtp::SetSMTPServer(const char* SrvName,const unsigned short port)
{
	m_iSMTPSrvPort = port;
	m_sSMTPSrvName.erase();
	m_sSMTPSrvName.insert(0,SrvName);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: GetErrorText (friend function)
// DESCRIPTION: Returns the string for specified error code.
//   ARGUMENTS: CSmtpError ErrorId - error code
// USES GLOBAL: none
// MODIFIES GL: none 
//     RETURNS: error string
//      AUTHOR: Jakub Piwowarczyk
// AUTHOR/DATE: JP 2010-01-28
////////////////////////////////////////////////////////////////////////////////
const char * ECSmtp::GetErrorText() const throw() // PRQA S 4020
{
	switch(ErrorCode)
	{
	case ECSmtp::CSMTP_NO_ERROR:
        {
			return "";
        }
	case ECSmtp::WSA_STARTUP:
        {
			return "Unable to initialise winsock2";
        }
	case ECSmtp::WSA_VER:
        {
			return "Wrong version of the winsock2";
        }
	case ECSmtp::WSA_SEND:
        {
			return "Function send() failed";
        }
	case ECSmtp::WSA_RECV:
        {
			return "Function recv() failed";
        }
	case ECSmtp::WSA_CONNECT:
        {
			return "Function connect failed";
        }
	case ECSmtp::WSA_GETHOSTBY_NAME_ADDR:
        {
			return "Unable to determine remote server";
        }
	case ECSmtp::WSA_INVALID_SOCKET:
        {
			return "Invalid winsock2 socket";
        }
	case ECSmtp::WSA_HOSTNAME:
        {
			return "Function hostname() failed";
        }
	case ECSmtp::WSA_IOCTLSOCKET:
        {
			return "Function ioctlsocket() failed";
        }
	case ECSmtp::BAD_IPV4_ADDR:
        {
			return "Improper IPv4 address";
        }
	case ECSmtp::UNDEF_MSG_HEADER:
        {
			return "Undefined message header";
        }
	case ECSmtp::UNDEF_MAIL_FROM:
        {
			return "Undefined mail sender";
        }
	case ECSmtp::UNDEF_SUBJECT:
        {
			return "Undefined message subject";
        }
	case ECSmtp::UNDEF_RECIPIENTS:
        {
			return "Undefined at least one recipient";
        }
	case ECSmtp::UNDEF_RECIPIENT_MAIL:
        {
			return "Undefined recipent mail";
        }
	case ECSmtp::UNDEF_LOGIN:
        {
			return "Undefined user login";
        }
	case ECSmtp::UNDEF_PASSWORD:
        {
			return "Undefined user password";
        }
	case ECSmtp::COMMAND_MAIL_FROM:
        {
			return "Server returned error after sending MAIL FROM";
        }
	case ECSmtp::COMMAND_EHLO:
        {
			return "Server returned error after sending EHLO";
        }
	case ECSmtp::COMMAND_AUTH_LOGIN:
        {
			return "Server returned error after sending AUTH LOGIN";
        }
	case ECSmtp::COMMAND_DATA:
        {
			return "Server returned error after sending DATA";
        }
	case ECSmtp::COMMAND_QUIT:
        {
			return "Server returned error after sending QUIT";
        }
	case ECSmtp::COMMAND_RCPT_TO:
        {
			return "Server returned error after sending RCPT TO";
        }
	case ECSmtp::MSG_BODY_ERROR:
        {
			return "Error in message body";
        }
	case ECSmtp::CONNECTION_CLOSED:
        {
			return "Server has closed the connection";
        }
	case ECSmtp::SERVER_NOT_READY:
        {
			return "Server is not ready";
        }
	case ECSmtp::SERVER_NOT_RESPONDING:
        {
			return "Server not responding";
        }
	case ECSmtp::FILE_NOT_EXIST:
        {
			return "File not exist";
        }
	case ECSmtp::MSG_TOO_BIG:
        {
			return "Message is too big";
        }
	case ECSmtp::BAD_LOGIN_PASS:
        {
			return "Bad login or password";
        }
	case ECSmtp::UNDEF_XYZ_RESPONSE:
        {
			return "Undefined xyz SMTP response";
        }
	case ECSmtp::LACK_OF_MEMORY:
        {
			return "Lack of memory";
        }
	case ECSmtp::TIME_ERROR:
        {
			return "time() error";
        }
	case ECSmtp::RECVBUF_IS_EMPTY:
        {
			return "RecvBuf is empty";
        }
	case ECSmtp::SENDBUF_IS_EMPTY:
        {
			return "SendBuf is empty";
        }
	case ECSmtp::OUT_OF_MSG_RANGE:
        {
			return "Specified line number is out of message size";
        }
    default:
        {
			return "Undefined error id";
        }
	}
}


//Added methods - Content Type parametrizable
const char* CSmtp::GetTextContentType() const
{
	return m_sTextContentType.c_str();
}

void CSmtp::SetTextContentType(const char *TextContentType)
{
	m_sTextContentType.erase();
	m_sTextContentType.insert(0,TextContentType);
}

