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
 * 	$Prod: A.C.S. PackagerOptimiser_OneItemPlusRpt Class HEADER FILE $
 * 
 * 	$Id$
 * 
 * 	$Author$
 *     
 * 	$Log$
 * 	Revision 1.2  2009/03/24 14:29:43  enrcar
 * 	EC:: c++rules
 * 	
 * 	Revision 1.1  2008/01/31 09:49:53  enrcar
 * 	EC:: Imported source
 * 	
 *     
 */


#ifndef _PackagerOptimiser_OneItemPlusRpt_H_
#define _PackagerOptimiser_OneItemPlusRpt_H_

#include <PackagerOptimiser.h>


_ACS_BEGIN_NAMESPACE(acs)


class PackagerOptimiser_OneItemPlusRpt : public PackagerOptimiser
{

public:	
  
	PackagerOptimiser_OneItemPlusRpt(PACKAGEROPT_ARGS);
	virtual ~PackagerOptimiser_OneItemPlusRpt() throw() ;
  
	PackagerOptimiser_OneItemPlusRpt(const PackagerOptimiser_OneItemPlusRpt & right);
	PackagerOptimiser_OneItemPlusRpt &operator=(const PackagerOptimiser_OneItemPlusRpt &);


protected:
    virtual void doOptimise() ;
 
 
private:
	PackagerOptimiser_OneItemPlusRpt();	/* Unimplemented */

} ;


_ACS_END_NAMESPACE

#endif  // _PackagerOptimiser_OneItemPlusRpt_H_
