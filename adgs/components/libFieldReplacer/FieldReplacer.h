/*
	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libFieldReplacer $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2014/03/04 13:55:37  marfav
	characters signature depends on Xercers major version
	
	Revision 1.3  2012/02/13 16:56:32  marpas
	refactoring
	
	Revision 1.2  2008/11/14 14:50:13  manuel.maccaroni
	fixed cvs tags
	
	Revision 1.1.1.1  2008/11/14 14:47:24  manuel.maccaroni
	Import libFieldReplacer
	


        
*/ 

#ifndef _FieldReplacer_H_
#define _FieldReplacer_H_

#include <exException.h>

#include <XMLExtractorHandlerBase.h>
#include <XMLDataConsumer.h>

namespace acs
{

// Helper class for XML tags replacement in complex files
class FieldReplacer : public XMLDataConsumer
{
public:
	FieldReplacer() ;
    ~FieldReplacer() throw() ;
    using XMLDataConsumer::nodeMatch;
#ifdef XERCES_HAS_NAME_SPACE
    virtual bool nodeMatch(XMLExtractorHandlerBase &eh, const std::string &nodepath, const XERCES_CPP_NAMESPACE::AttributeList &att, const XMLCh* name = 0) ;
#else
    virtual bool nodeMatch(XMLExtractorHandlerBase &eh, const std::string &nodepath, const AttributeList &att, const XMLCh* name = 0) ;
#endif
    using XMLDataConsumer::nodeValue;
    virtual bool nodeValue(XMLExtractorHandlerBase &, const std::string &xmlnode, const std::string &value, const XMLCh* name) ;

#if ( XERCES_VERSION_MAJOR > 2 )
    virtual bool characters(XMLExtractorHandlerBase &eh, const XMLCh* chars, const XMLSize_t lenght) ;
#else
    virtual bool characters(XMLExtractorHandlerBase &eh, const XMLCh* chars, const unsigned int lenght) ;
#endif

    void replace(const std::string& field, const std::string& value);
private:
    bool _found ;
    bool _done ;
    std::vector<std::pair<std::string,std::string> > _toReplace;
    std::string _fieldValue;
} ;


}//close namespace

#endif //_FieldReplacer_H_
