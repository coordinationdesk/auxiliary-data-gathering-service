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
	Revision 5.4  2014/05/26 16:37:40  marfav
	Thread safety improved
	Destructors performs try-catch and notify
	hpTrimmedString robustness improved
	
	Revision 5.3  2014/02/13 12:42:29  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/11/28 15:21:52  marpas
	coding best practices applied and warnings removed
	
	Revision 5.1  2013/09/17 12:51:19  marpas
	reliability bug discovered by S1PDGS-31106
	checking FILE* pointer before using it in all involved methods.
	Exception is thrown in case the file is not opened
	
	Revision 5.0  2013/07/18 17:54:33  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/02/13 16:47:40  marpas
	refactoring
	
	Revision 2.3  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
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
#include <XMLHandler.h>
#include <Filterables.h>
#include <cstdio>
#include <xercesc/util/Janitor.hpp> // PRQA S 1013 5
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLExceptMsgs.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/RuntimeException.hpp>
#include <exStream.h>
#include <exception>

#include <XMLInitializer.h>

_ACS_BEGIN_NAMESPACE(acs)

#ifdef XERCES_HAS_NAME_SPACE
using namespace XERCES_CPP_NAMESPACE;
#endif

// ---------------------------------------------------------------------------
//  BinSeekFileInputStream: Constructors and Destructor
// ---------------------------------------------------------------------------
acs::BinSeekFileInputStream::BinSeekFileInputStream(const XMLCh* const fileName, const int pos) :
    XERCES_CPP_NAMESPACE::BinInputStream(),
    fSource(0),
	_pos(pos)
{
    // Create the instance if needed and initialize the XMLPlatformUtils
    XMLInitializer::instance();
    
    // Try to open the file
    fSource = XMLPlatformUtils::openFile(fileName);
	reset();
}

acs::BinSeekFileInputStream::BinSeekFileInputStream(const FileHandle toAdopt) :
    XERCES_CPP_NAMESPACE::BinInputStream(),
    fSource(toAdopt),
	_pos(0)
{
    // Create the instance if needed and initialize the XMLPlatformUtils
    XMLInitializer::instance();
}

acs::BinSeekFileInputStream::~BinSeekFileInputStream() throw() 
{
    // PRQA S 4631 L1
    try {
    
        if (fSource) { 
            XMLPlatformUtils::closeFile(fSource); 
            fSource = 0 ;
        } // PRQA S 4631
    }
    catch (const XERCES_CPP_NAMESPACE::XMLException& x)
    {
        const XMLCh * err = x.getMessage() ;
        std::string errmsg ;
        ACS_LOG_ERROR("XMLException caugth: \"" << XMLHandler::hpTrimmedString(err, errmsg) << '"') ;
    }    
    catch(std::exception & x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}


// ---------------------------------------------------------------------------
//  BinSeekFileInputStream: Getter methods
// ---------------------------------------------------------------------------
 #if ( XERCES_VERSION_MAJOR > 2 )
XMLSize_t acs::BinSeekFileInputStream::getSize() const // PRQA S 4214
 #else
unsigned int acs::BinSeekFileInputStream::getSize() const // PRQA S 4214
 #endif
{
    return XMLPlatformUtils::fileSize(fSource)-_pos; // PRQA S 3000, 3084
}


// ---------------------------------------------------------------------------
//  BinSeekFileInputStream: Stream management methods
// ---------------------------------------------------------------------------
void acs::BinSeekFileInputStream::reset() // PRQA S 4211
{
    if (not getIsOpen()) {
        ACS_THROW(exFileOpenException("file is not opened")) ; // PRQA S 3081
    }
	// Seek to the start of the file
    if (0 != fseek(reinterpret_cast<FILE*>(fSource), _pos, SEEK_SET)) { // PRQA S 3081
        ThrowXML(XMLPlatformUtilsException, // PRQA S 3508, 3509
                 XMLExcepts::File_CouldNotResetFile);
    }
}


// ---------------------------------------------------------------------------
//  BinSeekFileInputStream: Implementation of the input stream interface
// ---------------------------------------------------------------------------
 #if ( XERCES_VERSION_MAJOR > 2 )
    XMLFilePos acs::BinSeekFileInputStream::curPos() const
 #else
    unsigned int acs::BinSeekFileInputStream::curPos() const
 #endif
{
    if (not getIsOpen()) {
        ACS_THROW(exFileOpenException("file is not opened")) ; // PRQA S 3081
    }
    return XMLPlatformUtils::curFilePos(fSource)-_pos; // PRQA S 3000, 3084
}

 #if ( XERCES_VERSION_MAJOR > 2 )
    XMLSize_t acs::BinSeekFileInputStream::readBytes(XMLByte *toFill, XMLSize_t maxToRead)
 #else
unsigned int acs::BinSeekFileInputStream::readBytes(XMLByte* const toFill, const unsigned int maxToRead)
 #endif
{
    if (not getIsOpen()) {
        ACS_THROW(exFileOpenException("file is not opened")) ; // PRQA S 3081
    }
    //
    //  Read up to the maximum bytes requested. We return the number
    //  actually read.
    //
    return XMLPlatformUtils::readFileBuffer(fSource, maxToRead, toFill);
}

 #if ( XERCES_VERSION_MAJOR > 2 )
const XMLCh * acs::BinSeekFileInputStream::getContentType () const
{
    return 0 ;
}
 #endif
 
// ---------------------------------------------------------------------------
//  BinSeekFileInputStream: Getter methods
// ---------------------------------------------------------------------------
bool acs::BinSeekFileInputStream::getIsOpen() const
{
    return (fSource != 0);
}


_ACS_END_NAMESPACE

