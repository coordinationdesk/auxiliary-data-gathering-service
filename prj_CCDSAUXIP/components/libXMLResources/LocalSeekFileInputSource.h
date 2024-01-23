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
	Revision 2.8  2012/12/02 20:03:48  marpas
	qa rules
	
	Revision 2.7  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.6  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.5  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.4  2008/02/21 10:20:08  clanas
	removed unneeded class declarastion for old xerces
	
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

#ifndef _LocalSeekFileInputSource_H_
#define _LocalSeekFileInputSource_H_

#include <acs_c++config.hpp>
#include <xercesc/sax/InputSource.hpp> // PRQA S 1013

#ifndef XERCES_HAS_NAME_SPACE
	#define XERCES_CPP_NAMESPACE
#endif


_ACS_BEGIN_NAMESPACE(acs)



class LocalSeekFileInputSource : public XERCES_CPP_NAMESPACE::InputSource { // PRQA S 2109

public:

	LocalSeekFileInputSource(const XMLCh* const basePath, 
							 const XMLCh* const relativePath);

	explicit LocalSeekFileInputSource(const XMLCh* const filePath);

	LocalSeekFileInputSource(const XMLCh* const filePath,
							 const int pos);

	virtual ~LocalSeekFileInputSource() throw() ;

	virtual XERCES_CPP_NAMESPACE::BinInputStream* makeStream() const;
private:
    LocalSeekFileInputSource() ; // not implemented
    LocalSeekFileInputSource(const LocalSeekFileInputSource &) ; // not implemented
    LocalSeekFileInputSource & operator= (const LocalSeekFileInputSource &) ; // not implemented
private:
	int _pos;
		
};

_ACS_END_NAMESPACE



#endif //_LocalSeekFileInputSource_H_






