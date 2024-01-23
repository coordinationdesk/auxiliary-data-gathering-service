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
	
	Revision 1.2  2004/12/06 14:09:22  marpas
	header fixed
	
	Revision 1.1  2003/11/25 16:30:39  marfav
	Import XMLIstringstream
	


*/


#ifndef _XMLIstringstream_H_
#define _XMLIstringstream_H_

#include <acs_c++config.hpp>
#include <XMLIstream.h>

_ACS_BEGIN_NAMESPACE(acs)

class XMLIstringstream : public XMLIstream // PRQA S 2109, 2153
{
public:
	explicit XMLIstringstream (const std::string&);
	virtual ~XMLIstringstream () throw() ;

	virtual void setDoExternalSchemaValidation(bool) ;

protected:
	virtual XERCES_CPP_NAMESPACE::InputSource* getInputSource();

	virtual void doInputCheck();

private:
	XMLIstringstream();
	// copy contructor
	XMLIstringstream(const XMLIstringstream &);
	// operator =
	XMLIstringstream &operator=(const XMLIstringstream &);

// attributes
private:
	std::string _inputString;
};

_ACS_END_NAMESPACE


#endif //_XMLIstringstream_H_


