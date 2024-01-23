// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDataStorage Library $

	$Id$

	$Author $
	
    $Log$
    Revision 1.1  2013/10/22 12:40:07  lucio.pulvirenti
    First issue.



*/


#ifndef _AiDcConfigurationFunctions_H_
#define _AiDcConfigurationFunctions_H_


#include <exException.h>
#include <dbPersistent.h>
#include <rsResourceSet.h>

/**
 * \brief
 *  Collection of both structures and functions needed to configure both AI and DC given xml configuration file 
 **/

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbSmartPersistent;

 
class AiDcConfigurationFunctions  { 

public:
		
    exDECLARE_EXCEPTION(AiDcConfigurationFunctionsException, exException) ; // Base Exception. // PRQA S 2131, 2502
    exDECLARE_EXCEPTION(RecordNotExisting, AiDcConfigurationFunctionsException) ;  // PRQA S 2131, 2502
	
				
	/*! \brief This structure contains the values extracted from IORepositoryType nodes of xsd files to configure both DC and AI 	 
	*/
	struct Repository {

		Repository() ;
		~Repository() throw() {} 
		Repository(const Repository& ) ;
		Repository& operator=(const Repository& ) ;

		std::string name;
		std::string transferMethod;
		std::string host;
		std::string path;
		std::string user;
		std::string password;
		std::string tmpFilename;
	  	std::string tmpFilenameValue;
	} ;

	/*! \brief function to get id of t_knownhost table given hostname. If record does not exist and createIfNotExisting flag set it will be created  

	  \param hostname  : host name 
	  \param conn : db connection
	  \param createIfNotExisting : if set, record will be created if not existing.
	  \return      t_knownhost id;
	  \throw      - AiDcConfigurationFunctions::RecordNotExisting if createIfNotExisting flag not set and record not existing 
            	  - exception if some error occurs

	*/
	static dbPersistent::IdType retrieveHostId( const std::string & hostname, dbConnection & conn, bool createIfNotExisting = true);

	/*! \brief function to get id of t_repositories table. If record does not exist and createIfNotExisting flag set it will be created  

	  \param rep  : Repository struct with all needed information
	  \param hostId  : t_knownhosts id
	  \param conn : db connection
	  \param createIfNotExisting : if set, record will be created if not existing.
	  \return      t_repositories id;
	  \throw      - AiDcConfigurationFunctions::RecordNotExisting if createIfNotExisting flag not set and record not existing 
            	  - exception if some error occurs

	*/
	static dbPersistent::IdType retrieveRepositoryId( const Repository & rep, dbPersistent::IdType hostId, dbConnection &, bool createIfNotExisting = true);

	 /*! \brief function to get id of t_repositories table.

	  \param symbolicName  : Repository symbolicname
	  \param conn : db connection
	  \return      t_repositories id;
	  \throw      - exception if some error occurs

	*/
	static dbPersistent::IdType retrieveRepositoryId( const std::string & symbolicName, dbConnection &);


	 /*! \brief function to get id of t_filetypes table.

	  \param filetype  : filetype value. If record does not exist and createIfNotExisting flag set it will be created 
	  \param group  : filetype group
	  \param conn : db connection
	  \param regExp : regular expression associated to filetype. If not valorized, set to ^.*filetype.*$;
	  \param createIfNotExisting : if set, record will be created if not existing.
	  \return      t_filetypes id;
	  \throw      - AiDcConfigurationFunctions::RecordNotExisting if createIfNotExisting flag not set and record not existing 
            	  - exception if some error occurs

	*/
	static dbPersistent::IdType retrieveFiletypeId( const std::string & filetype, const std::string & group, dbConnection & conn, const std::string & regExp = std::string(), bool createIfNotExisting = true, bool excludeHiddenFiles = false);

private:
		
	static bool UpdateRepository( const Repository &, int, int, bool, int, const std::string &, dbSmartPersistent *, dbConnection &);
	static bool TransferStrategyDefined( const Repository & , std::string & , int &, const std::string & )	;
		
};

std::ostream &operator<<(std::ostream &, const AiDcConfigurationFunctions::Repository &) ;

acs::exostream &operator<<(acs::exostream &, const AiDcConfigurationFunctions::Repository &) ;

template<>
void rsResourceSet::getValue(const std::string &res, AiDcConfigurationFunctions::Repository &v) const ;

_ACS_END_NAMESPACE

#endif // _AiDcConfigurationFunctions_H_
