// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libUserNotification$

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/05/10 14:34:46  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.2  2013/10/03 10:30:35  nicvac
	acs sdk warnings processed.
	
	Revision 5.1  2013/06/17 13:22:48  marpas
	finalized adoption of libException 5.x standards
	coding best practices applied
	qa rules applied
	
	Revision 5.0  2013/06/13 18:24:45  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.1  2013/05/24 18:31:27  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
        
*/ 

#include <InventoryEmailNotification.h>

#include <ConfigurationSingleton.h>

#include <Application.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <XMLOstream.h>
#include <XMLTransformer.h>

#include <File.h>

#include <Filterables.h>

#include <boost/algorithm/string.hpp> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InventoryEmailNotification)

InventoryEmailNotification::InventoryEmailNotification(string const& dbKey):
	EmailUserNotification("InventoryEmailNotification", dbKey),
	_filename(),
	_filetype(),
	_groupId(),
	_groupName(),
	_groupDescription(),
	_groupEmailDests(),
	_groupFiletypes()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id) ;
}

InventoryEmailNotification::~InventoryEmailNotification() throw() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631

}

InventoryEmailNotification& InventoryEmailNotification::setInfo( std::string const& filename, std::string const& filetype, std::string const& groupId) {
	_filename = filename;
	_filetype = filetype;
	_groupId = groupId;
	return *this; //Allow serial .setX() calls
}


bool InventoryEmailNotification::setUp() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.") ;

	dbSet set;        
	try {

		int groupId(0); istringstream ss(_groupId); ss>>groupId;

		//Take notification info from DB
		//This query is SQL standard
		ostringstream sqlStrm; sqlStrm<<"\
			SELECT ft.filetype, crGr.name, crGr.description, crGr.mailaddresses \
			FROM t_cr_x_filetypes crFt \
			INNER JOIN t_crgroups crGr \
				on crFt.crgroup_id  = crGr.id_crgroup \
			INNER JOIN t_filetypes ft \
				on crFt.filetype_id = ft.id_filetype \
			WHERE crGr.id_crgroup = "<<groupId;

		SQLString querySQL ( this->getDbKey() , sqlStrm.str() );

		this->executeSQLStatement (querySQL, set);

	} catch(exception& ex) {
		ACS_THROW( exInventoryEmailNotificationCriticalException(ex, "error getting mail notifications info for notification ["+getDescription()+"]") );
	}


	ACS_COND_THROW( // PRQA S 3081
		set.empty(),
		exInventoryEmailNotificationCriticalException("No data configured for t_crgroups, id ["+_groupId+"]"));

	_groupFiletypes.clear();
	string filetype("");
	dbRow row(this->getDbKey() );
	while ( set.getNextRow(row) ) {
		uint16_t i = 0 ;

		row.column(i++, filetype);	_groupFiletypes.push_back(filetype);
		if ( _groupName == "" ) {
			row.column(i++, _groupName);
			row.column(i++, _groupDescription);
			row.column(i++, _groupEmailDests);
		}
	}

	return true;

}

bool InventoryEmailNotification::prepare() {

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Preparing mail for Notification ["<<getDescription()<<"]") ;

	//Ref to config
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	//Format the subject from template
	string subjectTemplate("");
	conf.getValue( "InventoryEmailNotification.SubjectTemplate", subjectTemplate );

	boost::replace_all( subjectTemplate, "${filename}"        , _filename        );
	boost::replace_all( subjectTemplate, "${filetype}"        , _filetype        );
	boost::replace_all( subjectTemplate, "${groupId}"         , _groupId         );
	boost::replace_all( subjectTemplate, "${groupName}"       , _groupName       );
	boost::replace_all( subjectTemplate, "${groupDescription}", _groupDescription);

	setSubject(subjectTemplate) ;
	
	//Format the body
	{
		ostringstream os;
		{
			XMLOstream xos(os, "MailNotification");
			{
				string utcs; DateTime currentTime; currentTime.utcs(utcs);
				XMLOstream::Tag tag(xos, "UtcsTime", utcs, "", true);
			}
			{	XMLOstream::Tag tag(xos, "Filename", _filename, "", true);	             }
			{	XMLOstream::Tag tag(xos, "Filetype", _filetype, "", true);	             }
			{	XMLOstream::Tag tag(xos, "GroupId", _groupId, "", true);	                 }
			{	XMLOstream::Tag tag(xos, "GroupName", _groupName, "", true);	             }
			{	XMLOstream::Tag tag(xos, "GroupDescription", _groupDescription, "", true); }
			{	XMLOstream::Tag tag(xos, "GroupEmailDests", _groupEmailDests, "", true);	 }
			{
				ostringstream count;
				count << "count=\"" << _groupFiletypes.size() << "\"";
				XMLOstream::Tag tag(xos, "List_of_Filetypes", "", count.str());
				for ( vector<string>::iterator it=_groupFiletypes.begin(); it!=_groupFiletypes.end(); ++it ) { // PRQA S 4238
					XMLOstream::Tag tag2(xos, "Filetype", *it, "", true);
				}
			}
		}

		string mailFormat ;
		conf.getValue("InventoryEmailNotification.MailFormat", mailFormat);

		string mailFormatXslFile ;
		conf.getValue("InventoryEmailNotification.MailFormatXSL."+mailFormat, mailFormatXslFile);

		//If xsl not defined, the text is kept as the original xml
		if ( not mailFormatXslFile.empty() ) {
			XMLTransformer xmlTransformer;
			string formattedBody("");

			string mailFormatXslFilePathAbs(mailFormatXslFile);
			string mailFormatXslFilePathDef( string(base::defaultConfPath()) +"/"+ mailFormatXslFile );
			if ( not File::exists( mailFormatXslFilePathAbs ) && File::exists( mailFormatXslFilePathDef ) ) {
				mailFormatXslFilePathAbs = mailFormatXslFilePathDef;
			}

			xmlTransformer.string2string( mailFormatXslFilePathAbs, os.str(), formattedBody); // PRQA S 3081

			os.str(formattedBody);
		}

		//Set the actual body
		setBody(os.str()) ;

	}

	//*/ Set remaining fields on super class
	setDestAddresses(_groupEmailDests) ;

	string conf_mailFormat ; 
    bool cbtmp = false ;
	conf.getValue("InventoryEmailNotification.MailFormat.attachBody.value", cbtmp);
    setAttachBody(cbtmp) ;
	conf.getValue("InventoryEmailNotification.MailFormat", conf_mailFormat);
    string cstmp ;
	conf.getValue("InventoryEmailNotification.MailFormatXSL."+conf_mailFormat+".contentType.value", cstmp);
    setContentType(cstmp) ;
	//*/

	return true;
}

void InventoryEmailNotification::clean() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	return;
}


_ACS_END_NAMESPACE
