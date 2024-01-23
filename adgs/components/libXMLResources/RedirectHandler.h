// PRQA S 1050 EOF
/*
    Copyright 2002-2013, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. XMLResources Library $

    $Id$

    $Author$

    $Log$
    Revision 5.0  2013/07/18 17:54:33  marpas
    adopting libException 5.x standards
    performances improved
    coding best practices applied
    qa warnings removed
    compilation warnings removed

    Revision 2.11  2012/12/02 20:03:49  marpas
    qa rules

    Revision 2.10  2012/02/13 16:47:41  marpas
    refactoring

    Revision 2.9  2009/09/16 11:25:29  marpas
    old xerces w/o namespace fixing

    Revision 2.8  2009/09/16 11:14:07  marpas
    XMLValidator used in XMLIstream
    xerces namespace management simplified

    Revision 2.7  2009/09/14 12:51:52  marpas
    XMLValidator class added
    unitary test added
    other test improved

    Revision 2.6  2009/09/10 14:55:24  marpas
    Xercers 3.0.x i/f compatibilty

    Revision 2.5  2009/07/29 15:35:23  marpas
    debug & robustness improved

    Revision 2.4  2009/05/28 12:08:12  marpas
    debug improved

    Revision 2.3  2008/01/18 18:55:55  clanas
    fixed namespace management for new xerces version

    Revision 2.2  2006/07/14 09:23:20  marpas
    documentation fixed and improved.
    global variables are now encaspulated into classes better than before
    interruption of saxExtract method allowed when the last data consumer leave.
    new test to show this feature added

    Revision 2.1  2006/03/17 16:14:21  danalt
    added RedirectHandler


*/

#ifndef _RedirectHandler_H_
#define _RedirectHandler_H_


#include <acs_c++config.hpp>
#include <xercesc/sax/HandlerBase.hpp> // PRQA S 1013

#ifndef XERCES_HAS_NAME_SPACE
	#define XERCES_CPP_NAMESPACE
#endif


_ACS_BEGIN_NAMESPACE(acs)


class RedirectHandler : public XERCES_CPP_NAMESPACE::HandlerBase // PRQA S 2109, 2153
{

public:
    // -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    RedirectHandler();
    virtual ~RedirectHandler() throw() ;

    // -----------------------------------------------------------------------
    //  Handlers for the SAX EntityResolver interface
    // -----------------------------------------------------------------------
    virtual XERCES_CPP_NAMESPACE::InputSource* resolveEntity(const XMLCh* const publicId, const XMLCh* const systemId);

	static void getValues(std::string & path, bool &enabled) ;
	
	static const std::string & getNameSpace() throw();

private:
	RedirectHandler(const RedirectHandler &);
	RedirectHandler &operator=(const RedirectHandler &);

private:
	static const std::string _nameSpace ; //  = "XML_DTDConfiguration";
	static const std::string _enableKey ; // = "EnableDTDRedirect";
	static const std::string _pathKey ; //  = "RedirectPath";
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RedirectHandler) ;

};

_ACS_END_NAMESPACE


#endif //_RedirectHandler_H_

