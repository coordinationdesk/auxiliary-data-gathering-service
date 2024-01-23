// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libExporterBase$

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/06/20 12:32:48  marpas
	qa rules
	coding best practices applied
	instrumented to avoid qac++ analisys crash
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.2  2013/03/15 16:23:33  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.1  2013/03/15 10:58:58  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
        
*/

#ifndef _ExporterCommon_H_
#define _ExporterCommon_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <DataPackage.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Common definition and general utilities 
**/

/** Shared pointer Types definitions */
typedef boost::shared_ptr<dataprovider::DataPackage> DataPackageSptr;
typedef boost::shared_ptr<const dataprovider::DataPackage> DataPackageConstSptr;


_ACS_END_NAMESPACE

#endif //_ExporterCommon_H_

