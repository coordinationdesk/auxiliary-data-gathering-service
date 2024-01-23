/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.1  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _WEIGHTNORMALIZER_H_
#define _WEIGHTNORMALIZER_H_

#include <acs_c++config.hpp>
#include <exException.h>



_ACS_BEGIN_NAMESPACE(acs)
	
class TaskTableElement;

class WeightNormalizer
{
	/**
	 *\brief This class is used for normalize the weight of arrays of
	 *       tasks, pools, sequences 
	 *       The weight is assumed to be 1/n for each n elements if 
	 *       at least one weight is not specified (zero valued)
	 */ 

public:
	// Ctor and Dtor 
	WeightNormalizer();
	virtual ~WeightNormalizer() throw() ;

	/**
	 * Methods in charge of handling the normalization of
	 * - vector of tasks
	 * - vector of pools
	 * - vector of sequences
	 * 
	 * Pools and Sequences provide a method for normalization computing
	 * that will be called inside the normalization method in order to
	 * reach each node of the Task Table structure
	 * Virtual methods allow re-definition of the Normalization algorithms
	 */ 
	
	virtual void normalize (TaskTableElement&);
	

protected:
	// Declared but not implemented
	WeightNormalizer (const WeightNormalizer&);
	WeightNormalizer& operator= (const WeightNormalizer&);

private:
	// Attributes
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(WeightNormalizer) ;
};

_ACS_END_NAMESPACE

#endif //_WEIGHTNORMALIZER_H_

