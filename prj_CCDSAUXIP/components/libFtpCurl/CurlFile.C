// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.2  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.1  2012/11/28 18:16:23  marpas
 *  qa rules,
 *  optimization and robustness
 *  still work in progress
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.2  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.1  2011/01/19 16:42:58  micmaz
 *  added the openConnection() and closeConnection() to cURL operations.
 *
 *
 *
 */

#include <CurlFile.h>

#include <exException.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs,curl) 

CurlFile::CurlFile(std::string const& name) :
    _filename(name), 
	_is_ofuntarstream(false)
{
}

CurlFile::CurlFile(ofuntarstream* stream) :
    _stream(stream), 
	_is_ofuntarstream(true)
{
}

CurlFile::~CurlFile() noexcept {
    if ( _needs_fclose && _stream ) {
        ::fclose( static_cast<FILE*>(_stream) ); /* close the local FILE*/ // PRQA S 4631
		_stream = nullptr ;  
    }
}

void CurlFile::fopen() {
    ACS_COND_THROW(0 != _stream, exIOException("CurlFile stream already valorized (" + _filename + ")")) ; // PRQA S 3081
    _stream = ::fopen(_filename.c_str(), "wb");
	_needs_fclose = (nullptr != _stream) ;
    int eno = errno ;
    ACS_COND_THROW(0 == _stream, exIOException("Cannot open file: \"" + _filename + "\"", eno)); // PRQA S 3081
}


bool CurlFile::flush(int& exit_code, int& exit_signal)
{
	bool status (true) ; exit_code = exit_signal = 0 ;
    ACS_COND_THROW( !_stream, exIllegalValueException("Null pointer")) ; 
	if (true == _is_ofuntarstream) { status = static_cast<ofuntarstream*>(_stream)->flush(exit_code, exit_signal) ; }
	return status ; 
}


bool CurlFile::flush()
{
	bool status (true) ; 
    ACS_COND_THROW( (0 == _stream), exIllegalValueException("Null pointer")) ; 
	if (true == _is_ofuntarstream) { status = static_cast<ofuntarstream*>(_stream)->flush() ; }
	return status ; 
}


_ACS_END_NESTED_NAMESPACE
