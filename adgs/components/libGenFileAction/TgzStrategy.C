// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Generic FileAction Library $

	$Id$

	$Author$

	$Log$
	Revision 7.1  2016/03/26 21:42:14  marpas
	using new db I/F
	
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.1  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.5  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.4  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.3  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.2  2008/12/03 17:19:16  ivafam
	Work in progress
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
*/


#include <TgzStrategy.h>
#include <File.h>
#include <Dir.h>
#include <dbInventoryObject.h>
#include <dbConnectionPool.h>
#include <Filterables.h>
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(TgzStrategy) ;

TgzStrategy::TgzStrategy(dbPersistent& obj, dbConnectionPool &p) : SMPackingStrategy(obj, p)
{
}
	
TgzStrategy::~TgzStrategy() throw() {}


void TgzStrategy::pack(const std::string &input, XMLIstream &, std::string &output)
{
	ACS_COND_THROW(input.empty(), TgzStrategyException("TgzStrategy::pack source path must be specified ")) ; // PRQA S 3081

	dbInventoryObject* obj = dynamic_cast<dbInventoryObject*>(&getObject()); // PRQA S 3081
	off_t headerSize=obj->getHeaderSize();
	bool split= (headerSize != 0);
	string filename = File::getFileName(input);
	string tmpname = File::getAbsolutePath(input);
	string dirname = File::tempname(tmpname); // PRQA S 3050
	
	File::mkdir(dirname);
	
	if(split)
	{
		string DBLFilePath=dirname + "/" + filename + ".DBL";
		string HDRFilePath=dirname + "/" + filename + ".HDR";
		string TGZFilePath="/tmp/" + filename + ".TGZ";

		File::split( input , DBLFilePath, HDRFilePath, headerSize, false); // PRQA S 3000
		
		
		ostringstream tgz;
		tgz << " cd " << dirname << "; tar cvfz " << TGZFilePath << " * "  ;
		int status = system((tgz.str()).c_str());
	
		if(status!=0)
		{	
			File::remove(dirname);
	     	ACS_THROW(TgzStrategyException("occurred while execute command <<<<< " + tgz.str() + " >>>>")); // PRQA S 3081
		}
		output = TGZFilePath;
	}
	else
	{
		string EEFFilePath=dirname + "/" + filename+".EEF";
		string TGZFilePath="/tmp/" + obj->getFileName() + ".TGZ";

		ostringstream move;
		move << "cp " << input <<  " " << EEFFilePath;
		int status = system((move.str()).c_str());
		if(status!=0)
		{	
			File::remove(dirname);
			ACS_THROW(TgzStrategyException("error occurred while execute command <<<<< " + move.str() + " >>>>")); // PRQA S 3081
		}
		status =0;
		
		ostringstream tgz;
		tgz << " cd " << dirname << "; tar cvfz " << TGZFilePath << " * "  ;
		status = system((tgz.str()).c_str());
	
		if(status!=0)
		{	
			File::remove(dirname);
	     	ACS_THROW(TgzStrategyException("error occurred while execute command <<<<< " + tgz.str() + " >>>>")); // PRQA S 3081
		}
		output = TGZFilePath;
		
	}
	File::remove(dirname); 
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input " << input
			<< " Exists Input " << boolalpha << File::exists(input)
			<< " output " << output) ;
}

void TgzStrategy::unpack(const std::string &input, XMLIstream &, std::string &output)
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input " << input
			<< " Exists Input " << boolalpha << File::exists(input)
			<< " output " << output) ;
	
	ACS_COND_THROW(input.empty(), TgzStrategyException("source path must be specified ")) ; // PRQA S 3081
	
	
	ACS_COND_THROW(output.empty(),TgzStrategyException("destination path must be specified ")) ; // PRQA S 3081
	
	string tmpname = File::getAbsolutePath(input);
	string dirname = File::tempname(tmpname); // PRQA S 3050
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current directory " << getenv("PWD")
            << " temp path" <<  dirname) ;
	try{
	
	    File::mkdir(dirname);
	
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current directory " << getenv("PWD")) ;
	
	    ostringstream tgz;
	    tgz << " cd " << dirname << "; tar " <<  " zxvf " << tmpname ;
	    int status = system((tgz.str()).c_str());
	
	    if(status!=0)
	    {	    
	    	File::remove(dirname);
	    	ACS_THROW(TgzStrategyException("error occurred while execute command \"" + tgz.str() + "\"")) ; // PRQA S 3081
	    }
	    status=0;
	
	    Dir mydir(dirname);
	    	    
	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
	    	ostringstream list;
	    	list << "ls  " << dirname ;
	    	status = system((list.str()).c_str());
	    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "directory " <<  dirname
	    		<< " size " << mydir.size()) ;
	    ACS_CLASS_END_DEBUG
	    dbInventoryObject* obj = dynamic_cast<dbInventoryObject*>(&getObject()); // PRQA S 3081
	
	    if(mydir.size()>2) // PRQA S 4400
	    {
	    	File::remove(dirname);
	    	ACS_THROW(TgzStrategyException("Too many files in tgz archive"));  // PRQA S 3081
	    }
#warning MP EEF extension hardcoded	
	    if(File::getExt(mydir[0])=="EEF")
	    {
	    	ostringstream move;
	    	move << "mv " << dirname + "/" + mydir[0] <<  " " << output;
	    	status = system((move.str()).c_str());
	    	if(status!=0)
	    	{	    
	    		File::remove(dirname);
	    		ACS_THROW(TgzStrategyException("error occurred while execute command \"" + move.str()+ "\"")) ; // PRQA S 3081
	    	}
	    	    
	    }
#warning MP HDR & DBL extension hardcoded	
	    else if((File::getExt(mydir[0])=="HDR") && (File::getExt(mydir[1])=="DBL") && (mydir.size()==2)) // PRQA S 4400
	    {
	    	if(File::exists(output)) {
	    		File::unlink(output);
            }

	    	string file1 = dirname + "/" + mydir[1];
	    	string file2 = dirname + "/" + mydir[0];
		    //Set HeaderSize and DataSize
		    if((obj->getHeaderSize()==0) && (obj->getDataSize()==0))
		    {
		    
		    	obj->setHeaderSize( File::size(file2 )  );  // PRQA S 3000 2
		    	obj->setDataSize  ( File::size(file1 )  );
                dbConnectionWrapper w(getPool()) ;
                dbConnection &conn = w ; // PRQA S 3050
				obj->update(conn); 
	    	}
	    	File::concat(file1, file2,output);
	
	    }
	    else if( (File::getExt(mydir[0])=="DBL") && (File::getExt(mydir[1])=="HDR") && (mydir.size()==2)) // PRQA S 4400
	    {
	    	    if(File::exists(output)) {
	    		    File::unlink(output);
                }
	    	    string file1 = dirname + "/" + mydir[0];
	    	    string file2 = dirname + "/" + mydir[1];
	    	    
		    if((obj->getHeaderSize()==0) && (obj->getDataSize()==0))
		    {
		    
		    	obj->setHeaderSize( File::size(file2 )  ); // PRQA S 3000 2
		    	obj->setDataSize  ( File::size(file1 )  );
                dbConnectionWrapper w(getPool()) ;
                dbConnection &conn = w ; // PRQA S 3050
				obj->update(conn); 
	    	}

	    	File::concat(file1, file2,output);
	    }
	    else
	    {
	    	File::remove(dirname);
	    	ACS_THROW(TgzStrategyException("directory " + dirname + "contain unrecognized files (not DBL, HDR or EEF)")); // PRQA S 3081
	    }	
	    File::remove(dirname); 
	 }
	 catch( TgzStrategyException & ex)
	 {
		 ACS_THROW(ex);
	 } 
	 catch( exception &e)
	 {
	 	File::remove(dirname);
		ACS_THROW(TgzStrategyException(e,"exception caught")) ; // PRQA S 3081
	 } 
	 catch(...)
	 {
	 	File::remove(dirname);
		throw ;
	 }   
}


_ACS_END_NAMESPACE
