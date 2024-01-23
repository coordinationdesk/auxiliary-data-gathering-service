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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/11/19 13:36:15  ricfer
	Compiler warnings removed. New logging macros adopted.
	
	Revision 2.2  2013/03/12 17:49:01  marpas
	dbCOnnectionPool is mandatory in file actions
	
	Revision 2.1  2006/08/03 16:41:52  davcas
	platform factory key forced to capital letters
	
	Revision 2.0  2006/03/01 16:41:23  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.1.1.1  2006/01/31 13:44:10  seralb
	Imported libAuxFileHandlingBase
	
	

*/

#ifndef _AuxDownloaderSingleton_H_
#define _AuxDownloaderSingleton_H_ 

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <Singleton.hpp>
#include <map>
#include <AuxDownloader.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;
class dbConnectionPool ;

class AuxDownloaderSingleton: public Singleton<AuxDownloaderSingleton> // PRQA S 2109, 2153
{
    friend AuxDownloaderSingleton* Singleton<AuxDownloaderSingleton>::instance(); // PRQA S 2107

public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502
	virtual ~AuxDownloaderSingleton() throw() {} // PRQA S 2131

	void registerFactory(std::string const &, AuxDownloader* (*)(dbConnectionPool &) );
	void unregisterFactory(std::string const &, AuxDownloader* (*)(dbConnectionPool &) );

////////////////////////////////////////////////////////////////////////////////////////////
// Specific Factory Methods
////////////////////////////////////////////////////////////////////////////////////////////

	/**
	  * \brief Return the AuxDownloader object pointer associated the the key in input
	  * \param The key must be the Satellite Name
	  */										
	AuxDownloader* newAuxDownloader(std::string const &, dbConnectionPool &);


    static void registerAuxDownloaderFactoryMethod(std::string const & , AuxDownloader* (*)(dbConnectionPool &) );
    static void unregisterAuxDownloaderFactoryMethod(std::string const & , AuxDownloader* (*)(dbConnectionPool &) );

protected:
	AuxDownloaderSingleton();
private:
	AuxDownloaderSingleton(const AuxDownloaderSingleton & ); // not implemented 
	AuxDownloaderSingleton& operator= (const AuxDownloaderSingleton & ); // not implemented 

private:

	AuxDownloader *newObject(std::string const &, dbConnectionPool &);
	std::map<std::string, AuxDownloader* (*)(dbConnectionPool &)> _map;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AuxDownloaderSingleton);


};


_ACS_END_NAMESPACE

#endif // _AuxDownloaderSingleton_H_
