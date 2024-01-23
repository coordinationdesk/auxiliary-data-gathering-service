// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.1  2016/09/29 09:12:45  marpas
	fileClass description has to be written into XML
	
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:49  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:50  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:18  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.5  2012/11/28 14:53:45  marpas
	bug fixed, file class must be unchanged - not the description
	
	Revision 2.4  2012/11/19 22:51:15  marpas
	compile with PDS_OLD_SCHEMA to use old pds schema.
	dbReceptionRule and related Query class introduced
	
	Revision 2.3  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/04/08 10:08:19  marpas
	system concept introduced in order to have a writer for a system with no satellite
	
	Revision 2.1  2006/04/04 15:03:41  paoscu
	GoceFixedHeaderFactory added.
	EarthExplorerFixedHeaderWriter replaces CryosatFixedHeaderWriter
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/11/12 08:35:49  paoscu
	fixed
	
	Revision 1.4  2004/10/01 16:01:47  marpas
	File_class description instead of its code into XML header
	
	Revision 1.3  2004/05/13 13:51:26  paoscu
	files closed
	
	Revision 1.2  2004/05/05 18:23:38  paoscu
	Work in progress.
	
	Revision 1.1  2004/05/03 14:53:59  paoscu
	New haeader writers.
	
		

*/

#include <EarthExplorerFixedHeaderWriter.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbInventoryObject.h>
#include <dbConnectionPool.h>
#include <dbFileType.h>
#include <FileClasses.h>
#include <DateTime.h>
#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

static string nameDate(DateTime const &d)
{
	//Datetime exported in format CCSDS compact.
	ostringstream os;
    DateTime::UTCD utcd = d ;
	os
	   <<"UTC=" 
	   <<setfill('0')
	   << setw(4) << utcd.year
	   << "-" 
	   << setw(2) << utcd.month
	   << "-" 
	   << setw(2) << utcd.day
	   <<"T"
	   << setw(2) << utcd.hour 
	   << ":" 
	   << setw(2) << utcd.min
	   << ":" 
	   << setw(2) << utcd.sec;
	   
	return os.str();
	
}



EarthExplorerFixedHeaderWriter::EarthExplorerFixedHeaderWriter( std::string const & satelliteId, 
                                                                dbInventoryObject const & inv,
																string const &system,
																XMLOstream &s) : 
	dbFixedHeaderWriter(inv,system,s)  ,
	_satelliteId(satelliteId) ,
	_eartExplorerFileTag(0) ,
	_dataBlockTag(0)
{}


EarthExplorerFixedHeaderWriter::~EarthExplorerFixedHeaderWriter() throw() 
{
	end() ;
}


void EarthExplorerFixedHeaderWriter::start()
{
	_eartExplorerFileTag = new XMLOstream::Tag(getXmlStream(),"Earth_Explorer_File");
}


void EarthExplorerFixedHeaderWriter::run(bool addDataBlock)
{
	bool set;
    const string dbKey = "" ;

	dbFileType *fileTypeRecord=dynamic_cast<dbFileType*>(dbPersistentFactoryChain::instance()->newObject("dbFileType", dbKey));
	if(!fileTypeRecord)
	{
		exIllegalValueException e("dbFileType dynamic cast failed");
		ACS_THROW(e);
	}
	dbPersistentScopeGuard fileTypeRecordGuard(fileTypeRecord);

	
	string fileType=getInventoryObj().getFileType(&set);
	if(!set || fileType.empty())
	{
		exIllegalValueException e("file type not set");
		ACS_THROW(e);
	}

	fileTypeRecord->setFileType(fileType);
	if(!fileTypeRecord->load(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey))))
	{
		exIllegalValueException e(string("Can't load file type \"")+fileType+"\" .");
		ACS_THROW(e);
	}
	
	string fileName= getInventoryObj().getFileName(&set);
	if(!set || fileName.empty())
	{
		exIllegalValueException e("file name not set");
		ACS_THROW(e);
	}

	string description=fileTypeRecord->getDescription();
	
    FileClasses* fc = FileClasses::instance(dbKey) ;
    string fclass = getInventoryObj().getFileClass(&set) ;
	if(!set || fclass.empty())
	{
		exIllegalValueException e("file class not set");
		ACS_THROW(e);
	}
	string fileClassDesc= fc->name(fclass) ;

#ifdef PDS_OLD_SCHEMA
	string userFileType= fileTypeRecord->getUserFileType(&set);
#else
	string userFileType= fileTypeRecord->getFileType(&set);
#endif
	if(!set || userFileType.empty())
	{
		exIllegalValueException e("User file type not set");
		ACS_THROW(e);
	}

	
	DateTime validityStart=getInventoryObj().getValidityStart(&set);
	if(!set )
	{
		exIllegalValueException e("validity start not set");
		ACS_THROW(e);
	}

	DateTime validityStop=getInventoryObj().getValidityStop(&set);
	if(!set)
	{
		exIllegalValueException e("validity stop not set");
		ACS_THROW(e);
	}

	string fileVersion= getInventoryObj().getFileVersion(&set);
	if(!set || fileVersion.empty())
	{
		exIllegalValueException e("file version not set");
		ACS_THROW(e);
	}
	
	string creator=getInventoryObj().getSource(&set);
	if(!set || creator.empty())
	{
		exIllegalValueException e("Source not set");
		ACS_THROW(e);
	}
	
	string creatorVersion=getInventoryObj().getSourceSwVersion(&set);
	if(!set || creatorVersion.empty())
	{
		exIllegalValueException e("Source version not set");
		ACS_THROW(e);
	}
	
	DateTime generationTime=getInventoryObj().getGenerationTime(&set);
	if(!set)
	{
		exIllegalValueException e("generation time not set");
		ACS_THROW(e);
	}

	
	//Start Header
	XMLOstream::Tag earthExplorerHeader(getXmlStream(),"Earth_Explorer_Header");

	//Start Fixed Header Subsection
	XMLOstream::Tag Fixed_Header(getXmlStream(),"Fixed_Header");

	//Add all fields
	XMLOstream::Tag File_Name(getXmlStream(),"File_Name",fileName,"",true);
	XMLOstream::Tag File_Description(getXmlStream(),"File_Description",description,"",true);
	XMLOstream::Tag Notes(getXmlStream(),"Notes","","",true);
	XMLOstream::Tag Mission(getXmlStream(),"Mission", _satelliteId ,"",true);
	XMLOstream::Tag File_Class(getXmlStream(),"File_Class",fileClassDesc,"",true);
	XMLOstream::Tag File_Type(getXmlStream(),"File_Type",userFileType,"",true);
	{
		XMLOstream::Tag Validity_Period(getXmlStream(),"Validity_Period");
		XMLOstream::Tag Validity_Start(getXmlStream(),"Validity_Start",nameDate(validityStart),"",true );
		XMLOstream::Tag Validity_Stop(getXmlStream(),"Validity_Stop",nameDate(validityStop),"",true );
	}
	XMLOstream::Tag File_Version(getXmlStream(),"File_Version",fileVersion,"",true);
	{
		XMLOstream::Tag Source(getXmlStream(),"Source");
		XMLOstream::Tag System(getXmlStream(),"System",getSystem(),"",true);
		XMLOstream::Tag Creator(getXmlStream(),"Creator",creator,"",true);
		XMLOstream::Tag Creator_Version(getXmlStream(),"Creator_Version",creatorVersion,"",true);
		XMLOstream::Tag Creation_Date(getXmlStream(),"Creation_Date",nameDate(generationTime),"",true);
	}

	Fixed_Header.close() ;

	//Addd Variable Header Subsection (empty)
	XMLOstream::Tag variableHeaderTag (getXmlStream(),"Variable_Header","","",true);

	earthExplorerHeader.close() ;
	
	
	if (addDataBlock) {
		_dataBlockTag= new XMLOstream::Tag(getXmlStream(),"Data_Block","","type=\"xml\"");
	}
}

void EarthExplorerFixedHeaderWriter::end()
{
	delete _dataBlockTag;
	_dataBlockTag=0 ;

	delete _eartExplorerFileTag;
	_eartExplorerFileTag=0 ;
}


_ACS_END_NAMESPACE

