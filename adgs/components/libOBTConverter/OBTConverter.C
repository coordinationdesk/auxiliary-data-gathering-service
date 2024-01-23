/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.0  2006/02/28 09:36:19  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2004/12/07 10:06:15  giucas
	Import libOBTConverter
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/10/12 12:53:05  giucas
	Added reference to OBFFileParser and not to file name
	
	Revision 1.1  2004/09/16 16:57:32  giucas
	First release
	


*/

#include <OBTConverter.h>
#include <OBTFileParser.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OBTConverter)

//Class constructor
OBTConverter::OBTConverter()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

//Class constructor
OBTConverter::OBTConverter(OBTFileParser* p)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

//Class destructor
OBTConverter::~OBTConverter() throw()
{
}


_ACS_END_NAMESPACE
