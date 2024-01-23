/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TaskTable library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/06/10 15:49:47  marfav
	Added support to DynProcParams and SensingTimeFlag
	
        
*/ 

#ifndef _DynProcParam_H_
#define _DynProcParam_H_

#include <exException.h>
#include <acs_c++config.hpp>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)


/**
 *\brief This class is the representation of the dynamic parameter section foressen for the task table according to S2Gpp PDGS-IPF Interface Definition
 */

class DynProcParam // PRQA S 2109
{
public:

    static const std::string STRING; // PRQA S 2100 3
    static const std::string STRINGCAP; // PRQA S 2100 3
    static const std::string NUMBER;
    static const std::string DATENUMBER;



	/** DynProcParam Exceptions */
	exDECLARE_EXCEPTION(exDynProcParamException, acs::exException) ; // Base DynProcParam Exception. // PRQA S 2131, 2502

	/**
	 * CTOR and copy CTOR
	 */ 
	DynProcParam(const std::string& aParamName, const std::string& aParamType, const std::string& aParamDefault);
	DynProcParam(const DynProcParam& d);
	DynProcParam();
	
	virtual ~DynProcParam() throw();

	/**
	 * Operator=
	 */ 
	DynProcParam& operator = (const DynProcParam& d);

	/**
	 *  Methods for setting and getting the ParamName of the configuration file
	 */
	bool isSetParamName() const throw();
	const std::string& getParamName() const throw();
	void setParamName(const std::string& aParamName);

	/**
	 *  Methods for setting and getting the ParamType of the configuration file
	 */
	bool isSetParamType() const throw();
	const std::string& getParamType() const throw();
	void setParamType(const std::string& aParamType);

	/**
	 *  Methods for setting and getting the ParamDefault of the configuration file
	 */
	bool isSetParamDefault() const throw();
	const std::string& getParamDefault() const throw();
	void setParamDefault(const std::string& aParamDefault);

private:
    void checkParamType() const;

private:
	// attributes
	std::string _paramName;
	bool _paramNameSet;
	std::string _paramType;
	bool _paramTypeSet;
	std::string _paramDefault;
	bool _paramDefaultSet;
	
    ACS_CLASS_DECLARE_DEBUG_LEVEL(DynProcParam)
};
	
_ACS_END_NAMESPACE
	  
#endif //_DynProcParam_H_

