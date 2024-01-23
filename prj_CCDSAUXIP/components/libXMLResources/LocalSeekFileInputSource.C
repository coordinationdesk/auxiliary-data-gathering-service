// PRQA S 1050 EOF
/*

	Copyright 2009-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

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
	
	Revision 2.6  2012/02/13 16:47:40  marpas
	refactoring
	
	Revision 2.5  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.4  2009/03/02 10:28:55  marpas
	minor changes for CODECHEK
	
	Revision 2.3  2008/01/22 13:16:51  clanas
	fixed memory management for xerces > 2.3
	
	Revision 2.2  2008/01/18 18:55:55  clanas
	fixed namespace management for new xerces version
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/06 14:09:22  marpas
	header fixed
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources
	


*/

#include <BinSeekFileInputStream.h>
#include <LocalSeekFileInputSource.h>
#include <xercesc/util/PlatformUtils.hpp> // PRQA S 1013

#include <XMLInitializer.h>

_ACS_BEGIN_NAMESPACE(acs)

#ifdef XERCES_HAS_NAME_SPACE
using namespace XERCES_CPP_NAMESPACE;
#endif

// ---------------------------------------------------------------------------
//  LocalSeekFileInputSource: Constructors and Destructor
// ---------------------------------------------------------------------------
acs::LocalSeekFileInputSource::LocalSeekFileInputSource( const XMLCh* const basePath, 
													const XMLCh* const relativePath) :
    XERCES_CPP_NAMESPACE::InputSource(),
    _pos()
{

    // Create the instance if needed and initialize the XMLPlatformUtils
    XMLInitializer::instance();
    
    //
    //  If the relative part is really relative, then weave it together
    //  with the base path. If not, just take the relative path as the
    //  entire path.
    //
    if (XMLPlatformUtils::isRelative(relativePath))
    {
        XMLCh* tmpBuf = XMLPlatformUtils::weavePaths(basePath, relativePath);
        LocalSeekFileInputSource::setSystemId(tmpBuf);
        delete tmpBuf;
    }
    else
    {
        LocalSeekFileInputSource::setSystemId(relativePath);
    }
}

acs::LocalSeekFileInputSource::LocalSeekFileInputSource(const XMLCh* const filePath) :
    XERCES_CPP_NAMESPACE::InputSource(),
    _pos()
{

    // Create the instance if needed and initialize the XMLPlatformUtils
    XMLInitializer::instance();
    
    //
    //  If the path is relative, then complete it acording to the current
    //  working directory rules of the current platform. Else, just take
    //  it as is.
    //
    if (XMLPlatformUtils::isRelative(filePath))
    {
        XMLCh* tmpBuf = XMLPlatformUtils::getFullPath(filePath);
        LocalSeekFileInputSource::setSystemId(tmpBuf);
        delete tmpBuf;
    }
     else
    {
        LocalSeekFileInputSource::setSystemId(filePath);
    }
}

acs::LocalSeekFileInputSource::LocalSeekFileInputSource(const XMLCh* const filePath, const int pos) :
   XERCES_CPP_NAMESPACE::InputSource(),
    _pos(pos)
{

    // Create the instance if needed and initialize the XMLPlatformUtils
    XMLInitializer::instance();
    
    //
    //  If the path is relative, then complete it acording to the current
    //  working directory rules of the current platform. Else, just take
    //  it as is.
    //
	if (XMLPlatformUtils::isRelative(filePath))
    {
        XMLCh* tmpBuf = XMLPlatformUtils::getFullPath(filePath);
        LocalSeekFileInputSource::setSystemId(tmpBuf);
        delete tmpBuf;
    }
     else
    {
        LocalSeekFileInputSource::setSystemId(filePath);
    }
}


acs::LocalSeekFileInputSource::~LocalSeekFileInputSource() throw() {}


// ---------------------------------------------------------------------------
//  LocalSeekFileInputSource: InputSource interface implementation
// ---------------------------------------------------------------------------
XERCES_CPP_NAMESPACE::BinInputStream* acs::LocalSeekFileInputSource::makeStream() const // PRQA S 4020
{
    acs::BinSeekFileInputStream* retStrm = new acs::BinSeekFileInputStream(getSystemId(),_pos);
    if (!retStrm->getIsOpen())
    {
        delete retStrm;
        return 0;
    }
    else {
        return retStrm;
    }
}

_ACS_END_NAMESPACE
