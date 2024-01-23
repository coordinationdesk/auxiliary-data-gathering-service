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
	Revision 2.7  2012/12/02 20:03:48  marpas
	qa rules
	
	Revision 2.6  2012/02/13 16:47:40  marpas
	refactoring
	
	Revision 2.5  2009/09/16 11:25:29  marpas
	old xerces w/o namespace fixing
	
	Revision 2.4  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
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

#ifndef _BinSeekFileInputStream_H_
#define _BinSeekFileInputStream_H_


#include <acs_c++config.hpp>
#include <xercesc/util/BinInputStream.hpp>  // PRQA S 1013
 #if ( XERCES_VERSION_MAJOR > 2 )
  #include <xercesc/util/PlatformUtils.hpp>  // PRQA S 1013
 #endif

#ifndef XERCES_HAS_NAME_SPACE
	#define XERCES_CPP_NAMESPACE
#endif

_ACS_BEGIN_NAMESPACE(acs)


class BinSeekFileInputStream : public XERCES_CPP_NAMESPACE::BinInputStream { // PRQA S 2109

public:

		
	// -----------------------------------------------------------------------
    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    BinSeekFileInputStream (const XMLCh* const fileName, const int pos);
 #if ( XERCES_VERSION_MAJOR > 2 )
    explicit BinSeekFileInputStream (const XERCES_CPP_NAMESPACE::FileHandle toAdopt);
 #else
    explicit BinSeekFileInputStream (const FileHandle toAdopt);
 #endif


    virtual ~BinSeekFileInputStream() throw() ;


    // -----------------------------------------------------------------------
    //  Getter methods
    // -----------------------------------------------------------------------
    bool getIsOpen() const;
 #if ( XERCES_VERSION_MAJOR > 2 )
    XMLSize_t getSize() const;
 #else
    unsigned int getSize() const;
 #endif
    void reset();


    // -----------------------------------------------------------------------
    //  Implementation of the input stream interface
    // -----------------------------------------------------------------------
 #if ( XERCES_VERSION_MAJOR > 2 )
    virtual XMLFilePos curPos() const;
    virtual XMLSize_t readBytes(XMLByte* toFill, XMLSize_t maxToRead);
    virtual const XMLCh * getContentType () const;
 #else
    virtual unsigned int curPos() const;
    virtual unsigned int readBytes(XMLByte* const toFill, const unsigned int maxToRead);
 #endif

private:
    BinSeekFileInputStream() ;
    BinSeekFileInputStream(const BinSeekFileInputStream &) ;
    BinSeekFileInputStream & operator= (const BinSeekFileInputStream &) ;

private :
    // -----------------------------------------------------------------------
    //  Private data members
    //
    //  fSource
    //      The source file that we represent. The FileHandle type is defined
    //      per platform.
    // -----------------------------------------------------------------------
 #if ( XERCES_VERSION_MAJOR > 2 )
    XERCES_CPP_NAMESPACE::FileHandle fSource ;
 #else
    FileHandle      fSource;
 #endif
	
	int _pos;
};



_ACS_END_NAMESPACE

#endif //_BinSeekFileInputStream_H_






