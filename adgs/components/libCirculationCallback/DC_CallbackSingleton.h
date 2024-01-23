// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libCirculationCallback$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _DC_CallbackSingleton_H_
#define _DC_CallbackSingleton_H_

#include <exException.h>
#include <Singleton.hpp>
#include <CirculationCallbackSoap.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

class CirculationCallbackInterface 
{
public:
	CirculationCallbackInterface();
	virtual ~CirculationCallbackInterface() ACS_NOEXCEPT;
	
	virtual void manageCallbackRequest( CirculationCallback::pds__CallbackRequestType *  in, pds__CallbackResponseType* out )=0;
	dbConnectionPool & getPool();
	
private:	
	// copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	CirculationCallbackInterface(const CirculationCallbackInterface & );
	/** Operator = */
	CirculationCallbackInterface &operator=(const CirculationCallbackInterface &);

	dbConnectionPool & _connPool;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationCallbackInterface)
};

/**
* \brief
* Data Circulation Callback singleton
**/

class DC_CallbackSingleton : public Singleton <DC_CallbackSingleton> { 
public:

	void setCallbackManager(CirculationCallbackInterface*) ;
	CirculationCallbackInterface* getCallbackManager() const;


protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	DC_CallbackSingleton();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	DC_CallbackSingleton(const DC_CallbackSingleton & );
	/** Operator = */
	DC_CallbackSingleton &operator=(const DC_CallbackSingleton &);
	friend DC_CallbackSingleton* Singleton<DC_CallbackSingleton>::instance(); //SINGLETON

protected:

private:
	CirculationCallbackInterface* _thePtr;
	static Mutex _mutex ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DC_CallbackSingleton)

};

_ACS_END_NAMESPACE

#endif //_DC_CallbackSingleton_H_

