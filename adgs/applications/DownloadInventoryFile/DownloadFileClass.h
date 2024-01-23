/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
	Revision 2.4  2013/03/12 17:58:07  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.3  2012/02/14 15:17:53  marpas
	fixing dtor
	
	Revision 2.2  2009/02/16 16:01:47  enrcar
	EC:: splitMatchExt used instead of split
	
	Revision 2.1  2007/03/23 17:34:09  ivafam
	This version includes -V optional parameter to retrieves from SDF the file with version.
	This version is not fully tested
	
	Revision 2.0  2006/02/28 10:38:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/15 15:01:44  serper
	After CMF update
	
*/

#ifndef __DownloadFileClass_H_
#define __DownloadFileClass_H_

#include <vector>
#include <dbException.h>
#include <dbPersistent.h>
#include <Thread.h>

using namespace std;

namespace acs{
class dbConnectionPool ;

class DownloadFileClass
{

public:

		/*! class zeroFileDownload declaration */
		exDECLARE_EXCEPTION(zeroFileDownload,dbException) ;
		
        /*! class DownloadFileClassException declaration */
		exDECLARE_EXCEPTION(DownloadFileClassException,exException) ;


		DownloadFileClass();  // default ctr
		~DownloadFileClass() throw() ; // destructor

		void setTargetDir(const string &) ;
		string getTargetDir() ;
		
		
		void addFilename(const string &n, const string &v="") ;
		void addFilenames(const vector <string> &);
		void getFilenames(vector <string> &) ;

		int getNumOfFiles();
		
		void readInFile(const string &) ;
		
		void download(dbConnectionPool &) ;

	private:
		// copy ctr
		DownloadFileClass(const DownloadFileClass&);
		// = Operator
		DownloadFileClass & operator=(const DownloadFileClass&);
    private:
		vector <string>	 _filenames ;
		vector <string>	 _versions ; // The usage of vector is not necessary
					     // but the next evolution could require
					     // this vector
		string			 _targetDir ;
		int				 _numOfFiles ;
		
		// HSM Parameters
		string 		 _severUrl   ;
		string  	 _user       ;
		string 		 _password   ;
		unsigned int _serverPort ;
		
		ACS_CLASS_DECLARE_DEBUG_LEVEL(DownloadFileClass)
};

}
#endif // __DownloadFileClass_H_
