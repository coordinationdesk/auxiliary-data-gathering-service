// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2013, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: A.C.S. SendMail Library -- HEADER FILE $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	Log: SendMail.h,v $
 * 
 */
   


#ifndef _SendMail_H_
#define _SendMail_H_


#include <string>
#include <vector>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class SendMail // PRQA S 2109
{
public:	
  
	/*! class SendMailException declaration */
	exDECLARE_EXCEPTION(SendMailException,exException) ; // PRQA S 2131, 2502
  

    SendMail(const std::string& server, const std::string& mailFromAddress, signed short int port=-1);
    SendMail(const std::string& server, const std::string& user, const std::string& pwd, const std::string &mailFromAddress, signed short int port=-1);
    /* ENRCAR, TODO: C'tor to handle SMTP secure connections may appear in the future.
	E.G. SendMail(std::string serverName, enum AUTH_TYPE, std::string userName,std::string password, signed int port=-1); */
	~SendMail() throw() ;
  
    void dumpServerParams() const ;
    
    void addToEntry (const std::string&, bool reset=false) ;
    void addToVector (const std::vector<std::string>&, bool reset=false) ;
    std::vector<std::string> getToEntries() const ;
    void resetToEntries() ;
    void addCcEntry (const std::string&, bool reset=false) ;
    void addCcVector (const std::vector<std::string>&, bool reset=false) ;
    std::vector<std::string> getCcEntries() const ;
    void resetCcEntries() ;
    void addBccEntry (const std::string&, bool reset=false) ;
    void addBccVector (const std::vector<std::string>&, bool reset=false) ;
    std::vector<std::string> getBccEntries() const ;
    void resetBccEntries() ;
    void dumpAllDestinations() ;
	void resetAllDestinations() ;

	void setSubjectAndBody(const std::string& subject, const std::string& body) ;
	void setSubjectAndBody(const std::string& subject, const std::vector<std::string>& vectorBody) ;
    void getSubjectAndBody(std::string& subject, std::vector<std::string>& v) const ;
    void dumpSubjectAndBody(char bodyDelimiter='\0') const ;

	void setTextContentType(const std::string& textContentType); //Not mandatory
	const std::string &getTextContentType() const throw() ;

    void addAttachment(const std::string &fileName, bool reset=false) ;
    void addAttachment(const std::vector<std::string> &fileVector, bool reset=false) ;
    void getAttachments(std::vector<std::string>&) const ;
    void resetAttachments() ;
    void dumpAttachments(bool checkFiles=false) const ;
    
    bool check(bool checkBodySubject=false) const ;
    
    bool send() const ;

private:

	SendMail() ;	/* unimplemented */
    
	SendMail(const SendMail &) ;
	SendMail &operator=(const SendMail &) ;

	static std::vector<std::string> stringToVector(const std::string &, std::vector<std::string>&) ;

private:

	unsigned short int _smtpPort ;
	bool _authRequired ;
    std::string _smptServerName ;
    std::string _smtpUserName ;
    std::string _smtpPassword ;
	std::vector<std::string> _ToVector ;
    std::vector<std::string> _CcVector ; 
    std::vector<std::string> _BccVector; 
    std::vector<std::string> _attachmentsVector; 
    std::vector<std::string> _mailBody ;
    std::string _mailSubject ;
    std::string _mailFromAddress ;

	std::string _textContentType;
    
	static const unsigned short smtpNonSecurePort = 25 ;
	/* static const unsigned short smtpSecurePort = ... ; */

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(SendMail) ;

} ;

_ACS_END_NAMESPACE

#endif  /* _SendMail_H_ */
