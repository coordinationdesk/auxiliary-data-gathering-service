// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	Revision 2.6  2013/11/22 20:18:32  marpas
	coding best practices applied
	
	Revision 2.5  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.4  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.3  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.2  2008/11/18 14:22:32  marpas
	setAutoremove changed int setAutoRemove
	
	Revision 2.1  2008/11/18 14:20:31  marpas
	setAutoRemove method added
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/07/25 15:42:35  ivafam
	Header format fixed
	
	Revision 1.4  2004/03/08 15:46:39  envisat
	rewritten to use ifdstream and ofdstream from libFile 1_17
	
	Revision 1.3  2003/04/30 16:30:06  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/04/30 10:05:42  marpas
	using namespace std was removed from includes
	
	Revision 1.1  2003/02/25 18:51:57  marpas
	ftmpstream classes added
	
	
		

*/

#ifndef _ftmpstream_H_
#define _ftmpstream_H_

#include <fucstream.h>



_ACS_BEGIN_NAMESPACE(acs)

/*! \class oftmpstream
	
	Utility class to support creation of temporary output files
*/
class oftmpstream // PRQA S 2109
{
public:
	oftmpstream() ;
	explicit oftmpstream(const std::string &prefix, const std::string &ext="", File::CompressionMode m = File::Plain, bool autoRemove=true) ;
	~oftmpstream() throw() ;
	
	const std::string &path() const throw() ;
	void close() ; // PRQA S 2502 2
	void open(const std::string &prefix, const std::string &ext="", File::CompressionMode m = File::Plain, bool autoRemove=true) ;
	std::ostream &operator()() ;
	bool setAutoRemove(bool) ;
	
private:
	// declared but not defined
	oftmpstream(const oftmpstream &) ;
	oftmpstream &operator=(const oftmpstream &) ;

	
	void closeAndRemove() ;
	
private:
	
	bool _autoRemove ;
	std::string _path ;
	ofucstream _theStream ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL (oftmpstream);
} ;

_ACS_END_NAMESPACE

#endif	// _ftmpstream_H_
