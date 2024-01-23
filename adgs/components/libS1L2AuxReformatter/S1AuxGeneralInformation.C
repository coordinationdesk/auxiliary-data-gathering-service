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
	Revision 1.6  2013/10/29 16:00:23  nicvac
	S1PDGS-1899: implementation.
	
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

#include <S1AuxGeneralInformation.h>

#include <Filterables.h>

#include <cstring>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxGeneralInformation)


S1AuxGeneralInformation::S1AuxGeneralInformation(
                                                 const string & safeNamespace, 
                                                 const string& type, 
                                                 const string& val, 
                                                 const string& gen, 
                                                 unsigned int instrConfId) :
    WrappedMetadata(safeNamespace, ""),
    _auxProdType(type), 
    _validity(val), 
    _generation(gen), 
    _instrConfigId(instrConfId)
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);

	if (getSafeNamespace().empty())
	{
		ACS_LOG_WARNING( SimpleDebugSignature<<"empty namespace.");
	}
	writeXmlStream();
}

void S1AuxGeneralInformation::writeXmlStream()
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ostringstream xmlInfo;
	xmlInfo << "<xmlData>\n" ;
	//xmlInfo << "\t<generalProductInformation  xmlns=\"" << _safeNamespace << "\">\n" ;
	xmlInfo << "\t<s1auxsar:standAloneProductInformation>\n" ;
		xmlInfo << "\t\t<s1auxsar:auxProductType>" << _auxProdType << "</s1auxsar:auxProductType>\n" ;
		xmlInfo << "\t\t<s1auxsar:validity>" << _validity << "</s1auxsar:validity>\n" ;
		xmlInfo << "\t\t<s1auxsar:generation>"  << _generation  << "</s1auxsar:generation>\n" ;
		xmlInfo << "\t\t<s1auxsar:instrumentConfigurationId>" << _instrConfigId << "</s1auxsar:instrumentConfigurationId>\n" ;
	xmlInfo << "\t</s1auxsar:standAloneProductInformation>\n" ;
	xmlInfo << "</xmlData>\n" ;

	setXMLStream(xmlInfo.str());
}

void S1AuxGeneralInformation::readXmlStream(const rsResourceSet & rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_THROW (exNotImplemented("Method not implemented!"));
}

_ACS_END_NAMESPACE
