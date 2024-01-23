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
 * 	$Prod: A.C.S. PackagerOptimiser Factory Class HEADER FILE $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 2.4  2014/05/13 16:00:46  lucio.pulvirenti
 * 	Debug macro updated.
 * 	
 * 	Revision 2.3  2013/02/07 15:08:10  enrcar
 * 	EC:: Updated to handle the new interface of PackagerOptimiser
 * 	
 *
 */


#ifndef _PackagerOptimiserFactory_H_
#define _PackagerOptimiserFactory_H_ 

#include <PackagerOptimiser.h>



_ACS_BEGIN_NAMESPACE(acs)


class PackagerOptimiserFactory
{
public:
	PackagerOptimiserFactory();
	~PackagerOptimiserFactory();


	static PackagerOptimiser* buildPackagerOptimiser_DateSize(PACKAGEROPT_ARGS);  	  
	static PackagerOptimiser* buildPackagerOptimiser_OneItem(PACKAGEROPT_ARGS);
	static PackagerOptimiser* buildPackagerOptimiser_OneItemPlusRpt(PACKAGEROPT_ARGS);

	PackagerOptimiserFactory(const PackagerOptimiserFactory & right);
	PackagerOptimiserFactory &operator=(const PackagerOptimiserFactory &);

private:

    ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagerOptimiserFactory) ;

};


_ACS_END_NAMESPACE

#endif /* _PackagerOptimiserFactory_H_ */

