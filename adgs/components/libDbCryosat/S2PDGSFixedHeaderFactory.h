// PRQA S 1050 EOF
/*

	Copyright 2012-2013, Advanced Computer Systems , Inc.
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
	
	Revision 2.1  2013/03/20 16:37:05  chicas
	added S2PDGSFixedHeaderFactory
	

*/

#ifndef _S2PDGSFixedHeaderFactory_H_
#define _S2PDGSFixedHeaderFactory_H_


#include <acs_c++config.hpp>
#include <dbFixedHeaderFactoryChain.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbFixedHeaderWriter ;
class dbInventoryObject;


class S2PDGSFixedHeaderFactory : public dbFixedHeaderFactory { // PRQA S 2109
public:
	S2PDGSFixedHeaderFactory() ;
	virtual ~S2PDGSFixedHeaderFactory() throw() ;

	/**
	 * allocates a dbFixedHeaderWriter ... its destruction is left to the caller.
	 */
	virtual void getWriter(dbInventoryObject const &, std::string const &system, XMLOstream &, dbFixedHeaderWriter *&) const ;
	virtual std::string getFilename(dbInventoryObject const &) const ;
	virtual int getSatelliteId() const ;
    static void checkConfiguration(void *) ;
    
private:
    static std::string centreid_ ;
	// declared but not implemented
	S2PDGSFixedHeaderFactory(const S2PDGSFixedHeaderFactory & ) ;
	S2PDGSFixedHeaderFactory &operator=(const S2PDGSFixedHeaderFactory & ) ;
} ;


_ACS_END_NAMESPACE


#endif // _S1PDGSFixedHeaderFactory_H_
