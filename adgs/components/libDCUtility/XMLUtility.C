// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.0  2013/06/06 18:10:25  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.11  2013/04/29 17:20:37  marpas
 *  removing useless code
 *
 *  Revision 1.10  2013/04/29 13:10:01  marpas
 *  simplified logic and getValue template method added
 *
 *  Revision 1.9  2013/04/03 17:02:44  marpas
 *  coding best practices applied
 *  interfaces rationalization
 *
 *  Revision 1.8  2013/01/25 14:14:35  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.7  2012/12/21 15:27:08  micmaz
 *  work in progress
 *
 *  Revision 1.6  2012/12/06 14:32:53  micmaz
 *  work in progress
 *
 *  Revision 1.5  2012/12/06 11:49:49  micmaz
 *  work in progress
 *
 *  Revision 1.4  2012/12/04 16:54:35  micmaz
 *  work in progress
 *
 *  Revision 1.3  2012/11/08 16:53:11  micmaz
 *  correct errors in parsing xml.
 *
 *  Revision 1.2  2012/11/08 11:41:15  micmaz
 *  reworked xml parser
 *
 *  Revision 1.1  2012/10/31 18:36:07  micmaz
 *  added xml utility
 *
 *
 */

#include <XMLUtility.h>
#include <XMLIstringstream.h>
#include <rsResourceSet.h>
#include <ConfigurationProperty.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <boost/shared_ptr.hpp> // PRQA S 1013 2

_ACS_BEGIN_NESTED_NAMESPACE(acs, utility)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL (XMLUtility);

void XMLUtility::writeToXML(std::string const& rootTagName, std::vector<ConfigurationProperty> const& properties,
        ostream& xml) {

    xml << "<" << rootTagName << ">\n";
    for (vector<ConfigurationProperty>::const_iterator iter = properties.begin(); iter != properties.end(); ++iter) {
        xml << " <" << iter->name << ">";
        xml << iter->defaultValue;
        xml << "</" << iter->name << ">\n";
    }
    xml << "</" << rootTagName << ">\n";
}


void XMLUtility::readFromXML(ConfigurationProperty & prop, rsResourceSet const& resultSet) {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"readFromXML " << prop.name);
    
    string xpath = StringUtils::transform(prop.xpath, '/', '.')  ;
    string value ;
    if (prop.mandatory) {
        resultSet.getValue(xpath, value); // PRQA S 3084
        prop.value = value;
        prop.isFilled= true;
    } else {
        if (resultSet.hasKey(xpath)) { // PRQA S 3084
            resultSet.getValue(xpath, value); // PRQA S 3084
            prop.value = value;
            prop.isFilled= true;
        }else{
            prop.value = prop.defaultValue;
            prop.isFilled= false;
        }
    }
}


_ACS_END_NESTED_NAMESPACE
