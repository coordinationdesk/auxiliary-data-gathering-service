// PRQA S 1050 EOF
/*
 *
 *  Copyright 2013-2017, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.1  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 1.2  2015/02/19 14:56:52  francesco.avanzi
 *  getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
 *
 *  Revision 1.1  2013/08/07 14:17:58  lucio.pulvirenti
 *  First issue.
 *
 *
 *
 *
 */

#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

// PRQA S 4101, 4214 EOCLASS
class Repository : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit Repository(dbConnection &conn) ;
	Repository(const Repository &) ;

	virtual ~Repository() throw() ;
	Repository &operator=(const Repository &) ; // operator =

	void setSymbolicname(const std::string &v, dbConnection &c) { set("symbolicname",v, c) ; }
	std::string getSymbolicname(bool &set, dbConnection &c) const { std::string v ; get("symbolicname",v, set, c) ; return v ; }
	void unsetSymbolicname() { unset("symbolicname") ; }

	void setHost(int v, dbConnection &c) { set("host_id",v, c) ; }
	int getHost(bool &set, dbConnection &c) const { int v ; get("host_id",v, set, c) ; return v ; }
	void unsetHost() { unset("host_id") ; }

	void setUsername(const std::string &v, dbConnection &c) { set("username",v, c) ; }
	std::string getUsername(bool &set, dbConnection &c) const { std::string v ; get("username",v, set, c) ; return v ; }
	void unsetUsername() { unset("username") ; }

	void setPassword(const std::string &v, dbConnection &c) { set("password",v, c) ; }
	std::string getPassword(bool &set, dbConnection &c) const { std::string v ; get("password",v, set, c) ; return v ; }
	void unsetPassword() { unset("password") ; }

	void setTransferProtocol(short v, dbConnection &c) { set("transferprotocol_id",v, c) ; }
	short getTransferProtocol(bool &set, dbConnection &c) const { short v ; get("transferprotocol_id",v, set, c) ; return v ; }
	void unsetTransferProtocol() { unset("transferprotocol_id") ; }

	void setRemotePath(const std::string &v, dbConnection &c) { set("remotepath",v, c) ; }
	std::string getRemotePath(bool &set, dbConnection &c) const { std::string v ; get("remotepath",v, set, c) ; return v ; }
	void unsetRemotePath() { unset("remotepath") ; }

	void setTempNameValue(const std::string &v, dbConnection &c) { set("tempname_value",v, c) ; }
	std::string getTempNameValue(bool &set, dbConnection &c) const { std::string v ; get("tempname_value",v, set, c) ; return v ; }
	void unsetTempNameValue() { unset("tempname_value") ; }

	void setTempNameMode(int v, dbConnection &c) { set("tempname_id",v, c) ; }
	int getTempNameMode(bool &set, dbConnection &c) const { int v ; get("tempname_id",v, set, c) ; return v ; }
	void unsetTempNameMode() { unset("tempname_id") ; }

	void setAllowedCiphers(const std::string &v, dbConnection &c) { set("allowedciphers",v, c) ; }
	std::string getAllowedCiphers(bool &set, dbConnection &c) const { std::string v ; get("allowedciphers",v, set, c) ; return v ; }
	void unsetAllowedCiphers() { unset("allowedciphers") ; }

    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_repositories" ; } 

private:
    Repository() ; // not implemented
} ;
// PRQA L:EOCLASS

typedef db::SmartQuery<Repository> RepositoryQuery ;

_ACS_END_NESTED_NAMESPACE


#endif /* REPOSITORY_H_ */
