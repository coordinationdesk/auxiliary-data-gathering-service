// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	Revision 6.1  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.0  2013/09/19 15:40:44  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:21  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.4  2013/03/28 14:30:52  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.3  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.2  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
	
*/


#include <GzStrategy.h>
#include <File.h>
#include <Dir.h>
#include <dbInventoryObject.h>
#include <Filterables.h>
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(GzStrategy)

GzStrategy::GzStrategy(dbPersistent& obj, dbConnectionPool &p) : SMPackingStrategy(obj,p)
{
}
	
GzStrategy::~GzStrategy() throw() {}

void GzStrategy::pack(const std::string &input, XMLIstream &, std::string &output)
{
	ACS_COND_THROW(input.empty(), GzStrategyException("source path must be specified ")); // PRQA S 3081
	dbInventoryObject* obj = dynamic_cast<dbInventoryObject*>(&getObject()); // PRQA S 3081
	bool split = (obj->getHeaderSize() != 0 );
	string filename = File::getFileName(input);	

	ACS_COND_THROW(split,GzStrategyException("It is not possible to apply this strategy on HDR/DBL format ")) ; // PRQA S 3081

	string GZFilePath="/tmp/" + obj->getFileName()+".EEF";

	ostringstream move;
	move << "cp -f " << input <<  " " << GZFilePath;
	int status = system((move.str()).c_str());
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "status " << status) ;
	ACS_COND_THROW(status!=0, GzStrategyException("error occurred while execute command <<<<< " + move.str() + " >>>>")) ; // PRQA S 3081
	
	status =0;

	ostringstream gz;
	gz << " cd " << "/tmp" << "; gzip -S .GZ " << GZFilePath  ;
	status = system((gz.str()).c_str());

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GZFilePath " << GZFilePath) ;

	ACS_COND_THROW(status!=0, GzStrategyException("error occurred while execute command <<<<< " + gz.str() + " >>>>")) ; // PRQA S 3081

	output = GZFilePath+".GZ";

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input " << input 
			<< " Exists now ? " << boolalpha << File::exists(input)
			<< " output " << output) ;
}

void GzStrategy::unpack(const std::string &input, XMLIstream &, std::string &output)
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "input " << input 
			<< " Exists now ? " << boolalpha << File::exists(input)
			<< " output " << output) ;
            
	ACS_COND_THROW(input.empty(),GzStrategyException("source path must be specified ")) ; // PRQA S 3081
	
	
	ACS_COND_THROW(output.empty(),GzStrategyException("destination path must be specified ")); // PRQA S 3081
	
	try{
	
        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
	        ostringstream list;
	        list << "ls " ;
	        system((list.str()).c_str());
        ACS_CLASS_END_DEBUG   
	    
	    string GZFilePath= File::getAbsolutePath(input )+".GZ";
	    
	    ostringstream move;
	    move << "mv " << File::getAbsolutePath(input ) <<  " " << GZFilePath;
	    int status = system((move.str()).c_str());
	    ACS_COND_THROW(status!=0,GzStrategyException("error occurred while execute command <<<<< " + move.str() + " >>>>")) // PRQA S 3081

	    status =0;
		
	    ostringstream gz;
	    gz << " gunzip -S .GZ " << GZFilePath ;
	    status = system((gz.str()).c_str());
	
	    ACS_COND_THROW(status!=0,GzStrategyException("error occurred while execute command \"" + gz.str() + "\"")) ; // PRQA S 3081
	   
        
	    status=0;
	
	    if(input!=output)
	    {
	    	ostringstream move2; 	
	    	move2 << "mv " << File::getAbsolutePath(input ) << "* " <<  File::getAbsolutePath(output );
	    	status = system((move2.str()).c_str());
	    	ACS_COND_THROW(status!=0, GzStrategyException("error occurred while execute command \"" + move2.str()+ "\"")) // PRQA S 3081
	    }
	 }
	 catch( GzStrategyException & ex)
	 {
		 throw ;
	 } 
	 catch( exception &e)
	 {
	 	//File::remove(dirname);
		ACS_THROW(GzStrategyException(e,"exception caught")) ;
	 } 
}



_ACS_END_NAMESPACE
