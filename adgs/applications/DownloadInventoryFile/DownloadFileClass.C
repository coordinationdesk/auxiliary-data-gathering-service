/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

  $ACheader: /CalVal/components/libLDSHSM/DownloadFileClass.C 0008 05/06/09 07:53:12 cvfmgrcvf02$
	
  $AClog: $
  /CalVal/components/libLDSHSM/DownloadFileClass.C 0008 05/06/09 07:53:12 cvfmgrcvf02
  Comment: Exception catch bug fixed
  /CalVal/components/libLDSHSM/DownloadFileClass.C 0007 05/04/04 14:16:14 cvfmgrcvf02
  Comment: Versioning added
  
  $Log$
  Revision 2.12  2016/04/11 13:45:17  marpas
  adopting new db interfaces


*/

#include <sstream>
#include <vector>
#include <dbDownloadInventoryFile.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbGeneralQuery.h>
#include <dbGenQueryScopeGuard.h>
#include <dbCryosatFactory.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>
#include <dbFileType.h>
#include <dbConnectionPool.h>
#include <File.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <dbConv.h>
#include <dbResourceSpaceFactory.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <SMPluginLoader.h>
#include <unistd.h>
#include <DownloadFileClass.h>

#include <Filterables.h>


using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DownloadFileClass)

		

//-----------------------------------------------------------------------------
// FUNCTION 	: Constructor
// DESCRIPTION 	: Default Constructor
// DATA IN 	: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
DownloadFileClass::DownloadFileClass() : 
    _filenames(),
    _versions(),
    _targetDir(""),
    _numOfFiles(0),
    _severUrl(""),
    _user(""),
    _password(""),	
    _serverPort(0)	
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"CLASS DownloadFileClass - DownloadFileClass::DownloadFileClass Default Constructor\n") ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: Destructor
// DESCRIPTION 	: Default Constructor
// DATA IN 	: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
DownloadFileClass::~DownloadFileClass() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: addFilename
// DESCRIPTION 	: This method fills private member data
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
void DownloadFileClass::addFilename(const string &name, const string &version)
{
	_filenames.push_back(string()) ;
	_filenames[_filenames.size() - 1] = name ;
	
	_versions .push_back(string()) ;
	if (version != "")
		_versions[_versions.size() - 1] = version ;
	else
		_versions[_versions.size() - 1] = "NONE" ;
	
	_numOfFiles=_filenames.size();
	
	
}

//-----------------------------------------------------------------------------
// FUNCTION 	: addFilenames
// DESCRIPTION 	: This method fills private member data
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
void DownloadFileClass::addFilenames(const vector <string> &names)
{
	unsigned int actualSize = _filenames.size() ;
	unsigned int finalSize = _filenames.size() + names.size();
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Value of actualSize : " <<  actualSize  << "\n" << "Value of finalSize  : " <<  finalSize  << "\n" );

	for (unsigned int i = actualSize; i < finalSize ; i++)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Value of i               : " <<  _filenames.size()  << "\n") ;
		_filenames.push_back(string()) ;
		_filenames[i] = names[i - actualSize] ;
	}
	_numOfFiles=_filenames.size();
	return ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: getNumOfFiles
// DESCRIPTION 	: This method returns the number of filenames set.
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
int DownloadFileClass::getNumOfFiles()
{
	return _numOfFiles ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: getFilenames
// DESCRIPTION 	: This method return a private member data value
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
void DownloadFileClass::getFilenames(vector <string> &names)
{
	for (unsigned int i = 0; i < _filenames.size() ;i++)
	{
		names.push_back(_filenames[i]) ;
	}
	return ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: setParam
// DESCRIPTION 	: This method return a private member data value
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
void DownloadFileClass::setTargetDir(const string &tDir)
{
	_targetDir = tDir ;
	return ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: setParam
// DESCRIPTION 	: This method return a private member data value
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
string DownloadFileClass::getTargetDir()
{
	return _targetDir;
}
//-----------------------------------------------------------------------------
// FUNCTION 	: readInFile
// DESCRIPTION 	: This method gets the output fields stored on resource set 
//                structure and store them on class.
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
void DownloadFileClass::readInFile(const string &InFile)
{
	// Open and read file
	FILE *tmpFile ;
	tmpFile = fopen(InFile.c_str(), "r") ;

	// if an error occurs, exit with error.
	if (tmpFile == NULL)
	{
	 ostringstream errmsg ;
	 errmsg << "Cannot open " << tmpFile << " errno = " << errno ;
	 exFileOpenException x(errmsg.str(), errno) ;
	 ACS_THROW (x) ;
	}
	
	// read the first line of input file
	char buf[1024] ;
	char *readErr ;
	memset(buf, 0, 1024) ;

	readErr = fgets(buf,1024,tmpFile) ;

	while (feof(tmpFile) == 0)
	{
		// if an error occurs
		if (readErr == NULL || ferror(tmpFile) != 0)
		{
			// return an error
			string errString = "Error reading input file" ;
			exIOException x(errString, errno) ;
        	ACS_THROW (x) ;
		}

		// remove end of line
		if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0' ;

		addFilename(buf) ;
		
		// read next line
		memset(buf, 0, 1024) ;
		readErr = fgets(buf,1024,tmpFile) ;
	}
	
	// Close temporary File
	fclose(tmpFile) ;
	
	return ;
}

//-----------------------------------------------------------------------------
// FUNCTION 	: download
// DESCRIPTION 	: This method gets the output fields stored on resource set 
//                structure and store them on class.
// DATA IN		: None
// DATA OUT 	: None
//-----------------------------------------------------------------------------
void DownloadFileClass::download(dbConnectionPool &pool)
{
	
	ostringstream oo ;
	
	unsigned int numOfFails = 0 ;
	unsigned int numOfSkip = 0 ;
	
					
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	for (unsigned int i = 0 ; i < _filenames.size() ; i++ )
	{
		// This boolean is used to skip the downloading operation
		bool skip=false;
		
		string fileName(_filenames[i]);
		string path(_targetDir + string("/") + fileName) ;
		string version(_versions[i]) ;
		
		dbInventoryObjectQuery * theQuery=0;
		dbGenQueryScopeGuard theQuesryScopeGuard(theQuery) ;

		try 
		{
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path : [" << path << "]\n") ;
				// If the output file already exist on file system SKIP it
				if(File::exists(path))
				{
					excerr << "INFO --> SKIP FILE: The file [" << _filenames[i] << "] already exist in target directory.\n" ;
					numOfSkip++;
					skip=true;			
				}

				// if NOT already on target directory
				if (!skip)
				{
					theQuery=dynamic_cast<dbInventoryObjectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", dbKey));
					if(theQuery==0)
					{
						excerr << "WARNING --> SKIP FILE: downloading [" << _filenames[i] << "] Error in dynamic cast.\n" ;
						numOfFails++;
						skip=true;			
					}

					dbQueryParameters parameters(dbKey) ; 
					parameters.setParamOperator(dbQueryParameters::OR);
					parameters.addParameter(dbParam(dbKey, "t_inventory","filename", dbParam::EQUAL_TO, db::toSQL(dbKey,File::removeExt(fileName)))) ;
					parameters.addParameter(dbParam(dbKey, "t_inventory","filename", dbParam::EQUAL_TO, db::toSQL(dbKey,fileName))) ;
					
					dbQueryParameters parameters2(dbKey) ; 
					if (version != "NONE")
					{
						parameters2.addParameter(dbParam(dbKey, "t_inventory","fileversion", dbParam::EQUAL_TO, db::toSQL(dbKey,version))) ;
					}
					
					theQuery->addParameter(parameters) ;
					theQuery->addParameter(parameters2) ;
					theQuery->setOrderBy("FileVersion DESC") ;
					theQuery->doQuery(theConnection) ;
					if(theQuery->size() == 0)
					{
						excerr << "WARNING --> SKIP FILE: downloading [" << _filenames[i] << "] No Inventory Objetc found in SDF.\n" ;
						// numOfFails++;
						// numOfSkip++;
						skip=true;			
					}

					if(theQuery->size() > 1)
					{
						excerr << "INFO --> "
						       << theQuery->size() 
							   << " entries found for Inventory Object [" 
							   << _filenames[i] 
							   << "] in SDF .\n" ;
						excerr <<"INFO --> I will download first one having file version [" << (*theQuery)[0]->getFileVersion() <<"].\n";

					}
					if( (*theQuery)[0]->getValidityFlag() == false )
					{
						excerr << "WARNING --> SKIP FILE: downloading [" << _filenames[i] << "] the Inventory Object validity Flag is False.\n" ;
						numOfFails++;
						skip=true;			
					}
					
					// Download the file 
					dbDownloadFile* downloader = dynamic_cast<dbDownloadFile*>(dbFileActionFactoryChain::instance()->newDownloadAction(pool, *(*theQuery)[0],path));

					if(!downloader)
					{
						exIllegalValueException e("DownloadInventoryFile  : Dynamic_cast failure.");
						excerr << "WARNING --> SKIP FILE: Error downloading [" << _filenames[i] << "] (see next error)." ;
						excerr << "WARNING --> " << e.what() << "\n" ; 
						numOfFails++;
						skip=true;			
					}
					
					dbFileActionScopeGuard guard(downloader);

					downloader->start() ;
					sleep(1);
					while(downloader->downloading() )
					{
						excerr << "INFO --> Still downloading \n";
						sleep(1) ;
					}
                    
                    
                    string fileType = (*theQuery)[0]->getFileType() ;
				     
                    dbFileType *ft= dynamic_cast<dbFileType*>(dbPersistentFactoryChain::instance()->newObject("dbFileType", dbKey));
                    dbPersistentScopeGuard ftGuard(ft) ;

                    for (unsigned int connRetry=0; 1; connRetry++) 
                    {
                        try
                        {

                            /* Query on t_filetype to fill filetype parameters above */
                            ft->reset();
                            ft->setFileType(fileType);
                            if(!ft->load(theConnection))
                            {
                                // unable to load filetypes
                                ostringstream msg ;
                                msg << "DownloadFileClass::download : " << __LINE__ << " Unable to load filetypes [" << fileType << "]" ;
                                DownloadFileClassException e(msg.str());      
                                ACS_THROW(e) ;
                            }
                            
                            break ; 
				        }
                        catch (exException &e) 
				        {
					        if (connRetry >= 3) 
					        {
                                ostringstream msg ;
                                msg <<"DownloadFileClass::download : " << __LINE__ << " Exception caught:" << " " <<e.what() ;
                                excerr <<msg.str(); 
                                DownloadFileClassException e(msg.str());      
                                ACS_THROW(e) ;
					        }
					        sleep(30+(30*connRetry)%600);
					        connRetry++;
				        }
			        } // end for         

  
  					string headerFile, dblFile ;     
          
                   File::splitMatchExt(path, headerFile, dblFile, (*theQuery)[0]->getHeaderSize(), (*theQuery)[0]->getDataSize(), ft->getSngExt(), ft->getHdrExt(), ft->getDblExt(), ft->getMngSngExt(), true, true ) ;
                 
              //      File::splitMatchExt(path, headerFile, dblFile, 0, (*theQuery)[0]->getHeaderSize()+(*theQuery)[0]->getDataSize(), ft->getSngExt(), ft->getHdrExt(), ft->getDblExt(), ft->getMngSngExt(), true, true ) ;

                     
                    if ( (headerFile == "") || (dblFile == "") )
                    {
                        excout << "Renaming file: \"" << path << "\" as: \"" 
                        << headerFile << dblFile << "\"\n";
                    }
                    else
                    {
                        excout << "Splitting file: \"" << path << "\" in header and binary: \"" 
                        << headerFile << "\" and \"" << dblFile << "\"\n";
                    }

                    if ((headerFile != path) && (dblFile != path))                  
	                {
                        File::unlink(path);	
					}
                    
				} // ind if !skip
		}
		catch(dbDownloadFile::DownloadAsynchException& ex)
		{
			excerr << "WARNING --> REQUESTED FILE: File " << _filenames[i]  <<" is not online, a download request from permanent storage has been submitted. Submit again download request as soon as file will be online\n";
			ex.notify();
		}
		catch(dbDownloadInventoryFile::PreconditionException &e)
		{
			excerr << "WARNING --> SKIP FILE: There's an error in dbInventoryObject: now comes record dump\n";
			excerr <<"**************** START DUMP ********************\n";
			excerr << *(*theQuery)[0] << "\n";
			excerr <<"***************** END DUMP *********************\n";
			numOfFails++;
			skip=true;
		}
		catch(exception &e)
		{
			ACS_LOG_NOTIFY_EX(e)
			numOfFails++;
			skip=true;
		}

	}
	
	/*
	excerr << "INFO --> Number of files : [" << _filenames.size()  << "]\n" ;
	excerr << "INFO --> Number of fail  : [" << numOfFails  << "]\n" ;
	excerr << "INFO --> Number of skip  : [" << numOfSkip  << "]\n" ;
	*/
	
	if (numOfFails == 0)
	{
		if (numOfSkip > 0)
			excerr << "The number of files retrieved from SDF Archive is : [" << _filenames.size() - numOfSkip << "/" << _filenames.size() << "]\n" ;
		else
			excerr << "All the files are successfully retrieved from SDF Archive [" << _filenames.size() << "/" << _filenames.size() << "]\n" ;
	}
	else if (numOfFails < _filenames.size())
	{
		if (numOfSkip > 0)
			excerr << "The number of files retrieved from SDF Archive is : [" << _filenames.size() - (numOfSkip + numOfFails) << "/" << _filenames.size() << "]\n" ;
		else
			excerr << "The number of files retrieved from SDF Archive is " << _filenames.size() - numOfFails << "/" << _filenames.size()  << "\n" ;
	}
	else if (numOfFails == _filenames.size())
	{
		if (!numOfSkip)
		{
			oo.str("") ;
			oo << "Cannot Retrieve any files from SDF Archive" ;
			string erroStr = string("Error during download of files: ") + oo.str() ;
			zeroFileDownload err(erroStr);
    		ACS_THROW(err) ;
		}
	}
	
	return ;
}
