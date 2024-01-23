// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/07/04 18:49:27  marpas
	finishing adoption of libException 5.x standards
	qa warnings removed
	compilation warnings removed
	coding best practices applied
	
	Revision 5.0  2013/06/20 16:43:53  marpas
	adoption of libException 5.x in progress
	
	Revision 2.7  2012/02/07 17:57:44  marpas
	refactoring in progress
	
	Revision 2.6  2009/06/26 10:42:32  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.5  2009/03/13 15:26:45  marpas
	some rule violations fixed
	
	Revision 2.4  2007/04/16 18:01:18  nicvac
	Time Delay increased in getResult
	
	Revision 2.3  2007/04/16 13:14:56  nicvac
	Debug Improved
	
	Revision 2.2  2007/04/13 17:31:36  marpas
	check improved
	
	Revision 2.1  2007/04/13 17:17:51  marpas
	new class to compute md5 in a separate thread
	

*/

#include <FileMD5Th.h>
#include <CRC_Digest_md5.h>
#include <File.h>
#include <Filterables.h>
#include <Timer.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

//////////////////////////////
//							//
// classe FileMD5Th			//
//							//
//////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(FileMD5Th) 


FileMD5Th::FileMD5Th(const string &path, size_t safeThr) : 
    Thread(),
	_path(path),
	_threshold(safeThr),
    _result(),
	_end()
{
}


FileMD5Th::~FileMD5Th() throw()
{
}


void FileMD5Th::run() // PRQA S 4020
{
	_end = false ;
	_result = "" ;
	// wait until file appears
	for (;;)
	{
		try {
			if (File::exists(_path)) {
				break ;
            }
			else {
				ACS_THROW(exIOException("Cannot stat \"" + _path + "\"")) ; // PRQA S 3081
            }
		}
		catch (std::exception &) {
			if (_end || timeoutOrCancel(10)) { // true if canceled // PRQA S 4400
				throw ;
            }
		}
	}
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file exists") ;

	off_t size = 0 ;
	CRC_Digest_md5 md5 ;
	size_t bufSize = 65536 ;

	// Create buffer using digest's expected Size 
	char buffer[ bufSize ] ;
	
	FILE *fp = fopen(_path.c_str(), "r"); // PRQA S 4412
	ACS_COND_THROW(!fp,exIOException("Cannot open \"" + _path+ "\"",errno)) ; // PRQA S 3081

	try {
		for(;;)	{

			try {
				if (_end || timeoutOrCancel(10)) { // true if cancelled // PRQA S 4400
					break ;
                }

				size = File::size(_path) ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "size:" << size) ;

				off_t position = ftello(fp) ;

				if (position == off_t(-1)) { // PRQA S 3081
					ACS_THROW(exIOException("ftello failed on \"" + _path+ "\"")) ; // PRQA S 3081
				}

				if ((position+sizeof(buffer)+_threshold) > size_t(size)) { // PRQA S 3000, 3081, 3084
					if (timeoutOrCancel(10) || _end) { // true if canceled - wait 10 msec // PRQA S 4400
						break ;
				    }
                }
				else {
					do {
						int readSize = fread (buffer, 1,  sizeof(buffer), fp) ; // PRQA S 3000, 3010
						ACS_COND_THROW(readSize < 0,exIOException("readSize failed on \"" + _path+ "\"")) ;  // PRQA S 3081
						md5.addBuffer(reinterpret_cast<unsigned char *>(buffer), readSize) ; // PRQA S 3000, 3030, 3081
						position+=readSize ;
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "position:" << position) ;
						if (_end || timeoutOrCancel(10)) { // PRQA S 4400
							break ;
                        }
					} while ((position+bufSize+_threshold) < size_t(size)) ; // PRQA S 3000, 3081, 3084
				}
			}
			catch (std::exception &) {
				if (_end || timeoutOrCancel(10)) { // true if canceled // PRQA S 4400
					throw ;
                }
			}			
		} 

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_end is : " << boolalpha << _end) ;
		if (!_end) {
			fclose(fp) ;
			return ;
		} 

		size = File::size(_path) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "final size: " << size) ;
		if (requestedCancel()) {
			fclose(fp);
			return ;
		}
		int readSize = 0 ;
		
		while ( (readSize = fread (buffer, 1,  sizeof(buffer), fp)) > 0) // PRQA S 3000, 3010
		{
			md5.addBuffer(reinterpret_cast<unsigned char *>(buffer), readSize) ; // PRQA S 3000, 3030, 3081
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ftello:" << ftello(fp)) ;

		fclose(fp);
        fp = 0 ;
		_result = md5.getResult() ;
	}			
	catch (std::exception &) {
		if (fp) { fclose(fp) ; }
		throw ;
	}	
}


std::string FileMD5Th::getResult() 
{
	_end = true ;
	Timer::delay(100) ; // PRQA S 4400
	stop() ;
	exception *e = 0 ;
	
	if ( runException(e) ) {
		exIOException ex(*e, "Computin MD5 over \"" + _path + "\"") ;
		delete e ;	
		ACS_THROW(ex) ;
	} 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "signature:" << _result) ;
	return _result ;
}

_ACS_END_NAMESPACE
