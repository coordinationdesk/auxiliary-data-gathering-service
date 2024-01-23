/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$

	$Log$
	Revision 1.11  2017/05/18 12:56:28  tergem
	Specific log definition restored
	
	Revision 1.10  2017/05/04 09:01:09  tergem
	S2PDGS-1641: call to LogRotationPolicy suppressed
	
	Revision 1.9  2016/10/24 10:12:13  francesco.avanzi
	OPMAN-214: the TTDecompress now creating log in current folder
	
	Revision 1.8  2015/06/25 14:23:57  damdec
	Compilation warnings removed.
	
	Revision 1.7  2013/04/29 18:09:14  marpas
	file type recognition class  interface changed
	
	Revision 1.6  2013/04/29 14:45:30  chicas
	Configurable Decompress app optional
	
	Revision 1.5  2013/04/29 08:10:31  chicas
	Passing connection to dbFileTypeRecognition
	
	Revision 1.4  2013/02/04 15:28:44  chicas
	minor changes
	
	Revision 1.3  2013/02/04 15:09:25  chicas
	 Configurable Decompress app from conf space
	
	Revision 1.2  2013/02/04 11:09:30  chicas
	Rewrite joborder with extracted filetypes/filenames
	
	Revision 1.1.1.1  2013/01/23 16:33:10  chicas
	Imported sources
	
	
	
	
        
*/ 

#include <TTDecompress.h>
#include "TTDecompressV.h"
#include <JobOrder.h>

#include <Filterables.h>
#include <DefaultJobOrder.h>

#include <SpawnProc.h>
#include <dbFileTypeRecognition.h>

#include <Dir.h>
#include <set>
//#include <Signal.h>
#include <dbConnectionPool.h>

//#include <boost/program_options.hpp>
//namespace po = boost::program_options;


using namespace std;

_ACS_BEGIN_NAMESPACE(acs)
ACS_CLASS_DEFINE_DEBUG_LEVEL(TTDecompress)

static TTDecompressV version;

PROJECT_APP::GenericBuilder<TTDecompress> theApplicationBuilder("TTDecompress","DSD");

TTDecompress::TTDecompress(const std::string& subsys, const std::string& app):
    PROJECT_APP("TTDecompress","DSD"),
    _decompressApp("Decompress.sh")
{
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}

TTDecompress::~TTDecompress() throw() {
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}
/*
 * STL Helper class
 * Merges all the Input Descriptor using a strings array
 */
class accumulate_inputs_ftype {
    map<string, vector< string> > & _theList;

public:
    accumulate_inputs_ftype(map<string, vector< string> > & v) : _theList(v) {}

    void operator() (const InputDescriptor& i) {
        for_each (i.begin<InputAlternative>(), i.end<InputAlternative>(), accumulate_inputs_ftype(_theList));
    }

    void operator() (const InputAlternative& a) {
        vector <string> thisList;
        a.getPathNames (thisList); 
        string theFileType = a.getFileType();

    _theList.insert(make_pair(theFileType,thisList) );
    }

 //   string operator() (const string& path) {
        //string theFileName = File::getFileName (path);
//        return (File::removeExt( File::getFileName( path)) ); 
//    }
};

class accumulate_extracted_ftype_map {
    map<string, vector< string> > & _theMap;
    dbConnection & _conn;

public:
    accumulate_extracted_ftype_map(map<string, vector< string> > & v, dbConnection & conn) : 
        _theMap(v) ,
        _conn(conn){}

    void operator() (const string& filename) { 
        
        string filetype;
        dbFileTypeRecognition fileTypeRecogniser;
        try {
    
            filetype = fileTypeRecogniser.fname2RealFType(File::getFileName(filename), _conn );
           
           _theMap[filetype].push_back(filename);
        }
        catch(dbFileTypeRecognition::NotFound &e)
        {
            ACS_LOG_WARNING("TTDecompress:  Extracted file " << filename << " not recognized; it will not be added to joborder");
            //excerr.notify(e);

            //some warning...
        } 

    }
};




int TTDecompress::usage(const std::string &exename) const 
{
    excerr << "usage: " << exename << " <JobOrder file> \n";

    return 0 ;
}

void TTDecompress::readConfiguration()
{
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB)

	addSpace(ConfigurationSingleton::ConfSpace("TTDecompress"),ConfigurationSingleton::NoLoad);
	loadConfiguration();  

    try {

        ConfigurationSingleton::instance()->get().getValue("TTDecompress.Decompress_Path", _decompressApp);

    } catch (rsResourceSet::NotFoundException) {
        ACS_LOG_WARNING("No \"TTDecompress.Decompress_Path\" param found: set to default ");
    }

    ACS_LOG_INFO("  Configurated decompress application path: " << _decompressApp<< "\n");

}


int TTDecompress::main(int argc, char const * const * argv, char const * const * env)
{
	//Command Line printing - Useful for log inspection
	{
		ostringstream msg; msg<<"TTDecompress::main: Executing: ";
		for ( int i=0; i<argc; ++i  ) msg<<argv[i]<<" ";
		msg<<endl;
		ACS_LOG_INFO(msg.str());
	}
	if (argc < 2) 
	{ 
		usage( argv[0]) ;
		return 1 ;
	}

    ostringstream pid;
    pid << getpid();    

    //create the log files the first time
    string logFullPath = "./"+applicationName() + ".log" ; // create the log path name
    // Rotate keeping 20 logs, compressed. 
    // The size has no effect as I don't call the rotate during the execution
    LogRotationPolicy lrp(logFullPath, LogRotationPolicy::FirstApplying, defaultLogAge, (1024*1024*1024), 20, true) ;
    setLRPolicy(lrp) ;
//     rotateLog() ; // open the first log

    // Initialize libMessage
    // Wait to register after DirectDataCapture concurrent task
    sleep(1);

    //*/
    //SendMessage::instance()->sendMsgSEND( static_cast<const char*> (applicationName().c_str()), "s");

    //SendMessage::instance()->sendMsgSEND(static_cast<const char *>(pid.str().c_str()), "P");
    //*/

    // Protect threads by signals (default SIGINT e SIGTERM)
    //Signal::DefaultSignalHandler signalCatcher;

    // SLEEP FOR SECURITY
    //@@@sleep(5);

    try {
        
        readConfiguration();
	rotateLog() ; // open the first log
        string joFileName(argv[1]);

        execute(joFileName);

        return 0;                               
    } catch (exException &e) {
        string msg(applicationName()+" : Exiting in ERROR...");
        //*/
        //SendMessage::instance()->sendMsgSEND(static_cast<const char*> (msg.c_str()), "f");
        //*/
        ACS_LOG_INFO(msg) ;
        ACS_LOG_NOTIFY_EX(e);
        try {
            exFEvent mcfAdvise("STATUS",EXFE_ERROR);
            excerr << mcfAdvise;
            excmcf << mcfAdvise;
            ACS_LOG_ERROR(applicationName()+" : Exiting in ERROR");
            //excmcf << msgErr;
        } catch (exException& e) {
            ACS_LOG_NOTIFY_EX(e);
        }
        return(0xff);
    }
}

void TTDecompress::execute(const string & jobOrderFileName)
{
	// Retrieve input list from the jobOrder
	
    ACS_SMARTPTR<JobOrder> jo(new DefaultJobOrder);

    try
    {
        XMLIstream is (jobOrderFileName);
        is >> *jo ;
    }
    catch (exException& e)
    {
        ostringstream os;
        os <<  "Unable to load JobOrder from file  \"" << jobOrderFileName << "\"";
        throwException (exTTDecompressException (e, os.str() ));
    }
    catch (...)
    {
        ostringstream os;
        os <<  "Unknown exception caught while loading JobOrder \"" << jobOrderFileName << "\"";
        throwException (exTTDecompressException(os.str() ));
    }

    vector<InputDescriptor> inputsDescr;
    // Retrieve input descriptors from the tasks
    for (vector<Task>::iterator it=jo->begin(); it!=jo->end(); ++it)
    {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting input descriptors from task [" << it->getName() << "]" );
        it->getInputList (inputsDescr);
    }
    map<string, vector< string> > filetypeVsInputFile;
    for_each( inputsDescr.begin(), inputsDescr.end(), accumulate_inputs_ftype(filetypeVsInputFile) );

    map<string, vector< string> >::iterator r;
    map<string, vector< string> > filetypeVsExtractedFiles;
     
    //for each FILETYPE 
    for (r = filetypeVsInputFile.begin(); r != filetypeVsInputFile.end(); r++ )
    {   
    	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ostringstream msgs;
            msgs <<  "TTDecompress::execute Found Filetype: " << r->first << endl;
            copy(  (r->second).begin(), (r->second).end(),ostream_iterator<string>( msgs, "\n" )); 
            ACS_LOG_DEBUG(msgs.str()) ;
	ACS_CLASS_END_DEBUG
       
        if (  ! (r->second).size() ) continue;
        
        string workingdir =  File::getDirPath( (r->second)[0]);
        Dir wdir(workingdir);
        //unpack ALL FILES
        for_each( (r->second).begin(), (r->second).end(),  bind1st(mem_fun(&TTDecompress::unpackFile), this)  );
        //get files
        Dir postwdir(workingdir);
        vector<string> extractedFiles;
        postwdir.setDifference(wdir,extractedFiles);  //no path 
        //add path 
       for (unsigned int f =0; f<   extractedFiles.size(); f++)
           extractedFiles[f] = workingdir + "/" + extractedFiles[f] ;

    	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ostringstream msgs;
            msgs <<  "TTDecompress::execute : new file list for job order for this filetype is:" << endl;	
            copy( extractedFiles.begin(),extractedFiles.end(),ostream_iterator<string>( msgs, "\n" ));
            ACS_LOG_DEBUG(msgs.str()) ;
	ACS_CLASS_END_DEBUG
  
        // get new files and filetypes
        // 
        dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
	    dbConnection& conn (connWrp);
        
        for_each(   extractedFiles.begin(),
                    extractedFiles.end(),
                    accumulate_extracted_ftype_map(filetypeVsExtractedFiles,conn) );

    } //new input filetype 

     //write Job Order
     //change job order with new file list
     
     //clean tasks
    jo->clear();

    Task t;
    t.setName("INPUT_TASK");//ptask->getName());
    
    // - copy input file inside input descriptor
    {
        for (r= filetypeVsExtractedFiles.begin(); r != filetypeVsExtractedFiles.end(); r++ )
        {   
            string filetype = r->first; 
            InputDescriptor newInput;
            InputAlternative altern;
            //necessary for preinventory plugin
            altern.setFileType( filetype); 
            altern.setFileNameType( FileNameTypePHYSICAL); 
                        
            //get all files of same filetype

            for (size_t i = 0; i < (r->second).size(); i++)
                altern.addPathName((r->second)[i]);            
  
            newInput.push_back(altern);

           t.addInput(newInput);

         } //new input descriptor 

    }
   
    jo->push_back(t);
    
    ofstream o( jobOrderFileName.c_str());
        throwConditionedException (o.fail(), exTTDecompressException("Cannot create "+ jobOrderFileName + " file" ) );
		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing " << jobOrderFileName.c_str());

	o << (*jo) ;

   //-- 
}
	

void TTDecompress::unpackFile( string tarfile) const
{
            // unpack file in current dir
            //SpawnProc * proc = new SpawnProc("Decompress.sh");
            SpawnProc * proc = new SpawnProc(_decompressApp);
            proc->args()[0] = tarfile;  
            int exitCode = 0;
            int exitSignal = 0;

            //spawnExecutable(returnCode, "Decompress.sh", "");
            proc->spawn();
//
            // error message in case an error occurs
            ostringstream errMsg ;
            errMsg << "Error executing \" " << _decompressApp << " \" " ;
            while (proc->isRunning()) 
            {
	       ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "DecompressPreInventory: - Spawn Proc running  with pid: " << proc->pid() );
               usleep(100000);
            }
            proc->exitSignal(exitSignal);
            proc->exitCode(exitCode);
            // checks for exitSignal
            if (exitSignal > 0)
            {
                delete proc;
                proc = 0;
                ACS_LOG_WARNING("DecompressPreInventory: received exit Signal [" << exitSignal << "] while executing " << _decompressApp);
                
                errMsg << " (signal: " << exitSignal << "). ";
                throwException(exTTDecompressException(errMsg.str()));
            }
            // checks for exitCode
            if (exitCode > 0)
            {
                delete proc;
                proc = 0;
                errMsg << " (exitcode: " << exitCode << ") ";
                throwException(exTTDecompressException(errMsg.str()));
            }
            
            //remove original file
            File::unlink(tarfile);

            delete proc;
            proc = 0;

//
}
_ACS_END_NAMESPACE



