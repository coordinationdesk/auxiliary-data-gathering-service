// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 10:14:19  marpas
	adoption of libException 5.x standards
	
	Revision 2.8  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.7  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.6  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.5  2011/03/01 11:56:58  marpas
	GCC 4.4.x support
	
	Revision 2.4  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.3  2008/12/16 14:51:11  marpas
	Standalone SystemMode implemented
	
	Revision 2.2  2008/12/15 18:51:06  marpas
	SystemMode implementation done
	
	Revision 2.1  2008/12/15 18:43:52  marpas
	SystemMode implementation in progress
	
*/


#ifndef _SystemMode_H_
#define _SystemMode_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;


//
//
//
// class SystemMode
//
//
//
class SystemMode // PRQA S 2109
{
public:
	/*! class SpaceException declaration, base class for exceptions in SystemMode */
	exDECLARE_EXCEPTION(ModeException,exException) ;    // PRQA S 2131, 2502

public:

	explicit SystemMode(const std::string &app, const std::string &sub = "");
	SystemMode(const SystemMode &) ;
	SystemMode &operator=(const SystemMode &) ;
	virtual ~SystemMode() throw() ;

	virtual std::string path(const std::string &app, const std::string &sub) const ;
	virtual std::string load() ;
	
	static std::string buildPathWithoutExtension(const std::string &, const std::string &) ;

	const std::string &app() const throw() ;
	const std::string &sub() const throw() ;

	void app(const std::string &) ;
	void sub(const std::string &) ;
	
	static std::string defaultExtension() ;
	static std::string systemGlobal() ;
private:
	// declared but not implemented
	SystemMode() ;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SystemMode) ;
	std::string _app ;
	std::string _sub ;
	static std::string _systemGlobal ;
} ; 

#if __cplusplus > 199711L
typedef std::unique_ptr<SystemMode> SystemModeScopeGuard ;
#else
typedef std::auto_ptr<SystemMode> SystemModeScopeGuard ;
#endif


_ACS_END_NAMESPACE


#endif // _SystemMode_H_
