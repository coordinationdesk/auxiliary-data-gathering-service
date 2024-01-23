// PRQA S 1050 EOF
/*

	Copyright 1995-2020, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$


*/

#ifndef _ftarstream_H_
#define _ftarstream_H_

#include <acs_c++config.hpp>

#include <File.h>
#include <exException.h>

#include <fstream>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////////
//							//
// classe iftarstream		//
//							//
//////////////////////////////

class CmdIORedir ;

class iftarstream // PRQA S 2109
{
/**
	Given the name of a directory, creates a stream [istream] containing the tar.
*/
public:

	iftarstream() throw() ;
	explicit iftarstream(const char *source_dir_name, const File::CompressionMode& tar_mode, const bool encapsulate_dir) ;
	explicit iftarstream(const char *source_dir_name, const File::CompressionMode& tar_mode, \
							const size_t& tar_block_factor, const bool encapsulate_dir) ;
	~iftarstream() throw() ;
	
	void close() throw() ; // PRQA S 2502 2
	void open(const char *source_dir_name, const File::CompressionMode& tar_mode, \
				const size_t& tar_block_factor, const bool encapsulate_dir) ;
	std::istream &operator()() ;
	inline const std::string &path() const throw() { return _path ; }
	
private:
	size_t create_dir_tree(const char *dir_name, const bool encapsulate_dir, const std::string& list_file, std::string& tar_base_dir) ;

	// declared but not defined
	iftarstream(const iftarstream &) ;
	iftarstream &operator=(const iftarstream &) ;
	
private:
#if __cplusplus > 199711L
	std::unique_ptr<CmdIORedir> _popenp ;
	std::unique_ptr<std::ifstream> _input ;
#else
	std::auto_ptr<CmdIORedir> _popenp ;
	std::auto_ptr<std::ifstream> _input ;
#endif
	std::string _path ;
	std::string _list_file ; 

	ACS_CLASS_DECLARE_DEBUG_LEVEL (iftarstream);
} ;


class ofuntarstream // PRQA S 2109
{
/**
	Given a stream [ostream] containing a tar, expand the content to a local directory.
*/

public:
	ofuntarstream() ;
	explicit ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode) ;
	explicit ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode, const size_t& tar_block_factor) ;
	explicit ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode, \
							const std::vector<std::string>& decompress_opts) ;
	explicit ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode, \
							const size_t& tar_block_factor, const std::vector<std::string>& decompress_opts) ;
	~ofuntarstream() throw() ;

	void close() throw() ; // PRQA S 2502 2
	bool flush(int& exit_code, int& exit_signal) ;	/* Send a EOF to the stream (if the process is still running). Return TRUE if process exited successfully, FALSE o/w */
	bool flush() ;	/* As above, but just return the boolean */
	void open(const char *dest_dir_name, const File::CompressionMode& tar_mode, 
				const size_t& tar_block_factor, const std::vector<std::string>& decompress_opts) ;
	std::ostream &operator()() ;
	inline const std::string &path() const throw() { return _path ; }
private:
	// declared but not defined
	ofuntarstream(const ofuntarstream &) ;
	ofuntarstream &operator=(const ofuntarstream &) ;

private:
#if __cplusplus > 199711L
	std::unique_ptr<CmdIORedir> _popenp ;
	std::unique_ptr<std::ofstream> _output ;
#else
	std::auto_ptr<CmdIORedir> _popenp ;
	std::auto_ptr<std::ofstream> _output ;
#endif
	std::string _path ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL (ofuntarstream);
} ;


class tarstream_test
{
/**
	Given a stream [ostream] containing a tar, expand the content to a local directory.
*/

public:
	static int self_test() ;
	static void write_to_file(const std::string& s, const std::string& filename) ;
	static std::string read_from_file(const std::string& filename) ;
	static bool compare_from_file(const std::string& s, const std::string& filename) ;

private:
	tarstream_test() ;
	~tarstream_test() throw() ;
	tarstream_test(const tarstream_test &) ;
	tarstream_test &operator=(const tarstream_test &) ;

} ;

	
_ACS_END_NAMESPACE

#endif /* _iftarstream_H_ */


