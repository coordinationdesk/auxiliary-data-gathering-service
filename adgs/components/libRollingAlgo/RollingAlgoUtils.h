// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libRollingAlgo$

	$Id$

	$Author$

	$Log$
	Revision 1.5  2017/12/05 16:32:16  nicvac
	S2PDGS-1856: Query optimization.
	
	Revision 1.4  2017/12/04 18:08:16  nicvac
	S2PDGS-1856: Policy select optimization query for performances.
	
	Revision 1.3  2017/11/28 17:03:20  marpas
	qa warning partially fixed
	
	Revision 1.2  2015/12/09 17:07:29  nicvac
	S3MPM-650: Virtual filetype handling.
	
	Revision 1.1  2015/02/26 16:44:07  nicvac
	RollingAlgoUtils.h
	
        
*/

#ifndef _RollingAlgoUtils_H_
#define _RollingAlgoUtils_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>

#include <dbPersistent.h>
#include <dbConnection.h>

#include <set>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Common utils to support Rolling Algo
**/

class RollingAlgoUtils : public Singleton <RollingAlgoUtils> { // PRQA S 2109, 2153
public:

	/** RollingAlgoUtils Exceptions */
	exDECLARE_EXCEPTION(exRollingAlgoUtilsCriticalException, exCriticalException); // RollingAlgoUtils Critical Exception. // PRQA S 2131, 2153, 2502
	virtual ~RollingAlgoUtils() throw() {} // PRQA S 2131

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	RollingAlgoUtils();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	RollingAlgoUtils(const RollingAlgoUtils & );
	/** Operator = */
	RollingAlgoUtils &operator=(const RollingAlgoUtils &);
	friend RollingAlgoUtils* Singleton<RollingAlgoUtils>::instance(); //SINGLETON // PRQA S 2107

public:

	/** Initialization */
	void init(std::string const& connKey);

	/** Type commodities: Query Name, Sql query */
	typedef std::pair<std::string, std::string> Pair_QueryName_SqlQuery;

	/** Get:
	 * List of: (Query Name, Query sql command). Each entry is a sql to get Inventory Items to be preserved.
	 * invIdAttributeName: the Inventory Id's attribute name. You can use this for example to join these in other queries. */
	const std::vector<Pair_QueryName_SqlQuery>& getInventoryItemsToBePreservedQueries( std::string& invIdAttributeName ) const;

	/** Set of all the inventory items ids that need to be safeguarded (i.e. Items that are being migrated or exported) */
	void getInventoryItemsToBePreserved( dbConnection &conn, std::set< dbPersistent::IdType >& invIdsToPreserve ) const;

	/** Sql string to get real filetypes from an input filetype.
	 * Input is a real filetype ==> Return the real filetype itself
	 * Input is a virtual filetype ==> Return the related real filetypes */
	std::string getSqlRealFiletypes(dbConnection &conn, std::string const& filetype) const;

private:

	/** Sql string for all the inventory items ids that need to be safeguarded (i.e. Items that are being migrated or exported) */
	std::string getSqlInventoryItemsToBePreserved(std::string const& connKey) const;

	/** Each query return inventory ids to be preserved */
	std::vector< Pair_QueryName_SqlQuery > _inventoryPreserveQueries;

	/** CONVENTION: Each query in _inventoryPreserveQueries return inventory id with this attribute name. */
	const std::string _invIdAttributeName;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingAlgoUtils)

};

_ACS_END_NAMESPACE

#endif //_RollingAlgoUtils_H_

