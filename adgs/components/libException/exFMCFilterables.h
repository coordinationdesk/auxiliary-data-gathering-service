// PRQA S 1050 EOF
/*

	Copyright 2013-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2015/08/13 15:25:51  marfav
	Adding function per thread configurability
	
	Revision 5.7  2015/08/12 09:02:24  marfav
	MCF messages format implemented
	Added the code per thread in the singleton
	
	Revision 5.6  2015/07/01 12:35:55  marfav
	Template stream inserter fixed
	
	Revision 5.5  2015/06/30 09:31:35  marpas
	MC_xxx filterables i/f aligned to exFMCInfo in order to support params and metrci inserters with the ACS_LOG_MC_xxx macros
	
	Revision 5.4  2015/06/26 18:34:19  marpas
	introducing new MC framework
	
	Revision 5.3  2013/09/27 14:05:17  marpas
	streams have names
	
	Revision 5.2  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.1  2013/06/06 08:35:04  marpas
	includes fixed
	
	Revision 5.0  2013/06/04 15:35:01  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.1  2013/05/24 11:10:54  marpas
	introducing exFMCFilterables concept
	
	
*/

#ifndef _exFMCFilterable_H_
#define _exFMCFilterable_H_ 

#include <acs_c++config.hpp>
#include <Filterables.h>
#include <MCFilterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, mc)

/*! \class exFMCCritical
	
	\brief filterable object whose category is Critical specifically designed for MC.

    \see exFMessage 	
*/
class exFMCCritical : public exFError, public MCScopedParams // PRQA S 2153, 2156
{
public:
	explicit exFMCCritical(const std::string &n="exFMCCritical") ; 
	virtual ~exFMCCritical() ACS_NOEXCEPT ;
	template <typename T> 
    exFMCCritical &operator<<(const T &t) { exFError& theStream (*this); theStream << t; return *this ;} // PRQA S 2070, 2072

	virtual std::string message() const;
	
private:
	// declared but not implemented to prevent their use
	exFMCCritical(const exFMCCritical &) ;
	exFMCCritical &operator=(const exFMCCritical &) ;
} ;
template <> 
inline
exFMCCritical &exFMCCritical::operator<<(const MCParam &t) { MCScopedParams::add(t) ; return *this ;} // PRQA S 2133, 2134, 4222

template <> 
inline
exFMCCritical &exFMCCritical::operator<<(const MCMetric &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCCritical &exFMCCritical::operator<<(const MCCode &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCCritical &exFMCCritical::operator<<(const MCFunction &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

/*! \class exFMCError
	
	\brief filterable object whose category is Error specifically designed for MC.

    \see exFError 	
*/
class exFMCError : public exFError, public MCScopedParams // PRQA S 2153, 2156
{
public:
	explicit exFMCError(const std::string &n="exFMCError") ; 
	virtual ~exFMCError() ACS_NOEXCEPT ;
	template <typename T> 
    exFMCError &operator<<(const T &t) { exFError& theStream (*this); theStream << t; return *this;} // PRQA S 2070, 2072

	virtual std::string message() const;
	
private:
	// declared but not implemented to prevent their use
	exFMCError(const exFMCError &) ;
	exFMCError &operator=(const exFMCError &) ;
} ;
template <> 
inline
exFMCError &exFMCError::operator<<(const MCParam &t) { MCScopedParams::add(t) ; return *this ;} // PRQA S 2133, 2134, 4222

template <> 
inline
exFMCError &exFMCError::operator<<(const MCMetric &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCError &exFMCError::operator<<(const MCCode &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCError &exFMCError::operator<<(const MCFunction &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222


/*! \class exFMCWarning
	
	\brief filterable object whose category is Warning specifically designed for MC.

    \see exFWarning 	
*/
class exFMCWarning : public exFWarning, public MCScopedParams // PRQA S 2153, 2156
{
public:
	explicit exFMCWarning(const std::string &n="exFMCWarning") ; 
	virtual ~exFMCWarning() ACS_NOEXCEPT ;
	template <typename T> 
    exFMCWarning &operator<<(const T &t) { exFWarning& theStream (*this); theStream << t; return *this;} // PRQA S 2070, 2072

	virtual std::string message() const;
	
private:
	// declared but not implemented to prevent their use
	exFMCWarning(const exFMCWarning &) ;
	exFMCWarning &operator=(const exFMCWarning &) ;
} ;
template <> 
inline
exFMCWarning &exFMCWarning::operator<<(const MCParam &t) { MCScopedParams::add(t) ; return *this ;} // PRQA S 2133, 2134, 4222

template <> 
inline
exFMCWarning &exFMCWarning::operator<<(const MCMetric &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCWarning &exFMCWarning::operator<<(const MCCode &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCWarning &exFMCWarning::operator<<(const MCFunction &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

/*! \class exFMCInfo
	
	\brief filterable object whose category is Info specifically designed for MC.

    \see exFMessage 	
*/
class exFMCInfo : public exFMessage, public MCScopedParams // PRQA S 2153, 2156
{
public:
	explicit exFMCInfo(const std::string &n="exFMCInfo") ; 
	virtual ~exFMCInfo() ACS_NOEXCEPT ;
	template <typename T> 
    exFMCInfo &operator<<(const T &t) { exFMessage& theStream (*this); theStream << t; return *this;} // PRQA S 2070, 2072

	virtual std::string message() const;
   
private:
	// declared but not implemented to prevent their use
	exFMCInfo(const exFMCInfo &) ;
	exFMCInfo &operator=(const exFMCInfo &) ;
} ;

template <> 
inline
exFMCInfo &exFMCInfo::operator<<(const MCParam &t) { MCScopedParams::add(t) ; return *this ;} // PRQA S 2133, 2134, 4222

template <> 
inline
exFMCInfo &exFMCInfo::operator<<(const MCMetric &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCInfo &exFMCInfo::operator<<(const MCCode &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

template <> 
inline
exFMCInfo &exFMCInfo::operator<<(const MCFunction &t) { MCScopedParams::add(t) ; return *this ;}// PRQA S 2133, 2134, 4222

_ACS_END_NESTED_NAMESPACE




#endif // _exFMCFilterable_H_
