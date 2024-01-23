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

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2015/05/25 13:39:35  marpas
	fixing id type
	
	Revision 5.2  2015/02/26 16:47:56  marfav
	APF-256 added support to Basline name id associator
	
	Revision 5.1  2015/02/17 14:28:16  marfav
	Added new template for NameIdAssociator singletons
	

*/

#ifndef _NameIdAssociators_H_
#define _NameIdAssociators_H_ 

#include <NameIdAssociator.h>


// Unnamed namespace to hide constants 
namespace {
extern const char operationTypesTable[] = "t_operationtypes";
extern const char operationTypesId[] = "id_operationtype";
extern const char operationTypesName[] = "type";

extern const char baselineTable[] = "t_baselines";
extern const char baselineId[] = "id_baseline";
extern const char baselineName[] = "baseline";

};


_ACS_BEGIN_NAMESPACE(acs)

//
//
// class NameIdAssociators
//
// This is a container of NameIdAssociatorSingleton derived classes
//
//


typedef NameIdAssociatorSingleton<operationTypesTable, operationTypesId, operationTypesName, unsigned int> OperationTypes;

typedef NameIdAssociatorSingleton<baselineTable, baselineId, baselineName, long long> Baselines;


_ACS_END_NAMESPACE



#endif // _NameIdAssociators_H_
