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

#include <ftarstream.h>

#include <Filterables.h>
#include <SpawnProc.h>
#include <StringUtils.h>
#include <Dir.h>
#include <cstring>

#if __cplusplus > 199711L
    #define SMART_UNIQUE_PTR unique_ptr
    #define SMART_U_PTR_INITIALIZER nullptr
#else
    #define SMART_UNIQUE_PTR auto_ptr
    #define SMART_U_PTR_INITIALIZER 0
#endif


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

// Debub helper definitions
ACS_CLASS_DEFINE_DEBUG_LEVEL(iftarstream)
ACS_CLASS_DEFINE_DEBUG_LEVEL(ofuntarstream)


namespace {
    const string tar = "tar" ;
	const size_t _default_tar_block_factor = 20 ; // Thus a block size of (20*512) = 10240
}


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

                               CLASS:  iftarstream

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/***********************************************************************************
******                                                                        ******
******                            iftarstream                                 ******
******                                                                        ******
***********************************************************************************/
iftarstream::iftarstream() throw() :
	_popenp(SMART_U_PTR_INITIALIZER),
	_input(SMART_U_PTR_INITIALIZER),
	_path(),
	_list_file(File::tempname( "/tmp/list_file", "txt"))
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Method called" );
}


iftarstream::iftarstream(const char *entry_name, const File::CompressionMode& tar_mode, const bool encapsulate_dir) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_input(SMART_U_PTR_INITIALIZER),
	_path(),
	_list_file(File::tempname( "/tmp/list_file", "txt"))
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Source entry name: \"" << entry_name \
		<< "\"  Tar default block factor: " << _default_tar_block_factor \
		<< "\"  Tar CompressionMode: " << tar_mode << "  Explicit dir in archive: " << (encapsulate_dir?"T":"F"));
	
	open(entry_name, tar_mode, _default_tar_block_factor, encapsulate_dir) ;	
}

iftarstream::iftarstream(	const char *entry_name, 
							const File::CompressionMode& tar_mode, 
							const size_t& tar_block_factor,
							const bool encapsulate_dir) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_input(SMART_U_PTR_INITIALIZER),
	_path(),
	_list_file(File::tempname( "/tmp/list_file", "txt"))
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Source entry name: \"" << entry_name \
		<< "\"  Tar block factor: " << tar_block_factor \
		<< "  Tar CompressionMode: " << tar_mode << "  Explicit dir in archive: " << (encapsulate_dir?"T":"F"));
	
	open(entry_name, tar_mode, tar_block_factor, encapsulate_dir) ;	
}


/***********************************************************************************
******                                                                        ******
******                            ~iftarstream                                ******
******                                                                        ******
***********************************************************************************/
iftarstream::~iftarstream() throw() 
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Method called" );

	close() ;	// Deallocate any allocated pointer
	::unlink(_list_file.c_str()) ; 
}


/***********************************************************************************
******                                                                        ******
******                                  close                                 ******
******                                                                        ******
***********************************************************************************/
void iftarstream::close() throw() 
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Method called" );

	_input.reset(SMART_U_PTR_INITIALIZER) ;		// Deallocate pointer, if needed (calling dtor, so flushing the buffer) and replace it with NULL ptr
	_popenp.reset(SMART_U_PTR_INITIALIZER) ;	// Deallocate pointer, if needed (calling dtor) and replace it with NULL ptr
}


/***********************************************************************************
******                                                                        ******
******                               create_tree                              ******
******                                                                        ******
***********************************************************************************/
size_t iftarstream::create_dir_tree(const char *entry_name, const bool encapsulate_dir, const string& list_file, string& tar_base_dir)
{
	/**
		* If the argument is a directory, create a tree from a directory and save the list to a (user-provided) temporary file.
		The meaning of "encapsulate_dir" is to wrap the entries inside the last level of the tree (the "container") or just explore the content of the last level itself.
		
		Note that if encapsulate_dir is SET, the tar will take into account to create the tree, therefore this method will just return the base directory.
		Otherwise, if encapsulate_dir is UNSET, the method will return a list of the first level of depth. The tar will take into account to expand the subdirectories.
		
		E.G. given this tree: /raid0/dir1 which contains: file1, file2, dir2/file3, dir3/
		
		- If encapsulate_dir is TRUE the method will return one entry: "dir1/"
			The tar command will expand to: dir1/file1, dir1/file2, dir1/dir2/file3, dir3/  (as expected)
		
		- If encapsulate_dir is UNSET the method will return four entries: "file1" "file2" "dir2/" "dir3/"
			The tar command will expand "dir2/" as "dir2/file3"

		* If the argument is a file, return the file name, if needed encapsulating in its own root directory.
		E.G. given this file: /raid0/file1
		
		- If encapsulate_dir is TRUE the method will return one entry: "/"
			The tar command will insert the item to: "raid0/file1"
		
		- If encapsulate_dir is UNSET the method will return one entry: "/raid0"
			The tar command will insert the item to: "file1"

	*/


	const bool show_hidden (true) ; 
	const string dir_separator ("/") ;
	ACS_COND_THROW ( (false == File::exists(entry_name)), exIllegalValueException("Source entry \"" + string(_path) + "\" not existing") ) ; 
	const bool isDir ( File::isDir(entry_name) ) ; 
	size_t cnt=0 ; 
	
	ofstream ofile( list_file.c_str(), ios::out|ios::binary|ios::trunc|ios::ate );
	if (ofile.fail()) { ostringstream o ; o << "Error opening file: \"" << list_file << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }	

	if ( ((true == isDir) && (true == encapsulate_dir)) || ((false == isDir) && (false == encapsulate_dir)) )
	{
		ofile << File::getFileName(entry_name) << ((true == isDir)?dir_separator:"") << "\n" ; cnt++ ; 	
		if (ofile.bad()) { ostringstream o ; o << "Error writing to file: \"" << list_file << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ;  }	

		tar_base_dir = File::getDirPath(entry_name) ;
	}
	else if ( (true == isDir) && (false == encapsulate_dir) )
	{
		DirTree d(entry_name, dir_separator, show_hidden) ;

		d.maxdepth(1) ;				// Only the first level of files and directories is returned. The tar command will expand the directories.				
		d.relative("") ;			// Otherwise, the whole absolute path (e.g. /raid0/dir1) would be saved -- ugly.

		if (d.size() >= 1)
		{	// Note that the for below starts from "1" (and not "0") since the first entry (the [0]) contains the dot-dir (".") which might confuse the untar.
			for (size_t i=1; i<d.size(); i++) {
				ofile << d[i] << "\n" ; cnt++ ; 	
				if (ofile.bad()) { ostringstream o ; o << "Error writing to file: \"" << list_file << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ;  }	
			}
		}
		
		tar_base_dir = entry_name ;
	}
	else 
	{
		// Only when: ( (false == isDir) && (true == encapsulate_dir) )
		
		string entry ; 
		entry = File::getFileName(entry_name) ; tar_base_dir = File::getDirPath(File::getAbsolutePath(entry_name)) ; 
		if ( (dir_separator != tar_base_dir) && (false == tar_base_dir.empty()) )
		{
			entry = StringUtils::pathJoin(File::getFileName(tar_base_dir), entry) ; tar_base_dir = File::getDirPath(tar_base_dir) ; 
		}

		ofile << entry << "\n" ; cnt++ ; 	
		if (ofile.bad()) { ostringstream o ; o << "Error writing to file: \"" << list_file << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ;  }	
	}
	
	ofile.close() ; 
	
	return cnt ; 
}


/***********************************************************************************
******                                                                        ******
******                                    open                                ******
******                                                                        ******
***********************************************************************************/
void iftarstream::open(	const char *entry_name_in, 
						const File::CompressionMode& tar_mode, 
						const size_t& tar_block_factor, 
						const bool encapsulate_dir )
{
	close() ;

	string base_dir ; 
    _path = entry_name_in ; 

	ACS_COND_THROW ( (false == File::exists(_path)), exIllegalValueException("Source entry \"" + string(_path) + "\" not existing") ) ; 

	// Create the directory tree on the temporary list file
	create_dir_tree(entry_name_in, encapsulate_dir, _list_file, base_dir) ; 

    // do not allow ifdstream to auto close the FD. It will be closed by CmdIORedir
    // a new signature of ifdstream/ofdstream allows to specify the autoclose boolean (defaulted to true)

	vector <string> arguments_V ; 
	if (tar_block_factor != _default_tar_block_factor)
	{
		ostringstream os ; os << tar_block_factor ;
		arguments_V.push_back("-b") ; 
		arguments_V.push_back(os.str()) ;
	}
	arguments_V.push_back("-C") ; 
	arguments_V.push_back(base_dir) ;
	arguments_V.push_back("-T") ; 
	arguments_V.push_back(_list_file) ; 

	switch(tar_mode) 
	{
		case File::Plain:
		case File::TARred:
	    {
			const string tar_option( "-c" ) ; 
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Plain tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
		    break ;
	    }
		case File::Compressed:
	    {
			const string tar_option( "-cZ" ) ; 
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Compressed tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
		    break ;
	    }

		case File::GZipped:
	    {
			const string tar_option( "-cz" ) ; 
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "GZ-compressed tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
		    break ;
	    }
#ifdef __linux__
		case File::BZipped:
	    {
			const string tar_option( "-cj" ) ; 
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "BZ-compressed tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
		    break ;
	    }
#endif
		default:
        {
			ostringstream o ; o << "Unknown mode for \"" << _path << "\"  compression type: " << tar_mode ;
		    ACS_THROW(exFileOpenException(o.str(), errno)) ; // PRQA S 3081
        }

	}	// END: switch(tar_mode) 

	_popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(tar,false,"/dev/null","",arguments_V)) ;
	_input = SMART_UNIQUE_PTR<ifstream>(new File::ifdstream(*_popenp, false)) ;
}


/***********************************************************************************
******                                                                        ******
******                              operator()                                ******
******                                                                        ******
***********************************************************************************/
istream &iftarstream::operator()()   // PRQA S 4211
{
	if (0 == _input.get()) {
		ACS_THROW(exFileOpenException("Stream was not opened")) ; // PRQA S 3081
	}

	return *_input ;
}
	
	
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

                               CLASS:  ofuntarstream

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/***********************************************************************************
******                                                                        ******
******                            ofuntarstream                               ******
******                                                                        ******
***********************************************************************************/
ofuntarstream::ofuntarstream() :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
}

ofuntarstream::ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Destination directory name: \"" << dest_dir_name \
		<< "\"  Tar CompressionMode: " << tar_mode << "  Tar default block factor: " << _default_tar_block_factor) ;

	vector<string> decompress_opts ; // Empty

	open(dest_dir_name, tar_mode, _default_tar_block_factor, decompress_opts) ;	
}

ofuntarstream::ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode, const size_t& tar_block_factor) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Destination directory name: \"" << dest_dir_name \
		<< "\"  Tar CompressionMode: " << tar_mode << "  Tar block factor: " << tar_block_factor) ;

	vector<string> decompress_opts ; // Empty

	open(dest_dir_name, tar_mode, tar_block_factor, decompress_opts) ;	
}


ofuntarstream::ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode, \
								const vector<string>& decompress_opts) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
	const string debug_decompress_opts(StringUtils::join(decompress_opts, ' ')) ; 
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Destination directory name: \"" << dest_dir_name \
		<< "\"  Tar CompressionMode: " << tar_mode << "  Tar default block factor: " << _default_tar_block_factor \
		<< "  Tar decompress options: \"" << debug_decompress_opts << "\"" ) ;

	open(dest_dir_name, tar_mode, _default_tar_block_factor, decompress_opts) ;	
}


ofuntarstream::ofuntarstream(const char *dest_dir_name, const File::CompressionMode& tar_mode, \
								const size_t& tar_block_factor, const vector<string>& decompress_opts) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
	const string debug_decompress_opts(StringUtils::join(decompress_opts, ' ')) ; 
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Destination directory name: \"" << dest_dir_name \
		<< "\"  Tar CompressionMode: " << tar_mode << "  Tar block factor: " << tar_block_factor \
		<< "  Tar decompress options: \"" << debug_decompress_opts << "\"" ) ;

	open(dest_dir_name, tar_mode, tar_block_factor, decompress_opts) ;	
}


/***********************************************************************************
******                                                                        ******
******                            ~ofuntarstream                              ******
******                                                                        ******
***********************************************************************************/
ofuntarstream::~ofuntarstream() throw() 
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Method called" );

	close() ;	// Deallocate any allocated pointer
}


/***********************************************************************************
******                                                                        ******
******                                close                                   ******
******                                                                        ******
***********************************************************************************/
void ofuntarstream::close() throw() 
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Method called" );
	
	_output.reset(SMART_U_PTR_INITIALIZER) ;	// Deallocate pointer, if needed (calling dtor, so flushing the buffer) and replace it with NULL ptr
	_popenp.reset(SMART_U_PTR_INITIALIZER) ;	// Deallocate pointer, if needed (calling dtor) and replace it with NULL ptr
}


/***********************************************************************************
******                                                                        ******
******                                flush                                   ******
******                                                                        ******
***********************************************************************************/
bool 																	/* TRUE: process exited successfully, o/w FALSE */
		ofuntarstream::flush()
{
	int unused_e (0), unused_s(0) ; 

	return flush(unused_e, unused_s) ; 
}


/***********************************************************************************
******                                                                        ******
******                                flush                                   ******
******                                                                        ******
***********************************************************************************/
bool 																	/* TRUE: process exited successfully, o/w FALSE */
		ofuntarstream::flush(int& exit_code, int& exit_signal)
{
	/**
		Execute the "hangup" method of the underlaying class CmdIORedir.
		First, we call the destructor of the ofstream piped to the process' input stream.
		This, in order, flushes the ofstream from any data is was retaining, send a EOF through the streams (which the process receives) and deallocates it.
		As result, the process receive a "data+flush" of its own input stream, followed by EOF, so it terminates all the operations related to the input.
	*/

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Method called");

	ACS_COND_THROW ( (! _popenp.get()), exIllegalValueException("Null pointer") ) ; 

	exit_code = exit_signal = 0 ; 

	// Deallocate [flush, send EOF and close] the ostream, if still allocated
	if ( _output.get() )
	{ _output.reset(SMART_U_PTR_INITIALIZER) ; } // Call the destructor and replace with NULL ptr.

	bool hangup_success ( _popenp->hangup(exit_code, exit_signal) ) ;

	const bool success = ( (true == hangup_success) && (0 == exit_code) && (0 == exit_signal) ) ; 
	
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Returning: " << (success?"T":"F") );

	return success ; 
}


/***********************************************************************************
******                                                                        ******
******                                 open                                   ******
******                                                                        ******
***********************************************************************************/
void ofuntarstream::open(const char *dest_dir_name, const File::CompressionMode& tar_mode, \
							const size_t& tar_block_factor, const vector<string>& decompress_opts)
{
    // do not allow ifdstream to auto close the FD. It will be closed by CmdIORedir
    // a new signature of ifdstream/ofdstream allows to specify the autoclose boolean (defaulted to true)
	 
	close() ;

	_path = dest_dir_name ;

	if ( (false == File::exists(_path)) || (false == File::isDir(_path)) ) 
	{
		ACS_THROW(exIllegalValueException("Destination directory \"" + string(_path) + "\" invalid or not existing")) ; 
	}

	vector <string> arguments_V ; 

	if (tar_block_factor != _default_tar_block_factor)
	{
		ostringstream os ; os << tar_block_factor ;
		arguments_V.push_back("-b") ; 
		arguments_V.push_back(os.str()) ;
	}

	arguments_V.push_back("-C") ; 
	arguments_V.push_back(_path) ;

	if (false == decompress_opts.empty())
	{ arguments_V.insert( arguments_V.end(), decompress_opts.begin(), decompress_opts.end() ) ; }

	switch (tar_mode) {
  
    	case File::Plain:
 		case File::TARred:
    	{
			const string tar_option( "-x" ) ; 
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Plain tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
			break ;
    	}
    	case File::Compressed:
    	{
			const string tar_option( "-xZ" ) ; 
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Compressed tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
			break ;
    	}
    	case File::GZipped:
    	{
			const string tar_option( "-xz" ) ; 
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "GZ-compressed tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
			break ;
    	}
    	case File::BZipped:
    	{
			const string tar_option( "-xj" ) ; 
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "BZ-compressed tar. Option: " + tar_option);
			arguments_V.push_back(tar_option) ; 
			break ;
    	}
		default:
        {
			ostringstream o ; o << "Unknown mode for \"" << _path << "\"  compression type: " << tar_mode ;
	    }
    }	// END: switch (m)
	
	_popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(tar,false,"","/dev/null",arguments_V)) ; // PRQA S 3050, 3081 2
	_output = SMART_UNIQUE_PTR<ofstream>(new File::ofdstream(*_popenp, false)) ;
}


/***********************************************************************************
******                                                                        ******
******                              operator                                  ******
******                                                                        ******
***********************************************************************************/
ostream &ofuntarstream::operator()() // PRQA S 4211
{
	if (0 == _output.get()) {
		ACS_THROW(exFileOpenException("Stream was not opened")) ; // PRQA S 3081
	}

	return *_output ;
}


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

                              CLASS:  tarstream_test

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


/***********************************************************************************
******                                                                        ******
******                             write_to_file                              ******
******                                                                        ******
***********************************************************************************/
void tarstream_test::write_to_file(const string& s, const string& filename) 
{
	ofstream ofile( filename.c_str(), ios::out|ios::binary|ios::trunc|ios::ate );

	if (ofile.fail()) { ostringstream o ; o << "Error opening file: \"" << filename << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }	
	ofile << s ; 
	if (ofile.bad()) { ostringstream o ; o << "Error writing to file: \"" << filename << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }	
	ofile.close() ; 

	return ; 
}


/***********************************************************************************
******                                                                        ******
******                           read_from_file                               ******
******                                                                        ******
***********************************************************************************/
string tarstream_test::read_from_file(const string& filename) 
{
	ACS_COND_THROW( (false==File::exists(filename)), exIOException("File not found: \"" + filename + "\"") ) ; 
	
	ifstream ifs( filename.c_str(), ios::in|ios::binary );
	if (ifs.fail()) { ostringstream o ; o << "Error opening file: \"" << filename << "\": " << strerror(errno) ; ACS_THROW( exIOException(o.str()) ) ; }	

	ostringstream s ; s << ifs.rdbuf() ;
	ifs.close();

	return s.str() ; 
}


/***********************************************************************************
******                                                                        ******
******                            compare_from_file                           ******
******                                                                        ******
***********************************************************************************/
bool tarstream_test::compare_from_file(const string& s, const string& filename) 
{
	const string s_from_file( read_from_file(filename) ) ;
	
	return (s == s_from_file) ;
}


/***********************************************************************************
******                                                                        ******
******                                self_test                               ******
******                                                                        ******
***********************************************************************************/
int tarstream_test::self_test()
{
	const string dir1("dir1") ; 
	const string dir2("dir2") ; 
	const string file1("file1") ; 
	const string file2("file2") ; 
	const string file3(StringUtils::pathJoin(dir2,"file3")) ; 

	const string content_of_file1("Content_of_file1") ; 
	const string content_of_file2("CONTENT_of_file_2") ;
	const string content_of_file3("THE_CONTENT_OF_FILE3") ;

	FileGuard FG__tempfile( File::tempname("/tmp/tmpfile") ) ;		const string tempfile (FG__tempfile) ;
	FileGuard FG__reference_dir( File::tempdir("/tmp/refdir") ) ;	const string reference_dir (FG__reference_dir) ;
	FileGuard FG__output_root_dir( File::tempdir("/tmp/outdir") ) ;	const string output_root_dir (FG__output_root_dir) ;
//const string output_root_dir (File::tempdir("/tmp/outdir")) ; File::mkdir(output_root_dir, 0775) ; DO NOT ENABLE THIS

	const string dir1_inpath( StringUtils::pathJoin(reference_dir, dir1) ) ; 

	File::mkdir( dir1_inpath ) ; 
	File::mkdir( StringUtils::pathJoin(dir1_inpath, dir2) ) ; 

	write_to_file( content_of_file1,  StringUtils::pathJoin(dir1_inpath, file1) ) ; 
	write_to_file( content_of_file2,  StringUtils::pathJoin(dir1_inpath, file2) ) ; 
	write_to_file( content_of_file3,  StringUtils::pathJoin(dir1_inpath, file3) ) ; 

	for (size_t k=0; k<16; k++)
	{	
		// Run 16 tests, each one a different scenario (all the combinations of explicit dir (T/F), 
		//	tar_mode (4 kind of compressions), default and customized tar-block-size

		File::CompressionMode tar_mode ; 
		switch (k % 4)
		{
			case 0: { tar_mode = File::Plain ;		break ; }
			case 1: { tar_mode = File::Compressed ;	break ; }
			case 2: { tar_mode = File::GZipped ; 	break ; }
			case 3: { tar_mode = File::BZipped ; 	break ; }
			default:	
			{}
		}
		if (0 == (k % 4)) { tar_mode = File::Plain ; }// FIXME REMOVE AS USELESS

		const bool encapsulate_dir( (k % 8) >= 4 ) ; 
		const bool custom_tar_block_factor( k >= 8 ) ; 

		{ 
			ostringstream o ; 
			o << "\nRunning test: " << (k+1) << "/16 : TAR Compression: " << tar_mode \
				<< "  directory wrapping: " << (encapsulate_dir?"T":"F") \
				<< "  customized tar block factor: " << (custom_tar_block_factor?"T":"F") << endl ;
			cout << o.str() ; 
		}

		if (false == custom_tar_block_factor)
		{
			// Scope for file handling -- only after the scope (or explicit closes) the output files are created
		
			ofstream saved_stream (tempfile.c_str(), ios::out|ios::binary|ios::trunc|ios::ate) ; // This file will contain A COPY of stream, in order to probe file content (tar compression type)
		
			iftarstream to_tar(dir1_inpath.c_str(), tar_mode, encapsulate_dir) ;
			ofuntarstream from_tar(output_root_dir.c_str(), tar_mode) ;

			while ( (true == to_tar().good()) && (true == from_tar().good()) )	/* EOF on istream will unset goodbit, so no check on istream::eof is needed */
			{
				unsigned char buffer[1024] ; 
				to_tar().read(reinterpret_cast <char*>(buffer), sizeof(buffer));
				const ssize_t r(to_tar().gcount()) ;
				// cout << "Just read: " << r << endl ;

				string s (reinterpret_cast <char*>(buffer), r) ;
				from_tar() << s ;
				
				saved_stream << s ; // Just to create a copy of the stream to a local file -- NOT NEEDED.
			}	// END: while

			ACS_COND_THROW ( (true == to_tar().bad()), exIllegalValueException("Problem writing the tar stream") ) ;
			ACS_COND_THROW ( (true == from_tar().bad()), exIllegalValueException("Problem reading from the tar stream") ) ;

			to_tar.close() ;
			from_tar.close() ;
			saved_stream.close() ; 
		}	// Close scope for input/output streams 
		else
		{
			// Scope for file handling -- only after the scope (or explicit closes) the output files are created
		
			const size_t tar_block_factor(1024) ; // Use a block of 1024*512 bytes (i.e. 512kB)
		
			ofstream saved_stream (tempfile.c_str(), ios::out|ios::binary|ios::trunc|ios::ate) ; // This file will contain A COPY of stream, in order to probe file content (tar compression type)
		
			iftarstream to_tar(dir1_inpath.c_str(), tar_mode, tar_block_factor, encapsulate_dir) ;
			ofuntarstream from_tar(output_root_dir.c_str(), tar_mode, tar_block_factor) ;

			while ( (true == to_tar().good()) && (true == from_tar().good()) )	/* EOF on istream will unset goodbit, so no check on istream::eof is needed */
			{
				unsigned char buffer[1024] ; 
				to_tar().read(reinterpret_cast <char*>(buffer), sizeof(buffer));
				const ssize_t r(to_tar().gcount()) ;
				// cout << "Just read: " << r << endl ;

				string s (reinterpret_cast <char*>(buffer), r) ;
				from_tar() << s ;
				
				saved_stream << s ; // Just to create a copy of the stream to a local file -- NOT NEEDED.
			}	// END: while

			ACS_COND_THROW ( (true == to_tar().bad()), exIllegalValueException("Problem writing the tar stream") ) ;
			ACS_COND_THROW ( (true == from_tar().bad()), exIllegalValueException("Problem reading from the tar stream") ) ;

			to_tar.close() ;
			from_tar.close() ;
			saved_stream.close() ; 
		}	// Close scope for input/output streams 


		string f, output_dir(output_root_dir) ;
		if (true == encapsulate_dir) { output_dir = StringUtils::pathJoin(	output_root_dir, dir1) ; }

		// Check the compression type from the temporary file (it just contains a copy of the generated stream)
		const File::CompressionMode extracted_tar_mode (File::compressionMode(tempfile)) ;
		
		 // Remember that even if we choose a "plain" tar (i.e. an uncompressed tar), the unix file utility will return that type is "tar file".
		 // So in order to perform the next comparison, the "plain" type should be compared as "tar"
		const File::CompressionMode expected_tar_mode ( (File::Plain == tar_mode) ? File::TARred : tar_mode ) ;
				
		// Check if the file type extracted by the stream matches the expected one.
		ACS_COND_THROW ( (extracted_tar_mode != expected_tar_mode), exIllegalValueException("Stream generated by class iftarstream does not match requested compression type !" ) ) ; 

		f = StringUtils::pathJoin(output_dir, file1) ;  cout << "Checking file content for: \"" << f << "\" ...\t" ; 
		ACS_COND_THROW( (false == compare_from_file(content_of_file1, f)), \
			exIllegalValueException("Error comparing the content of file1") ) ;  
		cout << "Successfull." << endl ;

		f = StringUtils::pathJoin(output_dir, file2) ;  cout << "Checking file content for: \"" << f << "\" ...\t" ; 
		ACS_COND_THROW( (false == compare_from_file(content_of_file2, f)), \
			exIllegalValueException("Error comparing the content of file2") ) ;  
		cout << "Successfull." << endl ;

		f = StringUtils::pathJoin(output_dir, file3) ;  cout << "Checking file content for: \"" << f << "\" ...\t" ; 
		ACS_COND_THROW( (false == compare_from_file(content_of_file3, f)), \
			exIllegalValueException("Error comparing the content of file3") ) ;  
		cout << "Successfull." << endl ;

	}	// END: for (size_t k=0; k<8; k++)

	return 0 ; 
}

_ACS_END_NAMESPACE
