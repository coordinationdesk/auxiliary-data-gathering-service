// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:51  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.1  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.2  2004/05/05 18:23:38  paoscu
	Work in progress.
	
	Revision 1.1  2004/05/03 14:53:59  paoscu
	New haeader writers.
	
		

*/

#include <NoneFixedHeaderFactory.h>
#include <dbFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>

using namespace acs;
using namespace std;


NoneFixedHeaderFactory::NoneFixedHeaderFactory() : dbFixedHeaderFactory("NONE") 
{
}

NoneFixedHeaderFactory::~NoneFixedHeaderFactory() throw()
{
}


void NoneFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	fhw = new dbFixedHeaderWriter(inv, system, os) ;
}




string NoneFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	return "" ;
}



int NoneFixedHeaderFactory::getSatelliteId() const 
{
	return 0 ;
}


