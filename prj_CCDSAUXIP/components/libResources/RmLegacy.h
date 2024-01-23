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


acs::ThreadSafe &mutex() 
{
    static acs::ThreadSafe m ;
    static acs::ThreadSafe *mutex_ = nullptr ;
    if (!mutex_) { 
        acs::ThreadSafe::Lock lock(m) ;
        if (!mutex_) {
            mutex_ = new acs::ThreadSafe ; // NOSONAR
        }
    }
    return *mutex_ ;
}

std::vector<acs::rsResourceSet> & rsArray() 
{
    static acs::ThreadSafe m ;
    static std::vector<acs::rsResourceSet> *rsArray_ ;
    if (!rsArray_) { 
        acs::ThreadSafe::Lock lock(m) ;
        if (!rsArray_) {
            rsArray_ = new std::vector<acs::rsResourceSet> ; // NOSONAR
        }
    }
    return *rsArray_ ;
}


/** the returned value is the numebr of db created to which you need to refer to for any other operation. 
	In case of error it will be < 0 and non db are created */
inline int rmCreateDb(const char *) { // PRQA S 2134
  acs::rsResourceSet a ;
  acs::ThreadSafe::Lock lock(mutex()) ;
  rsArray().push_back(a) ;
  return rsArray().size() -1 ; // PRQA S 3000, 3010, 3084
}
	

/** the given db is cleared and no error is returned even referring to wrong db number */
inline int rmDeleteDb(unsigned int db) // PRQA S 2134
{ 
  acs::ThreadSafe::Lock lock(mutex()) ;
  if (db < rsArray().size()) {
		rsArray()[db].clear() ;
  }
  return 0 ;
}


typedef enum { // NOSONAR
	rmUppercase, /** not supported any longer - kept for compatibility */
	rmLowercase, /** not supported any longer - kept for compatibility */
	rmNoEncoding
} rmEncodeModeT ; // NOSONAR

/** read the file, please note that rmUppercase and rmLowercase are not supported and 
	 the boolean merge will only be used to support  rsResourceSet::ModeChange or rsResourceSet::ModeError */
inline int rmRead(int *,   // NOSONAR
                 int db, 
                 const char *file, 
                 const char *space, 
                 rmEncodeModeT, 
                 bool merge, bool)
{
  acs::ThreadSafe::Lock lock(mutex()) ;
  if (db >= int(rsArray().size())) { // PRQA S 3081
	ACS_LOG_ERROR("RmLegacy::rmRead db# " << db << " out of range [0," << rsArray().size() << ")") ;
	return -1 ;
  }

  std::string path = (space ? space : "" ) ; // PRQA S 3380, 3384
  path += (! path.empty() ? "/" : "") ; // PRQA S 3380
  path += (file ? file : "" ) ; // PRQA S 3380

  std::ifstream input(path.c_str()) ;

  try {
	rsArray()[db].mergeMode(merge ? acs::rsResourceSet::ModeChange : acs::rsResourceSet::ModeError) ; // PRQA S 3000, 3380
	input >> rsArray()[db] ; // PRQA S 3000
  }
  catch (std::exception &x) { // NOSONAR - legacy C callable: no exceptions from here
	ACS_LOG_NOTIFY_EX(x) ;
	return -1 ;
  }

  return 0 ;
}


typedef enum { // NOSONAR
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
} rmTypeT ; // NOSONAR




/** returns 0 if successfull */
inline int rmGetValue(int db, const char *k, rmTypeT t, void* p) // NOSONAR - legacy "any type"
{
  acs::ThreadSafe::Lock lock(mutex()) ;
  if (db >= int(rsArray().size())) { // PRQA S 3081
	ACS_LOG_ERROR("RmLegacy::rmGetValue db# " << db << " out of range [0," << rsArray().size() << ")") ;
	return -1 ;
  }
  std::string key = (k?k:"") ; // PRQA S 3380, 3384
  try {
	switch(t) {
	case rmInt:
		rsArray()[db].getValue(key, *static_cast<int *>(p)) ; // PRQA S 3000, 3081
		break ;
	case rmFloat:
		rsArray()[db].getValue(key, *static_cast<float *>(p)) ; // PRQA S 3000, 3081
		break ;
	case rmDouble:
		rsArray()[db].getValue(key, *static_cast<double *>(p)) ; // PRQA S 3000, 3081
		break ;
	case rmChar:
		rsArray()[db].getValue(key, *static_cast<char *>(p)) ; // PRQA S 3000, 3081
		break ;
	case rmString:
		if (p) {
		  std::string s ;
		  rsArray()[db].getValue(key, s) ; // PRQA S 3000
		  strcpy(static_cast<char *>(p),s.c_str()) ; // PRQA S 3081
		}
		else {
		  ACS_LOG_ERROR("RmLegacy::rmGetValue(rmString) null buffer value " << int(t)) ; // PRQA S 3081
		  return -1 ;
		}
		break ;
	default:
		ACS_LOG_ERROR("RmLegacy::rmGetValue type value not allowed:" << int(t)) ; // PRQA S 3081
		return -1 ;
	}
  }
  catch (const std::exception &x) { // NOSONAR - C callabale: no exceptions from here
      ACS_LOG_NOTIFY_EX(x) ;
	  return -1 ;
  }

  return 0 ;
}



_ACS_END_NAMESPACE

using namespace RmLegacy ; // NOSONAR : allow segregation of legacy code

#endif /* !__LIBRMLEGACY_H__ */
