// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. XMLResources Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2014/02/13 12:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/05 17:06:36  marpas
	coding best practices applied
	
	Revision 5.2  2013/10/15 14:29:30  nicvac
	Fixed call to open function.
	
	Revision 5.1  2013/07/19 09:01:20  marpas
	doc fixed
	qa warnings fixed
	compilation warnings fixed
	
	Revision 5.0  2013/07/18 17:54:35  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/07/12 11:26:01  marpas
	refactoring to apply modern methods to deal wih tmp files
	
	Revision 2.4  2013/03/26 16:37:51  marpas
	file2file method added
	
	Revision 2.3  2012/02/15 12:38:21  marpas
	potential memory corruption fixed
	
	Revision 2.2  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.1  2011/03/01 12:09:34  marpas
	GCC 4.4.x support
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.13  2005/02/02 16:16:42  marpas
	Xalan timeout in a class constant and has been increased up to 10 min
	
	Revision 1.12  2004/07/15 14:46:53  marpas
	libProc I/F changed
	
	Revision 1.11  2004/05/04 15:19:17  marpas
	delays downscaled to msec
	
	Revision 1.10  2004/04/28 12:08:11  marpas
	usleep changed into Timer::delay
	
	Revision 1.9  2004/04/15 19:35:14  paoscu
	Xalan is the default transformer. If it's not present xslproc is used
	
	Revision 1.8  2003/11/10 16:39:14  paoscu
	Timeout timer removed.
	More debug options.
	
	Revision 1.7  2003/11/03 15:27:26  paoscu
	better error message.
	
	Revision 1.6  2003/10/03 15:48:10  marpas
	bug fixed
	
	Revision 1.5  2003/09/26 10:29:41  paoscu
	xsltproc is used instaed of xalan
	It's possible to specify a working path if string2string and string2string methods.
	
	Revision 1.4  2003/04/30 16:38:00  marpas
	using namespace std no longer into acs includes

	Revision 1.3  2003/03/11 13:40:53  marpas
	some string::lenght substituted with string::empty

	Revision 1.2  2003/02/04 18:50:18  paoscu
	Finished

	Revision 1.1  2003/01/31 16:29:56  paoscu
	Imported.


*/

#include <XMLTransformer.h>
#include <Timer.h>
#include <File.h>
#include <SpawnProc.h>
#include <Filterables.h>

#include <sstream>
#include <fstream>

#include <cstdlib>
#include <unistd.h>
#include <sys/types.h> // PRQA S 1013 2
#include <sys/stat.h>
#include <fcntl.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLTransformer)

XMLTransformer::XMLTransformer() :
    _xslFilePath(),
    _xmlFilePath(),
    _outputFilePath()

{	
}


XMLTransformer::~XMLTransformer() throw() {}

void XMLTransformer::xslFilePath(std::string const & path) // PRQA S 4121
{
	_xslFilePath=path;
}

void XMLTransformer::xmlFilePath(std::string const & xml) // PRQA S 4121
{
	_xmlFilePath=xml;
}

void XMLTransformer::outputFilePath(std::string const & path) // PRQA S 4121
{
	_outputFilePath=path;
}

void  XMLTransformer::writeOutputFile() // PRQA S 4020
{
	
	ACS_COND_THROW(_xmlFilePath.empty(),exIOException("XML file path was not specified.")) ; // PRQA S 3081
	
	ACS_COND_THROW(!File::exists(_xmlFilePath),exIOException("XML file \"" + _xmlFilePath + "\" does not exist.")) ; // PRQA S 3081
	
	ACS_COND_THROW(_outputFilePath.empty(),exIOException("Output file path was not specified.")) ; // PRQA S 3081

	ACS_COND_THROW(_xslFilePath.empty(),exIOException("XSL file path was not specified.")) ; // PRQA S 3081

	ACS_COND_THROW(!File::exists(_xslFilePath),exIOException("XSL file \"" + _xslFilePath + "\" does not exist.")) ; // PRQA S 3081

	//Try first xalan
	string haveXalan ;
	try{
		haveXalan=File::which("Xalan") ;
	}
	catch(exception &){}
	if(!haveXalan.empty())
	{
		writeOutputFileXalan() ;
		return ;
	}


	//Then try xsltproc
	string haveXsltProc ;
	try{
		haveXsltProc= File::which("xsltproc") ;
	}
	catch(exception &){}
	
	if(!haveXsltProc.empty())
	{
		writeOutputFileXsltProc() ;
		return ;
	}

	ACS_THROW(exIOException("Can not find a valid transformation program.")); // PRQA S 3081

}




void  XMLTransformer::writeOutputFileXalan() // PRQA S 4211
{
	WaitPid::ProcArgs xalanArgs;
	xalanArgs[0]=WaitPid::ProcArg("-o"); // PRQA S 3081 4
	xalanArgs[1]=WaitPid::ProcArg(_outputFilePath);
	xalanArgs[2]=WaitPid::ProcArg(_xmlFilePath); // PRQA S 4400 2
	xalanArgs[3]=WaitPid::ProcArg(_xslFilePath);
	SpawnProc sp("Xalan");
	sp.args(xalanArgs);
	sp.spawn();
	unsigned long timer=0;
	Timer watchdog ;
	bool wdTimeout = false ;
	watchdog.start() ;
	while(sp.isRunning())
	{
		Timer::delay(31); // PRQA S 4400
		timer++;
		if(watchdog.isOlder(XalanSecTimeout*1000)) // PRQA S 4400
		{
			sp.kill();
			wdTimeout = true ;
		}
	}
    // here if no longer running
    int exitCod = -1 ;
    int exitSig = 0 ;
    bool xited = sp.exitCause(exitCod, exitSig) ;
    if (xited) {
        // correctly exited (not signaled) 
        // take the exitCod 
    }
    else {
		ostringstream os ;
		if (wdTimeout) {
			os << watchdog.elapsed() / 1000 // PRQA S 3084, 4400
			   << " [sec] elapsed since Xalan was started. It is bigger than the timeout ("
			   << XalanSecTimeout << " [sec]). The process was killed. " ;
			
			ACS_THROW(exIOException(os.str())) ; // PRQA S 3081
		}
		os << "Xalan terminated with exitCode: " << exitCod  << " exitSignal: " << exitSig ;
		ACS_THROW(exIOException(os.str())) ; // PRQA S 3081
    }
}

	/*      *****************  Xslt OPTIONS  **************************
		
		--maxdepth value
		Adjust the maximum depth of the template stack before libxslt concludes it is  in  an
		infinite loop. The default is 500.

		--param name value

		Pass  a parameter of name name and value value to the stylesheet. You may pass multi-
		ple name/value pairs up to a maximum of 32. If the value being  passed  is  a  string
		rather than a node identifier, use --stringparam instead.

		--nonet

		Do not use the Internet to fetch DTD's or entities.

		--warnnet

		Output notification when DTD's or entities are fetched over the Internet.

		--xinclude

		Process the input document using the Xinclude specification. More details on this can
		be   found   in   the   Xinclude    specification:    http://www.w3.org/TR/xinclude/

		--profile or --norman

		Output  profiling  information detailing the amount of time spent in each part of the
		stylesheet. This is useful in optimizing stylesheet performance.
	 */



void  XMLTransformer::writeOutputFileXsltProc() // PRQA S 4211
{
	WaitPid::ProcArgs xsltprocArgs;
	xsltprocArgs[0]=WaitPid::ProcArg("-o"); // PRQA S 3081 3
	xsltprocArgs[1]=WaitPid::ProcArg(_outputFilePath);
	xsltprocArgs[2]=WaitPid::ProcArg("--noout"); // PRQA S 4400


	xsltprocArgs[3]=WaitPid::ProcArg(_xslFilePath); // PRQA S 3081, 4400 2
	xsltprocArgs[4]=WaitPid::ProcArg(_xmlFilePath);
	SpawnProc sp("xsltproc");
	sp.args(xsltprocArgs);
	sp.spawn();
	while(sp.isRunning())
	{
		Timer::delay(29); // PRQA S 4400
	}
    // here if no longer running
    int exitCod = -1 ;
    int exitSig = 0 ;
    bool xited = sp.exitCause(exitCod, exitSig) ;
    if (not xited) {
        // signaled
		int exitSignal = 0 ;
		sp.exitSignal(exitSignal) ;
		ostringstream os ;
		os << "xsltproc was signaled ( " << exitSig  << ")" ;
		ACS_THROW(exIOException(os.str())) ; // PRQA S 3081
	}

    // here if exited
    
	if(exitCod != 0)
	{
		string bad;

		/*         *****************  RETURN VALUES  **************************

			xsltproc's return codes provide information that can be used when calling it from scripts.

			0: normal
			1: no argument
			2: too many parameters
			3: unknown option
			4: failed to parse the stylesheet
			5: error in the stylesheet
			6: error in one of the documents
			7: unsupported xsl:output method
			8: string parameter contains both quote and double-quotes

		*/

		switch(exitCod)
		{
			case 1: {bad="No argument";	break; }
			case 2: {bad="Too many parameters";	break; } // PRQA S 4402 7
			case 3: {bad="Unknown option";	break; }
			case 4: {bad="Failed to parse the stylesheet"; break; }
			case 5: {bad="Error in the stylesheet"; break; }
			case 6: {bad="Error in one of the documents"; break; }
			case 7: {bad="Unsupported xsl:output method"; break; }
			case 8: {bad="String parameter contains both quote and double-quotes"; break; }
			default: {
				ostringstream os;
				os<< "exit code was " <<exitCod;
			 	bad=os.str() ;
				break;
			}
		}
		ACS_THROW(exIOException("Error while executing xsltproc : " + bad)) ; // PRQA S 3081
	}


}

void XMLTransformer::file2file(const std::string & XslFilePath, 
                               const std::string & XmlFilePath, 
                               const std::string & OutputFilePath, 
                               bool removeInput)
{
	XMLTransformer transf;

	transf.xmlFilePath(XmlFilePath);
	transf.outputFilePath(OutputFilePath);
	transf.xslFilePath(XslFilePath);
	transf.writeOutputFile();

    bool remove=true;
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file \"" << XmlFilePath << "\" is not removed") ;
        remove = false ;
	ACS_CLASS_END_DEBUG

	if(removeInput && remove) {
		File::unlink(XmlFilePath);
    }
}



void XMLTransformer::string2file(std::string const & XslFilePath, 
                                 std::string const & XmlString, 
                                 std::string const & OutputFilePath, 
                                 std::string const & workingPath)
{

	XMLTransformer transf;
	string inputFile ;
    try {
	    if(!workingPath.empty())
	    {
		    inputFile = File::tempname(workingPath + "/XMLTransformer", "xml") ; // PRQA S 3050
	    }
	    else {
		    inputFile = File::tempname("XMLTransformer", "xml") ; // PRQA S 3050
        }
    }
    catch(exception &x) {
        ACS_THROW(exIOException(x, "Error while creating temporary input file")) ;
	}
    
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Opening write mode the temporary file: "<<inputFile);

    int inputFileDesc = ::open(inputFile.c_str(), O_WRONLY|O_CREAT, 0644) ;
	if( inputFileDesc == -1 )
	{
		ACS_THROW(exIOException("Error while opening temporary input file", errno)) ;
	}
	//Fill imput file
	if( ::write(inputFileDesc, XmlString.c_str() , XmlString.size() ) != signed(XmlString.size()) ) // PRQA S 3081
	{
		::close(inputFileDesc);
		File::unlink(inputFile);
		ACS_THROW(exIOException("Error while writing temporary input file", errno)) ;
	}
	::close(inputFileDesc);


	try
	{
		transf.xmlFilePath(inputFile);
		transf.outputFilePath(OutputFilePath);
		transf.xslFilePath(XslFilePath);
		transf.writeOutputFile();
	}
	catch(...)
	{
		bool remove=true;
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ;
		    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file \"" << inputFile << "\" is not removed") ;
        	remove = false ;
		ACS_CLASS_END_DEBUG
		if(remove) {
			File::unlink(inputFile);
        }
		throw ;
	}

	bool remove=true;
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file \"" << inputFile << "\" is not removed") ;
		remove = false ;
	ACS_CLASS_END_DEBUG
	if(remove) {
		File::unlink(inputFile);
    }


}



void XMLTransformer::string2string(std::string const & XslFilePath, 
                                   std::string const & XmlString, 
                                   std::string & OutString, 
                                   std::string const & workingPath)
{
	string outputFile ;
    try {
	    if(!workingPath.empty())
	    {
		    outputFile = File::tempname(workingPath + "/XMLTransformer", "xml") ; // PRQA S 3050
	    }
	    else {
		    outputFile = File::tempname("XMLTransformer", "xml") ; // PRQA S 3050
        }
    }
    catch(exception &x) {
        ACS_THROW(exIOException(x, "Error while creating temporary output file")) ;
	}
 
	try
	{
		XMLTransformer::string2file(XslFilePath , XmlString , outputFile , workingPath);
	}
	catch(...)
	{
		File::unlink(outputFile);
		throw ;
	}
	//Read output file.
	ifstream outStream(outputFile.c_str());
	if(!outStream)
	{
		File::unlink(outputFile);
		ACS_THROW(exIOException("Error while opening temporary output file")); // PRQA S 3081
	}
	outStream.seekg(0, ios::end);
	streampos length=outStream.tellg();
	outStream.seekg(0, ios::beg);
	char * buffer=new char[length]; // PRQA S 3000, 3050
    try {
	    outStream.read(buffer , length); // PRQA S 3050
	    OutString.assign(buffer , length); // PRQA S 3000, 3050
	    delete [] buffer;
    }
    catch(...) {
        delete [] buffer ; 
        throw ;
    }
	outStream.close();

	File::unlink(outputFile);

}


_ACS_END_NAMESPACE



