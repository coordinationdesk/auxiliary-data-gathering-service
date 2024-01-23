// PRQA S 1050 EOF
/*
	Copyright 2011-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2017/09/22 11:41:41  marpas
	QA warnings fixed
	
	Revision 5.5  2017/03/22 12:44:24  marpas
	c++0x11 standard adoption in progress
	
	Revision 5.4  2014/06/27 09:42:25  marpas
	comment fixed
	
	Revision 5.3  2013/10/03 18:03:29  marpas
	Statiticssingleton is not released at exit
	
	Revision 5.2  2013/09/26 18:17:44  marpas
	housekeeping, types renaming and cleanup
	
	Revision 5.1  2013/06/05 15:56:21  marpas
	new macros for real time debugging adopted
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.25  2013/05/14 17:57:20  marpas
	Singleton's release at exit for Statistics
	
	Revision 4.24  2013/04/09 07:46:43  marpas
	statistics are now in math namespace
	
	Revision 4.23  2013/04/04 18:34:37  marpas
	Statistics interface improved (reset, zero, zeroTree, resetTree and clearTree)
	
	Revision 4.22  2013/03/25 19:42:10  marpas
	qa warnings
	
	Revision 4.21  2013/02/26 12:50:49  marpas
	Statistics now accepts size_t values
	
	Revision 4.20  2013/01/24 10:45:37  marpas
	the following patterns are new and defined into the acs::pattern namespace:
	Singleton
	InitializableSingleton
	ParamInitializableSingleton
	coverage test added
	
	Revision 4.19  2012/12/11 10:25:52  marpas
	qa rules
	
	Revision 4.18  2012/11/27 12:55:02  marpas
	qa-rules
	
	Revision 4.17  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.16  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.15  2012/06/22 13:41:46  marpas
	pr qa instrumentation
	
	Revision 4.14  2012/06/14 17:16:40  marpas
	qacpp instrumentation
	
	Revision 4.13  2012/06/12 17:37:00  marpas
	qacpp instrumentation
	
	Revision 4.12  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.11  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.10  2012/03/26 14:48:23  marpas
	Singleton implicit operators prohibited
	
	Revision 4.9  2012/03/19 15:49:24  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.8  2012/02/28 11:35:22  marpas
	empty stats are no longer notified
	
	Revision 4.7  2011/12/15 14:23:18  lucio.pulvirenti
	StatNode + operator: mutex added to prevent multi-thread overwrite.
	
	Revision 4.6  2011/10/20 17:55:04  marpas
	improved (1.5 sigma)
	
	Revision 4.5  2011/10/20 10:00:33  marpas
	statistics name column enlarged
	
	Revision 4.4  2011/10/19 10:43:15  marpas
	 statistics accumulation types fixed
	
	Revision 4.3  2011/10/18 17:23:06  marpas
	some display improvements
	
	Revision 4.2  2011/10/18 15:21:26  marpas
	compilation warning removed on 32 bits
	
	Revision 4.1  2011/10/18 14:39:38  marpas
	Stat computation fixed
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.4  2011/09/29 11:15:06  marpas
	signature slightly changed
	
	Revision 3.3  2011/04/29 13:09:55  marpas
	Statistics work in progress
	
	Revision 3.2  2011/04/28 15:09:02  marpas
	Statistics work in progress
	
	Revision 3.1  2011/03/10 18:57:08  marpas
	accumulators work in progress
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	

*/

#ifndef _Statistics_H_
#define _Statistics_H_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <Singleton.hpp>
#include <EventNotifyFactory.h>
#include <exStream.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <limits>
#include <algorithm>
#include <memory>
#include <cmath>


_ACS_BEGIN_NESTED_NAMESPACE(acs, math)



/*! \addtogroup math 
 *  @{
 */

/*! \brief the base class for statistics accumulators */
class AccuBase { // PRQA S 2109,2153 
public: 
	static const unsigned short __indent = 2U ; // PRQA S 2026 EOF
	static const unsigned short __titlew = 48U ;
	static const unsigned short __itemsw = 9U ; // billions
	static const unsigned short __sump = 3U ; // billions
	static const unsigned short __sumw = 9U+1U+__sump ; // billions.xxx // PRQA S 3084 8
	static const unsigned short __meanp = 3U ; // billions
	static const unsigned short __meanw = 9U+1U+__meanp ; // billions.xxx
	static const unsigned short __sigmap = 3U ; // billions
	static const unsigned short __sigmaw = 9U+1U+__sigmap ; // billions.xxx
	static const unsigned short __sgm_alrm = 1U ; // sigma alarm
	static const unsigned short __mmp = 3U ; // billions
	static const unsigned short __mmw = 9U+1U+__mmp ; // billions.xxx

public:
	virtual ~AccuBase() ACS_NOEXCEPT ; 
	AccuBase() : _mutex() {}
	virtual AccuBase &operator+=(int) = 0 ; // PRQA S 2657 6
	virtual AccuBase &operator+=(long) = 0 ;
	virtual AccuBase &operator+=(long long) = 0 ;
	virtual AccuBase &operator+=(size_t) = 0 ;
	virtual AccuBase &operator+=(double) = 0 ;
	virtual AccuBase &operator+=(long double) = 0 ;
	virtual AccuBase &zero() = 0 ;

	virtual void writeTo(std::ostream &) const = 0 ;
	virtual AccuBase *clone() const = 0 ;       // PRQA S 2502, 2504
    virtual size_t n() const ACS_NOEXCEPT = 0 ;
    operator const Mutex& () const { return _mutex; } // PRQA S 2181
protected:
	AccuBase(const AccuBase &) : _mutex() {} ;
	AccuBase &operator=(const AccuBase &) { return *this ; } // PRQA S 4072, 4075, 4250 # no assignement of _mutex
private:
    Mutex _mutex ;
} ;

/*! \brief traits for accu */
template <typename _NumT> class Accu_Traits {
public:
	typedef double float_type ;
	static float_type sqrt_t(float_type v) { return sqrt(v) ; }
	typedef long long sum_type ;
} ;

/*! \brief long double specialization */
template <> 
class Accu_Traits<long double> {
public:
	typedef long double float_type ;
	static float_type sqrt_t(float_type v) { return sqrtl(v) ; }
	typedef long double sum_type ;
} ;
/*! \brief double specialization */
template <> 
class Accu_Traits<double> {
public:
	typedef long double float_type ;
	static float_type sqrt_t(float_type v) { return sqrtl(v) ; }
	typedef long double sum_type ;
} ;

/*! \brief float specialization */
template <> 
class Accu_Traits<float> {
public:
	typedef long double float_type ;
	static float_type sqrt_t(float_type v) { return sqrtl(v) ; }
	typedef long double sum_type ;
} ;


/*! \brief a template representing a statistic accumulator over the template parameter */
template <typename _NumT, typename Traits = Accu_Traits<_NumT> >
class Accumulator : public AccuBase
{
public:
	typedef typename Traits::float_type float_type ;
	typedef typename Traits::sum_type sum_type ;
    // PRQA S 2100 2
	static const float __sgm_alrm_th ; // sigma alarm threshold
	
	static std::string title(bool __end=false) ACS_NOEXCEPT 
	{
		std::ostringstream s ;
		if (not __end) { s << "\n" ; }
		s << std::left << std::setw(__titlew) << "statistics" ;
		s << " " ;
		s << std::right << std::setw(__itemsw) << "#" ;
		s << " " ;
		s << std::right << std::setw(__sumw) << "sum" ;
		s << " " ;
		s << std::right << std::setw(__meanw) << "mean" ;
		s << " " ;
		s << std::right << std::setw(__sigmaw) << "sigma" ;
		s << " " ;
		s << std::right << std::setw(__sigmaw) << "sigma-1" ;
		s << " " ;
		s << std::right << std::setw(__mmw) << "max" ;
		s << " " ;
		s << std::right << std::setw(__mmw) << "min" ;
		s << std::right << std::setw(__sgm_alrm) << " " ;
		size_t l = s.str().length() ;
		std::ostringstream o ;
		if (__end) {
			o << std::setw(l) << std::setfill('-') << '\n' ;    // PRQA S 3000,3010,3051,3130
        }
		o << s.str() ;
		if (!__end) {
			o << '\n' << std::setw(l) << std::setfill('-') << "" ; // PRQA S 3000,3010,3051,3130
        }
		return o.str() ;
	}

	typedef _NumT __type ;
	typedef Accumulator<_NumT> __accumulator_type ;


	Accumulator() :
		_sum(),
		_max(std::numeric_limits<_NumT>::min()),
		_min(std::numeric_limits<_NumT>::max()),
		_sq_sum(),
		_n(),
		_sOn(true)
	{  	}

	explicit Accumulator(const _NumT &v) :
        AccuBase(),
		_sum(),
		_max(std::numeric_limits<_NumT>::min()),
		_min(std::numeric_limits<_NumT>::max()),
		_sq_sum(),
		_n(),
		_sOn(true)
	{ add(v); }

	virtual ~Accumulator() ACS_NOEXCEPT {} ;

	Accumulator(const Accumulator &r) : 
		AccuBase(r),
		_sum(r._sum),
		_max(r._max),
		_min(r._min),
		_sq_sum(r._sq_sum),
		_n(r._n),
		_sOn(r._sOn)

	{ }

	Accumulator & operator=(const Accumulator &r) 
	{
		Lock const ll(*this) ; // PRQA S 3050 
		Lock rl(r) ;
		if (this != &r) {
			Lock const ll(*this) ;
			Lock rl(r) ;
			_sum = r._sum ;
			_max = r._max ;
			_min = r._min ;
			_sq_sum = r._sq_sum ;
			_n = r._n ;

			_sOn = r._sOn ;
		}
		return *this ;
	}

	virtual AccuBase *clone() const     // PRQA S 2502,2504
	{
		return new Accumulator(*this) ;
	}

	virtual size_t n() const ACS_NOEXCEPT { Lock const ll(*this) ; return _n ; } // PRQA S 3050

	float_type mean() const { Lock const ll(*this) ; return _sum / float_type(_n) ; }  // PRQA S 3050, 3081

	float_type sigma() const { Lock const ll(*this) ; float_type const m = mean() ; return Traits::sqrt_t((_sq_sum - _n*m*m) / _n); } // PRQA S 3011, 3050, 3051, 3084

	float_type sigma_1() const { Lock const ll(*this) ; float_type const m = mean() ; return Traits::sqrt_t((_sq_sum - _n*m*m) / (_n-1)); } // PRQA S 3011, 3050, 3051, 3084, 3130

	size_t add(const _NumT& x) // PRQA S 2620
	{ 
		Lock const ll(*this) ;   // PRQA S 3050 
		_max = std::max(x,_max) ; 
		_min = std::min(x,_min) ;
		_sum+=x ; // PRQA S 3000, 3051, 3084
		_sq_sum += sum_type(x)*sum_type(x) ; // PRQA S 3081

		return ++_n ; // PRQA S 3360
	}

	virtual Accumulator & zero() // PRQA S 2139
	{ 
		Lock const ll(*this) ;   // PRQA S 3050 
		_max = 0 ; 
		_min = 0 ;
		_sum = 0 ; 
		_sq_sum = 0 ;

		_n = 0 ;
        return *this ;
	}
	__type sum() const ACS_NOEXCEPT {  Lock const ll(*this) ; return _sum ; } // PRQA S 3012, 3050, 3051, 3130
	__type max() const ACS_NOEXCEPT {  Lock const ll(*this) ; return _max ; } // PRQA S 2502, 3050
	__type min() const ACS_NOEXCEPT {  Lock const ll(*this) ; return _min ; } // PRQA S 2502, 3050


	virtual void writeTo(std::ostream &s) const
	{
		Lock const ll(*this) ;  // PRQA S 3050 
		s << std::right << std::setw(__itemsw) << std::setfill(' ') << n() ; 
		s << " " ;
		s << std::right << std::setw(__sumw) << std::setprecision(__sump) << std::fixed << sum() ;
		if (isStatOn()) {
			if (n() > 0 ) {
                float_type sgm_ = sigma() ;
                float_type avg_ = mean() ;
                float_type const sgm_alrm = sgm_*__sgm_alrm_th ; // PRQA S 3051
				s << " " ;
				s << std::right << std::setw(__meanw) << std::setprecision(__meanp) << std::fixed << avg_ ;
				s << " " ;
				s << std::right << std::setw(__sigmaw) << std::setprecision(__sigmap) << std::fixed << sgm_ ;
				s << " " ;
				if(n() > 1) {
					s << std::right << std::setw(__sigmaw) << std::setprecision(__sigmap) << std::fixed << sigma_1() ;
                }
				else {
					s << std::right << std::setw(__sigmaw) << "(NaN)" ;
                }
				
				s << " " ;
				s << std::right << std::setw(__mmw) << std::setprecision(__mmp) << std::fixed << max() ;
                s << (((n() > 1) && (max() > (avg_+sgm_alrm))) ? '*':' '); // PRQA S 3051, 3231, 3380 
				s << std::right << std::setw(__mmw) << std::setprecision(__mmp) << std::fixed << min() ;
                s << (((n() > 1) && (min() < (avg_-sgm_alrm))) ? '*':' '); // PRQA S 3051, 3231, 3380 
			}
			else {
				s << " " ;
				s << std::right << std::setw(__meanw) << "(NaN)" << " " ;
				s << std::right << std::setw(__sigmaw) << "(NaN)" << " " ;
				s << std::right << std::setw(__sigmaw) << "(NaN)" << " " ;
			}
		}
	}

	virtual bool isStatOn() const ACS_NOEXCEPT {  Lock const ll(*this) ; return _sOn ; }  // PRQA S 3050 

	/*!
	 *  @brief  set on statistics output (default).
	 *
	 *  The output will contain all statistics
	 */
	virtual Accumulator &setStatOn() ACS_NOEXCEPT { Lock const ll(*this) ; _sOn = true ; return *this ; }  // PRQA S 3050 
	/*!
	 *  @brief  set off statistics output .
	 *
	 *  The output will only contain sum.
	 */
	virtual Accumulator &setStatOff() ACS_NOEXCEPT {  Lock const ll(*this) ; _sOn = false; return *this ;  }  // PRQA S 3050 
	virtual Accumulator &operator+=(int v) { add(v) ; return *this ; } // PRQA S 2139, 3011, 3051, 3130 6
	virtual Accumulator &operator+=(long v) { add(v) ; return *this ; }
	virtual Accumulator &operator+=(long long v) { add(v) ; return *this ; }
	virtual Accumulator &operator+=(size_t v) { add(v) ; return *this ; }
	virtual Accumulator &operator+=(double v) { add(static_cast<_NumT>(v)) ; return *this ; } // PRQA S 3081, 3100 2
	virtual Accumulator &operator+=(long double v) { add(static_cast<_NumT>(v)) ; return *this ; }

private:
	sum_type _sum ;
	_NumT _max ;
	_NumT _min ;
	sum_type _sq_sum ;
	size_t _n ;

	bool _sOn ;
} ;

template<typename _NumT, typename Traits>
const float acs::math::Accumulator<_NumT, Traits>::__sgm_alrm_th = 1.5F ; // sigma alarm threshold // PRQA S 2100

/*! \brief inserter operator */
template<typename NumT_, typename T_> 
inline
Accumulator<NumT_> &operator<<(Accumulator<NumT_> &acc__, const T_ &v) { acc__.operator+= (v) ; return acc__ ; }

/*! \brief inserter manipulator helper */
template <typename _NumT>
inline 
Accumulator<_NumT> &
operator<<(Accumulator<_NumT>& ab__, Accumulator<_NumT>& (*__pf)(Accumulator<_NumT>&)) { Lock const ll(ab__) ; return __pf(ab__) ;}  // PRQA S 3050 

/*! \brief inserter manipulator */
template <typename _NumT>
inline 
Accumulator<_NumT> &
statOn(Accumulator<_NumT> & __a)
{ Lock const ll(__a) ; return __a.setStatOn() ; } // PRQA S 3050

/*! \brief inserter manipulator helper */
template <typename _NumT>
inline 
Accumulator<_NumT>& 
statOff(Accumulator<_NumT>& __a)
{ Lock const ll(__a) ; return __a.setStatOff(); } // PRQA S 3050

/*! \brief inserter  */
inline 
std::ostream & operator << (std::ostream &s, const AccuBase &a) // PRQA S 2134
{
	a.writeTo(s) ;
	return s ; // PRQA S 4028
}



/*! \brief StatNode inserter operator 
 *
 *  \relates StatNode 
 */
class StatNode ; // fwd decl
std::ostream &operator<<(std::ostream &os, const StatNode &a) ;


/*! \brief statistic node - item of the statistic tree */
class StatNode { // PRQA S 2109,2153
	// by design this is the inserter
    friend std::ostream &operator<<(std::ostream &, const StatNode &) ; // PRQA S 2107
public:
	virtual ~StatNode() ACS_NOEXCEPT ;
	/*! default constructor, will build an undefined accumulator, it will be instantiated only when used */
	StatNode() ;

	/*! a constructor that builds a typed accumulator initialized with the given value */
	template<typename T_>
    StatNode(T_ v) : _value(new Accumulator<T_>(v)), _subtree(), _key(), _labels(), _nodePath(), _mutex()	{} // PRQA S 2180
	
	/*! copy ctor */
	StatNode(const StatNode &) ;

	/*! assignement 
	 *	\note does not copy the _key field if the assigning object has ane empty value for the _key
	 */
	StatNode &operator=(const StatNode &rhs) ;

	/*! the operator to add a new item to the statistic node */
	template<typename T_>
	StatNode &operator+=(const T_ &v)
	{
		
		{
			Lock const ll(*this) ;  

			if (!_value) {
				_value = new Accumulator<T_>(v);
			}
			else {
				(*_value) += v ;
			}
		}	// Lock released here
		
		// Notify statistics update
		EventNotifier* notifier = EventNotifyFactorySingleton::instance()->newItem();
		if (notifier) {
			// Notify statistics update
			notifier->notifyStatistics(_nodePath, v);
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Event notifier cannot be created. Statistics will not be notified.");
		}
		
		return *this;
	}

	/*! this is the access method to a subnode of the statistics tree
	 *	if the subnode does not exists it will create it
	 */
	StatNode &operator[](const std::string &) ;

	/*! a convenience inserter, calls operator +=(const T_ &)
	 *	\sa operator+=(const T_ &)
	 */
	template<typename T_>
	StatNode &operator<<(const T_ &v) { operator+=<T_> (v) ; return *this ; } // PRQA S 2070
    unsigned long long sum_n() const ;
    StatNode &reset() ;
    StatNode &zero() ;
    
    const std::string& getNodePath() const ;
    void getLabels(std::vector<std::string>&) const ;
    
    void resetTree() ; // recursive 
    void zeroTree() ; // recursive 
    void clearTree() ; 
    operator const Mutex& () const { return _mutex; } // PRQA S 2181
private:
	/*! the recursive method to write a subtree into the given stream */
	void writeTo(std::ostream &s, int level) const ;
	const StatNode &operator[](const std::string &) const ; // just declared
    StatNode *findParent(StatNode const &) ;
    void clearEntry(StatNode const &) ;
private:
	AccuBase * _value ;
	std::map<std::string,StatNode> _subtree ;
	std::string _key ;
	std::vector<std::string> _labels;
	std::string _nodePath;
    Mutex _mutex ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(StatNode) ;

} ;




/*! \brief the main Statistics class (singleton) */
class Statistics : // PRQA S 2109,2153
	public pattern::Singleton<Statistics>
{
// friendship needed in order to construct Statistics from a singleton see Singleton doc
friend Statistics *pattern::Singleton< Statistics>::instance(); // PRQA S 2107

public:	
	virtual ~Statistics() ACS_NOEXCEPT ;

	/*! the function to get the statistics
	 * \sa enableStatistics(bool)
	 */
	static void getStatistics(std::string &stat) ;
	/*! \brief access to the root statistics node */
	StatNode &operator()() ;

	/*! enables or disables the job of the getStatistics(std::string &) method
	 * \returns the previous state
	 */
	bool enableStatistics(bool) ;
protected:
	Statistics() ;

private:
    Statistics(const Statistics &) ;
    Statistics& operator=(const Statistics &) ;
    
private:
	bool statsAreEnabled_ ; ///< the flag that enables or disables the statistic report
	StatNode root_ ; ///< the root node of the statistics tree

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Statistics) ;
} ;


/*!
 *  @}
 */

_ACS_END_NESTED_NAMESPACE

#endif // _Statistics_H_
