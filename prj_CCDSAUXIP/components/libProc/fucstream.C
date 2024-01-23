// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
	Revision 2.12  2014/12/17 11:39:04  marfav
	Fixed management of FD closure
	
	Revision 2.11  2014/04/02 11:32:25  marpas
	new throwing macros adopted
	if ifucstream (uncompress) does not find the executable, will try with gunzip
	[io]fucstream classes refactorized to not deal directly with pointers (auto_ptr used)
	SpawnProc ctor has a new flag defaulted to true to resolve symbolic link on commands.
	CmdIORedir class uses the above flag to false (because compress and uncompress commands are the same executable, but their behaviour depends on the argv[0] i.e. the summoned name)
	
	Revision 2.10  2013/11/28 15:40:05  marpas
	CmdIORedir is able to redirect simultaneously input and output on files
	When just one of the two streams is redirected, the class act as the other end of a pipe,
	either input or output according to which stream is redirected.
	If both ends are redirected to file system, the class cannnot act as pipe end
	
	Revision 2.9  2013/11/22 20:18:32  marpas
	coding best practices applied
	
	Revision 2.8  2013/07/25 15:02:13  marfav
	Including Filterables insted of deprecated headers
	
	Revision 2.7  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.6  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.5  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.4  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.3  2009/12/22 17:39:06  marpas
	using CmdIORedir instead of popen, this to control the correct closure of
	open file descriptors not used by the popen.
	
	Revision 2.2  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	Revision 2.1  2008/09/30 13:41:16  marpas
	fucstream::path was forgotten
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/07/25 15:42:35  ivafam
	Header format fixed
	
	Revision 1.6  2004/03/08 20:41:28  envisat
	misleading commented part now removed from last version
	
	Revision 1.5  2004/03/08 15:46:39  envisat
	rewritten to use ifdstream and ofdstream from libFile 1_17
	
	Revision 1.4  2004/03/08 10:48:47  envisat
	ios::in changed into ios::out according the open mode of the file descriptor
	
	Revision 1.3  2003/04/30 16:30:06  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/04/15 17:54:51  marpas
	GCC3.2
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
	

*/

#include <fucstream.h>

#include <Filterables.h>
#include <SpawnProc.h>

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
ACS_CLASS_DEFINE_DEBUG_LEVEL(ifucstream)
ACS_CLASS_DEFINE_DEBUG_LEVEL(ofucstream)


ifucstream::ifucstream() throw() :
	_popenp(SMART_U_PTR_INITIALIZER),
	_input(SMART_U_PTR_INITIALIZER) 
{
}

ifucstream::ifucstream(const char *name) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_input(SMART_U_PTR_INITIALIZER)
{
	open(name) ;	
}



ifucstream::~ifucstream() throw() 
{
}

void ifucstream::close() throw() 
{
	_input.reset(SMART_U_PTR_INITIALIZER) ;
    _popenp.reset(SMART_U_PTR_INITIALIZER) ;
}

namespace {
    const string uncompress = "uncompress" ;
    const string unzip = "unzip" ;
    const string gunzip = "gunzip" ;
    const string bunzip2 = "bunzip2" ;
}


void ifucstream::open(const char *name)
{
	close() ;

    // do not allow ifdstream to auto close the FD. It will be closed by CmdIORedir
    // a new signature of ifdstream/ofdstream allows to specify the autoclose boolean (defaulted to true)

    string fname = name ;
	switch(File::compressionMode(fname)) { // PRQA S 3081
	case File::Compressed:
	    {
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, uncompress);

		    try {
                _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(uncompress,false,fname,"")) ; // PRQA S 3050, 3081
            }
            catch (SpawnProc::SpawnException &x) {
		        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Exception: " << x);
                ACS_LOG_WARNING("Warning: \"" << uncompress << "\" not spawned, trying with \"" << gunzip <<'\"') ;
            }
		    
            ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "_popen: " << _popenp.get());

            if (not _popenp.get()) { // PRQA S 3050
                _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(gunzip,false,fname,"")) ;// PRQA S 3050, 3081
            }
		    _input = SMART_UNIQUE_PTR<ifstream>(new File::ifdstream(*_popenp, false)) ; // PRQA S 3050, 3081
		    break ;
	    }
	case File::GZipped:
	    {
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, gunzip);

		    _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(gunzip,false,fname,"")) ;// PRQA S 3050, 3081 2
		    _input = SMART_UNIQUE_PTR<ifstream>(new File::ifdstream(*_popenp, false)) ;

		    break ;
	    }
	case File::Plain:
        {
	        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, " opening " << fname << " assuming uncompressed");

	        _input = SMART_UNIQUE_PTR<ifstream>(new ifstream(fname.c_str())) ; // PRQA S 3050, 3081
	        break ;
        }
#ifdef __linux__
	case File::BZipped:
	    {
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, bunzip2);

		    _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(bunzip2,false,fname,"")) ;// PRQA S 3050, 3081 2
		    _input = SMART_UNIQUE_PTR<ifstream>(new File::ifdstream(*_popenp, false)) ; 
		    break ;
	    }
#endif
	default:
        {
		    ACS_THROW(exFileOpenException("Unknown mode for \"" + fname +"\"",errno)) ; // PRQA S 3081
        }
	}
}

istream &ifucstream::operator()()   // PRQA S 4211
{
	if (0 == _input.get()) {
		ACS_THROW(exFileOpenException("Stream was not opened")) ; // PRQA S 3081
	}

	return *_input ;
}
	






ofucstream::ofucstream() :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
}

ofucstream::ofucstream(const char *name, File::CompressionMode m) :
	_popenp(SMART_U_PTR_INITIALIZER),
	_output(SMART_U_PTR_INITIALIZER),
    _path()
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, " name: !" << name << "!" << " CompressionMode: " << m);

	open(name,m) ;	
}



ofucstream::~ofucstream() throw() 
{
}

void ofucstream::close() throw() 
{
	_output.reset(SMART_U_PTR_INITIALIZER) ;
    _popenp.reset(SMART_U_PTR_INITIALIZER) ;
}

namespace {
    const string compress = "compress" ;
    const string gzip = "gzip" ;
    const string bzip2 = "bzip2" ;
}

void ofucstream::open(const char *name, File::CompressionMode m)
{
    // do not allow ifdstream to auto close the FD. It will be closed by CmdIORedir
    // a new signature of ifdstream/ofdstream allows to specify the autoclose boolean (defaulted to true)

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, " name: !" << name << "!" << " CompressionMode: " << m);
	 close() ;
	
	_path = name ;
	switch (m) {
	case File::Compressed:
	    {
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, compress);
		    _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(compress,false,"",_path)) ; // PRQA S 3050, 3081 2
		    _output = SMART_UNIQUE_PTR<ofstream>(new File::ofdstream(*_popenp, false)) ;
		    break ;
	    }
	case File::GZipped:
	    {
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, gzip);
		    _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(gzip,false,"",_path)) ;// PRQA S 3050, 3081 2
		    _output = SMART_UNIQUE_PTR<ofstream>(new File::ofdstream(*_popenp, false)) ; 

		    break ;
	    }
#ifdef __linux__
	case File::BZipped:
	    {
		    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, bzip2);

		    _popenp = SMART_UNIQUE_PTR<CmdIORedir>(new CmdIORedir(bzip2,false,"",_path)) ; // PRQA S 3050, 3081 2
		    _output = SMART_UNIQUE_PTR<ofstream>(new File::ofdstream(*_popenp, false)) ; 
		    break ;
	    }
#endif
    case File::Plain:
        {
		    _output = SMART_UNIQUE_PTR<ofstream>(new ofstream(_path.c_str())) ; // PRQA S 3050, 3081
		    break ;
        }
	default:
        {
		    ACS_THROW(exFileOpenException("Unknown mode for \"" + _path + "\"",errno)) ; // PRQA S 3081
	    }
    }
}

ostream &ofucstream::operator()() // PRQA S 4211
{
	if (0 == _output.get()) {
		ACS_THROW(exFileOpenException("Stream was not opened")) ; // PRQA S 3081
	}

	return *_output ;
}
	


const string & ofucstream::path() const throw() // PRQA S 4120
{
	return _path ;
}

_ACS_END_NAMESPACE
