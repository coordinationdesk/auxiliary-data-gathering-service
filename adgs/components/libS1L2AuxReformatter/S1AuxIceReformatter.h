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
	Revision 1.2  2013/10/17 15:56:11  nicvac
	compilation warnings processed.
	
	Revision 1.1  2012/04/23 08:02:38  damdec
	Class added.
	
        
*/

#ifndef _S1AuxIceReformatter_H_
#define _S1AuxIceReformatter_H_

#include <exException.h>
#include <S1L2AuxReformatter.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* S1 Sea Ice Auxiliary Data Formatter 
**/

class S1AuxIceReformatter : public S1L2AuxReformatter {  // PRQA S 2109
public:

	/** S1AuxIceReformatter Exceptions */
	exDECLARE_EXCEPTION(exS1AuxIceReformatterException, exException) ; // Base S1AuxIceReformatter Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exS1AuxIceReformatterCriticalException, exS1AuxIceReformatterException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	S1AuxIceReformatter();
	/** Destructor */
	virtual ~S1AuxIceReformatter() throw() ;

protected:

private: // copy constructor and operator= defined but not implemented
	S1AuxIceReformatter(const S1AuxIceReformatter & ); // not implemented
	S1AuxIceReformatter &operator=(const S1AuxIceReformatter &); // not implemented
	
	virtual void setValidity(const std::string & validity);
	virtual void handleDataFiles(SafeManifest & manifest, const std::string & dataDir) ;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1AuxIceReformatter)

};

_ACS_END_NAMESPACE

#endif //_S1AuxIceReformatter_H_
