// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 7.2  2018/02/09 10:47:39  marpas
	configureInvRelationsRules function added and tested
	InvRelationsRules::reset_update_rules method added and tested
	
	Revision 7.1  2018/02/08 16:50:47  marpas
	adding InvRelationsRules singleton to register rules allowing actions on inv_relations table
	inventory will use those rules when need to perform actions such as update
	to avoid regressions on projects not interested in this, the singleton will allow any action if
	there is no filetype-relation pair registered on it.
	unit test added
	
*/


#include <InvRelationsRules.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>

#include <StringUtils.h>
#include <Filterables.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

using namespace std;

InvRelationsRules::InvRelationsRules() : 
    Singleton <InvRelationsRules>(),
    _req_update()
{
}

InvRelationsRules::~InvRelationsRules() throw() {}


bool InvRelationsRules::update_required(const std::string &filetype, const std::string &relation) const // PRQA S 4020
{
    if (_req_update.empty()) {
        return true ; // no regression in projects that do not use the feature
    }  
    return _req_update.find(pair<string, string>(filetype, StringUtils::uppercase(relation))) != _req_update.end() ;
}

void InvRelationsRules::add_update_rule(const std::string &filetype, const std::string &relation) 
{
    _req_update.insert(pair<string, string>(filetype, StringUtils::uppercase(relation))) ;
}

_ACS_END_NAMESPACE // db - now in acs

struct InvRelationRule { // PRQA S 2173, 2175
    InvRelationRule() : filetype(), relation() {}
    InvRelationRule(const InvRelationRule &r) : filetype(r.filetype), relation(r.relation) {}
    InvRelationRule & operator =(const InvRelationRule &r)
    {
        if (this != &r) {
            filetype = r.filetype; 
            relation = r.relation;
        }
        return *this ;
    }
    ~InvRelationRule() throw() {}
    std::string filetype ; // PRQA S 2100 2
    std::string relation ;
} ;

template<>
inline
void rsResourceSet::getValue(const std::string &res, InvRelationRule &v) const // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
 	getValue(res+".Filetype")->get(v.filetype) ;
 	getValue(res+".Relation")->get(v.relation) ;
} 
_ACS_BEGIN_NAMESPACE(db) // again in acs::db

namespace {
    void add_update_rule_(const InvRelationRule &r) {
        InvRelationsRules::instance()->add_update_rule(r.filetype, r.relation) ;
    }
}

void configureInvRelationsRules(const std::string &rootNode) throw() 
{
    try {
        vector<InvRelationRule> updateRequired ;
        string base = rootNode + ".InvRelationsRules" ;
        ConfigurationSingleton::instance()->get().getArray(base + ".Update.Required", updateRequired) ;
        for_each(updateRequired.begin(), updateRequired.end(), add_update_rule_) ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        ACS_LOG_ERROR("Cannot configure InvRelationsRules - every relation will cause action on DB") ;
    }
}

_ACS_END_NESTED_NAMESPACE

