// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2014/02/03 13:17:12  giucas
	New macros adopted
	
	Revision 2.1  2006/10/18 16:39:38  marpas
	new interfaces to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/11/17 18:52:27  giucas
	Added EnvisatOBTConverter
	
	Revision 1.4  2005/11/02 17:53:43  giucas
	Modified to use Singleton
	
	Revision 1.3  2005/09/12 14:57:32  seralb
	Added Spot satellite
	
	Revision 1.2  2005/07/06 10:00:15  seralb
	Added Ers classes
	
	Revision 1.1.1.1  2004/12/07 10:06:15  giucas
	Import libOBTConverter
	
	Revision 1.4  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.3  2004/10/08 09:02:33  giucas
	Added EnvisatOBTFileParser
	
	Revision 1.2  2004/09/17 12:37:57  giucas
	Changed interfaces and added builder methods for OBTFileParser objects
	
	Revision 1.1  2004/09/16 16:57:32  giucas
	First release
	



*/

#ifndef _OBTConverterFactory_H_
#define _OBTConverterFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <OBTConverter.h>

#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class OBTConverterFactory // PRQA S 2109
{
public:

	/**
	  * \brief Factory class constructor registers instances of OBTConverter objects
	  */
	OBTConverterFactory();

	/**
	  * \brief Factory class destructor unregisters instances of OBTConverter objects
	  */				
	~OBTConverterFactory();

	/**
	  * \brief Builder method for GoceOBTConverter object retrieving
	  */								
	static OBTConverter* buildGoceOBTConverter();

	/**
	  * \brief Builder method for ErsOBTConverter object retrieving
	  */								
	static OBTConverter* buildErsOBTConverter();

	/**
	  * \brief Builder method for SpotOBTConverter object retrieving
	  */								
	static OBTConverter* buildEnvisatOBTConverter();
private:
	OBTConverterFactory(const OBTConverterFactory &); // not implemented
	OBTConverterFactory& operator= (const OBTConverterFactory &); // not implemented

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OBTConverterFactory) ;

};

_ACS_END_NAMESPACE

#endif // _OBTConverterFactory_H_

