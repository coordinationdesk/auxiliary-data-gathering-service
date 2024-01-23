/*
    Copyright 1995-2020, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libRollingArchive$

*/

#ifndef _RollingArchive_H_
#define _RollingArchive_H_

#include <exException.h>

#include <ProjectAppIncludes>

#include <RollingPolicyManager.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* This class implement a daemon able to clean storages and/or database
* depending on configured policies.
**/

class RollingArchive : public PROJECT_APP {
public:

    /** Class constructor */
    RollingArchive(const std::string &appName, const std::string &subSystem);
    /** Destructor */
    virtual ~RollingArchive() ACS_NOEXCEPT ;

    ACS_DECLARE_NO_COPY_CLASS(RollingArchive);

public:
    /** Main */
    virtual int main(int argc, char const * const * argv, char const * const * env) ;

    /** Signal handlers */
    virtual void customSigIntHandler();
    virtual void customSigTermHandler();

private:
    /** Collects exit code of the managed workers and reports TRUE if any worker is running */
    bool manageWorkers();

    /** Creates new workers, according to available works to do and available workers slots */
    void createWorkers();

    /** Exit now */
    void terminateWorkers();

    /** Load configuration spaces and configure attributes */
    bool getConfiguration();

    /** Update my app status on DB - Used for example by SDFBroker (cf. S2PDGS-673) */
    void updateStatusOnDb(size_t const& numRolledAtThisLoop);


private:

    /** Default values */
    static const size_t _defaultReloadConfTime = 60 ; // [sec]
    static const size_t _defaultRestartTime    = 3600*24 ; // [sec] (one day)
    static const size_t _defaultPoliciesRunMax = 10;

    /** time between two Configuration Reloading */
    size_t _confReloadConfTime;
    /** Autorestart time */
    size_t _confRestartTime;
    /** Max number of policies can be queued to be run */
    size_t _confPoliciesRunMax;

    /** Timer to reload configuration */
    Timer _timerReloadConf;

    /** App status: the configuration needs to be reloaded */
    bool _configurationToBeReloaded;

    /** Pointer to the rolling policy manager */
    RollingPolicyManager::Sptr _rollingPolicyManagerSptr;

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingArchive)

};

_ACS_END_NAMESPACE

#endif //_RollingArchive_H_

