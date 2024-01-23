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
 *  Revision 5.2  2014/04/15 16:19:21  marpas
 *  minor changes
 *
 *  Revision 5.1  2013/07/12 09:01:45  marpas
 *  header fixed
 *
 *  Revision 5.0  2013/06/06 18:10:24  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.11  2013/04/29 13:10:01  marpas
 *  simplified logic and getValue template method added
 *
 *  Revision 1.10  2013/04/03 17:02:44  marpas
 *  coding best practices applied
 *  interfaces rationalization
 *
 *
 *
 */
 
#ifndef _Property_h_
#define _Property_h_

#include <acs_c++config.hpp>
#include <exException.h>
#include <list>
#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp> // PRQA S 1013 2

_ACS_BEGIN_NESTED_NAMESPACE(acs, utility)

struct Property { // PRQA S 2173, 2175
    Property();
    Property(const std::string& n, std::string const& v);
    Property(const Property&);
    Property& operator=(const Property&);
    virtual ~Property() throw ();

    std::string name; // PRQA S 2100 2
    std::string value;
};

enum PropertyEncryption {
    PropertyValueUnencrypted=0 ,PropertyValueEncrypted
};

struct ConfigurationProperty: public Property { // PRQA S 2153, 2173, 2175

    ConfigurationProperty();

    template<typename Type>
    explicit ConfigurationProperty(const std::string& xpath,
                                 std::string const& description = "",
                                 Type const& defaultValue = Type(),
                                 bool mandatory = false,
                                 PropertyEncryption encrypted=PropertyValueUnencrypted) :
          Property(xpath, ""),
          xpath(xpath),
          mandatory(mandatory),
          isFilled(false),
          description(description),
          defaultValue(boost::lexical_cast<std::string>(defaultValue)),
          isEncrypted(encrypted == PropertyValueEncrypted)
    {
            // empty
    }
    
    explicit ConfigurationProperty(const std::string& xp) ;
    ConfigurationProperty(ConfigurationProperty const&);
    ConfigurationProperty& operator=(ConfigurationProperty const&);
    virtual ~ConfigurationProperty() throw ();
    std::string str() const;

    template<typename T> 
    T &getValue(T& t) const {
        try {
            t = boost::lexical_cast<T>(value) ;
        }
        catch(std::exception &x) {
            ACS_THROW(acs::exIllegalValueException(x,"Getting " + xpath + "value (" + value + ")" )) ;
        }
        return t ;
    }

    std::string xpath; // PRQA S 2100 6
    bool mandatory;
    bool isFilled;
    std::string description;
    std::string defaultValue;
    bool isEncrypted;
};

typedef std::list<Property> Properties;

bool operator==(ConfigurationProperty const& x, ConfigurationProperty const& y) ;

template<typename Stream>
Stream& operator<<(Stream& stream, const ConfigurationProperty obj) {
    stream << obj.str();
    return stream;
}


_ACS_END_NESTED_NAMESPACE

#endif /* _Property_h_ */
