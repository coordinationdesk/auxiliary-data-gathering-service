// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Configuration Library $

*/


#ifndef _StandaloneResourceSpace_H_
#define _StandaloneResourceSpace_H_ 

#include <acs_c++config.hpp>
#include <ResourceSpace.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;

//
//
//
// class StandaloneResourceSpace
//
//
//
class StandaloneResourceSpace : public ResourceSpace // PRQA S 2109, 2153
{
public:

	/*! Base class StandaloneResourceSpaceException declaration */
	exDECLARE_EXCEPTION(StandaloneResourceSpaceException,SpaceLoadException) ; // PRQA S 2131, 2153, 2502

	/*! StandaloneResourceSpace not found exception */
	exDECLARE_EXCEPTION(StandaloneSpaceNotFoundException,SpaceNotFoundException) ; // PRQA S 2131, 2153, 2502

	StandaloneResourceSpace(const std::string &, const std::string &, bool absPath = false);
	StandaloneResourceSpace(const StandaloneResourceSpace &) ;
	StandaloneResourceSpace &operator=(const StandaloneResourceSpace &) ;
	virtual ~StandaloneResourceSpace() ACS_NOEXCEPT ;

	virtual std::string path(bool std_path) const ;
	virtual bool mergeIn(rsResourceSet &, bool std_path) ;
	
/*! \brief get configuration space.
	\param conf space. 
	\param std_path if true search in the standard path, else in the optional one
    \see ResourceSpace::path
*/
	virtual void get(std::string &, bool std_path);

	std::string defaultExtension() const ;

	// Update operation has been removed to allow lock-free management of Standalone Configuration

private:
	// default ctor - not implemented
	StandaloneResourceSpace();
	void fillResources(const std::string &, rsResourceSet &) ;
	std::string getTheGreatestVersionResourcePath(bool std_path);
	std::string getResourcePath(bool);
	std::string getUpgradedVersionResourcePath( const std::string &, bool std_path ); 
	std::string getResourceVersion( const std::string & ); 
	std::string path(const std::string & version, bool std_path) const ;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StandaloneResourceSpace) ;
} ; 

_ACS_END_NAMESPACE

#endif // _StandaloneResourceSpace_H_
