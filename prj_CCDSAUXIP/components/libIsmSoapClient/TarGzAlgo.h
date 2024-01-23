// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
	
	
	
*/

#ifndef _TarGzAlgo_H_
#define _TarGzAlgo_H_ 

#include <TarAlgo.h>

_ACS_BEGIN_NAMESPACE(acs)

class TarGzAlgo :  public TarAlgo // PRQA S 2109
{

public:

	/**
	 * \brief  Class constructor 
	 *    
	 */
	TarGzAlgo(const std::map<std::string, std::string> &archiveCustomOptions, const std::map<std::string, std::string> &retrieveCustomOptions, const PackagingAlgo::FilesChangeFixStrategy &);

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~TarGzAlgo() throw();
	
	/**
	 * \brief pack item
	 * \param 
	 */
	virtual void pack( const std::string & itemPath, const std::string & packagePath, int computationTimeout, const long long int itemSize ) ;
	
	/**
	 * \brief unpack item
	 * \param 
	 */
	virtual void unpack( const std::string & packagePath, const std::string & itemPath, int computationTimeout );
	
	/**
	 * \brief estimate package size
	  * \param 
	 */
	virtual long long int estimatePackageSize( const long long int itemSize );
	
protected:
	
private:
	// copy constructor and operator= defined but not implemented
	TarGzAlgo(const TarGzAlgo &); 
	TarGzAlgo &operator=(const TarGzAlgo &);

// data
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TarGzAlgo)
};


_ACS_END_NAMESPACE

#endif //_TarGzAlgo_H_
