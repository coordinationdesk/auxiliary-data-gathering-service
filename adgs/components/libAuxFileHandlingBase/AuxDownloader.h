// PRQA S 1050 EOF
/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.3  2013/11/19 13:36:15  ricfer
	Compiler warnings removed. New logging macros adopted.
	
	Revision 2.2  2013/03/12 17:49:01  marpas
	dbCOnnectionPool is mandatory in file actions
	
	Revision 2.1  2008/08/27 13:17:44  lucio.pulvirenti
	virtual method downloadFromValClosest added, to replace similar method from libHarmAuxFileHandling
	
	Revision 2.0  2006/03/01 16:41:23  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.1.1.1  2006/01/31 13:44:10  seralb
	Imported libAuxFileHandlingBase
	

*/

#ifndef _AuxDownloader_H_
#define _AuxDownloader_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class AuxDownloader // PRQA S 2109
{
public:

	exDECLARE_EXCEPTION(NoFileReturnedByQuery,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor 
	 */	
	explicit AuxDownloader(dbConnectionPool &);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~AuxDownloader() throw() ;

	/**
	 * \brief Set the Satellite Name
	 * \param Satellite Name
	 */	
	virtual void setSatelliteName(const std::string&)=0;

	/**
	 * \brief Set the Satellite Name
	 * \param Satellite Mission
	 */	
	virtual void setSatelliteMission(int m)=0;

	/**
	 * \brief Download a file of the given type executing a dbInventoryValIntersectQuery into the specified path and validity interval.
	 *		  If the query returns multiple results, the file with the latest version is downloaded.
	 *		  If the query returns no result, a NoFileReturnedByQuery exception is raised.
	 *		  The download is executed only if the file does not exists at the specified path.
	 * \param The file Type
	 * \param The validity start time (MJD 2000)
	 * \param The validity start time (MJD 2000)
	 * \param The path where to download the file
	 * \retval The file name
	 */
	virtual std::string downloadFromValIntersect(const std::string&, long double, long double,const std::string & ="./")=0;

	/**
	 * \brief Download a file of the given type executing a dbInventoryValCoverQuery into the specified path and validity interval.
	 *		  If the query returns multiple results, the file for which the ValidityStart is nearest to the given StartTime is downloaded.
	 *		  In case of multiple files that satisfy the condition, the file with the latest version is downloaded. 
	 *		  If the query returns no result, a NoFileReturnedByQuery exception is raised.
	 *		  The download is executed only if the file does not exists at the specified path.
	 * \param The file Type
	 * \param The validity start time (MJD 2000)
	 * \param The validity start time (MJD 2000)
	 * \param The path where to download the file
	 * \retval The file name
	 */
	virtual std::string downloadFromValCover(const std::string&, long double, long double,const std::string & = "./")=0;

	/**
	 * \brief Download a file of the given type executing a dbInventoryValCoverQuery into the specified path and validity interval.
	 *		  If the query returns multiple results, the file for which the ValidityStart is nearest to the given StartTime is downloaded.
	 *		  In case of multiple files that satisfy the condition, the file with the latest version is downloaded. 
	 *		  If the query returns no result, a dbInventoryValIntersectQuery is executed.
	 *		  In this case, the file with maximum covarage respect to the given interval is downloaded. If this query returns no result, a NoFileReturnedByQuery exception is raised. 
	 *		  The download is executed only if the file does not exists at the specified path.
	 * \param The file Type
	 * \param The validity start time (MJD 2000)
	 * \param The validity start time (MJD 2000)
	 * \param The path where to download the file
	 * \retval The file name
	 */
	virtual std::string downloadConditionedFromValCover(const std::string&, long double, long double,const std::string & = "./")=0;

	/**
	 * \brief Executes a search into a configured path, looking for files of the given "file type" 
	 * whose validity time window includes provided time of interest "time0".
	 * 		If the search returns multiple results, the file for which the ValidityStart is nearest to the given 
	 * 		"time0" is copied into the destination path.
	 * 		If the search returns no result, a further search is executed, looking for the file of the 
	 *		given "file type" (even if its validity time window does not include provided time of interest
	 *		"time0") whose start or stop validity time is closer to the time of interest. This file is
	 *		copied into the destination path. 
	 *		The copy is executed only if the file does not exists at the specified path.
	 * \param The file Type
	 * \param The time of interest (MJD 2000)
	 * \param The path where to download the file
	 * \retval The file name
	 */
	virtual std::string downloadFromValClosest(const std::string& fileType, 
						   long double time0, 
						   const std::string &destPath = "./")=0;


	virtual std::string checkFileType(const std::string&, const std::string&);
protected:
    dbConnectionPool &getPool() ;
private:

	AuxDownloader(); 
	AuxDownloader(const AuxDownloader & ); 
	AuxDownloader &operator=(const AuxDownloader &);
private:
    dbConnectionPool &_connectionPool ;
};

_ACS_END_NAMESPACE



#endif //_AuxDownloader_H_
