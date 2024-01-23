// PRQA S 1050 EOF
/*
	Copyright 1995-2021 Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.27  2018/03/08 15:19:01  tergem
	APF-424: versioning handling
	
	Revision 1.26  2016/09/29 09:32:52  matteo.airoldi
	DBL modification fixed
	
	Revision 1.25  2016/09/29 09:13:54  matteo.airoldi
	Added CAST to MAX function in sqlstring
	
	Revision 1.24  2016/06/16 12:14:06  matteo.airoldi
	file version is now taken in account when versioning DBL files
	
	Revision 1.23  2016/04/14 09:35:53  matteo.airoldi
	The MPH Product name has been modified using xlst.
	
	Revision 1.22  2016/04/11 08:27:35  francesco.avanzi
	file to be PreInv is no consistent in each of his fields
	
	Revision 1.21  2015/11/30 11:00:42  francesco.avanzi
	qac++analisys run
	
	Revision 1.20  2015/09/29 09:42:54  matteo.airoldi
	removed extension from parent filename
	
	Revision 1.19  2015/07/17 14:55:15  matteo.airoldi
	APF-287 Updating sizes on metadata when patching XML files HDR/EOF
	
	Revision 1.18  2015/07/15 07:44:25  matteo.airoldi
	Added time precision to validity start and stop time
	
	Revision 1.17  2015/07/10 15:09:07  matteo.airoldi
	Added order_id metadata when in TL context
	
	Revision 1.16  2015/07/09 16:25:43  matteo.airoldi
	Versioning in the ADM context has been fixed
	XSLT is now used to change HDR and EEF contents
	
	Revision 1.15  2015/07/06 12:01:50  francesco.avanzi
	HDR file taken into account
	
	Revision 1.14  2015/07/02 14:50:39  francesco.avanzi
	HDR file are now taken in account without extenction
	
	Revision 1.13  2015/07/01 13:06:58  marfav
	APF-255 fixed the metadata to populate t_inv_relations
	
	Revision 1.12  2015/06/12 13:10:51  francesco.avanzi
	now taking in account _context.TL situation
	minor issues fixed
	
	Revision 1.11  2015/06/12 10:43:05  francesco.avanzi
	 conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.10  2015/06/09 15:15:01  francesco.avanzi
	now querying into database for Qualified Value
	
	Revision 1.9  2015/06/04 10:00:05  francesco.avanzi
	Qualified field now filled correctly
	Minor issues taken in account
	
	Revision 1.8  2015/06/04 08:15:04  francesco.avanzi
	hasBaseline tag removed in configuration, code rearranged
	
	Revision 1.7  2015/06/03 14:20:50  francesco.avanzi
	qac++analisys run
	
	Revision 1.6  2015/06/03 13:38:54  francesco.avanzi
	qualityFile Removed, Qualified field-filling Reimplemented
	
	Revision 1.5  2015/06/03 12:27:46  francesco.avanzi
	conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.4  2015/05/29 16:10:35  francesco.avanzi
	all noted issues previous
	
	Revision 1.3  2015/05/29 09:29:19  francesco.avanzi
	minor issues fixed
	
	Revision 1.2  2015/05/29 07:59:17  francesco.avanzi
	now ADMAPFPreInventory takes in account Baselines and ParentFiles
	
	Revision 1.1  2015/05/27 15:10:06  francesco.avanzi
	Class Added
	
	Revision 1.3  2015/03/04 11:16:45  francesco.avanzi
	minor issues fixed
	
	Revision 1.2  2015/03/04 11:14:55  francesco.avanzi
	minor issues fixed
	
	Revision 1.1.1.1  2015/03/04 09:04:25  francesco.avanzi
	Import libEOFADMPreInventory
	
	
	
        
*/ 

#include <EOFADMPreInventory.h>
#include <XMLIstream.h>
#include <XMLTransformer.h>
#include <ftmpstream.h>
#include <FieldReplacer.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <Filterables.h>
#include <SQLString.h>
#include <dbQuery.h>
#include <dbSet.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFADMPreInventory)

EOFADMPreInventory::EOFADMPreInventory() : 
    EOFPreInventory(),
    baseline_(),
    dsName_(),
    relFile_(),
    hasQuality_(),
    fileVersion_(),
    fileBaseName_(),
    fileExt_(),
    removeInputFile_(),
    doVersioning_()
{
}

EOFADMPreInventory::~EOFADMPreInventory() throw() 
{
}

void EOFADMPreInventory::tLFileNameVersioningQuery(dbConnection &conn)  
{
    
    const string &dbKey = conn.getKey() ;
    //select max(fileversion) from t_inventory where filename like 'FileNameBase%' 
    SQLString sqlString(dbKey, SQLString::SELECT ) ;
    sqlString.addTableList("t_inventory");
    {
		SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
       	SQLDialect::SQLCast sqlCast(SQLDialect::SQL_VARCHAR, "10") ;
        tmp.setFunctionName("MAX"); tmp.addFunctionParameter("t_inventory.fileversion");
		sqlString.addSelectList(tmp, sqlCast, "as_SQL_VARCHAR");   	
    }
    {
	fileExt_ = File::getFExt(getCurrentFileName()) ;
        if (! fileExt_.empty() )
	{
		fileExt_ = "." + fileExt_;
	}
	string fileNameLike = File::removeFExt( File::getFileName( getCurrentFileName() )  ) ;
	fileNameLike = fileNameLike.substr( 0 , fileNameLike.size() - 4) ; //PRQA S 3084, 4400
	fileBaseName_ = fileNameLike ;
	fileNameLike += '%' ;
    
        dbQueryParameters p(dbKey);
        p.addParameter( dbParam(dbKey, "t_inventory","filename" , dbParam::LIKE , db::toSQL(dbKey, fileNameLike))) ;
    	sqlString.addANDWhereParameter(p);
    }
    
    dbSet set ;
    dbQuery query(conn) ;
    query.sqlString( sqlString, "EOFADMPreInventory::tLFileNameVersioningQuery" );
    query.execForSet( set );

    fileVersion_ = "0000" ;

    if(not set.empty())
    {
        dbRow row(dbKey);
        set.getNextRow(row);
        bool isNull = false;
        row.column(0, fileVersion_, &isNull);
    }
    else
    {	
        fileVersion_ = "0000" ;
    }
}

string EOFADMPreInventory::getUniqueId()
{
	return "";
}



bool EOFADMPreInventory::isToBeRejected(dbConnection &conn)   //PRQA S 4020
{
    //select unique_id from t_inventory where filename = _filename 
    const string &dbKey = conn.getKey() ;
    if(getContext().TL != getContext().which)
    {
	    {
            SQLString sqlString(dbKey, SQLString::SELECT ) ;
            sqlString.addTableList("t_inventory");
            sqlString.addSelectList("t_inventory.unique_id");
            {
        	    string tempFileName = File::getFileName(getCurrentFileName());
		        if( isHdr() )
		        {
		            tempFileName = File::removeExt(tempFileName);
		        }

		        dbQueryParameters pp(dbKey) ; 
		        pp.addParameter(dbParam(dbKey,"t_inventory","filename", dbParam::EQUAL_TO, db::toSQL(dbKey, tempFileName ) )) ; // PRQA S 3050, 3081
		        sqlString.addANDWhereParameter(pp) ;
            }
            dbQuery query(conn) ;
            query.sqlString( sqlString, "SelInvByUniqueId" );
            dbSet set ;
            query.execForSet(set);

            if(not set.empty())
            {
        	    ACS_LOG_ERROR("File Already Present in DataBase")
        	    return true;
            }
	    }
    }
    else
    {
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Looking for file versions in database: context = TL")
	if (doVersioning_)
	{
		tLFileNameVersioningQuery(conn) ;
	}
	else
	{
		fileExt_ = File::getFExt(getCurrentFileName()) ;
        	if (! fileExt_.empty() )
		{
			fileExt_ = "." + fileExt_;
		}
		string fileNameLike = File::removeFExt( File::getFileName( getCurrentFileName() )  ) ;
		fileVersion_ = fileNameLike.substr( fileNameLike.size() - 4, fileNameLike.size()) ;
		fileNameLike = fileNameLike.substr( 0 , fileNameLike.size() - 4) ; //PRQA S 3084, 4400
		fileBaseName_ = fileNameLike ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "fileBaseName_: " << fileBaseName_ <<  " fileVersion_: " << fileVersion_ <<  " fileExt_: " << fileExt_);
	}
    	
    }    
    //select baseline_id from t_baselines where baseline = baseline_
    if(hasBaseline())
    {
        SQLString sqlString(dbKey, SQLString::SELECT ) ;
        sqlString.addTableList("t_baselines");
        sqlString.addSelectList("t_baselines.id_baseline");
        {
            dbQueryParameters pp(dbKey) ; 
	        pp.addParameter(dbParam(dbKey,"t_baselines","baseline", dbParam::EQUAL_TO,db::toSQL( dbKey , baseline_ ))) ; // PRQA S 3050, 3081
	        sqlString.addANDWhereParameter(pp) ;
        }
        dbQuery query(conn) ;
        query.sqlString( sqlString, "SelBaseline" );
        dbSet set ;
        query.execForSet(set);

        if(set.empty())
        {
            ACS_LOG_ERROR("Baseline could not be found into DataBase")
            return true;
        }

        
    }
    
    return false ;
}

void EOFADMPreInventory::addFurtherNodes(vector<string> &nodes) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
    nodes.push_back("Earth_Explorer_File/Earth_Explorer_Header/Variable_Header/Main_Product_Header/Baseline");
    if(needsPair())
    {
        nodes.push_back("Earth_Explorer_File/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/List_of_Dsds");
        nodes.push_back("Earth_Explorer_File/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/List_of_Dsds/Dsd/Ds_Name");
        nodes.push_back("Earth_Explorer_File/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/List_of_Dsds/Dsd/Filename");
    }    
}
namespace {


struct Dsd { //PRQA S 2173, 2175
    Dsd() : dsName(), fileName() {}
    Dsd(string dn, string fn) : dsName(dn), fileName(fn) {} //PRQA S 2010

    string dsName ; //PRQA S 2100 2
    string fileName;
} ;

struct findDsdFName { //PRQA S 2173, 2175, 2185
    findDsdFName(const string &f) : dsName (f) {} //PRQA S 2180
    bool operator()(const Dsd &r) const  { return r.dsName == dsName ; }
    
    string dsName ; //PRQA S 2100
} ;

}

/*
template<>
void rsResourceSet::setValue<Dsd>(const std::string &res, const Dsd &v)
{
	Lock lock(_mutex) ;
	findOrInsertValue(res+".Ds_Name")->set(v.dsName) ;
	findOrInsertValue(res+".Filename")->set(v.fileName) ;
}
*/

template<>
void rsResourceSet::getValue(const std::string &res, Dsd &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".Ds_Name")->get(v.dsName) ;
	getValue(res+".Filename")->get(v.fileName) ;
} 


void EOFADMPreInventory::extractFurther(rsResourceSet &rs)
{    
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
    if(isHdr())
    {
        if(rs.hasKey("Variable_Header.Main_Product_Header.Baseline" ))
        {
            rs.getValue("Variable_Header.Main_Product_Header.Baseline", baseline_);
        }
    }
    else
    {
        if(rs.hasKey("Earth_Explorer_Header.Variable_Header.Main_Product_Header.Baseline"))
        {
            rs.getValue("Earth_Explorer_Header.Variable_Header.Main_Product_Header.Baseline", baseline_);
        }
    }
    ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "baseline_= \"" << baseline_ << "\"");
    if( needsPair() )
    {
        size_t countValue = 0 ;
        if(rs.hasKey("Earth_Explorer_Header.Variable_Header.Specific_Product_Header.List_of_Dsds.count.value"))
        {
            rs.getValue("Earth_Explorer_Header.Variable_Header.Specific_Product_Header.List_of_Dsds.count.value", countValue) ;
        }
        else if(rs.hasKey("Variable_Header.Specific_Product_Header.List_of_Dsds.count.value"))
        {
            rs.getValue("Variable_Header.Specific_Product_Header.List_of_Dsds.count.value", countValue) ;
        }
        else
        {
            ACS_THROW(exIllegalValueException("list_of_Dsds could not be found")) //PRQA S 3081
        }
            
        
        ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "countValue= \"" << countValue << "\"");
	
    	vector<Dsd> reg ;
        if(rs.hasKey("Earth_Explorer_Header.Variable_Header.Specific_Product_Header.List_of_Dsds.Dsd"))
        {
            rs.getArray("Earth_Explorer_Header.Variable_Header.Specific_Product_Header.List_of_Dsds.Dsd", reg) ;
        }
        else if(rs.hasKey("Variable_Header.Specific_Product_Header.List_of_Dsds.Dsd"))
        {
            rs.getArray("Variable_Header.Specific_Product_Header.List_of_Dsds.Dsd", reg) ;
        }
        else
        {
            ACS_THROW(exIllegalValueException("Dsds could not be found")) //PRQA S 3081
        }
        
        
        ACS_LOG_INFO("Dsd entries: " << reg.size()) ;
        vector<Dsd>::const_iterator r = find_if(reg.begin(), reg.end(), findDsdFName(dsName_)) ; //PRQA S 3081

        ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "ds_Name= \"" << r->dsName << "\"");

        relFile_ = r->fileName ;
        ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Relation File Name= \"" << relFile_ << "\"");
    }
}

namespace {


struct Registration {  //PRQA S 2173, 2175
    Registration() : object(), dsdFileName(), hasQuality(), remInpFile(), doVersioning() {}
    Registration(string o, string pf, bool hq, bool rif, bool doV) : //PRQA S 2010
                                                            object(o), 
                                                            dsdFileName(pf), 
                                                            hasQuality(hq),
							    remInpFile(rif),
							    doVersioning(doV) {} 

    string object ; //PRQA S 2100 5
    string dsdFileName;
    bool hasQuality ;
    bool remInpFile ;
    bool doVersioning ;
    
} ;

struct findFType { //PRQA S 2173, 2175, 2185
    findFType(const string &f) : ftype_ (f) {} //PRQA S 2180
    bool operator()(const Registration &r) const { return r.object == ftype_ ; }
    
    string ftype_ ;//PRQA S 2100
} ;

}

/*
template<>
void rsResourceSet::setValue<Registration>(const std::string &res, const Registration &v)
{
	Lock lock(_mutex) ;
	findOrInsertValue(res+".Object")->set(v.object) ;
	findOrInsertValue(res+".DsName")->set(v.dsdFileName) ;
	findOrInsertValue(res+".HasQuality")->set(v.hasQuality) ;
	findOrInsertValue(res+".RemoveInputFile")->set(v.remInpFile) ;
	findOrInsertValue(res+".DoVersioning")->set(v.doVersioning) ;
}
*/

template<>
void rsResourceSet::getValue(const std::string &res, Registration &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".Object")->get(v.object) ;
	getValue(res+".DsName")->get(v.dsdFileName) ;
	getValue(res+".HasQuality")->get(v.hasQuality) ;
	getValue(res+".RemoveInputFile")->get(v.remInpFile) ;
	getValue(res+".DoVersioning")->get(v.doVersioning) ;
} 

void EOFADMPreInventory::checkForBlAndPf()
{
    
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
			
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    size_t registeredEntries = 0;
    rs.getValue( "PreInventory.PluginsRegistrationSettings.EOFADMPreInventoryFactory.List_Of_Registrations.count.value", registeredEntries);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "registeredEntries: " << registeredEntries ) ;
    vector<Registration> reg ;
    rs.getArray("PreInventory.PluginsRegistrationSettings.EOFADMPreInventoryFactory.List_Of_Registrations.Registration", reg) ;
    
    ACS_LOG_INFO("Registration entries: " << reg.size()) ;
    vector<Registration>::const_iterator r = find_if(reg.begin(), reg.end(), findFType(getCurrentFileType())) ; //PRQA S 3081
    
    ACS_COND_THROW(r == reg.end(), exEOFADMPreInventoryException("Cannot find fileType=\""+getCurrentFileType()+"\" in configuration")) ; //PRQA S 3081

    dsName_ = r->dsdFileName    ;
    hasQuality_ = r->hasQuality ;
    removeInputFile_ = r->remInpFile ; 
    doVersioning_ = r->doVersioning ; 
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dsName_: " << dsName_ <<  " hasQuality_: " << boolalpha << hasQuality_ <<  " doVersioning_: " << boolalpha << doVersioning_)

}

void EOFADMPreInventory::computeMetadata( const string& pathFileName, const string& fileType, Metadata& metadata, dbConnection &conn) //PRQA S 5500, 5510
{
   //NB: the pathFileName passed to this method is an absolute path
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
    try {

        ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "pathFileName= \"" << pathFileName << "\" filetype= \"" << fileType << '"');
        
	
	
	    setCurrentFileName(pathFileName ) ;
        setCurrentFileType(fileType ) ;


        checkForBlAndPf();
        FixedHeaderStruct fhs;
	
        readEOFFile( pathFileName, fhs);

		fhs.Validity_Start = fhs.Validity_Start + ".000000" ;
		fhs.Validity_Stop = fhs.Validity_Stop + ".999999" ;

        if( not isToBeRejected( conn) )
        {

            metadata = prepareMetadata( fileType, fhs, conn);
            if (getContext().TL == getContext().which)
	        {

		        ostringstream newVersion ;
		        {
		            unsigned short v = ::atoi(fileVersion_.c_str()) ; //PRQA S 3000, 3010
		            if (doVersioning_)
			            ++v ;
		            newVersion << setw(4) << setfill('0') << v; //PRQA S 4400
		        }	
		        if ("0001" != newVersion.str() && doVersioning_ == true) 
		        {
			        {
				        string xsltName = File::tempname ("./XmlTransformer","xslt"); //PRQA S 3050
				        ofstream oftmp(xsltName.c_str()); 
				        string theXSLT;
				        generateXSLT (fileBaseName_  + newVersion.str(), newVersion.str(), theXSLT);
				        oftmp << theXSLT;
				        oftmp.close();

				        string newFileName ( fileBaseName_  + newVersion.str() + fileExt_ ); 

				        XMLTransformer::file2file(xsltName, getCurrentFileName(), newFileName);

				        if (isHdr())
				        {
					        // Header file size could be different
					        metadata.setMetadataValue( "Header_Size", File::size (newFileName));	
				        }
				        else
				        {
					        // Data block has been changed
					        metadata.setMetadataValue( "Data_Size", File::size (newFileName));	
				        }
			        }

			        if( inputFileToBeRemoved() )
			        {
			            File::remove(getCurrentFileName());
			        }
			        if(isHdr())
			        {
			            string oldDBLName = File::fileWExt ( File::removeFExt (getCurrentFileName() ), "DBL");
			            string newDBLName = File::fileWExt ( File::getDirPath (getCurrentFileName()) + "/" + fileBaseName_  + newVersion.str(), ".DBL" );
			            if(inputFileToBeRemoved())
			            {

				            File::rename(oldDBLName, newDBLName) ;
			            }
			            else
			            {
				            File::copy(oldDBLName, newDBLName) ;
			            }

				        string newName= File::getFileName(File::removeFExt(newDBLName));

			            fstream o( newDBLName.c_str()) ;
                        o.seekp(9, ios::beg) ;
                        o.write(newName.c_str(), newName.size());
                        o.close() ;

			        }	
			        setCurrentFileName( fileBaseName_ + newVersion.str() + fileExt_);
			        fillMetadataFilename( getCurrentFileName(), fileType, metadata, conn); 


			        metadata.setFieldValue("File_Version", newVersion.str());
		        }

                // Here I am still in the TL context
                metadata.setFieldValue("Order_Id", getContext().metadataInfo.orderId);
	        }    
	    
	        ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "baseline= \"" << baseline_ << "\" relFile_= \"" << relFile_ << '"');
            if(hasBaseline())
            {
                metadata.setFieldValue("Baseline", baseline_) ;
            }
            if(needsPair())
            {
		        metadata.setFieldValue("Parent_Id", File::removeExt(relFile_)) ;
            }
    	    
	        const string &dbKey = conn.getKey() ;
            if(not hasQuality())
            {
        	    SQLString sqlString(dbKey, SQLString::SELECT ) ;
        	    sqlString.addTableList("t_qualifiedflag");
        	    sqlString.addSelectList("t_qualifiedflag.id");
        	    {
        	        dbQueryParameters pp(dbKey) ; 
		            pp.addParameter(dbParam(dbKey,"t_qualifiedflag","name", dbParam::EQUAL_TO, db::toSQL(dbKey, "UNMARKABLE" ) )) ; // PRQA S 3050, 3081
		            sqlString.addANDWhereParameter(pp) ;
        	    }
        	    dbQuery query(conn) ;
        	    query.sqlString( sqlString, "SEL t_qualifiedflag" );
        	    dbRow row(dbKey) ;

                dbSet set ;
		        query.execForSet(set) ;
                ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
                ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
                set.getNextRow(row) ; // first and only 
	    
	            unsigned int qualVal = 0 ;
		        row.column(0, qualVal) ;
                metadata.setFieldValue("Qualified", qualVal) ;
            }
	        else
	        {
		        metadata.setFieldValue("Qualified", "") ;
	        }
        }
        else
        {
            ACS_LOG_ERROR("PreInventory will NOT take place");
            ACS_THROW(exEOFADMPreInventoryException("File is to be Rejected")) //PRQA S 3081
        }
    }
    catch( exception& ex)
    {
        ACS_THROW(exEOFPreInventoryException( ex, "computeMetadata Exception")) ; // PRQA S 3081
    }
}      

void EOFADMPreInventory::generateXSLT (const string& newfilename, const string& newfileversion, string& xsltText) const
{
	xsltText.clear();
	xsltText += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	xsltText += "<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">";
	xsltText += "<xsl:output method=\"xml\" indent=\"yes\"/>";
	xsltText += "<xsl:strip-space elements=\"*\"/>";
	xsltText += "<xsl:template match=\"@*|node()\">";
	xsltText += "<xsl:copy>";
	xsltText += "<xsl:apply-templates select=\"@*|node()\"/>";
	xsltText += "</xsl:copy>";
	xsltText += "</xsl:template>";
	xsltText += "<xsl:template match=\"//*[local-name() = 'Earth_Explorer_Header']/*[local-name() = 'Fixed_Header']/*[local-name() = 'File_Name']\">";
	xsltText += "<xsl:element name=\"{local-name(.)}\" namespace=\"{namespace-uri(.)}\">" + newfilename + "</xsl:element>";
	xsltText += "</xsl:template>";
	xsltText += "<xsl:template match=\"//*[local-name() = 'Earth_Explorer_Header']/*[local-name() = 'Fixed_Header']/*[local-name() = 'File_Version']\">";
	xsltText += "<xsl:element name=\"{local-name(.)}\" namespace=\"{namespace-uri(.)}\">" + newfileversion + "</xsl:element>";
	xsltText += "</xsl:template>";
	xsltText += "<xsl:template match=\"//*[local-name() = 'Earth_Explorer_Header']/*[local-name() = 'Variable_Header']/*[local-name() = 'Main_Product_Header']/*[local-name() = 'Product']\">";
	xsltText += "<xsl:element name=\"{local-name(.)}\" namespace=\"{namespace-uri(.)}\">" + newfilename + "</xsl:element>";
	xsltText += "</xsl:template>";
	xsltText += "<xsl:template match=\"//*[local-name() = 'Product_Report']/*[local-name() = 'List_of_Messages']/*[local-name() = 'Message']/*[local-name() = 'Text']\">";
	xsltText += "<xsl:element name=\"{local-name(.)}\" namespace=\"{namespace-uri(.)}\">";
	xsltText += "<xsl:text disable-output-escaping=\"yes\">&lt;![CDATA[</xsl:text>";
	xsltText += "<xsl:value-of select=\".\" disable-output-escaping=\"yes\"/>";
	xsltText += "<xsl:text disable-output-escaping=\"yes\">]]&gt;</xsl:text>";
	xsltText += "</xsl:element>";
	xsltText += "</xsl:template>";
	xsltText += "</xsl:stylesheet>";
}

string EOFADMPreInventory::doPackaging( Metadata& metadata, const string & firstPath, const string & filetype, const string & secondPath, dbConnection & )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	string packagePath;
	string dblPath = secondPath;
	string hdrPath = firstPath;

	if( secondPath.empty() ) 
	{
	    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No package to be performed");

		string filename;
		string thepath = File::getDirPath (hdrPath);
		metadata.getFieldValue("File_Name", filename);
		hdrPath = thepath + "/" + filename;

		// The package has the name of the new created file
		packagePath = hdrPath;
	}
	else 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Package to be performed");

		string filename;
		string hdrextension = File::getExt (firstPath);
		string dblextension = File::getExt (secondPath);
		string thepath = File::getDirPath (hdrPath);

		metadata.getFieldValue("File_Name", filename);
		hdrPath = thepath + "/" + filename + "." + hdrextension;
		dblPath = thepath + "/" + filename + "." + dblextension;

		// Going to create the name of package file
		packagePath = File::removeExt(hdrPath);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Package file path to inventory is \"" << packagePath << "\". Going to concat \"" << hdrPath << "\" header file to \"" << dblPath << "\" data file.");
		
		//Stick HDR to the end of DBL in the new file packagePath
		File::concat( dblPath, hdrPath, packagePath);

	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning \"" << packagePath << "\" path to inventory");
	return packagePath;
}


_ACS_END_NAMESPACE
