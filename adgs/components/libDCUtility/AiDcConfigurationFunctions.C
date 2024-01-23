// PRQA S 1050 EOF
/*
	Copyright 2013-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDataStorage Library $

	$Id$

	$Author$
	
    $Log$
    Revision 1.7  2017/10/23 10:00:37  marpas
    version management is inside this file now

    Revision 1.6  2016/11/02 10:43:50  chicas
    Message improved

    Revision 1.5  2016/05/11 09:27:29  danalt
    use of ACS_THROW and ACS_COND_THROW

    Revision 1.4  2016/03/26 21:42:41  marpas
    using new db I/F

    Revision 1.3  2013/12/04 14:45:45  chicas
    added excludeHiddenFiles flag

    Revision 1.2  2013/11/08 14:30:35  lucio.pulvirenti
    retrieveFiletypeId: mngsngext is set to false.

    Revision 1.1  2013/10/22 12:40:07  lucio.pulvirenti
    First issue.



*/
#include <AiDcConfigurationFunctions.h>

#include <dcutilityV.h> 
#include <dbSmartQuery.h> 
#include <dbConnection.h>
#include <dbSmartPersistent.h>
#include <TransferProtocol.h>
#include <TmpNameFileTransfer.h>
#include <TmpNameStrategyTable.h>


#include <Network.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)


namespace { 
    dcutilityV version ;
    
    struct AiDcConfigurationFunctionsDebug {	

        ACS_CLASS_DECLARE_DEBUG_LEVEL(AiDcConfigurationFunctionsDebug); 
    } ;


    ACS_CLASS_DEFINE_DEBUG_LEVEL(AiDcConfigurationFunctionsDebug) ;

} //namespace

#define ACS_DCF_WRITE_DEBUG(level, m) ACS_FUNC_WRITE_DEBUG(AiDcConfigurationFunctionsDebug,level, m)
#define ACS_DCF_ANNOUNCE_DEBUG(level) ACS_FUNC_ANNOUNCE_DEBUG(AiDcConfigurationFunctionsDebug,level)
#define ACS_DCF_BGN_DEBUG(level) ACS_FUNC_BGN_DEBUG(AiDcConfigurationFunctionsDebug,level)
#define ACS_DCF_END_DEBUG ACS_CLASS_END_DEBUG


dbPersistent::IdType AiDcConfigurationFunctions::retrieveHostId( const string & hostname, dbConnection & conn, bool createIfNotExisting)
{
	ACS_DCF_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	ACS_COND_THROW(hostname.empty(), AiDcConfigurationFunctionsException("Host not valorized"));	 // PRQA S 3081

	const string &dbKey = conn.getKey() ;
	dbSmartQuery sq("t_knownhosts", dbKey);
	{
		dbQueryParameters p(dbKey);
		p.addParameter(dbParam(dbKey, "t_knownhosts","host_name", dbParam::EQUAL_TO, dbConv(dbKey, hostname))) ; // PRQA S 3050, 3081
		ACS_DCF_WRITE_DEBUG(ACS_MID_VERB, "Going to query t_knownhosts where \"" << p.getSqlString() << "\"" );
		sq.addParameter(p) ;
	}
	sq.doQuery(conn) ;
	dbPersistent::IdType rId = 0;
	if( !sq.size() ) {
		
		ACS_COND_THROW(!createIfNotExisting, RecordNotExisting("t_knownhosts with host_name=\""+hostname+"\" not existing")) ; // PRQA S 3081

		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "No t_knownhosts record with host_name=\"" << hostname << "\" occurring" );
		dbSmartPersistent sp("t_knownhosts", conn) ;
        sp.set("host_name", hostname, conn); 
		try {
			string ipAddress = net::Network::getIPAddresses(hostname)[0];
        	sp.set("ip_address", ipAddress, conn);
		}
		catch( exception &e )
		{
			ACS_LOG_WARNING("Error getting IP address for host \"" << hostname << "\": " << e.what()) ;
		}
        sp.save(conn);
		rId = sp.getId();
		ACS_LOG_INFO("New t_knownhosts record with host_name=\"" << hostname << "\" created: id=" << rId) ;

	}
	else {
		rId = sq.at(0) -> getId();
		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "t_knownhosts record already occurring with host_name=\"" << hostname << "\" id=" << rId);
	}

	return rId;

	
}

dbPersistent::IdType AiDcConfigurationFunctions::retrieveRepositoryId( const std::string & symbolicName, dbConnection & conn)
{
	ACS_DCF_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
// some checks	
	ACS_COND_THROW(symbolicName.empty(), AiDcConfigurationFunctionsException("Repository symbolicName not valorized"));	 // PRQA S 3081
	
	const string &dbKey = conn.getKey() ;
	dbSmartQuery sq("t_repositories", dbKey);
	{
		dbQueryParameters p(dbKey);
		p.addParameter(dbParam(dbKey, "t_repositories","symbolicname", dbParam::EQUAL_TO, dbConv(dbKey, symbolicName))) ; // PRQA S 3050, 3081
		sq.addParameter(p) ;
	}
	
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, AiDcConfigurationFunctionsException("No t_repositories rec with symbolicname=\""+symbolicName+"\" occurring"));	 // PRQA S 3081
	
	ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "t_repositories record with symbolicName=\"" << symbolicName << "\" has id <" << sq.at(0)->getId() << ">") ;
	return sq.at(0)->getId();
	
}


dbPersistent::IdType AiDcConfigurationFunctions::retrieveRepositoryId( const Repository & repository, dbPersistent::IdType hostId, dbConnection & conn, bool createIfNotExisting)
{
	ACS_DCF_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

// some checks	
	ACS_COND_THROW(repository.name.empty(), AiDcConfigurationFunctionsException("Repository Name not valorized"));	 // PRQA S 3081
	ACS_COND_THROW(repository.path.empty(), AiDcConfigurationFunctionsException("Repository Path not valorized"));	 // PRQA S 3081
	ACS_COND_THROW(repository.user.empty(), AiDcConfigurationFunctionsException("Repository User not valorized"));	 // PRQA S 3081
		
// check if repository already exists
	const string &dbKey = conn.getKey() ;
	dbSmartQuery sq("t_repositories", dbKey);
	string symbolicNameCondition;
	{
		dbQueryParameters p(dbKey);
		p.addParameter(dbParam(dbKey, "t_repositories","symbolicname", dbParam::EQUAL_TO, dbConv(dbKey, repository.name))) ; // PRQA S 3050, 3081
		symbolicNameCondition = p.getSqlString();
		sq.addParameter(p) ;
	}
	
	int transferProtocolId = TransferProtocol::instance(dbKey)->id(StringUtils::uppercase(repository.transferMethod));
	string fourSetCondition;
	{
		dbQueryParameters p(dbKey);
		p.addParameter(dbParam(dbKey, "t_repositories","transferprotocol_id", dbParam::EQUAL_TO, dbConv(dbKey, transferProtocolId))) ; // PRQA S 3050, 3081
		p.addParameter(dbParam(dbKey, "t_repositories","host_id", dbParam::EQUAL_TO, dbConv(dbKey, hostId))) ; // PRQA S 3050, 3081
		p.addParameter(dbParam(dbKey, "t_repositories","username", dbParam::EQUAL_TO, dbConv(dbKey, repository.user))) ; // PRQA S 3050, 3081
		p.addParameter(dbParam(dbKey, "t_repositories","remotepath", dbParam::EQUAL_TO, dbConv(dbKey, repository.path))) ; // PRQA S 3050, 3081
		fourSetCondition = p.getSqlString();
		sq.addParameter(p, dbGeneralQueryIF::OR) ;
	}
	
	sq.doQuery(conn) ;

	string tmpFilenameValue;
	int tmpFilenameId;
	bool transferStrategyDefined = TransferStrategyDefined( repository, tmpFilenameValue, tmpFilenameId, dbKey );
	
	dbPersistent::IdType rId = 0;
	if( sq.size() == 0 ) {
		
		ACS_COND_THROW(!createIfNotExisting, RecordNotExisting("t_repositories record with \""+symbolicNameCondition+ "\" OR \""+fourSetCondition+"\" not existing")) ; // PRQA S 3081

// create the new record
		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "No t_repositories record occurs with NEITHER \"" << symbolicNameCondition << "\" NOR \"" << fourSetCondition << "\": going to create") ;
		
 		dbSmartPersistent r("t_repositories", conn);	
		r.set("symbolicname", repository.name, conn);
		r.set("transferprotocol_id", transferProtocolId, conn);
		r.set("host_id", hostId, conn);
 		r.set("username", repository.user, conn);
 		r.set("remotepath", repository.path, conn);
		
		if( !repository.password.empty() ) {
			r.set("password", repository.password, conn);
		}
		
		r.set("tempname_id", tmpFilenameId, conn);
		r.set("tempname_value", tmpFilenameValue, conn);
		r.save(conn);
 
 		rId = r.getId();
		ACS_LOG_INFO("New t_repositories record just created with id=" << rId) ;

	}
	else if( sq.size() == 1 ) {
		rId = sq.at(0)->getId();
		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "t_repositories record already occurs with either \"" << symbolicNameCondition << "\" or \"" << fourSetCondition << "\": id=" << rId);

		bool updated = UpdateRepository( repository, transferProtocolId, hostId, transferStrategyDefined, tmpFilenameId, tmpFilenameValue, sq.at(0), conn);
		ACS_LOG_INFO("t_repositories record " << repository.name << " [ id=" << rId << (updated ? "] " : "]  NOT ") << "updated") ;
		
	} // else if( sq.size() == 1 )
	else { // sq.size() > 1	
            //
            string symbolicname_1;
            string symbolicname_2;
            bool null ;
	        sq.at(0)->get("symbolicname" , symbolicname_1, null , conn ) ;
            
	        sq.at(1)->get("symbolicname" , symbolicname_2, null , conn ) ;
		    
            ACS_LOG_ERROR("t_repositories record " << repository.name << " cannot be updated as new values conflict with repository " <<(repository.name == symbolicname_1 ?  symbolicname_2 : symbolicname_1)  ) ;
            ACS_LOG_ERROR( " Conflicting conditions are: \" "<< fourSetCondition << "\"");

            ostringstream exMsg;
            exMsg 	<< " " << sq.size() << " t_repositories records occur with \"" << symbolicNameCondition << "\" OR \"" << fourSetCondition << "\":  cannot decide what to do";

		    ACS_THROW(AiDcConfigurationFunctionsException(exMsg.str() ) );

	}

	return rId;
}


bool AiDcConfigurationFunctions::TransferStrategyDefined( const Repository & repository, string & tmpFilenameValue, int & tmpFilenameId, const string & dbKey)
{
	ACS_DCF_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	bool defined = false;
	string tmpFilename = dc::TmpNameFileTransfer::toString(dc::TmpNameFileTransfer::instance()->getTmpNameStrategy());
	tmpFilenameValue = dc::TmpNameFileTransfer::instance()->getTmpNameValue();
	if ( !repository.tmpFilename.empty() && !repository.tmpFilenameValue.empty() ) {

		try {
			dc::TmpNameFileTransfer::TmpNameStrategy strategy = dc::TmpNameFileTransfer::fromString(repository.tmpFilename);
			dc::TmpNameFileTransfer::instance()->checkConfigurationConsistency(strategy, repository.tmpFilenameValue);
			ACS_DCF_WRITE_DEBUG(ACS_MID_VERB, "Values from input are consistent: tmpFilename=\"" << repository.tmpFilename << "\" tmpFilenameValue=\"" << repository.tmpFilenameValue << "\"");
			tmpFilename = repository.tmpFilename;
			tmpFilenameValue = repository.tmpFilenameValue;
			defined = true;
		}
		catch(exception &e)
		{
			ACS_LOG_WARNING( e.what() );
		}
	}
	else {
		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "To be considered, transfer strategy parameters must be BOTH not empty: TmpFilename=\"" << repository.tmpFilename 
				<< "\" TmpFilenameValue=\"" << repository.tmpFilenameValue << "\"");
	}
	
	tmpFilenameId = TmpNameStrategyTable::instance(dbKey)->id(StringUtils::uppercase(tmpFilename));
	ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "Going to return tmpFilenameId=" << tmpFilenameId << " tmpFilenameValue=\"" << tmpFilenameValue << "\"");
	
	return defined;
}

bool AiDcConfigurationFunctions::UpdateRepository( const Repository & repository, 
												int transferProtocolId,
												int hostId,
												bool transferStrategyDefined,
												int tmpFilenameId,
												const string & tmpFilenameValue,
												dbSmartPersistent *rec,
												dbConnection & conn)
{

	ACS_DCF_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	string symbolicname;
    bool null ;
	rec->get("symbolicname" , symbolicname, null , conn ) ;
	bool update = false;
	if( repository.name != symbolicname ) {
		update = true;
		ACS_LOG_WARNING("t_repositories record symbolicname=\"" << symbolicname << "\" not matching the input one (\"" << repository.name << "\") going to update");
		rec->set("symbolicname" , repository.name, conn) ;
	}
	else {
		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "t_repositories record symbolicname matches the input one: \"" << symbolicname << "\"");

	// get all record values to check if record must be updated
		int transferprotocol_id;
		rec->get("transferprotocol_id" , transferprotocol_id, null, conn) ;
		int host_id;
		rec->get("host_id" , host_id, null, conn) ;
		string username;
		rec->get("username" , username, null, conn) ;
		string remotepath;
		rec->get("remotepath" , remotepath, null, conn) ;
		string password;
		rec->get("password" , password, null, conn) ;
		int tempname_id;
		rec->get("tempname_id" , tempname_id, null, conn) ;
		string tempname_value;
		rec->get("tempname_value" , tempname_value, null, conn) ;

		if( transferprotocol_id != transferProtocolId || host_id != hostId || username != repository.user || remotepath != repository.path ||
			( !repository.password.empty() && repository.password != password ) ||
			( transferStrategyDefined && (tempname_id != tmpFilenameId || tempname_value != tmpFilenameValue) ) 
		) {
			update = true;
			rec->set("transferprotocol_id" , transferProtocolId, conn) ;
			rec->set("host_id" , hostId, conn ) ;
			rec->set("username" , repository.user, conn ) ;
			rec->set("remotepath" , repository.path, conn ) ;
		}

	}

	if( update ) {
		if( !repository.password.empty() ) {
			rec->set("password" , repository.password, conn) ;
		}

		if( transferStrategyDefined ) {
			rec->set("tempname_id" , tmpFilenameId, conn) ;
			rec->set("tempname_value" , tmpFilenameValue, conn) ;
		}

		rec->update(conn);
	}
	
	return update;

}

dbPersistent::IdType AiDcConfigurationFunctions::retrieveFiletypeId( const string & filetype, const string & group, dbConnection & conn, const string & regExp, bool createIfNotExisting ,bool excludeHiddenFiles )
{
	ACS_DCF_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	ACS_COND_THROW(filetype.empty(), AiDcConfigurationFunctionsException("filetype not valorized"));	 // PRQA S 3081
	ACS_COND_THROW(group.empty(), AiDcConfigurationFunctionsException("group not valorized"));	 // PRQA S 3081

	const string &dbKey = conn.getKey() ;
	dbSmartQuery sq("t_filetypes", dbKey);
	{
		dbQueryParameters p(dbKey);
		p.addParameter(dbParam(dbKey, "t_filetypes","filetype", dbParam::EQUAL_TO, dbConv(dbKey, filetype))) ; // PRQA S 3050, 3081
		ACS_DCF_WRITE_DEBUG(ACS_MID_VERB, "Going to query t_filetypes where \"" << p.getSqlString() << "\"" );
		sq.addParameter(p) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() > 1, AiDcConfigurationFunctionsException("More than one t_filetypes records with filetype=\"" + filetype + "\""));	 // PRQA S 3081

	dbPersistent::IdType rId = 0;
	if( sq.size() == 0) {
		
		ACS_COND_THROW(!createIfNotExisting, RecordNotExisting("t_filetypes record with filetype=\""+filetype+ "\" not existing")) ; // PRQA S 3081

		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "t_filetypes record with filetype=\"" + filetype + "\" not occurring in database" );
		dbSmartPersistent sp("t_filetypes", conn) ;
        sp.set("filetype", filetype, conn); 
		string nameregularexpression;
		if( StringUtils::isBlank(regExp) ) { 
            if (excludeHiddenFiles)
            {
			    nameregularexpression = "^[^.].*" + filetype + ".*$";
            }
            else
            {
			    nameregularexpression = "^.*" + filetype + ".*$";
            }
            ACS_LOG_WARNING("\"" << filetype << "\" filetype not occurring in db and regular expression is missing: going to set \"" << nameregularexpression << "\"");
		}
		else { 
			nameregularexpression = regExp;
		}
		sp.set("nameregularexpression", nameregularexpression, conn); 
		sp.set("group", group, conn);
		sp.set("mngsngext", false, conn);
        sp.save(conn);
		rId = sp.getId();
		ACS_LOG_INFO("New t_filetypes record with filetype=\"" << filetype << "\" created: id=" << rId) ;
	}
	else {
		dbSmartPersistent *rec = sq.at(0) ;
		rId = rec->getId();
		ACS_DCF_WRITE_DEBUG(ACS_LOW_VERB, "t_filetypes record already occurring with filetype=\"" << filetype << "\" id=" << rId);
		if( !StringUtils::isBlank(regExp) ) { 
			string nameregularexpression;
			bool isNull;
			rec->get("nameregularexpression", nameregularexpression, isNull, conn);
			if( isNull || nameregularexpression != regExp ) {
				ostringstream exMsg;
				exMsg 	<< "nameregularexpression of t_filetypes record with filetype=\"" << filetype << "\" (id=" << rId << ") is \""
						<< nameregularexpression << "\" while the one occurring in configuration file is \"" << regExp << "\": cannot update the record";
				ACS_THROW(AiDcConfigurationFunctionsException(exMsg.str() ) );
			}
		}
	}
	
	return rId;

}
	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////******** Repository struct ctor, copy ctor and assignment operator **********/////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AiDcConfigurationFunctions::Repository::Repository():
									name(),
									transferMethod(),
									host(),
									path(),
									user(),
									password(),
									tmpFilename(),
									tmpFilenameValue()
{
}								

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AiDcConfigurationFunctions::Repository::Repository(AiDcConfigurationFunctions::Repository const &o):
																	name(o.name),
																	transferMethod(o.transferMethod),
																	host(o.host),
																	path(o.path),
																	user(o.user),
																	password(o.password),
																	tmpFilename(o.tmpFilename),
																	tmpFilenameValue(o.tmpFilenameValue)
{
}								

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AiDcConfigurationFunctions::Repository & AiDcConfigurationFunctions::Repository::operator = (AiDcConfigurationFunctions::Repository const &o)
{
	if( this != &o ) {
		name=o.name;
		transferMethod=o.transferMethod;
		host=o.host;
		path=o.path;
		user=o.user;
		password=o.password;
		tmpFilename=o.tmpFilename;
		tmpFilenameValue=o.tmpFilenameValue;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const AiDcConfigurationFunctions::Repository & v)
{
	s	<< "****************** Repository ********************\n"
		<< "\tname=\"" 				<< v.name 				<< "\"\n"
	  	<< "\ttransferMethod=\"" 	<< v.transferMethod 	<< "\"\n"
		<< "\thost=\"" 				<< v.host 				<< "\"\n"
		<< "\tpath=\"" 				<< v.path 				<< "\"\n"
		<< "\tuser=\"" 				<< v.user 				<< "\"\n"
		<< "\tpassword=\"" 			<< v.password 			<< "\"\n"
		<< "\ttmpFilename=\"" 		<< v.tmpFilename 		<< "\"\n"
		<< "\ttmpFilenameValue=\"" 	<< v.tmpFilenameValue 	<< "\"\n"
		<< "**************************************************\n";
	
	return s ;
}

exostream &operator<<(exostream &s, const AiDcConfigurationFunctions::Repository & v)
{
	s	<< "****************** Repository ********************\n"
		<< "\tname=\"" 				<< v.name 				<< "\"\n"
	  	<< "\ttransferMethod=\"" 	<< v.transferMethod 	<< "\"\n"
		<< "\thost=\"" 				<< v.host 				<< "\"\n"
		<< "\tpath=\"" 				<< v.path 				<< "\"\n"
		<< "\tuser=\"" 				<< v.user 				<< "\"\n"
		<< "\tpassword=\"" 			<< v.password 			<< "\"\n"
		<< "\ttmpFilename=\"" 		<< v.tmpFilename 		<< "\"\n"
		<< "\ttmpFilenameValue=\"" 	<< v.tmpFilenameValue 	<< "\"\n"
		<< "**************************************************\n";
	return s ;
}

template<>
void rsResourceSet::getValue(const std::string &res, AiDcConfigurationFunctions::Repository &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".Name")->get(v.name) ;
	{
		string tmp;
		getValue(res+".TransferMethod")->get(tmp) ;
		if( StringUtils::equalsNoCase(tmp, "fs") ) {
			tmp = "file";
		}
		else { /* empty */}
		v.transferMethod = tmp;
	}
	getValue(res+".Host")->get(v.host) ;
	getValue(res+".Path")->get(v.path) ;
	getValue(res+".User")->get(v.user) ;
	if( hasKey(res+".Password") ) {
		getValue(res+".Password")->get(v.password) ;
	}
    else { v.password = "" ; }
	if( hasKey(res+".TmpFilename") ) {
		getValue(res+".TmpFilename")->get(v.tmpFilename) ;
	}
    else { v.tmpFilename = "" ; }
	if( hasKey(res+".TmpFilenameValue") ) {
		getValue(res+".TmpFilenameValue")->get(v.tmpFilenameValue) ;
	}
    else { v.tmpFilenameValue = "" ; }
	
} 



//--

_ACS_END_NAMESPACE

