/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2014/01/10 14:33:17  nicvac
	S1PDGS-30085: support xsd added.
	
	Revision 1.3  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.2  2012/04/17 15:46:34  damdec
	srsName and safeNamespace added in MetadataWrapDescriptor
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/

#ifndef _SafeObjectDescriptor_H_
#define _SafeObjectDescriptor_H_

#include <exException.h>

namespace acs {

/**
* \brief
* Structures that map the content of Safe Manifest 
**/


struct DataObjectDescriptor { // PRQA S 2173, 2175

 // PRQA S 2100 L1
 	
	std::string id;         //internal id
	bool isSet;             //internal flag
	
	std::string textInfo;   //content unit info
	std::string unitType;
	std::string dmdId;
	
	std::string dataObjectId;   //dataObjectType	
	std::string repId;

	std::string mimeType;   //dataObjectType.bytestreamType
	
	std::string href;       //dataObjectType.referenceType
	std::string locatorType;
	
	std::string checksumName;

	// PRQA L:L1
	DataObjectDescriptor() : 
        id(), 
        isSet(false), 
        textInfo(), 
        unitType(), 
        dmdId(), 
        dataObjectId(), 
        repId(), 
        mimeType(), 
        href(), 
        locatorType(),
	    checksumName()
    {};
	
};


struct MetadataWrapDescriptor { // PRQA S 2173, 2175
	// PRQA S 2100 L2
	std::string id; //internal id
	
	std::string metadataObjectId; //optional
	std::string classification;
	std::string category;
	
	//metadataObjectType.metadataWrapType
	std::string mimeType;
	std::string textInfo;
	std::string vocabularyName;
	
	std::string xmlData;	
	
	std::string srsName;
	std::string safeNamespace;
	// PRQA L:L2
	MetadataWrapDescriptor() : 
        id(), 
        metadataObjectId(), 
        classification(), 
        category(), 
        mimeType(), 
        textInfo(), 
        vocabularyName(), 
        xmlData(), 
        srsName(),
        safeNamespace()
    {}; 
			
};


struct MetadataReferenceDescriptor { // PRQA S 2173, 2175

	// PRQA S 2100 L3
	std::string id;     //internal id
	
	bool isSet;         //internal flag
	
	std::string metadataObjectId; //optional
	std::string classification;
	std::string category;
	
	//metadataObjectType.metadataReferenceType
	std::string href;
	std::string hrefContent;
	std::string vocabularyName;
	std::string mimeType;
	std::string locatorType;
	// PRQA L:L3
	
	MetadataReferenceDescriptor() : 
        id(), 
        isSet(false), 
        metadataObjectId(), 
        classification(), 
        category(), 
        href(),
        hrefContent(),
        vocabularyName(), 
        mimeType(),
        locatorType() 
    {};
		
};

struct MetadataObjectDescriptor { // PRQA S 2173, 2175
	// PRQA S 2100 L4
	//internal id
	std::string id;
	
	std::string metadataObjectId; //optional
	std::string classification;
	std::string category;
	
	//metadataObjectType.dataObjectPointerType
	std::string dataObjectId;
	// PRQA L:L4
	
	MetadataObjectDescriptor() : 
        id(), 
        metadataObjectId(), 
        classification(), 
        category(), 
        dataObjectId()
    {};
		
};

struct RootContentUnitDescriptor // PRQA S 2173, 2175
{
	// PRQA S 2100 L5
	std::string id; 
	
	std::string unitType;
	std::string textInfo;
	std::string pdiId;
	std::string dmdId;
	// PRQA L:L5
	
	RootContentUnitDescriptor() : 
        id(), 
        unitType(), 
        textInfo(), 
        pdiId(), 
        dmdId()
    {};

};

}

#endif //_SafeObjectDescriptor_H_

