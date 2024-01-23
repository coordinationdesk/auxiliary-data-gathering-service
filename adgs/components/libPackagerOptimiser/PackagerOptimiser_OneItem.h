// PRQA S 1050 EOF
/*
 * 
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
 * 	$Prod: A.C.S. PackagerOptimiser_OneItem Class HEADER FILE $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 2.1  2009/03/24 14:29:43  enrcar
 * 	EC:: c++rules
 * 	
 *     
 */


#ifndef _PackagerOptimiser_OneItem_H_
#define _PackagerOptimiser_OneItem_H_

#include <PackagerOptimiser.h>


_ACS_BEGIN_NAMESPACE(acs)



class PackagerOptimiser_OneItem : public PackagerOptimiser // PRQA S 2153
{

public:	
  
  
    PackagerOptimiser_OneItem(PACKAGEROPT_ARGS);
	virtual ~PackagerOptimiser_OneItem() throw() ;
  
 	PackagerOptimiser_OneItem(const PackagerOptimiser_OneItem & right);
	PackagerOptimiser_OneItem &operator=(const PackagerOptimiser_OneItem &);


protected:
    virtual void doOptimise() ;


private:
	PackagerOptimiser_OneItem();	/* Unimplemented */

} ;


_ACS_END_NAMESPACE

#endif  /* _PackagerOptimiser_OneItem_H_ */
