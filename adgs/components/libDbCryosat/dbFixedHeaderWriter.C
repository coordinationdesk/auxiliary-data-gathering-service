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
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.1  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:23  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.1  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.7  2004/05/03 14:54:56  paoscu
	New header writers .
	
	Revision 1.6  2004/01/05 16:47:31  paoscu
	Creation_Date field added.
	
	Revision 1.5  2003/07/21 17:17:26  paoscu
	Syntax error fixed.
	
	Revision 1.4  2003/07/21 17:16:20  paoscu
	Datetime exported in format CCSDS compact.
	
	Revision 1.3  2003/07/11 20:47:31  paoscu
	exException::clone method implemented,
	order by clause implemented in distribution*query classes and inventoryquery
	test improved and added
	
	Revision 1.2  2003/03/25 14:40:13  paoscu
	Implemented.
	
	Revision 1.1  2003/03/14 19:02:22  paoscu
	Imported
	
		

*/

#include<dbFixedHeaderWriter.h>

using namespace std;
using namespace acs;




dbFixedHeaderWriter::dbFixedHeaderWriter(dbInventoryObject const & inv, string const &system, XMLOstream &s)
: _inventoryObj(inv), _system(system), _xmlStream(s) // PRQA S 2528
{}

dbFixedHeaderWriter::~dbFixedHeaderWriter() throw()
{}



void dbFixedHeaderWriter::start()
{
}

void dbFixedHeaderWriter::run(bool)
{
}

void dbFixedHeaderWriter::end()
{
}




