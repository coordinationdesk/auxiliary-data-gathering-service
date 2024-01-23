// PRQA S 1050 EOF
/*
	Copyright 1995-2023, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: libS1L2AuxReformatter$

*/


#ifndef _S1AuxTecReformatter_H_
#define _S1AuxTecReformatter_H_

#include <exException.h>
#include <S1L2AuxReformatter.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* S1 TEC Auxiliary Data Formatter 
**/

class S1AuxTecReformatter : public S1L2AuxReformatter {  // PRQA S 2109
public:

	/** S1AuxTecReformatter Exceptions */
	exDECLARE_EXCEPTION(exS1AuxTecReformatterException, exException) ; // Base S1AuxTecReformatter Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exS1AuxTecReformatterCriticalException, exS1AuxTecReformatterException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	S1AuxTecReformatter() = default;
	/** Destructor */
	virtual ~S1AuxTecReformatter() ACS_NOEXCEPT = default;

protected:

private: // copy constructor and operator= defined but not implemented
	S1AuxTecReformatter(const S1AuxTecReformatter & ); // not implemented
	S1AuxTecReformatter &operator=(const S1AuxTecReformatter &); // not implemented

	virtual void handleDataFiles(SafeManifest & manifest, const std::string & dataDir) ;
	virtual void setValidity(const std::string &) ;
	virtual void setSoftware(ProcessLog & pLog, Software & sw);
//	virtual void setPlatform(const std::string & safeNamespace, const std::string& safePrefix) {};
	virtual std::string buildOutputFilename();
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1AuxTecReformatter)

};

_ACS_END_NAMESPACE

#endif //_S1AuxTecReformatter_H_
