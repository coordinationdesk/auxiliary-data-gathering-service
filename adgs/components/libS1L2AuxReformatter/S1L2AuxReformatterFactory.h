// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.1  2012/04/23 08:02:39  damdec
	Class added.
	
        
*/

#ifndef _S1L2AuxReformatterFactory_H_
#define _S1L2AuxReformatterFactory_H_

#include <exException.h>
#include <S1L2AuxReformatterSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

class S1L2AuxReformatter;

/**
* \brief
* Sentinel-1 L2 Auxiliary Data File Formatter Factory 
**/

class S1L2AuxReformatterFactory  { 
public:

	/** S1L2AuxReformatterFactory Exceptions */
	exDECLARE_EXCEPTION(exS1L2AuxReformatterFactoryException, exException) ; // Base S1L2AuxReformatterFactory Exception. // PRQA S  2131, 2502 2
	exDECLARE_EXCEPTION(exS1L2AuxReformatterFactoryCriticalException, exS1L2AuxReformatterFactoryException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	S1L2AuxReformatterFactory();
	/** Destructor */
	virtual ~S1L2AuxReformatterFactory() throw() ;

	/**
	  * \brief Builder method for S1 L2 Auxiliary Reformatter object retrieving
	  */								
	static S1L2AuxReformatter* buildS1AuxIceReformatter();
    static S1L2AuxReformatter* buildS1AuxWndReformatter();
    static S1L2AuxReformatter* buildS1AuxWavReformatter();
    static S1L2AuxReformatter* buildS1AuxTroReformatter();
    static S1L2AuxReformatter* buildS1AuxTecReformatter();
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1L2AuxReformatterFactory)

};

_ACS_END_NAMESPACE

#endif //_S1L2AuxReformatterFactory_H_

