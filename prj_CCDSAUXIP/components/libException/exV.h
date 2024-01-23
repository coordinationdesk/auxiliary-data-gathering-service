// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/04 15:35:03  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.1  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.3  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2009/03/11 17:09:03  marpas
	c++rules violation removed
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.2  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.1.1.1  2002/09/30 19:36:15  marpas
	Import libException
	

*/

#ifndef _exV_H_
#define _exV_H_

#include <acs_c++pers.hpp>

namespace acs {

  // support class for versioning

  class exV { // PRQA S 2109, 2647
  public:
	  exV() = default ;
	  ~exV() = default ;
	  static const char*  tag() ;
	  static const char*  name() ;
  private:
	  static const char* 	_tag ;
	  static const char* 	_name ;
  } ;

} // Close namespace

#endif // _exV_H_

