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

	$Id$

	$Author$

	$Log$
	Revision 1.5  2013/10/17 15:56:11  nicvac
	compilation warnings processed.
	
	Revision 1.4  2012/10/11 15:14:50  damdec
	Class aligned to the new libACSSaformatter.
	
	Revision 1.3  2012/06/28 08:45:27  damdec
	Code aligned to S1.RS.MDA.52.7441_02_04 and related schemas [CFI-02.1] GMES-S1GS-EOPG-TN-12-0013-Sentinel-1-IPF-L1-L2-Auxiliary_files_Schemas_v2.4
	
	Revision 1.2  2012/04/17 16:01:32  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.1.1.1  2012/03/27 11:50:11  damdec
	Import libS1L2AuxReformatter
	
        
*/

#ifndef _S1AuxGeneralInformation_H_
#define _S1AuxGeneralInformation_H_

#include <exException.h>
#include <WrappedMetadata.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
/**
* \brief
* Class mapping the GeneralProductInformationType for the S1 L2 Aux SAFE files
**/

class S1AuxGeneralInformation : public WrappedMetadata { 
public:

	/** Class constructor */
	S1AuxGeneralInformation(const std::string & safenamespace, const std::string& type, const std::string& val, const std::string& gen, unsigned int instrConfId);
	/** Destructor */
	virtual ~S1AuxGeneralInformation() ACS_NOEXCEPT = default ;

	S1AuxGeneralInformation(const S1AuxGeneralInformation & ) = default;
	S1AuxGeneralInformation &operator=(const S1AuxGeneralInformation &)= default;
	

protected:

	virtual void writeXmlStream() ;
	virtual void readXmlStream(const rsResourceSet & rs);
	
private:
	std::string 	_auxProdType;
	std::string 	_validity;
	std::string 	_generation;
	unsigned int	_instrConfigId;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1AuxGeneralInformation)

};

_ACS_END_NAMESPACE

#endif //_S1AuxGeneralInformation_H_

