// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2015, Advanced Computer Systems , Inc.
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
 *  Revision 5.5  2016/05/11 09:27:29  danalt
 *  use of ACS_THROW and ACS_COND_THROW
 *
 *  Revision 5.4  2015/12/17 17:16:21  marpas
 *  logging best practices applied
 *
 *  Revision 5.3  2013/12/10 14:55:22  marfav
 *  Fixed default values management
 *
 *  Revision 5.2  2013/06/18 12:37:15  marpas
 *  final adoption of libException 5.x standards
 *
 *  Revision 5.1  2013/06/06 21:19:07  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 5.0  2013/06/06 18:10:25  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.19  2013/05/02 16:52:32  danalt
 *  using exFPrivateInfo for some info
 *
 *  Revision 1.18  2013/01/25 14:14:35  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.17  2013/01/16 18:41:16  micmaz
 *  work in progress
 *
 *  Revision 1.16  2012/12/20 14:53:42  micmaz
 *  work in progress
 *
 *  Revision 1.15  2012/12/18 17:59:13  micmaz
 *  work in progress
 *
 *  Revision 1.14  2012/12/14 16:01:28  micmaz
 *  work in progress
 *
 *  Revision 1.13  2012/12/11 13:33:16  micmaz
 *  work in progress
 *
 *  Revision 1.12  2012/12/06 18:27:32  micmaz
 *  work in progress
 *
 *  Revision 1.3  2012/11/08 11:41:15  micmaz
 *  reworked xml parser
 *
 *  Revision 1.2  2012/10/15 14:19:24  micmaz
 *  Added a new configuration template class in order to simplifying the properties reading
 *
 *  Revision 1.1  2012/10/15 13:36:33  micmaz
 *  Added a new configuration template class in order to simplifying the properties reading
 *
 *
 *
 */

#ifndef _DCConfigurer_h_
#define _DCConfigurer_h_

#include <acs_c++config.hpp>

#include <XMLUtility.h>

#include <Filterables.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <ConfigurationProperty.h>
#include <StringUtils.h>
#include <rsPushNSpace.h>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include<string>
#include<map>
#include<utility>

_ACS_BEGIN_NESTED_NAMESPACE(acs, utility)

exDECLARE_EXCEPTION(ConfigurationException,exException); // PRQA S 2131, 2502


template<typename T>
class DCConfigurer {

public:
    enum SpaceConfigurer {
        UseSpaceConfigurer = 0, DoNotUseSpaceConfigurer
    };
    DCConfigurer(SpaceConfigurer sc = UseSpaceConfigurer)
            : _onceForRun(true), t(), props() {
        if (sc == UseSpaceConfigurer) {
            ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace(t.getSpaceName()));
            ConfigurationSingleton::instance()->addCallback(confLoadedCallback, this);
        }
    }
private:
    DCConfigurer(DCConfigurer const& other);
    DCConfigurer& operator=(DCConfigurer const& other);
public:
    virtual ~DCConfigurer<T>() throw () {
        //empty.
    }

    /**
     * A dump method in order to inspect the configuration parameters read from file.
     */
    void dump() const {
        ACS_LOG_PRIVINFO(std::left << std::setw(50)
                << "--------------------------------------------------------------------------------------------------------------------------") ;
        ACS_LOG_PRIVINFO(std::left << std::setw(50) << t.getComponentName()) ;
        ACS_LOG_PRIVINFO(std::left << std::setw(50) << "Property Path" << std::setw(40) << "Value " << "Property Description") ;
        ACS_LOG_PRIVINFO(std::left << std::setw(50)
                << "--------------------------------------------------------------------------------------------------------------------------") ;
        for (ConfigurationProperties::const_iterator prop = props.begin(); prop != props.end(); ++prop) {
            if (prop->second.isFilled) {
                if (prop->second.isEncrypted){
                    ACS_LOG_PRIVINFO(std::left << std::setw(50) << prop->second.xpath << std::setw(30) << "***"<< std::setw(10) << " " << prop->second.description) ;
                }else{
                    ACS_LOG_PRIVINFO(std::left << std::setw(50) << prop->second.xpath << std::setw(30) << prop->second.value << std::setw(10) << " " << prop->second.description) ;
                }
            }
            else
            {
                std::ostringstream defval;
                defval << prop->second.value << " (def)";
                ACS_LOG_PRIVINFO(std::left << std::setw(50) << prop->second.xpath << std::setw(30) << defval.str() << std::setw(10) << " " << prop->second.description) ;
            }
	
        }
        ACS_LOG_PRIVINFO(std::left << std::setw(50)
                << "--------------------------------------------------------------------------------------------------------------------------") ;
    }

    /**
     * Loads the configuration parameters from file.
     */
    void load() {
        load(ConfigurationSingleton::instance()->get());
    }

    /**
     * Loads the configuration parameters from file.
     */
    void load(rsResourceSet & resultSet) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"load configuration");
        std::vector<ConfigurationProperty> propsToAdd;
        t.loadAllProperties(propsToAdd);
        bool errors = false;
        for (std::vector<ConfigurationProperty>::iterator prop = propsToAdd.begin(); prop != propsToAdd.end(); ++prop) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"load " << prop->xpath);
            try {
                XMLUtility::readFromXML(*prop, resultSet);
            } catch (std::exception const& ex) {
                if (prop->mandatory) {
                    ACS_LOG_ERROR(
                            "The configuration property " << prop->xpath << " is absent and it is mandatory, check the db/filesystem.");
                    errors = true;
                } else {
                    ACS_LOG_PRIVINFO(
                            "The configuration property " << prop->xpath << " is absent but it is not mandatory, the default value will be used.");
                }
            }
        }
        ACS_COND_THROW(errors,
                ConfigurationException("Some error occurred reading the configuration space(s)"));
        t.validateProperties(propsToAdd);
        for (std::vector<ConfigurationProperty>::iterator prop = propsToAdd.begin(); prop != propsToAdd.end(); ++prop) {
            props.insert(std::pair<std::string, ConfigurationProperty>(prop->xpath, *prop));
        }
    }

    static void confLoadedCallback(void* cp) {
        if (!cp) {
            return;
        }
        DCConfigurer<T>* c = reinterpret_cast<DCConfigurer<T> *>(cp);
        if (c->onceForRun()) {
            c->setOnceForRun(false);
            c->load();
            c->dump();
        }
    }

    bool onceForRun() {
        return _onceForRun;
    }
    void setOnceForRun(bool onceForRun) {
        _onceForRun = onceForRun;
    }
    bool isFilled(std::string const& name) {
        bool filled =  props.find(name)!= props.end();
        if (filled) {
            return props.find(name)->second.isFilled;
        }
        return false;
    }
    std::string& operator[](std::string const& name) {
        ConfigurationProperties::iterator it= props.find(name);
        if (it == props.end()) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Property not found : " << name);
            return _empty;
        }
        return (it->second.value);
    }
    template<typename OutputType>
    OutputType get(std::string const& name) {
        //if (isFilled(name)) {
        if (props.find(name)!=props.end()) {
            return boost::lexical_cast<OutputType>(operator[](name));
        }
        return OutputType();
    }
    int getInt(std::string const& name) {
        return get<int>(name);
    }
    bool getBoolean(std::string const& name) {
        std::string val =  getString(name);
        return (boost::iequals(val,"true") || boost::iequals(val, "1")|| boost::iequals(val, "yes") || boost::iequals(val, "y"));
    }
    std::string getString(std::string const& name) {
        return operator[](name);
    }
private:
    typedef std::map<std::string, ConfigurationProperty> ConfigurationProperties;
    bool _onceForRun;
    T t;
    ConfigurationProperties props;
    static std::string _empty;
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(DCConfigurer<T>) ;
};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(DCConfigurer, T) ;

template<typename T>
std::string DCConfigurer<T>::_empty = "";

template<typename T>
struct ConfigurerSingleton {
    typedef acs::pattern::Singleton<DCConfigurer<T> > SingletonType;
    typedef DCConfigurer<T> ConfigurerType;
};


_ACS_END_NESTED_NAMESPACE


/*
 *
 *
 *To activate the configurer, define the static class:
 static class LibraryConfigurerLoader {
 public:
 LibraryConfigurerLoader(){
 ConfigurerSingleton::instance();
 }
 } _LibraryConfigurerLoader;
 *
 *
 *
 */

#endif /* _DCConfigurer_h_ */
