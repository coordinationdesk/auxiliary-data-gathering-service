// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 5.3  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.2  2013/08/30 15:18:33  marfav
	Interface adopts dbPersistent::IdType for inventory id
	
	Revision 5.1  2013/08/29 13:56:07  marfav
	Inventory coverage rule checker stored procedure wrapper added
	
*/

#ifndef _InventoryCovRuleChecker_H_
#define _InventoryCovRuleChecker_H_ 

#include <exException.h>
#include <dbPersistent.h>

namespace acs
{

class dbConnection;

class InventoryCovRuleChecker // PRQA S 2109
{
public:

    exDECLARE_EXCEPTION(InventoryCoverageCheckException, exException) ; // PRQA S 2131, 2502

    struct StoredProcedureResult // PRQA S 2173, 2175
    {
        int w_exp_res; // PRQA S 2100 8
        int w_cov_res; 
        int w_ont_res; 
        int w_gap_res;
        bool w_exp_res_set;
        bool w_cov_res_set; 
        bool w_ont_res_set; 
        bool w_gap_res_set;

        // structure ctor
        StoredProcedureResult () :
            w_exp_res(0),
            w_cov_res(0),
            w_ont_res(0), 
            w_gap_res(0),
            w_exp_res_set(false),
            w_cov_res_set(false),
            w_ont_res_set(false),
            w_gap_res_set(false)
        {}
    };

    struct CoverageResult // PRQA S 2173, 2175
    {
        bool warningDelay; // PRQA S 2100 2
        bool warningCoverage;

        // structure ctor
        CoverageResult() :
            warningDelay (false),
            warningCoverage (false)
        {}
    };

    // Just a bunch of static functions
    static CoverageResult checkRules (dbConnection &, dbPersistent::IdType&);

private:
    // Not implemented
    InventoryCovRuleChecker();
    ~InventoryCovRuleChecker();
    InventoryCovRuleChecker (const InventoryCovRuleChecker&);
    InventoryCovRuleChecker& operator= (const InventoryCovRuleChecker&);

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL (InventoryCovRuleChecker)
};

};

#endif //InventoryCovRuleChecker
