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
	Revision 5.1  2013/07/19 09:01:20  marpas
	doc fixed
	qa warnings fixed
	compilation warnings fixed
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.3  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.2  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.1  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/06 14:09:22  marpas
	header fixed
	
	Revision 1.2  2004/02/20 16:20:07  marfav
	Fixed MemBufInputSource usage
	
	Revision 1.1  2003/11/25 16:30:39  marfav
	Import XMLIstringstream
	


*/

#include <XMLIstringstream.h>

#include <xercesc/framework/MemBufInputSource.hpp> // PRQA S 1013 2
#include <xercesc/util/PlatformUtils.hpp>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

#ifdef XERCES_HAS_NAME_SPACE
using namespace XERCES_CPP_NAMESPACE;
#endif


XMLIstringstream::XMLIstringstream (const std::string& inputString) :
	XMLIstream(""),
	_inputString(inputString)
{
}

XMLIstringstream::~XMLIstringstream() throw() {}

InputSource*
XMLIstringstream::getInputSource()
{
	static const char* gMemBufId = "bufIdInfo";
	MemBufInputSource* source= new MemBufInputSource (reinterpret_cast<const XMLByte*>(_inputString.c_str()), strlen (_inputString.c_str()), gMemBufId, false); // PRQA S 3030, 3081
	return source;
}

void
XMLIstringstream::doInputCheck()
{
	// Nothing to do... a string will always be a good input
}

void XMLIstringstream::setDoExternalSchemaValidation(bool v)
{
	ACS_COND_THROW(v,exIllegalValueException("XMLIstringstream does not allow external validation")) ; // PRQA S 3081
}

_ACS_END_NAMESPACE

