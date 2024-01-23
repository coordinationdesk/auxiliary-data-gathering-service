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

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2018/09/27 13:33:25  marpas
	auto_ptr changed into uniqie_ptr if supported (C++11)
	
	Revision 5.5  2017/09/25 15:32:41  lucio.pulvirenti
	S2PDGS-1843: absolute path optional input parm passed to CTOR to set private attribute. get and set methods added.
	
	Revision 5.4  2017/05/02 13:31:08  lucio.pulvirenti
	S2PDGS-1754: incrementVersion static method added.
	
	Revision 5.3  2017/04/27 10:16:02  lucio.pulvirenti
	S2PDGS-1754: get and update new IFs defined to be implemented in derived classes only (not implemented in base class so far).
	
	Revision 5.2  2017/04/26 15:43:43  lucio.pulvirenti
	S2PDGS-1754: WORK IN PROGRESS
	
	Revision 5.1  2013/07/18 11:21:40  marpas
	space not found exception are different (in their notifications) from other exceptions
	
	Revision 5.0  2013/06/18 10:14:18  marpas
	adoption of libException 5.x standards
	
	Revision 2.6  2013/03/14 15:23:12  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.5  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.4  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.3  2011/03/01 11:56:58  marpas
	GCC 4.4.x support
	
	Revision 2.2  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.1  2008/11/13 12:38:26  marpas
	New configuration approach using ConfigurationSingleton instead of MergeConfiguration
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.5  2003/07/11 13:00:00  marpas
	exException::clone implemented
	
	Revision 1.4  2003/04/30 17:00:26  marpas
	using namespace acs removed
	
	Revision 1.3  2003/04/30 10:20:24  marpas
	using namespace std was removed from includes
	
	Revision 1.2  2003/02/03 18:52:00  marpas
	Exception handling improved to distinguish parse errors
	
	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
	libConfiguration import
		

*/


#ifndef _ResourceSpace_H_
#define _ResourceSpace_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <memory>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ; 

//
//
//
// class ResourceSpace
//
//
//
class ResourceSpace  // PRQA S 2109
{
public:
	/*! class SpaceException declaration, base class for exceptions in ResourceSpace */
	exDECLARE_EXCEPTION(SpaceException,exException) ;                   // PRQA S 2131, 2502

	/*! class SpaceParseException declaration
	 */
	exDECLARE_EXCEPTION(SpaceParseException,SpaceException) ;           // PRQA S 2131, 2153, 2502


	/*! class SpaceLoadException declaration
	 */
	exDECLARE_EXCEPTION(SpaceLoadException,SpaceException) ;            // PRQA S 2131, 2153, 2502

	/*! class SpaceLoadException declaration
	 */
	exDECLARE_EXCEPTION(SpaceNotFoundException,SpaceException) ;            // PRQA S 2131, 2153, 2502

public:
	ResourceSpace(const std::string &, const std::string &, bool absolutePath = false);
	ResourceSpace(const ResourceSpace &) ;
	ResourceSpace &operator=(const ResourceSpace &) ;
	virtual ~ResourceSpace() throw() ;

    /*! \brief returns path to load
        returns the path where configuration file will be searched combining a configured path with the space and version names.
        If the internal _absolutePath is set to true, the method returns the _space content without applying any further modification. It is supposed the _space is absolute in that case.
        \param std_path if true it will use ResourceSpaceFactory::commonConfigurationPath as base path else ResourceSpaceFactory::optionalConfigurationPath()
        \see configurationPath()
     */
	virtual std::string path(bool std_path) const ;
    /*! the method tries to load the conf space and merge its content into the passed rsResourceSet
        \param std_path passed to ResourceSpace::path method
        \returns true if effectively loaded something, false otherwise
        \see path()
    */
	virtual bool mergeIn(rsResourceSet &, bool std_path) ;
	
/*! \brief get configuration space. Not implemented so far in the base class. 
	\param spc OUT configuration space. 
	\param std_path if set, search in the standard path, else in the optional one 
    \see ResourceSpace::path
*/
	virtual void get(std::string &spc, bool std_path);
	
/*! \brief update configuration space - not implemented so far in the base class 
	\param conf space resource. 
	\param upgradeVersion if set, compute a greater version and save a new instance. 
	\param std_path if set, search in the standard path, else in the optional one 
    \see ResourceSpace::path
*/
	virtual void update(const std::string &conf, bool upgradeVersion, bool std_path);
	
	static std::string buildPathWithoutExtension(const std::string &, const std::string &) ;

	const std::string &space() const throw() ; 
	const std::string &version() const throw() ;
	const bool &absolutePath() const throw() ;

	void space(const std::string &) ;
	void version(const std::string &) ;
	void absolutePath(const bool &) ;
	
	const std::string &replacement() const ;
    
	virtual std::string defaultExtension() const ;
	static void incrementVersion( const std::string & oldVers, std::string & newVers );
	
   /*! \brief returns path to load
        returns the path where configuration file will be searched 
        \param std_path if true it will return ResourceSpaceFactory::commonConfigurationPath else ResourceSpaceFactory::optionalConfigurationPath()
     */
    std::string configurationPath(bool std_path) const ;
    
protected:	
	void replacement(const std::string &) ;
private:
	// declared but not implemented
	ResourceSpace() ;
private:
	std::string _replacement ;
	std::string _space ;
	std::string _version ;
	bool _absolutePath;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ResourceSpace) ;
} ; 

#if __cplusplus > 199711L
typedef std::unique_ptr<ResourceSpace> ResourceSpaceScopeGuard ;
#else
typedef std::auto_ptr<ResourceSpace> ResourceSpaceScopeGuard ;
#endif

_ACS_END_NAMESPACE


#endif // _ResourceSpace_H_
