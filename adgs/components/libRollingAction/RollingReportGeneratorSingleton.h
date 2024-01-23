/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libRollingAction$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _RollingReportGeneratorSingleton_H_
#define _RollingReportGeneratorSingleton_H_

#include <exException.h>
#include <Singleton.hpp>

#include <RollingReportGenerator.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Handler for the RollingReportGenerator thread
**/

class RollingReportGeneratorSingleton : public Singleton <RollingReportGeneratorSingleton> { 
public:

	~RollingReportGeneratorSingleton() throw();

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	RollingReportGeneratorSingleton();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	RollingReportGeneratorSingleton(const RollingReportGeneratorSingleton & );
	/** Operator = */
	RollingReportGeneratorSingleton &operator=(const RollingReportGeneratorSingleton &);
	friend RollingReportGeneratorSingleton* Singleton<RollingReportGeneratorSingleton>::instance(); //SINGLETON

public:
	/** Get the running instance */
	RollingReportGenerator& getAgent() { return _agent; };

private:
	/** Running Instance */
	RollingReportGenerator _agent;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingReportGeneratorSingleton)

};

_ACS_END_NAMESPACE

#endif //_RollingReportGeneratorSingleton_H_

