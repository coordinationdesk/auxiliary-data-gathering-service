// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Product Type Descriptor Library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.1  2010/09/03 09:59:42  marpas
	removed useless or redundant methods
	In the header file there are suggestions how to replace them
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	

*/

#ifndef _L0Utils_H_
#define _L0Utils_H_ 

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>
#include <DateTime.h>
#include <unistd.h>

_ACS_BEGIN_NAMESPACE(acs)

/*
 * This class is a "utility" class that defines static methods useful for
 * many classes in L0PreInventory architecture. It defines methods for
 * ad hoc time conversion, remote machine operations like remote copy and
 * ping etc.
 */

class L0Utils
{
public:

/////////////////////////////////////////////		
//public static methods
/////////////////////////////////////////////

	/*
 	 * \brief Execute a remote copy of a file
	 * \param The remote file name to copy
	 * \param The remote address machine 
	 * \param The destination path
	 * \param The number of maximum tryes (in case of failure -> 0 means no retry)
 	 */
	static int copyFromRemoteDIS(const std::string&, const std::string&, const std::string&, const int retry=0);

	/*
 	 * \brief Check environment settings and return true if SSH usage is required
 	 */
	static bool useSSH();

	/*
 	 * \brief Execute a ping of a remote machine. Return 1 if remote DIS responds or -1 if not 
	 * \param The remote address machine
 	 */
	static int pingRemoteDIS(const std::string&);

	/*
 	 * \brief Convert Product Header Time format in XML Product Header time format.
	 * \param The std::string representing Product Header time.
 	 */
	static std::string convertTime(const std::string&);

	/*
 	 * \brief Create the DIS Synchronization file and lock it
	 * \param The file descriptor
	 * \param The file name
	 * \retval True if the file has been succesfully created and locked, false otherwise
 	 */
	static bool createDISLockingFile(int&, const std::string&);

	/*
 	 * \brief Create the DIS Synchronization file and lock it
	 * \param The file path
	 * \param The file name
	 * \retval The difference expressed in minutes between the current time and the DIS Synchronization file time
 	 */
	static long double checkDISLockingFileDate(const std::string&, const std::string&);


private:

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	L0Utils();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	~L0Utils();	

	// copy constructor 
	L0Utils(const L0Utils & ); 

	// operator =
	L0Utils &operator=(const L0Utils &);


	ACS_CLASS_DECLARE_DEBUG_LEVEL(L0Utils) ;

};
_ACS_END_NAMESPACE

#endif //_L0Utils_H_
