// 
// 	Copyright 1995-2013, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
// 	$Prod: A.C.S. SendMail Library $
// 
// 	$Id$
// 
// 	$Author$
// 
// 	$Log$
// 	Revision 5.2  2017/01/09 08:56:06  clanas
// 	fixed typo name space unneeded
// 	
// 	Revision 5.1  2013/12/09 16:53:51  enrcar
// 	EC:: Debug improved
// 	
// 	Revision 5.0  2013/06/17 18:25:08  marpas
// 	adopting libException 5.x standards
// 	coding best practices applied
// 	qa qules applied
// 	
// 	Revision 1.8  2013/02/27 17:20:27  nicvac
// 	Text ContentType is now a parameter. Not mandatory.
// 	
// 	Revision 1.7  2012/02/13 17:11:15  marpas
// 	removing compiler warnings
// 	
// 	Revision 1.6  2010/12/20 15:14:23  marpas
// 	smV class renamed into sendmailV
// 	
// 	Revision 1.5  2010/12/06 14:22:42  enrcar
// 	EC:: DumpServerParams method added
// 	
// 	Revision 1.4  2010/12/06 13:58:40  enrcar
// 	Work in progress
// 	
// 	Revision 1.3  2010/12/03 17:16:00  enrcar
// 	EC:: work in progress
// 	
// 	Revision 1.2  2010/11/29 15:01:12  enrcar
// 	EC:: work in progress
// 	
// 	Revision 1.1  2010/11/26 16:58:39  enrcar
// 	EC:: imported source
// 	
//


#include <SendMail.h>
#include <sendmailV.h>
#include <File.h>
#include <exException.h>
#include <Filterables.h>
#include <CSmtp.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace { // unnamed 
    sendmailV version ;
}
ACS_CLASS_DEFINE_DEBUG_LEVEL(SendMail);


//------------------------------------------------------------------------
//
//                           SendMail (2 params.)   
// 
//------------------------------------------------------------------------
SendMail::SendMail(const string& server, const string& mailFromAddress, signed short int port) :
    _smtpPort((port < 0) ? smtpNonSecurePort : port), // PRQA S 3000, 3010, 3380
    _authRequired(false),
    _smptServerName(server),
    _smtpUserName(),
    _smtpPassword(),
    _ToVector(),
    _CcVector(),
    _BccVector(),
    _attachmentsVector(),
    _mailBody(),
    _mailSubject(),
    _mailFromAddress(mailFromAddress),
	_textContentType()
{
}


//------------------------------------------------------------------------
//
//                           SendMail (4 params.)     
// 
//------------------------------------------------------------------------
SendMail::SendMail( const string& server, 
                    const string& user, 
                    const string& pwd, 
                    const string& mailFromAddress, 
                    signed short int port) :
    _smtpPort((port < 0) ? smtpNonSecurePort : port ),  // PRQA S 3000, 3010, 3380
    _authRequired(true),
    _smptServerName(server), 
    _smtpUserName(user), 
    _smtpPassword(pwd),
    _ToVector(),
    _CcVector(),
    _BccVector(),
    _attachmentsVector(),
    _mailBody(),
    _mailSubject(),
    _mailFromAddress(mailFromAddress),
	_textContentType()
{
}


//------------------------------------------------------------------------
//
//                          ~SendMail
//
//------------------------------------------------------------------------
SendMail::~SendMail() throw() {}


//------------------------------------------------------------------------
//
//                             dumpServerParams
//
//------------------------------------------------------------------------
void SendMail::dumpServerParams () const
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ; 

	ACS_LOG_INFO("SMTP Server: " << _smptServerName << ": " << _smtpPort) ;

	if (_authRequired)
	{
		ACS_LOG_INFO("SMTP Username: " << _smtpUserName) ;
		ACS_LOG_INFO("SMTP password: " << _smtpPassword) ;
	}
     
	ACS_LOG_INFO("E-Mail Originator: " << _mailFromAddress) ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished") ;
}


//------------------------------------------------------------------------
//
//                          resetAllDestinations
//
//------------------------------------------------------------------------
void SendMail::resetAllDestinations () 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;

    void resetToEntries() ;
    void resetCcEntries() ;
    void resetBccEntries() ;
}


//------------------------------------------------------------------------
//
//                          dumpAllDestinations
//
//------------------------------------------------------------------------
void SendMail::dumpAllDestinations () 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;

    ostringstream os ;
	os << "\"To:\" address-list: " ;	// Intentionally missing endl
	if (not _ToVector.empty()) 
    {
    	os << "\n" ;
        size_t tvsize = _ToVector.size();
    	for (size_t i=0; i<tvsize; ++i)
        {
        	os << "\t" << (setw(3)) << (i+1) << ".\t<" << _ToVector[i] << ">\n" ; // PRQA S 3084, 4400
    	}
    }
	else
    {
    	os << "<EMPTY LIST>\n" ; 
    }

	os << "\"Cc:\" address-list: " ;	// Intentionally missing endl
	if (not _CcVector.empty()) 
    {
    	os << "\n" ;
        size_t ccsize = _CcVector.size() ;
    	for (size_t i=0; i<ccsize; ++i)
        {
        	os << "\t" << (setw(3)) << (i+1) << ".\t<" << _CcVector[i] << ">\n" ; // PRQA S 3084, 4400
    	}
    }
	else
    {
    	os << "<EMPTY LIST>\n" ; 
    }

	os << "\"Bcc:\" address-list: " ;	// Intentionally missing endl
	if (not _BccVector.empty()) 
    {
    	os << "\n" ;
        size_t bccsize = _BccVector.size() ;
    	for (size_t i=0; i<bccsize; ++i)
        {
        	os << "\t" << (setw(3)) << (i+1) << ".\t<" << _BccVector[i] << ">\n" ; // PRQA S 3084, 4400
    	}
    }
	else
    {
    	os << "<EMPTY LIST>\n" ; 
    }
    ACS_LOG_DEBUG(os.str()) ;
}


//------------------------------------------------------------------------
//
//                          addToEntry
//
//------------------------------------------------------------------------
void SendMail::addToEntry (const std::string& dest, bool reset) 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"dest: !" << dest << "!  reset: " << boolalpha << reset) ; 


	if (reset) { resetToEntries() ; }
   
    _ToVector.push_back(dest) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _ToVector size: " << _ToVector.size()) ;
}



//------------------------------------------------------------------------
//
//                          addToEVector
//
//------------------------------------------------------------------------
void SendMail::addToVector (const vector<string>& v, bool reset) 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be added: !" << v.size() << "!  reset: " << boolalpha << reset) ;

	if (reset) { resetToEntries() ; }
   
   	for (size_t i=0; i<v.size(); i++) { _ToVector.push_back(v[i]) ; }
        
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _ToVector size: " << _ToVector.size()) ;
}


//------------------------------------------------------------------------
//
//                           getToEntries
//
//------------------------------------------------------------------------
vector<string> SendMail::getToEntries() const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be returned: !" << _ToVector.size()) ;

	return _ToVector ;
}


//------------------------------------------------------------------------
//
//                            resetToEntries
//
//------------------------------------------------------------------------
void SendMail::resetToEntries() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;
    _ToVector.clear() ;
}


//------------------------------------------------------------------------
//
//                          addCcEntry
//
//------------------------------------------------------------------------
void SendMail::addCcEntry (const std::string& dest, bool reset) 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"dest: !" << dest << "!  reset: " << boolalpha << reset) ;


	if (reset) { resetCcEntries() ; }
   
    _CcVector.push_back(dest) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _CcVector size: " << _CcVector.size()) ;
}



//------------------------------------------------------------------------
//
//                          addCcEVector
//
//------------------------------------------------------------------------
void SendMail::addCcVector (const vector<string>& v, bool reset) 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be added: !" << v.size() << "!  reset: " << boolalpha << reset) ;


	if (reset) { resetCcEntries() ; }
   
   	for (size_t i=0; i<v.size(); i++)  { _CcVector.push_back(v[i]) ; }
        
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _CcVector size: " << _CcVector.size()) ;
}


//------------------------------------------------------------------------
//
//                           getCcEntries
//
//------------------------------------------------------------------------
vector<string> SendMail::getCcEntries() const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be returned: !" << _CcVector.size()) ; 

	return _CcVector ;
}


//------------------------------------------------------------------------
//
//                            resetCcEntries
//
//------------------------------------------------------------------------
void SendMail::resetCcEntries() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;
    
    _CcVector.clear() ;
}
//------------------------------------------------------------------------
//
//                          addBccEntry
//
//------------------------------------------------------------------------
void SendMail::addBccEntry (const std::string& dest, bool reset) 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"dest: !" << dest << "!  reset: " << boolalpha << reset) ;


	if (reset) { resetBccEntries() ; }
   
    _BccVector.push_back(dest) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _BccVector size: " << _BccVector.size()) ; 
}



//------------------------------------------------------------------------
//
//                          addBccEVector
//
//------------------------------------------------------------------------
void SendMail::addBccVector (const vector<string>& v, bool reset) 
{    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be added: !" << v.size() << "!  reset: " << boolalpha << reset) ;


	if (reset) { resetBccEntries() ; }
   
   	for (size_t i=0; i<v.size(); i++) { _BccVector.push_back(v[i]) ; }
        
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _BccVector size: " << _BccVector.size()) ;
}


//------------------------------------------------------------------------
//
//                           getBccEntries
//
//------------------------------------------------------------------------
vector<string> SendMail::getBccEntries() const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be returned: !" << _BccVector.size()) ; 

	return _BccVector ;
}


//------------------------------------------------------------------------
//
//                            resetBccEntries
//
//------------------------------------------------------------------------
void SendMail::resetBccEntries() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;
    
    _BccVector.clear() ;
}

//------------------------------------------------------------------------
//
//                            addAttachment
//
//------------------------------------------------------------------------
void SendMail::addAttachment(const string &fileName, bool reset) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"file to be added: !" << fileName << "!  reset: " << boolalpha << reset) ;

	if (reset) { resetAttachments() ; }
    
    _attachmentsVector.push_back(fileName) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _attachments Vector size: " << _attachmentsVector.size()) ; 
}


//------------------------------------------------------------------------
//
//                            addAttachment
//
//------------------------------------------------------------------------
void SendMail::addAttachment(const vector<string> &fileVector, bool reset) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be added: !" << fileVector.size() << "!  reset: " << boolalpha << reset) ;
	
    if (reset) { resetAttachments() ; }
    
    size_t fvsize = fileVector.size() ;
	for (size_t i=0; i<fvsize; ++i) {
		_attachmentsVector.push_back(fileVector[i]) ;
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _attachments Vector size: " << _attachmentsVector.size()) ; 
}


//------------------------------------------------------------------------
//
//                            resetAttachments
//
//------------------------------------------------------------------------
void SendMail::resetAttachments() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called" ) ;
  
    _attachmentsVector.clear() ;
}


//------------------------------------------------------------------------
//
//                            getAttachments
//
//------------------------------------------------------------------------
void SendMail::getAttachments(vector<string> &v) const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector about to be returned: " << _attachmentsVector.size()) ; 

	v = _attachmentsVector ;
}


//------------------------------------------------------------------------
//
//                            dumpAttachment
//
//------------------------------------------------------------------------
void SendMail::dumpAttachments(bool checkFiles) const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of _attachmentsVector: " << _attachmentsVector.size()) ; 

    ostringstream os ;
	os << "Attachments list: " ;	// Intentionally missing endl
	if (not _attachmentsVector.empty()) 
    {
		os << "\n" ;
        size_t attsize = _attachmentsVector.size() ;
    	for (size_t i=0; i<attsize; ++i)
		{
			string fileName = _attachmentsVector[i] ;
			os << "\t" << (setw(3)) << (i+1) << ".\t" << fileName ; // PRQA S 3084, 4400
			if (checkFiles)
			{
				bool checkOk = ( File::exists(fileName) && File::isRegularFile(fileName) && File::isReadable(fileName) ) ;
				os << "\tFile status: " << (checkOk ? "GOOD" : "ERROR" ) ; // PRQA S 3380
			}

			os << "\n" ;
		}
    }
	else
    {
        os << "<EMPTY LIST>\n" ; 
    }
    ACS_LOG_DEBUG(os.str()) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _attachments Vector size: " << _attachmentsVector.size()) ; 
}


//------------------------------------------------------------------------
//
//                            getSubjectAndBody
//
//------------------------------------------------------------------------
void SendMail::getSubjectAndBody(string& subject, vector<string>& v) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;

    subject = _mailSubject ;
    v = _mailBody ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished") ;
}


//------------------------------------------------------------------------
//
//                            setSubjectAndBody
//
//------------------------------------------------------------------------
void SendMail::setSubjectAndBody(const std::string& subject, const std::string& body) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;


	vector<string> v ; 
	
    stringToVector(body, v) ;
    setSubjectAndBody( subject, v ) ;


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished") ;
}


//------------------------------------------------------------------------
//
//                            setSubjectAndBody
//
//------------------------------------------------------------------------
void SendMail::setSubjectAndBody(const std::string& subject, const vector<string> &vectorBody) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called. Input vector size: " << vectorBody.size() ) ;

    _mailSubject = subject ;
	_mailBody.clear() ;
	
	// Process input vector in order to split elements with CR (if any)
    size_t vbsize = vectorBody.size() ;
    
	for (size_t i=0; i<vbsize; ++i)
	{
		vector <string> v ;
		
		stringToVector(vectorBody[i], v) ;		
		
        size_t vsize = v.size() ;
		for (size_t j=0; j<vsize; ++j) { _mailBody.push_back( v[j] ) ; }
	}
	    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. _mailBody vector size: " << _mailBody.size()) ;
}


void SendMail::setTextContentType(const std::string& textContentType) { // PRQA S 4121
	_textContentType = textContentType;
}

const string &SendMail::getTextContentType() const throw() { // PRQA S 4120
    return _textContentType;
}


//------------------------------------------------------------------------
//
//                            dumpSubjectAndBody
//
//------------------------------------------------------------------------
void SendMail::dumpSubjectAndBody(char bodyDelimiter) const
{
    ostringstream os ;
	os << "Subject: " << _mailSubject << "\n" ;
	os << "Body: " << "\n" ;
    
    for (size_t i=0; i<_mailBody.size(); i++) 
    {
        os << bodyDelimiter << _mailBody[i] << bodyDelimiter<< "\n" ;
    }
    ACS_LOG_DEBUG(os.str()) ;
}


//------------------------------------------------------------------------
//
//                              check
//
//------------------------------------------------------------------------
bool 								// OUT: TRUE: Check successfull, FALSE: Check error
		SendMail::check(bool checkBodySubject) const // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;
	
	// Check if at least one address (To, Cc or Bcc) is provided
	bool checkAddressOk = ( (not _ToVector.empty()) || (not _CcVector.empty()) || (not _BccVector.empty() ) ) ;
	if (!checkAddressOk)
	{
		ACS_LOG_ERROR(SimpleDebugSignature << "Fatal error: No address provided") ; 
		return false ;
	}


	if (checkBodySubject)
	{
		// Check if both Subject and Body are not-null
	
		bool checkBodySubjectOk = ( (not _mailSubject.empty()) && (not _mailBody.empty() ) ) ;
		if (!checkBodySubjectOk)
		{
			ACS_LOG_ERROR(SimpleDebugSignature << "Fatal error: Both subject and body must be not-null") ;
			return false ;
		}
	}


	// Check attachments 
	for (size_t i=0; i<_attachmentsVector.size(); i++)
	{
		string fileName = _attachmentsVector[i] ;
		bool attachmentOk = ( File::exists(fileName) && File::isRegularFile(fileName) && File::isReadable(fileName) ) ;
		if (!attachmentOk)
		{
			ACS_LOG_ERROR(SimpleDebugSignature << "Fatal error:  Attachment: " << fileName << " not found or invalid") ;
			return false ;
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Attachment: " << fileName << "  check was successfull") ;
		}
	}


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method successfully finished") ;
	
	return true ;
}


//------------------------------------------------------------------------
//
//                              send
//
//------------------------------------------------------------------------
bool 								// OUT: TRUE: Send successfull, FALSE: Send error
		SendMail::send() const // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method called") ;

	const bool checkBodySubject = false ;
	
	if (! check(checkBodySubject) )
	{
	    ACS_LOG_ERROR(SimpleDebugSignature << "Pre-check failed") ;
		return false ;
	}
	
	CSmtp csmtp ;
	
	try
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"csmtp.SetSMTPServer: \"" << _smptServerName << "\" : " << _smtpPort) ;
		csmtp.SetSMTPServer(_smptServerName.c_str(), _smtpPort) ;

		if (_authRequired)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Auth:  \"" << _smtpUserName << "\" , \"" << _smtpPassword << "\"" ) ;
			csmtp.SetLogin(_smtpUserName.c_str());
			csmtp.SetPassword(_smtpPassword.c_str());
		}
		else
		{
			// Do nothing
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"No auth required") ;
		}

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Sender: \"" << _mailFromAddress << "\"" ) ;
		csmtp.SetSenderName(_mailFromAddress.c_str());
		csmtp.SetSenderMail(_mailFromAddress.c_str());
#if(0)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Reply to: \"" <<_mailFromAddress << "\"") ;
		csmtp.SetReplyTo(_mailFromAddress.c_str());
#endif
		for (size_t i=0; i<_ToVector.size() ; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"To: \"" <<_ToVector[i] << "\"") ;
			csmtp.AddRecipient(_ToVector[i].c_str()) ;
        }			
		for (size_t i=0; i<_CcVector.size() ; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Cc: \"" <<_CcVector[i] << "\"") ;
			csmtp.AddCCRecipient(_CcVector[i].c_str()) ;
        }			
	
		for (size_t i=0; i<_BccVector.size() ; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Bcc: \"" <<_BccVector[i] << "\"") ;
			csmtp.AddBCCRecipient(_BccVector[i].c_str()) ;
        }			

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Subject: \"" << _mailSubject << "\"") ;
		csmtp.SetSubject(_mailSubject.c_str());

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Body:") ;
		for (size_t i=0; i<_mailBody.size() ; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"\"" << _mailBody[i] << "\"") ;
			csmtp.AddMsgLine(_mailBody[i].c_str()) ;
        }			

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Attachments:") ;
		for (size_t i=0; i<_attachmentsVector.size() ; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"\"" << _attachmentsVector[i] << "\"") ;
			csmtp.AddAttachment(_attachmentsVector[i].c_str()) ;
        }			
	 
	 	const string xmailer ("Generated by A.C.S.libSendMail.  $Revision$");
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Xmailer: \"" << xmailer << "\"") ;
		csmtp.SetXMailer(xmailer.c_str());

		if ( !_textContentType.empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"SetTextContentType: \"" << _textContentType << "\"") ;
			csmtp.SetTextContentType( _textContentType.c_str() );
		}

	}
	catch(exception &e)
	{
		ACS_LOG_ERROR("Error setting SMTP parameters: " << e.what()) ;
		return false ;
	}

	try
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Sending message ... ...") ;
		csmtp.Send() ;
	}
	catch(exception &e)
	{
		ACS_LOG_ERROR("Error sending mail: " << e.what()) ;
		return false ;
	}


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method successfully finished" ) ;
	
	return true ;
}

//------------------------------------------------------------------------
//
//                            StringToVector
//
//------------------------------------------------------------------------
vector<string> SendMail::stringToVector(const string &s, vector<string>& v)
{    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"size of vector before calling the method: " << v.size()) ;

	size_t f = 0 ;
    size_t pos = 0;

	if (!s.empty())
    {

        do
        {
    	    f = s.find("\n", pos) ;

	        string t ;
    	    if (f != string::npos)
            {
                t = s.substr(pos, f-pos) ; // PRQA S 3084 2
                pos = f+1 ;
            }
            else {
        	    t = s.substr(pos) ;            
            }
            v.push_back(t) ;

        }
	    while(f != string::npos) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. Size of the vector: " << v.size()) ;
    
    return v ;
} 

_ACS_END_NAMESPACE

