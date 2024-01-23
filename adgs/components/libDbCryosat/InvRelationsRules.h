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
	Revision 7.1  2018/02/08 16:50:47  marpas
	adding InvRelationsRules singleton to register rules allowing actions on inv_relations table
	inventory will use those rules when need to perform actions such as update
	to avoid regressions on projects not interested in this, the singleton will allow any action if
	there is no filetype-relation pair registered on it.
	unit test added
	
*/

#ifndef _InvRelationsRules_H_
#define _InvRelationsRules_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <string>
#include <set>

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

using pattern::Singleton ;

class InvRelationsRules : public Singleton <InvRelationsRules> // PRQA S 2109, 2153
{
    friend InvRelationsRules* Singleton<InvRelationsRules>::instance(); // PRQA S 2107
public:
	virtual ~InvRelationsRules() throw() ; 

    /*! returns true if the update is required on t_inv_relations for the specific pair (filetype,relation)
        \note if no pair has been loaded in the singleton the method returns always true 
        this is to have no regressions in old systems
        \note the relation string is always converted to uppercase.
        
    */
	bool update_required(const std::string &filetype, const std::string &relation) const ;

    /*! to allow the update of the t_inv_relations when a specific filetype and a relation pair occurs,
        just call this method with the pair.
        to allow the update for A_B_C filetype when the relation is CHILD_TO_PARENT call 
        \code
        add_update_rule("A_B_C", "CHILD_TO_PARENT") ;
        \endcode
        \note the relation string is always converted to uppercase.
        
    */
    void add_update_rule(const std::string &filetype, const std::string &relation) ;
    const std::set<std::pair<std::string, std::string> > &get_update_rules() const throw() { return _req_update ; }
    void reset_update_rules() { _req_update.clear() ; }
    
protected:
	InvRelationsRules() ; 
	
private:
	InvRelationsRules &operator=(const InvRelationsRules &) ; 	// declared but not implemented
	InvRelationsRules (const InvRelationsRules &) ; 				// declared but not implemented
    
private:

    std::set<std::pair<std::string, std::string> > _req_update ;
} ; 

/*! \fn configureInvRelationsRules 
    this function reads the configuration from the configuration singleton 
    it is expected to be in the form:
    \code
        ...
        <rootNode>
           <InvRelationsRules>
               <Update>
                   <Required>
                       <Filetype>ftype</Filetype>
                       <Relation>CHILD_TO_PARENT</Relation>
                   </Required>
                   <Required>
                       <Filetype>ftype2</Filetype>
                       <Relation>RELATION</Relation>
                   </Required>
               </Update>
           </InvRelationsRules>
        </rootNode>
        ...
    \endcode 
*/
void configureInvRelationsRules(const std::string &rootNode) throw() ;

_ACS_END_NESTED_NAMESPACE

#endif // _InvRelationsRules_H_
