// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/07/10 12:49:15  lucio.pulvirenti
	ECICMF-108: Boolean flag added to versionControlByFileName IF to start searching filename and version from the current version.
	Deprecated method dbQuery::execOneRow() substituted.
	
	Revision 5.2  2014/05/14 15:39:10  marpas
	versionControlByFileName overload: new one returns a pair<filename,fileversion> more generic because can deal also with filename not havig the extension in the filename
	
	Revision 5.1  2014/05/14 14:54:58  marpas
	versionControlByFileName method now can also manage filename with baseline in the version e.g. filename_B001
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.2  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.1  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.10  2004/10/27 14:37:44  seralb
	New method: controlAndGetVersion
	
	Revision 1.9  2004/09/16 12:28:32  seralb
	Method versionControl renamed
	
	Revision 1.8  2004/08/31 14:51:54  seralb
	Modify delta time in query for version control
	
	Revision 1.7  2004/08/24 12:52:13  seralb
	versionControlByFileName method removed, added delta time for query
	
	Revision 1.6  2004/08/23 13:39:43  seralb
	Added versionControlByFileName method
	
	Revision 1.5  2003/09/11 14:46:40  seralb
	Added getVersion method
	
	Revision 1.4  2003/09/09 14:47:35  seralb
	Constructor and versionControl method improved, all the other methods removed
	
	Revision 1.3  2003/07/09 10:22:25  seralb
	Improvement
	
	Revision 1.2  2003/04/30 16:46:59  marpas
	using namespace std no longer into acs includes
	
	Revision 1.1  2003/01/28 10:02:13  seralb
	Control version class
	
	

*/

#ifndef _FileInventoryVersion_H_
#define _FileInventoryVersion_H_ 


#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>
#include <ProductFileNameFactory.h>
#include <ProductFileName.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObjectQuery ;
class dbConnection ;
class FileInventoryVersion 	// PRQA S 2109
{
public:

	/*! class FileInventoryVersionException declaration */
	exDECLARE_EXCEPTION(FileInventoryVersionException, exException) ;   // PRQA S 2131, 2502


	/**
	 * \brief  Default Class constructor 
	 *    
	 */
	explicit FileInventoryVersion(const std::string&);

	/**
	 * \brief  Class destructor 
	 *    
	 */    	
	~FileInventoryVersion() throw() ;

	//methods
	bool    versionControlByDate(const std::string&, std::string&, dbConnection &);
	std::string  getVersion(const std::string& filNam);
	bool versionControlByFileName(const std::string& , std::string&, dbConnection &, const std::string &versPrefix="", bool startFromCurrentVers = false);					
    /*- returns a pair<filename, fileversion>
     */
	std::pair<std::string, std::string> versionControlByFileName(const std::string&, dbConnection &, const std::string &versPrefix="", bool startFromCurrentVers = false);					
	bool controlAndGetVersion(const std::string& , std::string&, dbConnection &);
	
	/**
	 * \brief Check if a given entry name, with version in name, exists in Db. No version is used as query parameter. If version does not occur in name, throw exception.
	 * \param The filename
	 * \param The db connection
	 * \param version prefix, if any
	 * \retval the name with incremented version
	 */    	
	std::string controlByFileName(const std::string& filename, dbConnection &, const std::string &versPrefix="");
	
	/**
	 * \brief Check if a given entry name, with version in name, exists in Db. return the updated file name and return true if it is changed
	 * \param The filename
	 * \param The new name
	 * \param The db connection
	 * \param version prefix, if any
	 * \retval true if filename changed
	 */    	
	bool controlByFileName(const std::string& filename, std::string& outfilename, dbConnection &,  const std::string &versPrefix="");					
		

private:
	FileInventoryVersion(); 
	FileInventoryVersion(const FileInventoryVersion & ); 
	FileInventoryVersion &operator=(const FileInventoryVersion &);

private:		
	//methods

	//attributes
	rsResourceSet 			 _rsConf;
	std::string				 _satName;
	ProductFileNameFactory	 _prodFileName;
	ProductFileName* 		 _fileNameHandler;
	static const double		 _deltaTime;



    ACS_CLASS_DECLARE_DEBUG_LEVEL(FileInventoryVersion) ;

};

_ACS_END_NAMESPACE

#endif //_FileInventoryVersion_H_
