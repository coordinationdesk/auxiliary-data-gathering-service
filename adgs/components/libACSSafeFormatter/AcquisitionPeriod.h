// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.10  2018/10/04 13:10:26  giucas
	Typo fixed
	
	Revision 1.9  2018/10/04 12:41:19  giucas
	Wrong header change done before. Reintroduced old header.
	
	Revision 1.7  2018/10/03 09:23:56  tergem
	added public methods get_startTime and get_stopTime
	
	Revision 1.6  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.5  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.4  2012/06/29 11:01:54  damdec
	readXmlStream method added.
	
	Revision 1.3  2012/04/17 15:56:52  damdec
	Safe namespace passed as parameter to the constructor.
	
	Revision 1.2  2012/03/15 14:59:22  damdec
	Implementation finalized.
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/

#ifndef _AcquisitionPeriod_H_
#define _AcquisitionPeriod_H_

#include <exException.h>
#include <WrappedMetadata.h>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
/**
* \brief
* Specialized class for the SAFE Acquisition Period Type handling
**/

class AcquisitionPeriod : public WrappedMetadata { // PRQA S 2109
public:

	/** AcquisitionPeriod Exceptions */
	exDECLARE_EXCEPTION(exAcquisitionPeriodException, exException) ; // Base AcquisitionPeriod Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exAcquisitionPeriodCriticalException, exAcquisitionPeriodException) ; // Critical Exception. // PRQA S 2153


	/** Class constructor (write mode) */
	AcquisitionPeriod(const std::string & safeNamespace, const std::string& startTime, const std::string& stopTime="", const std::string& safePrefix="sentinel-safe:");
	/** Class constructors (read mode) */
	explicit AcquisitionPeriod(const rsResourceSet & rs);

	/** Copy Constructor */
	AcquisitionPeriod(const AcquisitionPeriod & );
	/** Operator = */
	AcquisitionPeriod &operator=(const AcquisitionPeriod &);

	/** Destructor */
	virtual ~AcquisitionPeriod() throw() ;
	
	
#if __cplusplus >= 201103L
		const std::string& get_startTime() const noexcept { return _startTime; } 
#else
		const std::string& get_startTime() const throw() { return _startTime; } 
#endif
#if __cplusplus >= 201103L
		const std::string& get_stopTime() const noexcept { return _stopTime; } 
#else
		const std::string& get_stopTime() const throw() { return _stopTime; } 
#endif
	

protected:
	virtual void writeXmlStream() ;
	virtual void readXmlStream(const rsResourceSet & rs);
private:
    AcquisitionPeriod() ; // not implemented
private:
	std::string _startTime;
	std::string _stopTime;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AcquisitionPeriod)

};

_ACS_END_NAMESPACE

#endif //_AcquisitionPeriod_H_

