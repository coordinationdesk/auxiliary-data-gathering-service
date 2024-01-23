// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Systematic Order Manager $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/02/06 16:20:36  marfav
	CSGACQ-112
	Perform periodically the waiting order cleanup to
	purge expired records
	
	Revision 5.2  2016/03/21 10:10:02  marpas
	adapting to last dev environment.
	now relies on HAS_MISSION_INCLUDE
	
	Revision 5.1  2014/09/19 12:11:05  marfav
	Added support to configurable restart time
	
	Revision 5.0  2013/06/19 14:33:35  marpas
	adoption of libException 5.x standards
	
	Revision 2.10  2013/05/12 14:39:44  marpas
	no longer master/slave check
	
	Revision 2.9  2013/03/14 12:36:59  marpas
	connection pool concept moved in classes derived from Application
	
	Revision 2.8  2013/02/13 18:23:25  marpas
	Connection Pool concept adopted
	application flow completely rewritten
	some methods moved into libOrderCreator (new class OrderExecutor)
	coding best practices applied
	qa rules
	
	Revision 2.7  2013/01/11 13:32:12  marfav
	Addedd support to external appname definition
	
	Revision 2.6  2012/03/16 08:26:41  marpas
	alignement for std app
	
	Revision 2.5  2012/02/09 17:32:14  marpas
	refactogring in progress
	
	Revision 2.4  2008/11/21 18:56:01  marpas
	ConfigurationSingleton new mode added
	
	Revision 2.3  2008/09/22 09:08:51  danalt
	added HAS_MISSION includes
	
	Revision 2.2  2007/04/18 14:00:56  marpas
	arguments improved, now supports -simul -help
	
	Revision 2.1  2006/06/09 08:40:39  giucas
	Because plugin, the very first loadConfiguration is invoked BEFORE the daemonize call. This hangs the process
	if no master database is found. So, the very first loadConfiguration should be smoother as for the SDFArbiter.
	
	Revision 2.0  2006/02/28 15:49:11  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/10/06 15:43:25  marfav
	Plugin support added
	Orders creator moved to specialized libraries
	
	Revision 1.10  2005/06/22 09:28:17  enrcar
	namespace std handling added
	
	Revision 1.9  2005/02/14 20:30:18  marpas
	headers fixed
	
	Revision 1.8  2004/09/06 09:10:20  danalt
	Cosmo MultiMission Environment Support Added
	
	Revision 1.7  2004/04/08 16:31:58  paoscu
	StationApp supported
	
	Revision 1.6  2003/08/01 12:41:32  marpas
	messages upgraded
	
	Revision 1.5  2003/08/01 11:47:32  marpas
	restart implemented
	reload configuration implemented
	-cap argument to know order processing capabilities
	
	Revision 1.4  2003/06/04 19:33:53  marpas
	V1 test.
	
	Revision 1.3  2003/03/13 17:32:01  marpas
	some improvements
	
	Revision 1.2  2003/02/18 16:57:23  marpas
	test in progress
	
	Revision 1.1.1.1  2003/02/04 18:47:30  marpas
	Import Order Generator
	

*/


#ifndef _OrderGenerator_H_
#define _OrderGenerator_H_ 

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#include <ProjectAppIncludes>

#include <PluginLoader.h>

_ACS_BEGIN_NAMESPACE(acs)

class OGListener ;
	
class OrderGenerator : // PRQA S 2109, 2153

	public PROJECT_APP
{		
public:
	OrderGenerator( const std::string&, const std::string& );
	virtual ~OrderGenerator() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);


	// inherited from Signal::DefaultSignalHandler
	virtual void customSigIntHandler() ;
	virtual void customSigTermHandler() ;
	virtual int usage(const std::string &) const ;

private:
	// declared but not implemented 
	OrderGenerator();
	OrderGenerator(const OrderGenerator&);
	OrderGenerator & operator=(const OrderGenerator&);

	std::string loadPlugins();
	bool loadConf();
	void doWork() const ;
    void mainLoop() ;
    void waitLoop(unsigned long secs, OGListener *listener=0) ;
    int loadFirstConf() ;
    void debugException() ;
    void writeMsgIfStopped() ;
	void performDbMaintenance() const ;

private:
	PluginLoader _pluginLoader;
	unsigned long _pollingTime;
	pthread_t _mainThread ;
    bool _warning ;
    unsigned int _restartTime;
	unsigned int _dbMaintenancePeriod; 

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderGenerator) ;

public:

    static const unsigned int default_pollingTime_ = 300 ;
    static const unsigned int _defaultRestartTime=3600*24 ; // [sec] (one day)
	static const unsigned int _defaultMaintenancePeriod = 60*5; // [sec] five minutes 

    // Root key for configuration spaces navigation
    static const std::string _ordGenRootKey ;   // PRQA S 2100 

    // Configuration spaces keys for values 
    static const std::string _pluginsPathKey ;  // PRQA S 2100 
    static const std::string _pluginRegExp ;    // PRQA S 2100 

};

_ACS_END_NAMESPACE

#endif /* _OrderGenerator_H_ */
