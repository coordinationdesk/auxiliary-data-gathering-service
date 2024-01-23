// PRQA S 1050 EOF
/*
 * 	Copyright 1995-2021, Exprivia SpA - DADF
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.exprivia.it
 *
 * 	All Rights Reserved.
 *
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Exprivia SpA.
 *
 * 	$Prod: PackagerOptimiserApp - HEADER FILE $
 *
 * 	$Id$
 *
 * 	$Author$
 *
 * 	$Log$
 * 	Revision 2.15  2016/01/08 15:34:36  francesco.avanzi
 * 	Usage Method added
 * 	Some ACS_LOG_INFO messages moved to ACS_LOG_PRIVINFO as such messages where intended to be.
 * 	Try/catch logic implemented in getConfiguration(): rsResourceSet::NotFoundException are now wrapped and will not blow to main anymore.
 *
 * 	Revision 2.14  2014/06/25 17:10:10  marpas
 * 	coding best practices applied
 *
 * 	Revision 2.13  2013/05/12 14:44:09  marpas
 * 	no longer master/slave check
 *
 * 	Revision 2.12  2013/03/14 15:42:46  marpas
 * 	connection pool concept is left to class inheriting from Application
 *
 * 	Revision 2.11  2013/02/14 09:12:57  marpas
 * 	mastr discovery implementation changed
 *
 * 	Revision 2.10  2013/02/07 15:50:11  enrcar
 * 	EC:: Updated to handle the new schema
 *
 * 	Revision 2.9  2013/01/11 13:47:47  marfav
 * 	Addedd support to external appname definition
 *
 * 	Revision 2.8  2012/02/09 17:54:40  marpas
 * 	refactoring in progress
 *
 * 	Revision 2.7  2011/04/26 09:39:59  marpas
 * 	useless header removed
 *
 * 	Revision 2.6  2010/06/15 11:05:40  marpas
 * 	HAS_MISSION_INCLUDES management added
 *
 * 	Revision 2.5  2009/07/13 14:19:43  enrcar
 * 	EC:: c++rules
 *
 * 	Revision 2.4  2009/03/24 15:16:49  enrcar
 * 	EC:: c++rules
 *
 * 	Revision 2.3  2009/01/22 14:22:50  enrcar
 * 	EC:: RemoveTransientsAtStartup. Method added.
 *
 * 	Revision 2.2  2008/11/26 09:24:38  danalt
 * 	Using ConfigurationSingleton instead of MergeConfiguration to load confspaces
 *
 * 	Revision 2.1  2008/02/13 14:26:57  enrcar
 * 	EC:: Report handling added
 *
 *
 */

#ifndef _PackagerOptimiserApp_H_
#define _PackagerOptimiserApp_H_

#include <exException.h>
#include <PackagerOptimiser.h>
#include <DateTime.h>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#if defined (HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
    #include <StationApp.h>
#elif COSMO
    #include <CosmoApp.h>
#else
    #include <CryosatApp.h>
#endif



_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;

class PackagerOptimiserApp : // PRQA S 2153
#if defined (HAS_MISSION_INCLUDES)
    public PROJECT_APP
#elif GENERIC_STATION_APP
    public StationApp
#elif  COSMO
   public CosmoApp
#else
    public CryosatApp
#endif
{
public:

    struct mediaParams // PRQA S 2173, 2175
    {
        mediaParams() throw() : mintr(), maxtr(), delay(), stabilityTime() {}
        ~mediaParams() throw() {}
        mediaParams(const mediaParams &r) : mintr(r.mintr), maxtr(r.maxtr), delay(r.delay), stabilityTime(r.stabilityTime) {}
        mediaParams& operator= (const mediaParams &r)
        {
            if (this != &r) {
                mintr = r.mintr ;
                maxtr = r.maxtr ;
                delay = r.delay ;
                stabilityTime = r.stabilityTime ;
            }
            return *this ;
        }
        unsigned long long int mintr ; // PRQA S 2100 4
        unsigned long long int maxtr ;
        DateTime delay ;
        DateTime stabilityTime ;
    } ;

    typedef std::map <std::string, mediaParams> MediaParamsMap ;


    /*! class PackagerOptimiserAppException declaration */
    exDECLARE_EXCEPTION(PackagerOptimiserAppException,exException) ; // PRQA S 2131, 2502


    PackagerOptimiserApp(const std::string &appName, const std::string &subSystem);

    virtual ~PackagerOptimiserApp() throw() ;

    virtual int main(int argc, char const * const * argv, char const * const * env);
    virtual int usage(const std::string &) const ;

    virtual void  customSigTermHandler() ;
    virtual void  customSigIntHandler() ;
    void setOptimiser(PackagerOptimiser* p) ;
    void stopOptimiser() ;

private:
    PackagerOptimiserApp() ;	/* unimplemented */

    // copy constructor and operator= defined but not implemented
    PackagerOptimiserApp(const PackagerOptimiserApp & ) ;
    PackagerOptimiserApp &operator=(const PackagerOptimiserApp &) ;


    //methods
    bool createOptimiser(	\
        const int& mediaFormatId, const std::string& mediaFormat, \
        const MediaParamsMap::iterator&, const int& packingAlgoId, const std::string& packingAlgoKey, PackagerOptimiser* &pkgOptimiser) ;
    void doWork();
    void getConfiguration();
    void loadConfigurationParam(rsResourceSet &set, std::string const & tag, unsigned int & value );
    void loadConfigurationParam(rsResourceSet &set, std::string const & tag, std::string & value );
    void removeTransientsAtStartup() ;
    void runPackagerOptimiserApp() ;
    void stop();

    // Attributes
    unsigned long long int _maxLogSize ;
    unsigned int _optimiseRescanTime ;
    unsigned int _reloadConfTime ;
    unsigned int _restartTime ;
    PackagerOptimiser::TypeReportFormats _reportFormats ;

    bool _reloadConfiguration;
    pthread_t _mainThread ;
    PackagerOptimiser* _optimiser ; /* for stop method */
    MediaParamsMap _mediaParams ;

    std::vector <std::string> _ignoredMediaTypes;

#ifdef PDS_OLD_SCHEMA
    std::vector <std::string> _mediaFormat ;
#endif

    ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagerOptimiserApp);

};

_ACS_END_NAMESPACE

#endif /* _PackagerOptimiserApp_H_ */
