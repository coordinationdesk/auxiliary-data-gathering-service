/*

	Copyright 2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 19:22:44  marpas
	adopting libException 5.x standards
	task tables are downloaded using temp names to aviod thread race conditions
	
	Revision 1.1  2012/12/13 16:49:49  chicas
	Added
	

*/


#include <dbAppResSet.h>
#include <dbSmartPersistent.h>
#include <File.h>


using namespace acs;
using namespace std;


int main(int argc, char **argv)
{

	try
	{
        if (argc < 2)    {
            excerr<<"Wrong parameters number!"<<endl;
            excerr<<argv[0]<<" ttfilename [-dbconf dbname]"<<endl;
            return false;
        }
	    
        string tasktableFile = string(argv[1]) ;

		string dbConfArg = "-dbconf" ;
		string dbConf ;
		if( argc == 4 )
			if (dbConfArg == argv[2] )
                dbConf= argv[3] ;

		dbAppResSetSingleton::instance(dbConf) ;

        if (File::exists(tasktableFile))
        {
            dbSmartPersistent sp("t_tasktables");
            sp.set("name",string("default"));
            sp.set("description",string("Test TT for importer"));
            sp.setFilePath("tasktable",tasktableFile);

            int id = sp.save();

            excout << "Saved; Id is " << id << endl;
        }

    }
    catch(std::exception &e)
	{
		excerr << "*********** exException caught****************" << endl; 
		excerr.notify(e) ;
		excerr << "**********************************************" << endl;
	}
	
	catch(...)
	{
		excerr << "*********** Unknown exception caught ***********" << endl; 
	}
	
	excerr << "#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" << endl ;


/*
id_receptionrule | filetype_id | repository_id | user_id | usermode_id | priority | active | delete_on_remote | generateheader | 
tasktable_id | tt_parameters | hostname |       localpath       | connretries | pollingperiod_sec | retryperiod_sec | connfailureaddress | procfailureaddress |         lastvisit          |          lastgood          | retrycount
er 

insert into t_receptionrule (
filetype_id, repository_id, user_id, usermode_id, tasktable_id, localpath, connretries, pollingperiod_sec,
retryperiod_sec, connfailureaddress, procfailureaddress) values 
( 45, 1, 1, 3, 5, /tmp/basket, 10, 2000, 99, s.avolio@acsys.it, c.castellani@acsys.it)




*/
}
