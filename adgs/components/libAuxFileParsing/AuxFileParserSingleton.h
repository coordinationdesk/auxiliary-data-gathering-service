// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2006/05/10 07:27:18  giucas
	newSTDMFileParser factory method added
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/10/26 08:03:04  giucas
	First release
	

*/

#ifndef _AuxFileParserSingleton_H_
#define _AuxFileParserSingleton_H_ 

#include <AuxFileParser.h>
#include <Singleton.hpp>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class AuxFileParserSingleton: public Singleton<AuxFileParserSingleton> // PRQA S 2109, 2153
{
    friend AuxFileParserSingleton* Singleton<AuxFileParserSingleton>::instance(); // PRQA S 2107

public:


	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502

	virtual ~AuxFileParserSingleton() throw() {} // PRQA S 2131

	void registerFactory(std::string const &, AuxFileParser* (*)() );
	void unregisterFactory(std::string const &, AuxFileParser* (*)() );

////////////////////////////////////////////////////////////////////////////////////////////
// Specific Factory Methods
////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  * \brief Return the PredictedOrbitFileParser object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	AuxFileParser* newPredictedFileParser(std::string const &);

	/**
	  * \brief Return the OBTFileParser object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	AuxFileParser* newObtFileParser(std::string const &);

	/**
	  * \brief Return the OrbitScenarioFileParser object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	AuxFileParser* newOrbitScenarioFileParser(std::string const &);

	/**
	  * \brief Return the OrbitDataFileParser object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	AuxFileParser* newOrbitDataFileParser(std::string const &);

	/**
	  * \brief Return the STDMFileParser object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	AuxFileParser* newSTDMFileParser(std::string const &);

protected:
	AuxFileParserSingleton();
private:
	AuxFileParserSingleton(const AuxFileParserSingleton &); // not implemented
	AuxFileParserSingleton& operator= (const AuxFileParserSingleton &); // not implemented

	AuxFileParser *newObject(std::string const &);
	std::map<std::string, AuxFileParser* (*)()> _map;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AuxFileParserSingleton) ;


};


void registerAuxFileParserFactoryMethod(std::string const & , AuxFileParser* (*)() );
void unregisterAuxFileParserFactoryMethod(std::string const & , AuxFileParser* (*)() );



_ACS_END_NAMESPACE

#endif // _AuxFileParserSingleton_H_
