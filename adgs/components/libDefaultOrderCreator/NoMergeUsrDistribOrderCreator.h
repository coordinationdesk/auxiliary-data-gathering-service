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

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.5  2012/10/26 10:40:24  marpas
	adding triggering user distribution to order parameters clob
	
	Revision 2.4  2012/10/25 13:29:07  marpas
	processing of invalid files allowed if configured
	
	Revision 2.3  2012/10/24 17:22:16  marpas
	new configuration concept
	
	Revision 2.2  2012/10/23 09:40:21  marpas
	algo fixed
	
	Revision 2.1  2012/09/18 09:53:27  marpas
	NoMergeUsrDistribOrderCreator class added to the repository
	
	
*/

#ifndef _NoMergeUsrDistribOrderCreator_H_
#define _NoMergeUsrDistribOrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <exException.h>
#include <map>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject ;

class dbConnection ;
class TemporaryOrder ;

class NoMergeUsrDistribOrderCreator: public OrderCreator // PRQA S 2109
{		
public:
	NoMergeUsrDistribOrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection &);
	virtual ~NoMergeUsrDistribOrderCreator() throw() ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;
    void loadConf() const ;

protected:
	virtual void createOrder(const TemporaryOrder &, const std::string &userdistrib, dbConnection &) const;
    virtual std::string getAlgoKeyString() const ; // inherited from OrderCreator

private:
	// declared but not implemented 
	NoMergeUsrDistribOrderCreator();
	NoMergeUsrDistribOrderCreator(const NoMergeUsrDistribOrderCreator&);
	NoMergeUsrDistribOrderCreator & operator=(const NoMergeUsrDistribOrderCreator&);

    std::string checkIfTriggeringDistributed(const TemporaryOrder &, dbConnection &) const ;
#ifdef TEST_ORDER_CREATOR
public:
#else
private:
#endif
    bool checkFtypeAgainstConf(const std::string &ftype, const std::string &user, dbConnection &) const ;
    void appendTmpOrder(double, 
                        const TemporaryOrder &, 
                        std::vector<std::pair<TemporaryOrder, std::string> > &, 
                        dbConnection &) const ;
private:
    mutable std::map<std::string, std::vector<std::string> > _users_ftype ;
    mutable bool processInvalidFiles_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NoMergeUsrDistribOrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _NoMergeUsrDistribOrderCreator_H_ */
