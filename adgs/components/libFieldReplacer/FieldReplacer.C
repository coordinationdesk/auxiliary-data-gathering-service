/*
	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libFieldReplacer  $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2012/02/13 16:58:18  marpas
	version management
	
	Revision 1.3  2012/02/13 16:56:32  marpas
	refactoring
	
	Revision 1.2  2008/11/14 14:50:13  manuel.maccaroni
	fixed cvs tags
	
	Revision 1.1.1.1  2008/11/14 14:47:24  manuel.maccaroni
	Import libFieldReplacer
	


        
*/ 
#include <FieldReplacer.h>
#include <FieldReplacerV.h>

using namespace std;
using namespace acs;

static FieldReplacerV version ;

FieldReplacer::FieldReplacer() : 
	_found(false), 
	_done(false),
    _toReplace(),
	_fieldValue("")
{
}

FieldReplacer::~FieldReplacer() throw() {}

void FieldReplacer::replace(const std::string& field, const std::string& value)
{
	pair<string,string> cpToReplace;
	cpToReplace.first = field ;
	cpToReplace.second = value ;
	_toReplace.push_back(cpToReplace);
}

// node to match for VERSION is /Earth_Explorer_Header/Fixed_Header/File_Version

#ifdef XERCES_HAS_NAME_SPACE
bool FieldReplacer::nodeMatch(XMLExtractorHandlerBase &xh, const string &node, const XERCES_CPP_NAMESPACE::AttributeList &atts, const XMLCh* name)
#else
bool FieldReplacer::nodeMatch(XMLExtractorHandlerBase &xh, const string &node, const AttributeList &atts, const XMLCh* name)
#endif
{
	if (_done) 
	{
		xh.endOfExtraction() ;
		return false ; 
	}
	
	vector<pair<string,string> >::iterator it = _toReplace.begin();
	while( it != _toReplace.end() )
	{
		if( node == (*it).first)
		{
			_fieldValue = (*it).second;
			_found = true ;
			xh.writeStart(name,atts) ;
			_toReplace.erase(it);
			return true ;
		}
		it++;
	}
	
	return false ;
}	

bool FieldReplacer::nodeValue(XMLExtractorHandlerBase &xh, const string &node, const string &value, const XMLCh* name) 
{
	if (_found) 
	{
		xh.formatBuf(_fieldValue) ;
		xh.writeEnd(name) ;
		_found = false ;
	
		if( _toReplace.size() == 0 )
			_done = true ;
		
		return true ;
	}
	return false ;
}

	
#if ( XERCES_VERSION_MAJOR > 2 )
bool FieldReplacer::characters(XMLExtractorHandlerBase &xh, const XMLCh* chars, const XMLSize_t lenght) 
#else
    virtual bool characters(XMLExtractorHandlerBase &eh, const XMLCh* chars, const unsigned int lenght) ;
bool FieldReplacer::characters(XMLExtractorHandlerBase &xh, const XMLCh* chars, const unsigned int lenght) 
#endif
{
	return _found ;
}
