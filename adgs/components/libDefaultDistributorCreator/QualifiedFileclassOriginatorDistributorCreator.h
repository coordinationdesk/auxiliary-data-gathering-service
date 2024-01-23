// PRQA S 1059 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDefaultDistributorCreator$

	$Id$

	$Author$

	$Log$
	Revision 2.4  2017/11/27 17:52:35  marpas
	qa warnings fixed
	
	Revision 2.3  2017/11/24 16:07:28  nicvac
	ECICMF-97: Component Test and Fix loop.
	
	Revision 2.2  2017/11/23 20:14:33  nicvac
	ECICMF-97: Implementation completed
	
	Revision 2.1  2017/11/22 13:52:30  nicvac
	ECICMF-97: QualifiedFileclassOriginatorDistributorCreator added.
	
        
*/

#ifndef _QualifiedFileclassOriginatorDistributorCreator_H_
#define _QualifiedFileclassOriginatorDistributorCreator_H_

#include <DistributorCreator.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* cf. http://jira.acsys.it/browse/ECICMF-97
*
* Create a distribution if:
* algorithm QUALIFIEDFILECLASSORIGINATOR case:
*  - the originator (processor name and version) is in the configured list, given a matched configured filetype and fileclass.
*  Cf. configuration settings for details and example.
**/

static const char QUALIFIEDFILECLASSORIGINATOR[] = "QUALIFIEDFILECLASSORIGINATOR"; 

class dbConnectionPool ;

class QualifiedFileclassOriginatorDistributorCreator: public DistributorCreator { // PRQA S 2109
public:

	/** Class constructor */
	explicit QualifiedFileclassOriginatorDistributorCreator( const std::string& algo );
	/** Destructor */
	virtual ~QualifiedFileclassOriginatorDistributorCreator() throw();

	/** Interface implementation */
	virtual void createDistribution(dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) const ;

private:
	//Do not allow copy
	QualifiedFileclassOriginatorDistributorCreator(const QualifiedFileclassOriginatorDistributorCreator & );
	QualifiedFileclassOriginatorDistributorCreator &operator=(const QualifiedFileclassOriginatorDistributorCreator &);
	//Do not allow default constuctor
	QualifiedFileclassOriginatorDistributorCreator();

private:
	/** Configured Rules */
	typedef std::pair< std::string, std::string > Proc_Name_Vers;
	struct RuleContent { // PRQA S 2173, 2175
		RuleContent(): procs(), actionIsDistribute(true) {}
	    ~RuleContent() throw() {}
		RuleContent(const RuleContent &r): procs(r.procs), actionIsDistribute(r.actionIsDistribute) {}
		RuleContent& operator =(const RuleContent &r) {
            if (this != &r) {
                procs = r.procs ;
            } 
            return *this ;   
        }

		std::vector< Proc_Name_Vers > procs; // PRQA S 2100 2
		bool actionIsDistribute;
	};

	/** Set of rules: (Filetype, Fileclass) --> (ProcA, V1), (ProcB, V1), (ProcB, V2) */
	typedef std::pair<std::string, std::string> KeyRule_FT_FC;
	typedef std::map< KeyRule_FT_FC, RuleContent > MapRules;
	MapRules _rules;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifiedFileclassOriginatorDistributorCreator)

};

_ACS_END_NAMESPACE

#endif //_QualifiedFileclassOriginatorDistributorCreator_H_

