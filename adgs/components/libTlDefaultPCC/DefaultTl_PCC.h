/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Default PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2015/06/22 12:43:02  damdec
	Compilation warnings removed.
	
	Revision 1.3  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.2  2008/11/07 15:41:27  marfav
	Dead code removed
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
	

*/

#ifndef _DEFAULTTL_PCC_H_
#define _DEFAULTTL_PCC_H_


#include <ThinLayer_PCC.h>


_ACS_BEGIN_NAMESPACE(acs)

class DefaultTl_PCC : public ThinLayer_PCC
{
public:

	exDECLARE_EXCEPTION (DefaultTl_PCC_Exception, ThinLayer_PCC_Exception) ;

	DefaultTl_PCC (StringKey factoryKey);
	virtual ~DefaultTl_PCC () throw() ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultTl_PCC)
};
	
_ACS_END_NAMESPACE

#endif //_DEFAULTTL_PCC_H_
