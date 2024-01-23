// PRQA S 1050 EOF
/*
 * 	Copyright 1995-2014, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: A.C.S. PackagerOptimiser ItemPartition class - Header File $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 2.5  2014/03/10 15:00:33  francesco.avanzi
 * 	better code practices applied
 * 	
 * 	Revision 2.4  2011/03/03 12:30:08  marpas
 * 	ihash_map changed into map
 * 	
 * 	Revision 2.3  2009/06/12 15:08:14  enrcar
 * 	EC:: modified in order to handle the cartId
 * 	
 * 
 */
   

#ifndef _ItemPartition_H_
#define _ItemPartition_H_

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class ItemPartition // PRQA S 2109
{
public:
    ItemPartition(const std::string &user, const std::string &cartId) ;
    ~ItemPartition() throw() ;

    ItemPartition(const ItemPartition &) ;
    ItemPartition &operator=(const ItemPartition&) ;

    void dumpItemPartition() const throw() ;

    const std::string &getUser() const throw() ;
   	const std::string &getCartId() const throw() ;

    size_t hashValue() const ;

private:             
	ItemPartition() ;	/* Unimplemented */
    
private:
	std::string _user ;
    std::string _cartId ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(ItemPartition) ;

} ;

bool operator==(const ItemPartition &, const ItemPartition &) throw() ;
bool operator<(const ItemPartition &, const ItemPartition &) throw() ;

_ACS_END_NAMESPACE

#endif /* _ItemPartition_H_ */
