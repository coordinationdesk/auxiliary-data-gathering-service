// PRQA S 1050 EOF
/*
	Copyright 2014-2018 Advanced Computer Systems , Inc.
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
	Revision 5.2  2018/09/04 15:53:43  marpas
	getExePath and getExeDir methods added
	
	Revision 5.1  2014/06/27 10:01:57  marpas
	ProcessStatus singleton added
	it will be initialized at process startup and, for the moment stores the current umask
	
	

*/

#ifndef _ProcessStatus_H_
#define _ProcessStatus_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)


/*! \addtogroup os 
 *  @{
 */

/*! \brief the main class (singleton) */
class ProcessStatus : // PRQA S 2109,2153
	public pattern::Singleton<ProcessStatus>
{
// friendship needed in order to construct ProcessStatus from a singleton see Singleton doc
friend ProcessStatus *pattern::Singleton< ProcessStatus>::instance(); // PRQA S 2107

public:	
	virtual ~ProcessStatus() ACS_NOEXCEPT ;

	/*! enables or disables the job of the getProcessStatus(std::string &) method
	 * \returns the previous state
	 */
	mode_t getStartupUmask() const ACS_NOEXCEPT ;
    const std::string & getExePath() const ACS_NOEXCEPT ;
    std::string getExeDir() const ;
    
protected:
	ProcessStatus() ;

private:
#if __cplusplus >= 201103L	
    ProcessStatus(const ProcessStatus &) = delete ; // PRQA S 2038 4
    ProcessStatus& operator=(const ProcessStatus &) = delete ;
    ProcessStatus(ProcessStatus &&) = delete ;
    ProcessStatus& operator=(ProcessStatus &&) = delete ;
#else
    ProcessStatus(const ProcessStatus &) ;
    ProcessStatus& operator=(const ProcessStatus &) ;
#endif

    void computeStartupUmask() ;     
    
private:
    mode_t startupUmask_ ;
    std::string exePath_ ;
   
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProcessStatus) ;
} ;


/*!
 *  @}
 */

_ACS_END_NESTED_NAMESPACE

#endif // _ProcessStatus_H_
