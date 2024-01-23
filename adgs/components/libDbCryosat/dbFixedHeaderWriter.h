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
	
	Revision 5.1  2016/04/15 14:37:04  marpas
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
	
	Revision 2.5  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.4  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.3  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.1  2009/04/08 10:08:19  marpas
	system concept introduced in order to have a writer for a system with no satellite
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.5  2004/05/03 14:54:56  paoscu
	New header writers .
	
	Revision 1.4  2003/07/11 20:47:31  paoscu
	exException::clone method implemented,
	order by clause implemented in distribution*query classes and inventoryquery
	test improved and added
	
	Revision 1.3  2003/04/30 13:43:10  paoscu
	using namespace std no longer used into headers
	
	Revision 1.2  2003/03/25 14:40:13  paoscu
	Implemented.
	
	Revision 1.1  2003/03/14 19:02:22  paoscu
	Imported
	
		

*/

#ifndef _dbFixedHeaderWriter_H_
#define _dbFixedHeaderWriter_H_


#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject;
class XMLOstream;

class dbFixedHeaderWriter // PRQA S 2109
{
public:
	dbFixedHeaderWriter(dbInventoryObject const &, std::string const &system, XMLOstream &);
	virtual ~dbFixedHeaderWriter() throw() ;

	// default implementation do nothing
	virtual void start() ;
	virtual void run(bool addDataBlock) ;
	virtual void end() ;
	XMLOstream &getXmlStream() throw() { return _xmlStream ; } // PRQA S 4024
	const dbInventoryObject &getInventoryObj() const throw() { return _inventoryObj ; }
	const std::string &getSystem() const throw() { return _system ; }
private:
	//Not implemented
	dbFixedHeaderWriter();
	dbFixedHeaderWriter(const dbFixedHeaderWriter&);
	dbFixedHeaderWriter &operator=(const dbFixedHeaderWriter&);

private:
	dbInventoryObject const &_inventoryObj;
	std::string _system;
	XMLOstream &_xmlStream ;
};

_ACS_END_NAMESPACE


#endif // _dbFixedHeaderWriter_H_
