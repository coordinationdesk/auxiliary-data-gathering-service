/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Ocada JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2009/03/02 14:40:14  chicas
	Just imported
	

*/

#include <OcadaJobOrderFactory.h>
#include <JobOrderFactoryChain.h>
#include <OcadaJobOrder.h>

#include <exStream.h>

#include <ocdjoV.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

static ocdjoV version;

static const char* _ocadaJobOrderConfigKey       = "TlDaemon.PluginsRegistrationSettings.OcadaJobOrderFactory";



// **********************************************************************
// ***************** OCADA JOB ORDER FACTORY **************************
// **********************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(OcadaJobOrderFactory)

OcadaJobOrderFactory::OcadaJobOrderFactory () :
		StringKeyFactory <JobOrder> (_ocadaJobOrderConfigKey, &OcadaJobOrderFactory::buildOcadaJobOrder)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}

JobOrder*
OcadaJobOrderFactory::buildOcadaJobOrder ()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	return new OcadaJobOrder;
}

// **********************************************************************
// ***************** OCADA JOB ORDER FACTORY **************************
// **********************************************************************



	OcadaJobOrderFactory EmbeddedOcadaJobOrderFactory;

// *************************************************************
// ***************** EMBEDDED FACTORY **************************
// *************************************************************
_ACS_END_NAMESPACE
