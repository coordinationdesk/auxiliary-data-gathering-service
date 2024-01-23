// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.1  2012/04/23 08:02:39  damdec
	Class added.
	
        
*/

#ifndef _S1AuxWndReformatter_H_
#define _S1AuxWndReformatter_H_

#include <exException.h>
#include <S1L2AuxReformatter.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* S1 L2 ECMWF Atmospheric model auxiliary data reformetter
**/

class S1AuxWndReformatter: public S1L2AuxReformatter  { 
public:

	/** S1AuxWndReformatter Exceptions */
	exDECLARE_EXCEPTION(exS1AuxWndReformatterException, exException) ; // Base S1AuxWndReformatter Exception. // PRQA S 211, 2502 2
	exDECLARE_EXCEPTION(exS1AuxWndReformatterCriticalException, exS1AuxWndReformatterException) ; // Critical Exception. // PRQA S 2153


	/** Class constructor */
	S1AuxWndReformatter();
	/** Destructor */
	virtual ~S1AuxWndReformatter() throw() ;

protected:

private: // copy constructor and operator= defined but not implemented
	S1AuxWndReformatter(const S1AuxWndReformatter & ); // not implemented
	S1AuxWndReformatter &operator=(const S1AuxWndReformatter &); // not implemented

	virtual void setValidity(const std::string & validity);
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1AuxWndReformatter)

};

_ACS_END_NAMESPACE

#endif //_S1AuxWndReformatter_H_

