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
 *  $Prod: A.C.S. libDCUtility Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.1  2013/07/12 09:01:45  marpas
 *  header fixed
 *
 *  Revision 5.0  2013/06/06 18:10:24  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.9  2013/04/03 17:02:44  marpas
 *  coding best practices applied
 *  interfaces rationalization
 *
 *
 *
 *
 */

#include "ConfigurationProperty.h"
#include <sstream>


_ACS_BEGIN_NESTED_NAMESPACE(acs, utility)
using namespace std;

Property::Property()
        : name(), value() {
    // empty
}
Property::Property(const std::string& n, const std::string & v)
        : name(n), value(v) {
    // empty
}

Property::Property(const Property& rhs)
        : name(rhs.name), value(rhs.value) {

}

Property& Property::operator=(const Property& rhs) {
    if (this != &rhs) {
        name = rhs.name;
        value = rhs.value;
    }
    return *this;
}

Property::~Property() throw () {
    // empty
}

ConfigurationProperty::ConfigurationProperty() : 
    Property(),
    xpath(),
    mandatory(false),
    isFilled(false),
    description(),
    defaultValue(),
    isEncrypted(false)
{

}
ConfigurationProperty::ConfigurationProperty(const ConfigurationProperty& rhs) : 
    Property(rhs.name, rhs.value),
    xpath(rhs.xpath),
    mandatory(rhs.mandatory),
    isFilled(rhs.isFilled),
    description(rhs.description),
    defaultValue(rhs.defaultValue),
    isEncrypted(rhs.isEncrypted)
{
//empty.
}

ConfigurationProperty& ConfigurationProperty::operator=(ConfigurationProperty const& rhs) {
    if (this != &rhs) {
        name = rhs.name;
        value = rhs.value;
        xpath = rhs.xpath;
        mandatory = rhs.mandatory;
        isFilled = rhs.isFilled;
        description = rhs.description;
        defaultValue = rhs.defaultValue;
        isEncrypted = rhs.isEncrypted;
    }
    return *this;

}

ConfigurationProperty::ConfigurationProperty(const std::string& xp) :
    Property(xp, ""),
    xpath(xp),
    mandatory(false),
    isFilled(false),
    description(),
    defaultValue(),
    isEncrypted(false)
{
        // empty
}

ConfigurationProperty::~ConfigurationProperty() throw () {
    // empty
}
string ConfigurationProperty::str() const {
    ostringstream os;
    os << "ConfigurationProperty: {";
    os << "name: " << name << ", " << "xpath: " << xpath << ", " << "mandatory: " << mandatory ;
    if (isEncrypted) {
        os << ", value: *** }";
    }else{
        os << ", value: "<< value<< "}";
    }
    return os.str();
}

bool operator==(ConfigurationProperty const& x, ConfigurationProperty const& y) {
    return x.xpath == y.xpath ;
}


_ACS_END_NESTED_NAMESPACE
