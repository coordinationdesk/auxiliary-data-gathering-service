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
	
	Revision 4.0  2013/09/30 12:30:50  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:18  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.2  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2009/05/13 14:45:26  marpas
	rules fixing
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/05/05 18:23:38  paoscu
	Work in progress.
	
	Revision 1.1  2004/05/03 14:53:59  paoscu
	New haeader writers.
	

*/

#ifndef _CryosatFixedHeaderFactory_H_
#define _CryosatFixedHeaderFactory_H_


#include <acs_c++config.hpp>
#include <dbFixedHeaderFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbFixedHeaderWriter ;
class dbInventoryObject;


class CryosatFixedHeaderFactory : public dbFixedHeaderFactory {

public:
	CryosatFixedHeaderFactory() ;
	virtual ~CryosatFixedHeaderFactory()  throw() ;

	/**
	 * allocates a dbFixedHeaderWriter ... its destruction is left to the caller.
	 */
	virtual void getWriter(dbInventoryObject const &, std::string const &system, XMLOstream &, dbFixedHeaderWriter *&) const ;
	virtual std::string getFilename(dbInventoryObject const &) const ;
	virtual int getSatelliteId() const ;

private:
	// declared but not implemented
	CryosatFixedHeaderFactory(const CryosatFixedHeaderFactory & ) ;
	CryosatFixedHeaderFactory &operator=(const CryosatFixedHeaderFactory & ) ;
} ;


_ACS_END_NAMESPACE



#endif // _CryosatFixedHeaderFactory_H_
