// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/09/27 11:23:05  lucio.pulvirenti
	S2PDGS-1843: methods added to add filepaths containing conf spaces to override the configuration. file paths must be loaded at the end.
	
	Revision 5.2  2013/10/07 14:34:53  marpas
	timeoutOrStop doc fixed
	
	Revision 5.1  2013/07/24 19:40:03  marpas
	new interface to reduce latency times from applications
	
	Revision 5.0  2013/06/18 10:14:18  marpas
	adoption of libException 5.x standards
	
	Revision 2.20  2013/05/14 18:00:06  marpas
	Singleton release at exit implemented
	
	Revision 2.19  2013/04/11 15:01:52  marpas
	SpaceLoadMode can be LoadStealth its effect is as in immediate BUT no callback will be invoked
	
	Revision 2.18  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.17  2013/01/25 11:28:55  marpas
	using pattern::Singleton from libException
	
	Revision 2.16  2013/01/24 19:23:54  marpas
	operator= is forbidden because the reference inside class
	operator() added
	
	Revision 2.15  2013/01/24 16:55:43  marpas
	useless macros removed
	minor improvements
	
	Revision 2.14  2012/12/02 19:36:06  marpas
	qa warning
	
	Revision 2.13  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.12  2012/07/27 14:54:18  marfav
	Adding patchResourceSet method to force static/private update on the resource set
	
	Revision 2.11  2012/07/27 13:07:41  marfav
	Statit settings management added
	
	Revision 2.10  2012/03/07 13:33:39  marfav
	Using Singleton pattern
	
	Revision 2.9  2012/03/07 11:34:57  marpas
	singleton rewritten
	
	Revision 2.8  2012/02/28 16:19:33  marpas
	callback concept implemented
	callbacks can be registered, they will be called when configuration is loaded (even for a single space)
	
	Revision 2.7  2012/02/27 17:25:20  marpas
	ConfigurationSingleton can safely be used before main
	
	Revision 2.6  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.5  2009/07/01 16:38:59  marpas
	robustness improved: the ConfigurationSingleton is allocated statically and no longer dynamically when calling istance()
	The above method alwasy returns the statically allocated object.
	Interface has no changed
	
	Revision 2.4  2008/12/05 22:52:28  marpas
	useless include removed
	
	Revision 2.3  2008/11/13 19:48:36  marpas
	mergePrivate method added
	
	Revision 2.2  2008/11/13 13:47:04  marpas
	StopPredicate introduced
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.2  2003/04/30 10:20:24  marpas
	using namespace std was removed from includes
	
	Revision 1.1  2002/11/12 13:58:35  marpas
	ConfigurationSingleton class added w/ test
	

*/


#ifndef _ConfigurationSingleton_H_
#define _ConfigurationSingleton_H_ 


#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <rsResourceSet.h>
#include <string>
#include <vector>
#include <utility>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

//
//
// class ConfigurationSingleton
//
//

class ConfigurationSingleton : public Singleton <ConfigurationSingleton> // PRQA S 2109, 2153
{
    friend ConfigurationSingleton* Singleton<ConfigurationSingleton>::instance(); // PRQA S 2107
public:
	/*! class Stopped declaration */
	exDECLARE_EXCEPTION(Stopped,exException) ; // PRQA S 2131, 2502

	class ConfSpace { // PRQA S 2109
	public:
		explicit ConfSpace(const std::string &, const std::string & = "", bool mandatory=true) ;
		explicit ConfSpace(const std::pair<std::string, std::string> &, bool mandatory=true) ;
		~ConfSpace() throw() ;
		ConfSpace(const ConfSpace &) ;
		ConfSpace &operator=(const ConfSpace &) ;
		const std::string &name() const throw() ;
		const std::string &version() const throw() ;
		bool mandatory() const throw() ;
	private:
		ConfSpace() ; // unimplmented
	private:
		std::string _name ;
		std::string _version ;
		bool _mandatory ;
	} ;

	/*! This class is used to interrupt the ConfigMerger::getResources method.
		It needs to be inherited and the method evalCondition to be reimplemented.
	*/
	class StopPredicate { // PRQA S 2109
	public:
		
		/*! default ctor, will use the reimplemented evalCondition() method to evaluate the predicate.
		*/
		StopPredicate() ;
		/*! convenience ctor, will use the  referenced boolean to evaluate the predicate.
		    \note: the referenced variable must not go out of scope until the ConfigMerger::getResources is running.
		*/
		explicit StopPredicate(volatile bool &stopCondition) ;
		virtual ~StopPredicate() throw() ;
		StopPredicate(const StopPredicate &) ;
		bool eval() const throw () ;
        bool operator()() const throw() { return eval() ; } // PRQA S 4214
        /**
           * Wait for the timeout or stop. Returns true if stop was requested, false if timeout expired
           *
           * \param msec the timeout.
           * \param remainder the remainder time if cancelled.
           * \param usecslice the slices after then check the cancel request - default 1e6 i.e. 1ms
           */
	    bool timeoutOrStop(size_t msec, size_t *remainder = 0, size_t nsecslice=1000000) const ; // PRQA S 4400
        static unsigned long nanosleep(unsigned long nanosec) ; // PRQA S 2502
        
	protected:
		/*! This method will interrupt the ConfigMerger::getResources if it returns true.
			\note the method must be reimplemented as the default one returns always false.
			\note No exception has to be thrown out of the method.
		*/
		virtual bool evalCondition() const throw () ;

    private:
		StopPredicate &operator=(const StopPredicate &) ; // not implemented because the reference
	
	private:
		bool _simpleExternConditionGiven ;
		volatile bool &_simpleExternCondition ;
		ACS_CLASS_DECLARE_DEBUG_LEVEL(StopPredicate) ;
	} ;

public:
	virtual ~ConfigurationSingleton() throw() ; 
	
    /*! to define loading time of various ConfSpaces \see addSpace, addOverrideFilepath */
	enum SpaceLoadMode { 
            /*! waits first chance, unless something has been loaded */
		LoadLazy, 
            /*! loads now */
		LoadImmediate, /** loads now */
/* the following attribute is deprecated
        LoadStealth, // DEPRECATED as immediate but do not invoke any callback
*/
            /*! do not load now */
		NoLoad=LoadImmediate+2 
	} ;

    static __attribute__ ((deprecated)) const SpaceLoadMode LoadStealth = SpaceLoadMode(LoadImmediate + 1);
	
    
    typedef void (*ConfSpaceLoadCallback )(void *) ;
    typedef std::pair<ConfSpaceLoadCallback, void*> Callback ;

    class InvokeCallback {
    public:
        InvokeCallback() throw() {} 
        void operator()( const Callback &c) throw() ;
    } ;
    
    
	/*! \brief adds post-load callbacks
     * \param f the callback to call if f is not null, after any change of the configuration (load any space) the callback will be called 
     * \param data passed to the callback
     * 
     * no memory management is foreseen 
	 */
    void addCallback(ConfSpaceLoadCallback f = 0, void *data=0) ;


	/* \brief adds a configuration space
	 * \returns true if triggered a loading
 	 */
	bool addSpace(const ConfSpace &, 
                  SpaceLoadMode rmode = LoadLazy, 
                  const StopPredicate &stop = StopPredicate()) ;

	/*
	 * returns true if triggered a loading
	 */
	bool addOverrideFilepath(const std::string &, 
                  SpaceLoadMode rmode = LoadLazy, 
                  const StopPredicate &stop = StopPredicate()) ;
	
	void loadConfiguration(const StopPredicate &stop = StopPredicate()) ;
	const std::vector< std::pair <std::string, std::string> > &getReplacements() const ;
	
	void set(const rsResourceSet &) ;
	/* returns the real conf set do not change it because next loadConfiguration will revert to previous state. use the getPrivate instead*/
	rsResourceSet &get() ;
	/* returns the private set that will be merged into the final one after load*/ 
	rsResourceSet &getPrivate() ;
	bool setHostnameSpaceMandatory(bool) ; // default is false returns old state 

	/* true if added */
	static bool addOrReplaceSpace(const ConfSpace&, std::vector<ConfSpace> &v) ;

	static bool addOrReplaceReplacements(const std::pair <std::string, std::string> &, std::vector< std::pair <std::string, std::string> >&) ;

	/* true if added */
	static bool addOrReplaceFilepath(const std::string &, std::vector<std::string> &v) ;
	
    /* calls all the callbacks 
    */
    void confLoaded() const ; 
    
    // Update the resource set merging the static and the private settings
    void patchResourceSet();
	
protected:
	ConfigurationSingleton() ; 
	void loadSingleSpace(const ConfSpace &s, const StopPredicate &stop, bool invokeCallbacks) ;
	void loadSingleOverrideFilepath(const std::string &, const StopPredicate &stop, bool invokeCallbacks) ;
	void addReplacements(const std::vector< std::pair <std::string, std::string> > &) ;
	void addReplacements(const std::pair <std::string, std::string> &) ;

private:
    void detectStaticSettings(const rsResourceSet&);
    void mergeStaticSettings();
	void mergePrivate() ;
	
	
private:
	ConfigurationSingleton &operator=(const ConfigurationSingleton &) ; 	// declared but not implemented
	ConfigurationSingleton (const ConfigurationSingleton &) ; 				// declared but not implemented

    
private:
	rsResourceSet _confSet ;
	rsResourceSet _privateConfSet ;
	std::vector<ConfSpace> _confSpaces ;
	std::vector< Callback > _callbacks ;
	bool _hostnameSpaceMandatory ;
	std::vector< std::pair <std::string, std::string> > _replacements ;
	bool _spacesLoadedOnce ;
    
    // Contains the set of configurations declared as static
    // and overriding any setting loaded from the configuration spaces
    rsResourceSet _staticConfSet;
	std::vector< std::string > _confFilepaths;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ConfigurationSingleton) ;
} ; 


_ACS_END_NAMESPACE


#endif // _ConfigurationSingleton_H_

