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

	$Prod: A.C.S. libProductNaming$

	$Id$

	$Author$

	$Log$
	Revision 1.7  2013/11/12 11:23:59  marpas
	class do not longer inherit from STL container
	
	Revision 1.6  2013/11/12 09:40:29  marpas
	coding best practices applied
	qa warnings and compilation warnings fixed
	
	Revision 1.5  2013/11/11 10:42:28  ricfer
	rsResourceSet including the productNaming conf file passed directly to the load NamingRules method.
	
	Revision 1.4  2012/10/19 10:21:14  damdec
	_allowedFieldNames initialized in the initialization list.
	
	Revision 1.3  2012/02/23 16:39:21  marant
	compilation warning solved
	
	Revision 1.2  2011/03/01 16:33:28  marpas
	GCC 4.4.x support
	
	Revision 1.1.1.1  2010/04/12 10:02:06  marant
	Import libProductNaming
	
        
*/ 

#include <NamingRules.h>
#include <rsResourceSet.h>
#include <Filterables.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
    const string _namingRule = "ProductNaming.List_Of_NamingRules.NamingRule";

}


ACS_CLASS_DEFINE_DEBUG_LEVEL(NamingRules)

Rule::~Rule() throw(){};

NamingRules::NamingRules() : 
    _rs_x_fn()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;

}


//void NamingRules::setAllowedFieldNames(const vector<string>& fieldnames) // PRQA S 4121
//{
//    _allowedFieldNames = fieldnames;
//}

void NamingRules::printFields(const FilenameField &field) 
{

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Field Name: " << field.name << " - " // PRQA S 3081
        << "Field Index: " << int(field.index)) ;
}

void NamingRules::load(rsResourceSet& _rsConf) {
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

    _rs_x_fn.clear();
    vector<Rule> listRules;
    _rsConf.getArray<Rule>(_namingRule, listRules);

    size_t N = listRules.size();	
    for(size_t k=0; k<N; ++k){ 
        _rs_x_fn.push_back(make_pair(listRules.at(k).getRegExp(), listRules.at(k).getFileFields()));
    }

    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        size_t sz = _rs_x_fn.size() ;
        for (size_t i=0; i< sz; ++i) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "=> NamingRule #" << (i+1)) ; // PRQA S 3084
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FilenameRegExp: " << getFilenameRegExp(i)) ;;

            vector<FilenameField> fields = getFilenameFields(i);
            for_each(fields.begin(), fields.end(), printFields);
        }
	
	ACS_CLASS_END_DEBUG

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executed.") ;

}


NamingRules::~NamingRules() throw()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ; // PRQA S 4631
}




_ACS_END_NAMESPACE


