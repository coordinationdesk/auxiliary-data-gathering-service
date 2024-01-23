// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	
	Revision 5.2  2013/08/30 15:18:31  marfav
	Interface adopts dbPersistent::IdType for inventory id
	
	Revision 5.1  2013/08/29 13:56:07  marfav
	Inventory coverage rule checker stored procedure wrapper added
	
*/

#include <InventoryCovRuleChecker.h>
#include <dbConnection.h>
#include <SQLString.h>
#include <dbQuery.h>
#include <dbConv.h>
#include <dbSet.h>

#include <string>


using namespace acs;
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InventoryCovRuleChecker)

InventoryCovRuleChecker::CoverageResult InventoryCovRuleChecker::checkRules (dbConnection & conn, dbPersistent::IdType& invId)
{
    CoverageResult result;
    try
    {
        string connKey (conn.getKey());
        dbSet set ;

        SQLString checkSqlString (connKey, SQLString::CUSTPROCEDURE ) ;
        checkSqlString.setProcedureName("f_cov_check_file");
        checkSqlString.addProcedureParameter (db::toSQL(connKey, invId));

        dbQuery theQuery (conn, checkSqlString, "f_cov_check_file_Call") ;
        theQuery.execForSet(set) ;

        // Two elements are expected from the result
        // - WarningDelay (true if has to be emitted)
        // - WarningCoverage (true if has to be emitted)

        ACS_COND_THROW(1 != set.size(), InventoryCoverageCheckException ("f_cov_check_file returned an invalid set size" )); // PRQA S 3081

        // Extracting the elements here

        dbRow row(connKey);
        set.getNextRow(row);
        StoredProcedureResult spr;
        row.columnSet (0, spr.w_exp_res, spr.w_exp_res_set);
        row.columnSet (1, spr.w_cov_res, spr.w_cov_res_set);
        row.columnSet (2, spr.w_ont_res, spr.w_ont_res_set); // PRQA S 4400 2
        row.columnSet (3, spr.w_gap_res, spr.w_gap_res_set);

        // write the debug messages dumping the query result
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "W_EXP_RES [" << spr.w_exp_res << "]" << (spr.w_exp_res_set ? "" : " - NULL" )); // PRQA S 3380 4
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "W_COV_RES [" << spr.w_cov_res << "]" << (spr.w_cov_res_set ? "" : " - NULL" ));
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "W_ONT_RES [" << spr.w_ont_res << "]" << (spr.w_ont_res_set ? "" : " - NULL" ));
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "W_GAP_RES [" << spr.w_gap_res << "]" << (spr.w_gap_res_set ? "" : " - NULL" ));

        // In case of all NULL the record does not exists or has no rules
        bool exists = (spr.w_exp_res_set || spr.w_cov_res_set || spr.w_ont_res_set || spr.w_gap_res_set);

        // Do not throw exception. Log the event and do not generate any kind of warning.
        if (!exists)
        {
            ostringstream os;
            os << "The inventory id [" << invId << "] does not exists or does not have any active coverage rule";
            ACS_LOG_WARNING (os.str());
        }
        else
        {

            // Valid results has EXP and COV set or ONT and GAP set. Mixed results are not allowed.
            bool valid = 
                (spr.w_exp_res_set && spr.w_cov_res_set && !spr.w_ont_res_set && !spr.w_gap_res_set) || 
                (!spr.w_exp_res_set && !spr.w_cov_res_set && spr.w_ont_res_set && spr.w_gap_res_set);

            // In case of no valid status throw an exception
            if (!valid)
            {
                ostringstream os;
                os << "The coverage check for inventory id [" << invId << "] returned a not valid status [";
                if (spr.w_exp_res_set)
                {
                    os << spr.w_exp_res << ", ";
                }
                else
                {
                    os << "NULL, ";
                }

                if (spr.w_cov_res_set)
                {
                    os << spr.w_cov_res << ", ";
                }
                else
                {
                    os << "NULL, ";
                }

                if (spr.w_ont_res_set)
                {
                    os << spr.w_ont_res << ", ";
                }
                else
                {
                    os << "NULL, ";
                }

                if (spr.w_gap_res_set)
                {
                    os << spr.w_gap_res << "]";
                }
                else
                {
                    os << "NULL]";
                }
        
                ACS_THROW(InventoryCoverageCheckException (os.str())); // PRQA S 3081
            }
            

            // A rules and B rules must be detected here
            if (spr.w_exp_res_set)
            {
                // A Rule
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"A\" rule detected for inventory id [" << invId << "]" );

                // The warnings are independent from each other
                if (0 == spr.w_exp_res)
                {
                    // Emit a WARNING_DELAY
                    result.warningDelay = true;
                }

                if (0 == spr.w_cov_res)
                {
                    // Emit a WARNING_COVERAGE
                    result.warningCoverage = true;
                }          
            }
            else
            {
                // B Rule
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"B\" rule detected for inventory id [" << invId << "]" );

                // The warnings are independent from each other
                if (0 == spr.w_ont_res)
                {
                    // Emit a WARNING_DELAY
                    result.warningDelay = true;
                }

                if (0 == spr.w_gap_res)
                {
                    // Emit a WARNING_COVERAGE
                    result.warningCoverage = true;
                }

            }
        }       
    }
    catch (exception& ex)
    {
        // wrap it and rethrow
        ACS_THROW(InventoryCoverageCheckException (ex, "Unable to execute the stored procedure f_cov_check_file on the db" ));
    }    

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, 
        "Returning detected warnings (Delay,Coverage) set to (" << boolalpha << result.warningDelay << 
        ", " << result.warningCoverage << ')');

    return result;
}


