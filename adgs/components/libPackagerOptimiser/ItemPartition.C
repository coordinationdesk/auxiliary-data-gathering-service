// PRQA S 1050 EOF
// 
// 	Copyright 1995-2014, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
// 	$Prod: A.C.S. PackagerOptimiser ItemPartition class $
// 
// 	$Id$
// 
// 	$Author$
// 
// 	$Log$
// 	Revision 2.5  2014/03/10 15:00:33  francesco.avanzi
// 	better code practices applied
// 	
// 	Revision 2.4  2013/02/07 15:06:12  enrcar
// 	EC:: Compilation warnings removed
// 	
// 	Revision 2.3  2011/03/03 12:30:08  marpas
// 	ihash_map changed into map
// 	
// 	Revision 2.2  2009/07/13 13:10:06  enrcar
// 	EC:: c++rules
// 	
// 	Revision 2.1  2009/06/12 15:08:14  enrcar
// 	EC:: modified in order to handle the cartId
// 	
// 


#include <ItemPartition.h>
#include <StringUtils.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using namespace __gnu_cxx;
   

ACS_CLASS_DEFINE_DEBUG_LEVEL(ItemPartition);


//------------------------------------------------------------------------
//                                 ItemPartition
//------------------------------------------------------------------------
ItemPartition::ItemPartition(const string &user, const string &cartId) :
	_user (user),
	_cartId (cartId)
{	
   
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method Called. User: !" << user << "!    CartId: ! " << cartId << '!' );
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) ;
        dumpItemPartition() ;
    ACS_CLASS_END_DEBUG;
}


//------------------------------------------------------------------------
//                                 ~ItemPartition
//------------------------------------------------------------------------
ItemPartition::~ItemPartition() throw() 
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB); // PRQA S 4631
}


//------------------------------------------------------------------------
//                                 getUser
//------------------------------------------------------------------------
const string &ItemPartition::getUser() const throw()
{ 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "!" << _user << "!" );
    
    return _user ;
}


//------------------------------------------------------------------------
//                                 getCartId
//------------------------------------------------------------------------
const string &ItemPartition::getCartId() const throw()
{ 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "!" << _cartId << "!" );
    
    return _cartId ;
}


//------------------------------------------------------------------------
//                               dumpItemPartition 
//------------------------------------------------------------------------
void ItemPartition::dumpItemPartition() const throw()
{
    ACS_LOG_INFO("user: !" << _user << "! cartId: !" << _cartId << "!" );
}


//------------------------------------------------------------------------
//                               hashValue()   
//------------------------------------------------------------------------
size_t ItemPartition::hashValue() const 
{
    string catString = _cartId + _user ;	/* Build Hash string */
   
    size_t hashVal =  StringUtils::hash(catString) ;	/* Evaluate hash */ // PRQA S 3000
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input: !" << catString << "! Hash value: 0x" << hex << hashVal << dec );
      
    return hashVal ;
}


//------------------------------------------------------------------------
//                                ::Assignment
//------------------------------------------------------------------------
ItemPartition::ItemPartition(const ItemPartition &e) :
	_user(e._user),
	_cartId(e._cartId)
{ }


//------------------------------------------------------------------------
//                                 ::Copy
//------------------------------------------------------------------------
ItemPartition& ItemPartition::operator=(const ItemPartition &e)
{
    if (this != &e)
    {
		_user = e._user ;
		_cartId = e._cartId ;
    }

    return *this;
}


bool operator==(const ItemPartition &a, const ItemPartition &b) throw()
{
    return ( (a.getUser() == b.getUser()) && (a.getCartId() == b.getCartId()) );
}



bool operator<(const ItemPartition &a, const ItemPartition &b) throw()
{
    return ( (a.getUser() < b.getUser()) || ((a.getUser() == b.getUser())  && (a.getCartId() < b.getCartId()) ) );
}


_ACS_END_NAMESPACE
