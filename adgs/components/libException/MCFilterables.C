// PRQA S 1050 EOF
/*

	Copyright 2015-2021, Exprivia SpA, Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.22  2018/09/27 13:21:09  marpas
	missing include
	
	Revision 5.21  2018/08/28 09:43:58  davide.tiriticco
	Added new metrics WARN, REF and TAG
	
	Revision 5.20  2017/05/10 11:56:10  danalt
	added new Metric_CONTACT
	
	Revision 5.19  2016/01/13 09:15:47  giucas
	Typo fixed
	
	Revision 5.18  2016/01/13 07:37:16  giucas
	New metric types added
	
	Revision 5.17  2016/01/08 15:20:19  lucio.pulvirenti
	APF-270: Metric_LOAD and Metric_UNLOAD added.
	
	Revision 5.16  2015/12/14 09:58:05  lucio.pulvirenti
	APF-270: Metric_UNLINK added.
	
	Revision 5.15  2015/12/01 15:49:24  marfav
	Updating metrics
	
	Revision 5.14  2015/11/26 13:25:08  lucio.pulvirenti
	APF-270: Metric_SMUPLOAD removed: used code SM with IN/OUT instead. Metric_PACKAGE turned into Metric_LINK.
	
	Revision 5.13  2015/11/24 10:37:05  marpas
	introducing mc_null_param() convenience function to insert a nop param - this is useful as ternary operator alternative
	
	Revision 5.12  2015/11/12 15:52:29  lucio.pulvirenti
	APF-270/273: metric types added.
	
	Revision 5.11  2015/10/23 08:16:29  marpas
	coding best practices applied, moreover, number of stl find algorithm calls reduced by 2 in multithread cases
	
	Revision 5.10  2015/09/23 13:15:23  giucas
	New Metric types added
	
	Revision 5.9  2015/08/17 13:03:21  marfav
	Adding DC and EXP metrics
	
	Revision 5.8  2015/08/17 07:54:47  marfav
	Added DELETE metric
	
	Revision 5.7  2015/08/13 15:25:51  marfav
	Adding function per thread configurability
	
	Revision 5.6  2015/08/12 16:00:09  marfav
	Adding new message metrics
	
	Revision 5.5  2015/08/12 09:02:24  marfav
	MCF messages format implemented
	Added the code per thread in the singleton
	
	Revision 5.4  2015/06/26 18:34:19  marpas
	introducing new MC framework
	
	Revision 5.3  2015/06/24 16:09:45  marpas
	coding best practice application in progress
	
	Revision 5.2  2015/06/24 16:04:00  marpas
	M&C logging framework work in progress
	
	Revision 5.1  2015/06/23 15:52:36  marpas
	introducing new M&C logging framework - still to be integrated into the std framework
	
	

*/

#include <MCFilterables.h>
#include <Filterables.h>
#include <algorithm>

_ACS_BEGIN_NESTED_NAMESPACE(acs, mc) 
using std::string ;
using std::map ;
using std::exception ;

MCParam::MCParam(const string &name, const string &values, const string &units) :
    name_ (name),
    values_ (values),
    units_(units) 
{
}

MCParam::~MCParam() ACS_NOEXCEPT {}

MCParam::MCParam(const MCParam &r) :
    name_ (r.name_),
    values_ (r.values_),
    units_(r.units_) 
{
}

MCParam &MCParam::operator=(const MCParam &r) 
{
    if (this != &r) {
        name_ = r.name_ ;
        values_  = r.values_ ;
        units_ = r.units_ ;
    }
    return *this ;
}

string MCParam::text() const // PRQA S 4020
{
	if (name_.empty() && values_.empty() && units_.empty()) { return "" ; }
    return name_ + '=' + values_ + (units_.empty() ? string("") : (':' + units_)) ; // PRQA S 3081, 3383, 3385
}

MCScopedParams::MCScopedParams() :
    params_(),
    prev_metric_(),
    prev_code_(),
	prev_function_ ()
{
    MCParameters::instance()->add(this) ;
    prev_metric_ = MCParameters::instance()->getMetric() ;
    prev_code_ = MCParameters::instance()->getCode();
    prev_function_ = MCParameters::instance()->getFunction();
}

MCScopedParams::~MCScopedParams() ACS_NOEXCEPT 
{
    // PRQA S 4631 L1
    try {
        MCParameters::instance()->rem(this) ;
        MCParameters::instance()->setMetric(prev_metric_) ;
        MCParameters::instance()->setCode(prev_code_) ;
        MCParameters::instance()->setFunction(prev_function_) ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }    
    // PRQA L:L1
}

MCScopedParams & MCScopedParams::add(const MCParam &p) 
{
    params_.push_back(p) ;
    return *this ;
}

MCScopedParams & MCScopedParams::add(const MCMetric &m) 
{
    MCParameters::instance()->setMetric(m.metric()) ;
    return *this ;
}

MCScopedParams & MCScopedParams::add(const MCCode &c) 
{
    MCParameters::instance()->setCode(c.code()) ;
    return *this ;
}

MCScopedParams & MCScopedParams::add(const MCFunction &f) 
{
    MCParameters::instance()->setFunction(f.function()) ;
    return *this ;
}

namespace {
    class concat_p { // PRQA S 2109
    public:
        concat_p(string &s, char sep) : res_(s), sep_(sep) {} // PRQA S 2528
        void operator()(const MCParam &p) {
			const string &txt = p.text() ;
            if (not res_.empty() && not txt.empty()) {
                res_ += sep_ ;
            }
            res_ += txt ;
        }
    private:
        concat_p() ; // not implemented
    private:
        string &res_ ;
        char sep_ ;
    } ;
}

string MCScopedParams::text(char sep) const 
{
	string res;
    for_each(params_.begin(), params_.end(), concat_p(res, sep)) ;
    return res ;
}

MCScopedParams &operator<<(MCScopedParams &sp, const MCParam &p) // PRQA S 4222
{
    return sp.add(p) ;
}

MCScopedParams &operator<<(MCScopedParams &sp, const MCMetric &m) // PRQA S 4222
{
    return sp.add(m) ;
}

MCScopedParams &operator<<(MCScopedParams &sp, const MCCode &c) // PRQA S 4222
{
    return sp.add(c) ;
}

MCScopedParams &operator<<(MCScopedParams &sp, const MCFunction &f) // PRQA S 4222
{
    return sp.add(f) ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(MCParameters) 

MCParameters::MCParameters() :
    pattern::Singleton<MCParameters>(),
    P_vs_Th_(),
    default_mc_function_(),
    default_mc_code_(),
	metric_types_map_(),
    M_vs_Th_(),
	C_vs_Th_(),
	F_vs_Th_()
{
	init();
}

MCParameters::~MCParameters() ACS_NOEXCEPT {}

void MCParameters::init()
{
	setMetricType (Metric_UNDEFINED, ""); // Undefined metric will be an empty field
	setMetricType (Metric_IN,        "IN");
	setMetricType (Metric_OUT,       "OUT");
	setMetricType (Metric_DC_OUT,    "OUT");
	setMetricType (Metric_EXP_OUT,   "OUT");
	setMetricType (Metric_RATE,      "RATE");
	setMetricType (Metric_RESULT,    "RESULT");
	setMetricType (Metric_FAIL,      "FAIL");
	setMetricType (Metric_DC_FAIL,   "DC_FAIL");
	setMetricType (Metric_EXP_FAIL,  "EXPORT_FAIL");
	setMetricType (Metric_RUNNING,   "RUNNING");
	setMetricType (Metric_RUNNING1,  "RUNNING1");
	setMetricType (Metric_RUNNING2,  "RUNNING2");
	setMetricType (Metric_LOG,       "LOG");
	setMetricType (Metric_DELETE,    "DELETE");
	setMetricType (Metric_DCD,       "DCD");
	setMetricType (Metric_RATE1,     "RATE1");
	setMetricType (Metric_STATS,     "STATS");
	setMetricType (Metric_STATS1,    "STATS1");
	setMetricType (Metric_STATS2,    "STATS2");
	setMetricType (Metric_STATS3,    "STATS3");
	setMetricType (Metric_STATS4,    "STATS4");
	setMetricType (Metric_STATS5,    "STATS5");
	setMetricType (Metric_STATS6,    "STATS6");
	setMetricType (Metric_STATS7,    "STATS7");
	setMetricType (Metric_STATS8,    "STATS8");
	setMetricType (Metric_STATS9,    "STATS9");
	setMetricType (Metric_STATS10,   "STATS10");
	setMetricType (Metric_LINK,   	 "LINK");
	setMetricType (Metric_PROGRESS,  "PROGRESS");
	setMetricType (Metric_UNLINK,    "UNLINK");
	setMetricType (Metric_LOAD,      "LOAD");
	setMetricType (Metric_UNLOAD,    "UNLOAD");
	setMetricType (Metric_PREDICT,   "PREDICT");
	setMetricType (Metric_NEXT,      "NEXT");
	setMetricType (Metric_CONTACT,   "CONTACT");
	setMetricType (Metric_WARN,      "WARN");
	setMetricType (Metric_REF,       "REF");
	setMetricType (Metric_TAG,       "TAG");
	setMetricType (Metric_REQUEST,   "REQUEST");
	setMetricType (Metric_DOWNLOAD,  "DOWNLOAD");
	setMetricType (Metric_DROPREQ,   "DROPREQ");
	setMetricType (Metric_CHECK,   	 "CHECK");
}

void MCParameters::add(MCScopedParams *p) 
{
    Lock const l_(_M_mutex()); // lock
    P_vs_Th_[pthread_self()].push_back(p) ;
}

void MCParameters::rem(const MCScopedParams *p) 
{
    Lock const l_(_M_mutex()); // lock
    MCThreadParameters &params = P_vs_Th_[pthread_self()] ;
    MCThreadParameters::iterator sp = find (params.begin(), params.end(), p) ;
    if (sp != params.end()) {
        params.erase(sp) ;
    }
    
}

void MCParameters::rem(pthread_t thread) 
{
    Lock const l_(_M_mutex()); // lock
    
    P_vs_Th_.erase(thread) ; 
    M_vs_Th_.erase(thread) ;
}

namespace {
    class concat_sp { // PRQA S 2109
    public:
        concat_sp(string &s, char sep) : res_(s), sep_(sep) {} // PRQA S 2528
        void operator()(const MCScopedParams *p) {
            if (p) {
                const string &txt = p->text(sep_) ;
                if (not res_.empty() && not txt.empty()) {
                    res_ += sep_ ;
                }
                res_ += txt;
            }
        }
    private:
        concat_sp() ; // not implemented
    private:
        string &res_ ;
        char sep_ ;
    } ;
}

string MCParameters::text(char sep) const 
{
    Lock const l_(_M_mutex()); // lock
    map<pthread_t, MCThreadParameters>::const_iterator currTh_spi = P_vs_Th_.find(pthread_self()) ;
    string res ;
    if (currTh_spi != P_vs_Th_.end()) { // always: current thread should have at least a scoped parameter
        const MCThreadParameters &params = currTh_spi->second ;
        for_each(params.begin(), params.end(), concat_sp(res, sep)) ;
    }
    return res ;
}

void MCParameters::setDefaultCode(const string &c)
{
    Lock const l_(_M_mutex()); // lock
    default_mc_code_ = c ;
}

void MCParameters::setCode(const string &c)
{
    Lock const l_(_M_mutex()); // lock
    C_vs_Th_[pthread_self()] = c;
}

string MCParameters::getCode() const // PRQA S 4020
{
    Lock const l_(_M_mutex()); // lock
    map<pthread_t, string>::const_iterator i = C_vs_Th_.find(pthread_self()) ;
	if (i != C_vs_Th_.end())
	{
		return i->second;
	}
	else
	{
	    return default_mc_code_ ; 
	}
}

void MCParameters::setDefaultFunction(const string &c)
{
    Lock const l_(_M_mutex()); // lock
    default_mc_function_ = c ;
}

void MCParameters::setFunction(const string &f)
{
    Lock const l_(_M_mutex()); // lock
    F_vs_Th_[pthread_self()] = f;
}

string MCParameters::getFunction() const // PRQA S 4020
{
    Lock const l_(_M_mutex()); // lock
    map<pthread_t, string>::const_iterator i = F_vs_Th_.find(pthread_self()) ;
	if (i != F_vs_Th_.end())
	{
		return i->second;
	}
	else
	{
	    return default_mc_function_ ; 
	}
}


void MCParameters::setMetricType (MCMetricType mt, const string& s)
{
    Lock const l_(_M_mutex()); // lock
	metric_types_map_ [mt] = s;
}

void MCParameters::setMetricTypes (const map <MCMetricType, string> & m)
{
    Lock const l_(_M_mutex()); // lock
	metric_types_map_ = m;
}

std::map <MCMetricType, std::string>  MCParameters::getMetricTypes () const 
{
    Lock const l_(_M_mutex()); // lock
    return metric_types_map_ ;
}


string MCParameters::getMetricDefinition() const
{
    Lock const l_(_M_mutex()); // lock
	MCMetricType m = getMetric();

	string definition("");

	if ( metric_types_map_.find (m) != metric_types_map_.end())
	{
		// A definition exists. Return it
		definition = metric_types_map_.find(m)->second;
	}
	
	return definition;
}



void MCParameters::setMetric(MCMetricType m) 
{
    Lock const l_(_M_mutex()); // lock
    if  ( (Metric_UNDEFINED != m ) && (metric_types_map_.find(m) == metric_types_map_.end()) )
	{
        M_vs_Th_[pthread_self()] = Metric_UNDEFINED ;
        ACS_LOG_WARNING("Unknown definition for the metric id: " << int(m) << ". Metric is now UNDEFINED" ) ; // PRQA S 3081
    }
    else {
        M_vs_Th_[pthread_self()] = m ;
    }
}

MCMetricType MCParameters::getMetric() const // PRQA S 4020
{
    Lock const l_(_M_mutex()); // lock
    map<pthread_t, MCMetricType>::const_iterator currTh_m_i = M_vs_Th_.find(pthread_self()) ;
    if (currTh_m_i == M_vs_Th_.end()) {
        return Metric_UNDEFINED ;
    }
    return currTh_m_i->second ;
}



_ACS_END_NESTED_NAMESPACE
