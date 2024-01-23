/*

	Copyright 2015-2022, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
    
	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.23  2018/08/28 09:43:58  davide.tiriticco
	Added new metrics WARN, REF and TAG
	
	Revision 5.22  2017/05/10 11:56:10  danalt
	added new Metric_CONTACT
	
	Revision 5.21  2016/01/13 07:37:16  giucas
	New metric types added
	
	Revision 5.20  2016/01/08 15:20:19  lucio.pulvirenti
	APF-270: Metric_LOAD and Metric_UNLOAD added.
	
	Revision 5.19  2015/12/14 09:58:05  lucio.pulvirenti
	APF-270: Metric_UNLINK added.
	
	Revision 5.18  2015/12/01 16:06:19  marpas
	qa warning fixed
	
	Revision 5.17  2015/12/01 15:49:24  marfav
	Updating metrics
	
	Revision 5.16  2015/11/26 13:25:08  lucio.pulvirenti
	APF-270: Metric_SMUPLOAD removed: used code SM with IN/OUT instead. Metric_PACKAGE turned into Metric_LINK.
	
	Revision 5.15  2015/11/24 10:38:32  marpas
	qa warning fixed
	
	Revision 5.14  2015/11/24 10:37:05  marpas
	introducing mc_null_param() convenience function to insert a nop param - this is useful as ternary operator alternative
	
	Revision 5.13  2015/11/12 15:52:29  lucio.pulvirenti
	APF-270/273: metric types added.
	
	Revision 5.12  2015/10/23 08:00:52  marpas
	qa warnings removed
	
	Revision 5.11  2015/09/23 13:15:23  giucas
	New Metric types added
	
	Revision 5.10  2015/08/17 13:03:21  marfav
	Adding DC and EXP metrics
	
	Revision 5.9  2015/08/17 07:54:47  marfav
	Added DELETE metric
	
	Revision 5.8  2015/08/13 15:25:51  marfav
	Adding function per thread configurability
	
	Revision 5.7  2015/08/12 16:00:09  marfav
	Adding new message metrics
	
	Revision 5.6  2015/08/12 09:02:24  marfav
	MCF messages format implemented
	Added the code per thread in the singleton
	
	Revision 5.5  2015/06/30 09:31:35  marpas
	MC_xxx filterables i/f aligned to exFMCInfo in order to support params and metrci inserters with the ACS_LOG_MC_xxx macros
	
	Revision 5.4  2015/06/26 18:34:19  marpas
	introducing new MC framework
	
	Revision 5.3  2015/06/24 16:09:45  marpas
	coding best practice application in progress
	
	Revision 5.2  2015/06/24 16:04:00  marpas
	M&C logging framework work in progress
	
	Revision 5.1  2015/06/23 15:52:36  marpas
	introducing new M&C logging framework - still to be integrated into the std framework
	
	

*/

#ifndef _MCFilterables_H_
#define _MCFilterables_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, mc) 


// Remember to keep aligned the MCParameters::init() with the default string values
enum MCMetricType { // NOSONAR - enum is absolutely enough
    Metric_UNDEFINED = 0, 
    Metric_IN ,
    Metric_OUT ,
	Metric_DC_OUT,
	Metric_EXP_OUT,
    Metric_RATE ,
    Metric_RESULT ,
    Metric_FAIL ,
    Metric_DC_FAIL ,
    Metric_EXP_FAIL ,
	Metric_RUNNING ,
	Metric_RUNNING1 ,
	Metric_RUNNING2 ,
	Metric_LOG ,
	Metric_DELETE ,
    Metric_DCD,
    Metric_RATE1 ,
    Metric_STATS ,
    Metric_STATS1 ,
    Metric_STATS2 ,
    Metric_STATS3 ,
    Metric_STATS4 ,
    Metric_STATS5 ,
    Metric_STATS6 ,
    Metric_STATS7 ,
    Metric_STATS8 ,
    Metric_STATS9 ,
    Metric_STATS10 ,
	Metric_LINK,
	Metric_PROGRESS,
	Metric_UNLINK,
	Metric_LOAD,
	Metric_UNLOAD,
	Metric_PREDICT,
	Metric_NEXT,
	Metric_CONTACT,
	Metric_WARN,
	Metric_REF,
	Metric_TAG,
	Metric_REQUEST,
	Metric_DOWNLOAD,
	Metric_DROPREQ,
	Metric_CHECK,
    // ...
    _Metric_MAX_ // define new metrics BEFORE this
} ;

class MCMetric { // PRQA S 2109
public:
    MCMetric() = delete ;
    explicit MCMetric(MCMetricType m) : metric_(m) {} 
    ~MCMetric() = default ;
    MCMetricType metric() const noexcept { return metric_ ; }
private: 
    MCMetricType metric_ ;  
} ;

class MCCode { // PRQA S 2109
public:
    MCCode() = delete ;
    explicit MCCode(const std::string& s) : code_(s) {} 
    ~MCCode() = default ;
    std::string code() const noexcept { return code_ ; }
private: 
    std::string code_ ;  
};


class MCFunction { // PRQA S 2109
public:
    MCFunction() = delete ;
    explicit MCFunction(const std::string& f) : function_(f) {} 
    ~MCFunction() = default ;
    std::string function() const noexcept { return function_ ; }
private: 
    std::string function_ ;  
};

class MCParam { // PRQA S 2109
public:
	MCParam() = delete ;
	MCParam(const std::string &name, const std::string &values, const std::string &units="") ; // PRQA S 4634
    ~MCParam() = default ;

    std::string text() const ;
    
private:
    std::string name_ ;
    std::string values_ ;
    std::string units_ ; 
} ;

using MCParams = std::list<MCParam> ;

/*! \brief the MCScopedParams class
 * purpose of the class is to collect a set of M&C parameters registering to the global 
 * singleton who collects parameters for each thread
 */
 
class MCScopedParams { // PRQA S 2109
public:
	MCScopedParams() ;
    MCScopedParams(const MCScopedParams &) = delete ;
    MCScopedParams& operator=(const MCScopedParams &) = delete ;
    virtual ~MCScopedParams() noexcept ;
    MCScopedParams & add(const MCParam &) ;
    MCScopedParams & add(const MCMetric &) ; 
    MCScopedParams & add(const MCCode &) ; 
    MCScopedParams & add(const MCFunction &) ; 
   
    std::string text(char sep = '|') const ;

private:
    MCParams params_ = {} ;
    MCMetricType prev_metric_ = {} ;
	std::string prev_code_ = "" ;
	std::string prev_function_ = "" ;
} ;


/*! MCScopedParams manipulator style convenience inserter */
MCScopedParams &operator<<(MCScopedParams &, const MCParam &) ; // PRQA S 2072
MCScopedParams &operator<<(MCScopedParams &, const MCMetric &) ; // PRQA S 2072
MCScopedParams &operator<<(MCScopedParams &, const MCCode &) ; // PRQA S 2072
MCScopedParams &operator<<(MCScopedParams &, const MCFunction &) ; // PRQA S 2072


inline
MCMetric mc_metric(MCMetricType value) // PRQA S 2134
{
    return MCMetric(value) ; // PRQA S 3081
}



template <typename T>
inline
MCParam mc_param(const std::string &name, const T &value, const std::string &units="")  // PRQA S 2134, 4634
{
    std::ostringstream os ; 
    os << value ;
    return MCParam(name, os.str(), units) ;
}

template <>
inline
MCParam mc_param<std::string>(const std::string &name, const std::string &value, const std::string &units) // PRQA S 2134 
{
    return MCParam(name,'"' + value + '"', units) ;
}

inline
MCParam mc_null_param() // PRQA S 2134
{
    return MCParam("", "", "") ;
}

inline
MCParam mc_param(const std::string &name, const std::vector<std::string> &v, const std::string &units="")  // PRQA S 2134, 4634
{
    std::ostringstream os ;
    size_t v_size = v.size() ;
    for (size_t i=0; i < v_size; ++ i) {
        if (i > 0) { os << ',' ; }
        os << '"' << v[i] << '"' ;
    }
    return MCParam(name, os.str(), units) ;    
}

template <typename T>
inline
MCParam mc_param(const std::string &name, const std::vector<T> &v, const std::string &units="")  // PRQA S 2134, 4634
{
    std::ostringstream os ;
    size_t v_size = v.size() ;
    for (size_t i=0; i < v_size; ++ i) {
        if (i > 0) { os << ',' ; }
        os << v[i] ;
    }
    return MCParam(name, os.str(), units) ;    
}

inline
MCParam mc_param(const std::string &name, const char *value, const std::string &units="")  // PRQA S 2134, 4634
{
    return mc_param(name, std::string(value), units) ; // PRQA S 2839, 3081
}

using MCThreadParameters = std::list<MCScopedParams *> ;

/*! \brief the main MCParameters class (singleton)
    collects MC parameters on thread basis
 */
class MCParameters : // PRQA S 2109,2153
	public pattern::Singleton<MCParameters>
{
// friendship needed in order to construct MCParameters from a singleton see Singleton doc
friend MCParameters *pattern::Singleton< MCParameters>::instance(); // PRQA S 2107

public:	
    MCParameters(const MCParameters &) = delete ;
    MCParameters& operator=(const MCParameters &) = delete ;

	~MCParameters() override = default ;
    /*! add the istance of the scoped parameters from the map (when it is created)
     */
    void add(MCScopedParams *) ;
    /*! removes the istance of the scoped parameters from the map (when it goes out of scope)
     */
    void rem(const MCScopedParams *) ;

    /*! removes the thread from the map
     */
    void rem(pthread_t ) ;  // PRQA S 2020
    void setDefaultCode(const std::string &) ; 
    void setCode(const std::string &) ; 
    std::string getCode() const ;
	void setDefaultFunction(const std::string& );
    void setFunction(const std::string &) ; 
    std::string getFunction() const ;
    std::string text(char sep = '|') const ;
	void setMetricType (MCMetricType, const std::string&);
	void setMetricTypes (const std::map <MCMetricType, std::string> &);
	std::map <MCMetricType, std::string> getMetricTypes () const ;
    void setMetric(MCMetricType m) ;
    MCMetricType getMetric() const ;
	std::string getMetricDefinition() const;

protected:
	MCParameters() ;

	void init();
    
private:
    /*! this is the thread vs parameters map
     */ 
    std::map<pthread_t, MCThreadParameters> P_vs_Th_ = {} ; 
    std::string default_mc_function_ = "" ; 
    std::string default_mc_code_ = "" ;  
	std::map <MCMetricType, std::string> metric_types_map_ = {} ;
    std::map<pthread_t, MCMetricType> M_vs_Th_ = {} ;
	std::map<pthread_t, std::string> C_vs_Th_ = {} ;
	std::map<pthread_t, std::string> F_vs_Th_ = {} ;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MCParameters) ;
} ;


_ACS_END_NESTED_NAMESPACE

#include <exFMCFilterables.h> // NOSONAR

#endif // _MCFilterables_H_
