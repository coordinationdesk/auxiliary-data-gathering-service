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
 *  Revision 5.2  2014/11/03 17:03:11  marpas
 *  simplified, init condition shall not be verified from external
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.2  2012/11/28 18:16:23  marpas
 *  qa rules,
 *  optimization and robustness
 *  still work in progress
 *
 *  Revision 2.1  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.3  2011/09/29 16:46:53  marpas
 *  rules fixing
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

#ifndef _CurlFile_H_
#define _CurlFile_H_


#include <acs_c++config.hpp>
#include <ftarstream.h>

#include <string>

#include <cstdio>

_ACS_BEGIN_NESTED_NAMESPACE(acs,curl) 

/**
 * Stores information about the download target file.
 *
 * This class handles the local FILE where to store the remote file
 * <P>
 * @see acs::curl::Curl 's get() function.
 *
 */
class CurlFile { // PRQA S 2109
public:
    /**
     * The ctor.
     * Create the object using the filename;
     * @param name: the local filename.
     */
    explicit CurlFile(std::string const& name);
	explicit CurlFile(ofuntarstream* stream);

    CurlFile(CurlFile const& ) = delete ;
    CurlFile& operator=(CurlFile const& ) = delete ;

    ~CurlFile() noexcept ;
    void fopen();

	/* Flush the buffer -- only if it's a ofuntarstream, otherwise do nothing */
	bool flush(int& exit_code, int& exit_signal) ;
	bool flush() ;

	inline const std::string &getFilename() const noexcept { return _filename; }

	inline void* getStream() const noexcept { return _stream ; } // NOSONAR - callback related type 
	inline void setStream(void* p) noexcept { _stream = p ; } // NOSONAR - callback related type 
	inline off_t getWrote() const noexcept { return _wrote; }
	inline void setWrote(off_t const off) noexcept { _wrote = off; }
	inline void addWrote(off_t const off) noexcept { _wrote += off; }

private:
    std::string _filename = "" ;
    void* _stream = nullptr ;
	bool _needs_fclose = false ;
	bool _is_ofuntarstream ;
    off_t _wrote = 0 ;
};


_ACS_END_NESTED_NAMESPACE


#endif /* _CurlFile_H_ */
