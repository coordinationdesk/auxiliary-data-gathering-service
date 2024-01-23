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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:52  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:20  marfav
	dbSMactions now include inventory id pointers
	
	Revision 1.5  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 1.4  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 1.3  2009/05/13 14:45:27  marpas
	rules fixing
	
	Revision 1.2  2009/05/13 09:09:55  marpas
	makeup and some rule fixing
	
	Revision 1.1  2008/12/16 15:56:52  marpas
	just added ... hoping someone test the contents
	

*/

#ifndef _VenusFixedHeaderFactory_H_
#define _VenusFixedHeaderFactory_H_


#include <acs_c++config.hpp>
#include <dbFixedHeaderFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbFixedHeaderWriter ;
class dbInventoryObject;


class VenusFixedHeaderFactory : public dbFixedHeaderFactory {

public:
	VenusFixedHeaderFactory() ;
	virtual ~VenusFixedHeaderFactory() throw() ;

	/**
	 * allocates a dbFixedHeaderWriter ... its destruction is left to the caller.
	 */
	virtual void getWriter(dbInventoryObject const &, std::string const &system, XMLOstream &, dbFixedHeaderWriter *&) const ;
	virtual std::string getFilename(dbInventoryObject const &) const ;
	virtual int getSatelliteId() const ;

private:
	// declared but not implemented
	VenusFixedHeaderFactory(const VenusFixedHeaderFactory & ) ;
	VenusFixedHeaderFactory &operator=(const VenusFixedHeaderFactory & ) ;
} ;


_ACS_END_NAMESPACE


#endif // _VenusFixedHeaderFactory_H_
