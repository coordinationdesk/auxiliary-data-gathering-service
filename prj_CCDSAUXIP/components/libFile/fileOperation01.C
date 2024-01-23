/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. fileOperation01 Handler $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/02 17:31:26  marpas
	adoption of libException 5.x standards in progress, qa rules adn compilation warning fixed
	
	Revision 5.0  2013/06/20 16:43:54  marpas
	adoption of libException 5.x in progress
	
	Revision 2.6  2012/03/01 16:08:15  marpas
	using acs::base for default static paths
	
	Revision 2.5  2012/02/07 17:57:44  marpas
	refactoring in progress
	
	Revision 2.4  2010/02/23 10:05:07  enrcar
	ENRCAR:: SEVERE REGRESSION FIXED. Unable to correctly handle application name during Operation02 file decoding. UPDATE TO THIS VERSION IN CASE OF PROBLEMS.
	
	Revision 2.3  2009/05/14 14:00:10  enrcar
	EC:: c++ rules
	
	Revision 2.2  2008/06/05 10:23:24  enrcar
	EC:: Output improved
	
	Revision 2.1  2008/06/04 15:11:24  enrcar
	EC:: fileOperation01, fileOperation01Factory added
	
	
*/


#include <fileOperation01.h>
#include <FileGuard.h>
#include <exException.h>
#include <Filterables.h>


#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h> // PRQA S 1013 2
#include <sys/stat.h>


_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(fileOperation01)

// constructor
fileOperation01::fileOperation01(const string &feature, const string &instance) :
	Operation01(feature,instance)
{
}




size_t fileOperation01::get(void *b, size_t bufSize, string &whichmod, const vector<string> &mods) const // PRQA S 4020
{
	// Multiple returns are used to keep the code legible

	string pathName = string(base::defaultConfPath()) + "/local" ;	/* Where the configuration files should be got */ // PRQA S 3081

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "inbuffer size is " << bufSize 
		<< " mods vector size is: " << mods.size() << " PathName: " << pathName) ;

	for (size_t i=0; i < mods.size(); i++) {
	
		ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
			ACS_LOG_DEBUG(SimpleDebugSignature << "Processing feature: !" << getFeature()
			    << "!   mod: !" << mods[i] << "!") ;
		ACS_CLASS_END_DEBUG
	
		string fileName = pathName + "/" + getFeature() + mods[i] ;	/* Get actual filename */

	    int fileD = open (fileName.c_str() , O_RDONLY) ;
		if ( fileD < 0)
		{
			ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
			    ACS_LOG_DEBUG(SimpleDebugSignature
				   << "File " << fileName
				   << " Not found or invalid") ;
			ACS_CLASS_END_DEBUG
			continue ;	/* File not found */
		}
		
		ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
			ACS_LOG_DEBUG(SimpleDebugSignature
				   << "File " << fileName
				   << " successfully opened with descriptor" << fileD) ;
		ACS_CLASS_END_DEBUG
	
		size_t extracted = read (fileD , b , bufSize ) ;	/* read file */ // PRQA S 3000
		ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
			ACS_LOG_DEBUG(SimpleDebugSignature
				   << "mod is: !" << mods[i] << "! read: " << extracted) ;
		ACS_CLASS_END_DEBUG
		if (0 != close(fileD) )
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));

		if (extracted <= 0) {
			continue ;	/* Invalid size / Can't read the file */
        }

		ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
			ACS_LOG_DEBUG(SimpleDebugSignature
				   << " File was found: " << fileName) ;
		ACS_CLASS_END_DEBUG


		whichmod = mods[i] ;
		return extracted ;	/* Read file successfully */
	}	/* for i */
	
	ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
		ACS_LOG_DEBUG(SimpleDebugSignature << "Unable to find a proper file.") ;
	ACS_CLASS_END_DEBUG
	
	return 0 ;
}


void fileOperation01::set(const void *b, size_t size, const string &whichmod) const
{
/* EC:: UNSUPPORTED -- FIXME -- TODO */
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
#warning EC fixme
#if(0)
	
	dbApplicationStatus *statusRecord=dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus")) ;
	dbPersistentScopeGuard statusGuard(statusRecord) ;
	ACS_COND_THROW(!statusRecord, exIllegalValueException("dbApplicationStatus dynamic cast failed"));

	statusRecord->setApplication(getFeature() + whichmod);
	statusRecord->setInstance(getInstance());
	ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
		ACS_LOG_DEBUG(SimpleDebugSignature
			   << statusRecord->getApplication() << " " <<  statusRecord->getInstance()) ;
	ACS_CLASS_END_DEBUG
 	FileGuard statusFileGuard(File::tempname("/tmp/dbOp01", whichmod)) ;

	ofstream statusFile(string(statusFileGuard).c_str());
	ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
		ACS_LOG_DEBUG(SimpleDebugSignature
			   << "tmp file: " << statusFileGuard
			   << " size: " << size ) ;
	ACS_CLASS_END_DEBUG
	statusFile.write(reinterpret_cast<const char *>(b),size) ;
	if (!statusFile.good()) {
        int err = errno ;
		statusFile.close();
		ostringstream os ;
		os << "dbOperation::set writing " << size  << " bytes." ;
		ACS_THROW(exIOException(os.str(),err)) ; // PRQA S 3081
	}
	statusFile.close();
	if(statusRecord->load()) {
		ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
			ACS_LOG_DEBUG(SimpleDebugSignature << "is present ") ;
		ACS_CLASS_END_DEBUG
		statusRecord->replaceStatusFile(statusFileGuard) ;
	}
	else {
		ACS_CLASS_EQ_DEBUG(171279)  // PRQA S 3050
			ACS_LOG_DEBUG(SimpleDebugSignature
				   << "is not present file is " << statusFileGuard
				   << "\n" ;
		ACS_CLASS_END_DEBUG
		statusRecord->setStatusUploadFileName(statusFileGuard); 
		statusRecord->save();
	}
#endif	
}


bool fileOperation01::remove(const string &mod) const
{
	/* EC:: UNSUPPORTED -- FIXME -- TODO */
#warning EC fixme
#if(0)
	dbApplicationStatus *statusRecord=dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus")) ;
	dbPersistentScopeGuard statusGuard(statusRecord) ;
	ACS_COND_THROW(!statusRecord, exIllegalValueException("dbApplicationStatus dynamic cast failed"));

	statusRecord->setApplication(getFeature() + mod);
	statusRecord->setInstance(getInstance());
	try {
		statusRecord->remove() ;
	}
	catch(exception &) {
		return false ;
	}
#endif
	return true ;
}

_ACS_END_NAMESPACE
 
