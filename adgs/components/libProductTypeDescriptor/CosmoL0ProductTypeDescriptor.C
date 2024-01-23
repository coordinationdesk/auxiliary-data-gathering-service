// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.4  2012/03/12 10:19:18  giucas
	Some compile warnings fixed
	
	Revision 2.3  2007/02/08 19:20:08  danalt
	fixed regression
	
	Revision 2.2  2007/02/02 11:26:56  danalt
	removed VCID check as per SPR ACQ-431
	
	Revision 2.1  2006/04/10 17:12:46  davcas
	Re-implemented method getProductIdByApidandVcid
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.3  2004/09/02 16:24:59  giucas
	Implemented getNameByApidVcidClass and init methods
	
	Revision 1.2  2004/08/06 14:09:52  giucas
	Added getNameByApidVcidClass method, still to be implemented.
	
	Revision 1.1  2004/08/06 11:48:13  giucas
	First release.
	

*/

#include <CosmoL0ProductTypeDescriptor.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

CosmoL0ProductTypeDescriptor::CosmoL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Cosmo")
{
}

CosmoL0ProductTypeDescriptor::CosmoL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
CosmoL0ProductTypeDescriptor::~CosmoL0ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE

