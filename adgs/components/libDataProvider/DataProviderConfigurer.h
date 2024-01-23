// PRQA S 1050 EOF
/*

 Copyright 1995-2015, Advanced Computer Systems , Inc.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.acsys.it

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Advanced Computer Systems, Inc.

 $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author$

 $Log$
 Revision 5.6  2017/02/23 16:36:58  nicvac
 S3EVOL-13: Distribution working area is not related anymore to download cache.

 Revision 5.5  2017/02/21 17:02:44  nicvac
 S3EVOL-13: Inter process cache mode

 Revision 5.4  2016/12/16 16:55:45  nicvac
 S3MPM-860: PackageExporter resources improvement.

 Revision 5.3  2015/05/28 09:33:31  marpas
 useless parameters no more read from configuration (BufrExtension, BufrConverter, CompressScriptName)
 coding best practice applied

 Revision 5.2  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.1  2013/10/28 17:34:09  marpas
 work in progress

 Revision 5.0  2013/06/06 18:51:19  marpas
 adopting libException 5.x standards


 */

#ifndef _DataProviderConfigurer_H_
#define _DataProviderConfigurer_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <rsResourceSet.h>
#include <FtpCurlConstants.h>
#include<map>
#include<vector>
#include<string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, conf)

class DataProviderConfigurer { // PRQA S 2109
public:
    DataProviderConfigurer();
    virtual ~DataProviderConfigurer() throw();
    /**
     * A dump method in order to inspect the configuration parameters read from file.
     */
    virtual void dump() const;
    /**
     * Loads the configuration parameters from file.
     */
    virtual void load();

    static void confLoadedCallback ( void *cp );
    static bool onceForRun() {
        return _onceForRun;
    }
    static void setOnceForRun ( bool onceForRun ) {
        _onceForRun = onceForRun;
    }

private:
    DataProviderConfigurer(const DataProviderConfigurer &); // not implemented
    DataProviderConfigurer& operator= (const DataProviderConfigurer &); // not implemented

#ifdef ACS_TEST
public:
#else
private:
#endif
    static bool _onceForRun ;
    static const std::string spaceName;

    static const unsigned long defaultDownloadSpaceSize = 16*acs::curl::Constants::Gigabyte ;

    static const std::string defaultProductPath;
    static const std::string defaultDownloadPath;
    static const size_t defaultMaxDBRetry = 3 ;

	static const bool defaultLowerPriorityFirst;

public:
    /**
     * Getters/Setters for all properties.
     */
    const std::string & getDownloadRepository() const throw() ;
    void setDownloadRepository ( std::string const& repositoryPath );

    unsigned long getDownloadSpaceSize() const throw() ;
    void setDownloadSpaceSize ( unsigned long size ) throw() ;

    const std::string & getProductPath() const throw() ;

    size_t getMaxDBRetry() const throw() ;

	bool getLowerPriorityFirst() const throw() ;

#ifdef ACS_TEST
public:
#else


private:
#endif

    std::string  _productPath;
    size_t  _maxDBRetry;

	bool _lowerPriorityFirst;

    std::string  _downloadRepository;
    unsigned long _availableDownloadSpaceSize;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(DataProviderConfigurer) ;

};

typedef acs::pattern::Singleton<DataProviderConfigurer> DPConfigurerSingleton;

_ACS_END_NESTED_NAMESPACE

#endif /* _DataProviderConfigurer_H_ */
