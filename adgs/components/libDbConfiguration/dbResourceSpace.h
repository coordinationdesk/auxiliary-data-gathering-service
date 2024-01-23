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

	$Prod: A.C.S. Database Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2018/09/27 15:20:00  marpas
	auto_ptr changed into unique_ptr whenever possible
	
	Revision 5.7  2017/11/27 15:32:57  marpas
	qa warning fixed
	
	Revision 5.6  2017/09/25 15:43:10  lucio.pulvirenti
	S2PDGS-1843: oprional boolean parameter to consider conf space as absolute path added to CTOR and coherently used in mergeIn,
	get and update methods.
	
	Revision 5.5  2017/05/03 10:21:23  lucio.pulvirenti
	S2PDGS-1754: Upgrade of version managed.
	
	Revision 5.4  2017/04/27 11:39:14  lucio.pulvirenti
	S2PDGS-1754: Base class get and update methods implemented to get resource space from db and reload it.
	
	Revision 5.3  2017/04/26 15:44:06  lucio.pulvirenti
	S2PDGS-1754: WORK IN PROGRESS.
	
	Revision 5.2  2016/04/28 09:20:44  marpas
	useless members removed and interface simplified
	
	Revision 5.1  2013/07/18 11:11:53  marpas
	different exception raised in case the space was not loaded or not found in the db.
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.9  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.8  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.7  2013/02/06 16:09:58  marpas
	coding best practices applied
	some qa rule fixed
	
	Revision 2.6  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.5  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.4  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.3  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.2  2009/08/03 18:26:10  marpas
	faster and less connections to db.
	
	Revision 2.1  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.0  2006/02/28 09:09:04  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/03/17 12:42:56  marpas
	headers fixed
	
	Revision 1.5  2004/06/04 08:17:31  marfav
	dbResourceSpaceException now inherits from SpaceLoadException
	
	Revision 1.4  2003/07/11 21:27:53  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 17:15:36  marpas
	using namespace acs removed
	
	Revision 1.2  2003/04/30 14:01:54  marpas
	using namespace std no longer fount into acs includes
	
	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
	libDbConfiguration Import
		

*/


#ifndef _dbResourceSpace_H_
#define _dbResourceSpace_H_ 

#include <acs_c++config.hpp>
#include <ResourceSpace.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class dbConnection ;
class dbConnectionPool ;
class dbConnectionGuard ;
class dbSmartPersistent;
class dbSmartQuery;
class oftmpstream;
//
//
//
// class dbResourceSpace
//
//
//
class dbResourceSpace : public ResourceSpace // PRQA S 2109, 2153
{
public:

	/*! class dbResourceSpaceException declaration */
	exDECLARE_EXCEPTION(dbSpaceLoadException,SpaceLoadException) ; // PRQA S 2131, 2153, 2502

	/*! class dbResourceSpaceException declaration */
	exDECLARE_EXCEPTION(dbSpaceNotFoundException,SpaceNotFoundException) ; // PRQA S 2131, 2153, 2502

	dbResourceSpace(const std::string &, const std::string &, dbConnectionPool &, bool absPath = false);
	dbResourceSpace(const dbResourceSpace &) ;
	dbResourceSpace &operator=(const dbResourceSpace &) ;
	virtual ~dbResourceSpace() throw() ;

    /*! the method tries to load the conf space and merge its content into the passed rsResourceSet
        \param std_path passed to ResourceSpace::path method
        \see ResourceSpace::path()
    */
	virtual bool mergeIn(rsResourceSet &, bool std_path) ;

/*! \brief get configuration space.
	\param spc OUT configuration space. 
	\param std_path if set, search in the standard path, else in the optional one 
    \see ResourceSpace::path
*/
	virtual void get(std::string &spc, bool std_path);
	
/*! \brief update configuration space.
	\param conf space resource. 
	\param upgradeVersion if set, compute a greater version and save a new instance. 
	\param std_path if set, search in the standard path, else in the optional one 
    \see ResourceSpace::path
*/
	virtual void update(const std::string &conf, bool upgradeVersion, bool std_path);

	virtual std::string defaultExtension() const ;

private:
	// default ctor: not implemented
	dbResourceSpace() ;
	void fillResources(const std::string &, rsResourceSet &) ;
	dbSmartPersistent getResourceRecord(dbConnection &, const std::string & = std::string());
	void getResourceRecords(dbSmartQuery &, dbConnection &, const std::string & = std::string() );
	void loadResourceRecord(const std::string &, const std::string & , dbConnection &, bool update );
#if __cplusplus > 199711L
	std::string getTemporaryResourceFilepath(  const std::string &, std::unique_ptr<oftmpstream> &, bool std_path  );
#else
	std::string getTemporaryResourceFilepath(  const std::string &, std::auto_ptr<oftmpstream> &, bool std_path  );
#endif
	std::string computeNewVersion( const std::string &, dbConnection & ); 
    void lockAndFill(const std::string &filepath, rsResourceSet &myrs) ;
private:
    dbConnectionPool &_connectionPool ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbResourceSpace) ;
} ; 

_ACS_END_NAMESPACE

#endif // _dbResourceSpace_H_
