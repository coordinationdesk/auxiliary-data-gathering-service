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
	Revision 2.10  2014/06/27 13:10:12  marpas
	qa warning
	
	Revision 2.9  2014/06/27 09:16:34  marpas
	qa warnings
	
	Revision 2.8  2014/04/02 11:32:25  marpas
	new throwing macros adopted
	if ifucstream (uncompress) does not find the executable, will try with gunzip
	[io]fucstream classes refactorized to not deal directly with pointers (auto_ptr used)
	SpawnProc ctor has a new flag defaulted to true to resolve symbolic link on commands.
	CmdIORedir class uses the above flag to false (because compress and uncompress commands are the same executable, but their behaviour depends on the argv[0] i.e. the summoned name)
	
	Revision 2.7  2013/11/22 20:18:32  marpas
	coding best practices applied
	
	Revision 2.6  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.5  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.4  2012/02/09 18:40:11  marpas
	refactoring in progress
	
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
	
	Revision 1.8  2005/07/25 15:42:35  ivafam
	Header format fixed
	
	Revision 1.7  2004/03/08 20:41:28  envisat
	misleading commented part now removed from last version
	
	Revision 1.6  2004/03/08 15:46:39  envisat
	rewritten to use ifdstream and ofdstream from libFile 1_17
	
	Revision 1.5  2003/04/30 16:30:06  marpas
	using namespace std no longer into acs includes
	
	Revision 1.4  2003/04/30 10:05:42  marpas
	using namespace std was removed from includes
	
	Revision 1.3  2003/04/15 17:54:51  marpas
	GCC3.2
	
	Revision 1.2  2003/02/25 18:51:57  marpas
	ftmpstream classes added
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
	

*/

#ifndef _fucstream_H_
#define _fucstream_H_

#include <acs_c++config.hpp>

#include <File.h>
#include <exException.h>

#include <fstream>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////////
//							//
// classe ifucstream		//
//							//
//////////////////////////////

class CmdIORedir ;

class ifucstream // PRQA S 2109
{

public:

	ifucstream() throw() ;
	explicit ifucstream(const char *name) ;
	~ifucstream() throw() ;
	
	void close() throw() ; // PRQA S 2502 2
	void open(const char *name) ;
	std::istream &operator()() ;
	
private:
	// declared but not defined
	ifucstream(const ifucstream &) ;
	ifucstream &operator=(const ifucstream &) ;
	
private:
#if __cplusplus > 199711L
	std::unique_ptr<CmdIORedir> _popenp ;
	std::unique_ptr<std::ifstream> _input ;
#else
	std::auto_ptr<CmdIORedir> _popenp ;
	std::auto_ptr<std::ifstream> _input ;
#endif
	ACS_CLASS_DECLARE_DEBUG_LEVEL (ifucstream);
} ;



class ofucstream // PRQA S 2109
{

public:
	ofucstream() ;
	explicit ofucstream(const char *name, File::CompressionMode m = File::Plain) ;
	~ofucstream() throw() ;
	
	void close() throw() ; // PRQA S 2502 2
	void open(const char *name, File::CompressionMode m = File::Plain) ;
	std::ostream &operator()() ;
	const std::string &path() const throw() ;
private:
	// declared but not defined
	ofucstream(const ofucstream &) ;
	ofucstream &operator=(const ofucstream &) ;
	
private:
#if __cplusplus > 199711L
	std::unique_ptr<CmdIORedir> _popenp ;
	std::unique_ptr<std::ofstream> _output ;
#else
	std::auto_ptr<CmdIORedir> _popenp ;
	std::auto_ptr<std::ofstream> _output ;
#endif
	std::string _path ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL (ofucstream);
} ;

_ACS_END_NAMESPACE

#endif /* _ifucstream_H_ */


