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

 	$Prod: A.C.S. Resources Library $

    $Id$

    $Author$

    $Log$
    Revision 5.0  2013/06/11 09:29:10  marpas
    adopting libException 5.x standards

    Revision 2.10  2013/02/22 13:56:41  marpas
    rsResourceSet does not longer inherit virtually from ThreadSafe, it has an internal mutex instead
    Interface rationalization
    coding best practices

    Revision 2.9  2012/11/20 22:39:55  marpas
    total quality: code, coverage etc.

    Revision 2.8  2012/01/31 18:13:57  marpas
    compiler warnings removed

    Revision 2.7  2011/03/01 11:24:19  marpas
    GCC 4.4.x support

    Revision 2.6  2007/09/17 12:40:00  enrcar
    EC:: Anchor was missing -- fixed

    Revision 2.5  2007/03/26 10:00:01  enrcar
    unable to compile if booleans already defined. Fixed.

    Revision 2.4  2007/03/08 14:33:51  enrcar
    excerr commented, since on qt a panel appears...

    Revision 2.3  2007/01/18 16:08:59  clanas
    inserted mutex to lock static vector of resources

    Revision 2.2  2006/10/11 10:03:27  marpas
    compilation error fixed

    Revision 2.1  2006/10/11 09:49:16  marpas
    RmLegacy header provided

*/


#ifndef __LIBRMLEGACY_H__
#define __LIBRMLEGACY_H__

#ifdef sgi
/*NOP*/
#else


#include <acs_c++config.hpp>
#include <exStream.h>
#include <Filterables.h>
#include <rsResourceSet.h>
#include <ThreadSafe.h>

#include <fstream>
#include <cstring>


_ACS_BEGIN_NAMESPACE(RmLegacy)

/*! \namespace RmLegacy
	Contains all needed method to substitute old libRm.
	Internally it uses rsResourceSet and libException.
	Does not longer uses libError.
	
	Not all the libRm functions are yet supported. Please notify M.Pastori/E.Carta any special needs.(prepare money.)
*/
using namespace acs ;

namespace {
     ThreadSafe _rmMutex ;

     std::vector<rsResourceSet> _rsArray ;
}

/** the returned value is the numebr of db created to which you need to refer to for any other operation. 
	In case of error it will be < 0 and non db are created */
inline int rmCreateDb(const char *) { // PRQA S 2134
  rsResourceSet a ;
  ThreadSafe::Lock lock(_rmMutex) ;
  _rsArray.push_back(a) ;
  return _rsArray.size() -1 ; // PRQA S 3000, 3010, 3084
}
	

/** the given db is cleared and no error is returned even referring to wrong db number */
inline int rmDeleteDb(unsigned int db) // PRQA S 2134
{ 
  ThreadSafe::Lock lock(_rmMutex) ;
  if (db < _rsArray.size()) {
		_rsArray[db].clear() ;
  }
  return 0 ;
}


typedef enum {
	rmUppercase, /** not supported any longer - kept for compatibility */
	rmLowercase, /** not supported any longer - kept for compatibility */
	rmNoEncoding
} rmEncodeModeT ;

/** read the file, please note that rmUppercase and rmLowercase are not supported and 
	 the boolean merge will only be used to support  rsResourceSet::ModeChange or rsResourceSet::ModeError */
inline int rmRead(int *linerr,   // PRQA S 2134, 4020
                 int db, 
                 const char *file, 
                 const char *space, 
                 rmEncodeModeT, 
                 bool merge, bool)
{
  ThreadSafe::Lock lock(_rmMutex) ;
  if (db >= int(_rsArray.size())) { // PRQA S 3081
	ACS_LOG_ERROR("RmLegacy::rmRead db# " << db << " out of range [0," << _rsArray.size() << ")") ;
	return -1 ;
  }

  std::string path = (space ? space : "" ) ; // PRQA S 3380, 3384
  path += (not path.empty() ? "/" : "") ; // PRQA S 3380
  path += (file ? file : "" ) ; // PRQA S 3380

  std::ifstream input(path.c_str()) ;

  try {
	_rsArray[db].mergeMode(merge ? rsResourceSet::ModeChange : rsResourceSet::ModeError) ; // PRQA S 3000, 3380
	input >> _rsArray[db] ; // PRQA S 3000
  }
  catch (std::exception &x) {
	ACS_LOG_NOTIFY_EX(x) ;
	return -1 ;
  }

  return 0 ;
}


typedef enum {
	rmInt,
	rmFloat,
	rmDouble,
	rmChar,
	rmString,
	rmIntArray,
	rmFloatArray,
	rmDoubleArray,
	rmCharArray,
	rmMaxType
} rmTypeT ;




/** returns 0 if successfull */
inline int rmGetValue(int db, const char *k, rmTypeT t, void* p) // PRQA S 2134, 4020
{
	ThreadSafe::Lock lock(_rmMutex) ;
  if (db >= int(_rsArray.size())) { // PRQA S 3081
	ACS_LOG_ERROR("RmLegacy::rmGetValue db# " << db << " out of range [0," << _rsArray.size() << ")") ;
	return -1 ;
  }
  std::string key = (k?k:"") ; // PRQA S 3380, 3384
  try {
	  switch(t) {
	  case rmInt:
        {
		  _rsArray[db].getValue(key, *static_cast<int *>(p)) ; // PRQA S 3000, 3081
		  break ;
        }
	  case rmFloat:
        {
		  _rsArray[db].getValue(key, *static_cast<float *>(p)) ; // PRQA S 3000, 3081
		  break ;
        }
	  case rmDouble:
        {
		  _rsArray[db].getValue(key, *static_cast<double *>(p)) ; // PRQA S 3000, 3081
		  break ;
        }
	  case rmChar:
        {
		  _rsArray[db].getValue(key, *static_cast<char *>(p)) ; // PRQA S 3000, 3081
		  break ;
        }
	  case rmString:
        {
		  if (p) {
			std::string s ;
			_rsArray[db].getValue(key, s) ; // PRQA S 3000
			strcpy(static_cast<char *>(p),s.c_str()) ; // PRQA S 3081
		  }
		  else {
			ACS_LOG_ERROR("RmLegacy::rmGetValue(rmString) null buffer value " << int(t)) ; // PRQA S 3081
			return -1 ;
		  }
		  break ;
        }
	  default:
        {
		  ACS_LOG_ERROR("RmLegacy::rmGetValue type value not allowed:" << int(t)) ; // PRQA S 3081
		  return -1 ;
        }
	  }
  }
	catch (std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
		return -1 ;
	}

	return 0 ;
}



_ACS_END_NAMESPACE

using namespace RmLegacy ;

#endif /* ifdef sgi */

#endif /* !__LIBRMLEGACY_H__ */
