// PRQA S 1050 EOF
/*
	Copyright 1995-2010, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDbExpress$

	$Id$

	$Author$

	$Log$
	Revision 3.0  2010/09/10 16:11:59  marpas
	new file dbxpressIf.h with template code to support DbExpress types and I/F
	new documentation
	other improvements in existing files
	
	Revision 2.0  2010/02/19 11:18:57  marpas
	ORACLE dependency from now on
	
	Revision 1.1.1.1  2008/07/21 15:28:10  paoscu
	Importing dbExpress
	
        
*/

#ifndef _DbExpress_H_
#define _DbExpress_H_

#include <dbxpressIf.h>

/*!	\mainpage
	This library is intended to support dbxpress driver framework.
	It provides facilities to deal with dbxpress types.
	Note that dbxpress is an interface framework, its real implementation will be in an appropriate shared object.
	the libDbClient provides all the code to deal with the above mentioned shared object loading and
	discovering of the main hook funcion returning an SQLDriver pointer to start with.
	
	\section introsec Introduction
	The SQLDriver, SQLConnection, SQLCommand, SQLCursor and SQLMetaData classes are all derived from SQLUnknown.
	The latter is an abstract pure class with no contructor or destructor and the only way to deal with is to obtain 
	an initialized pointer from some functions. SQLUnknown has also a reference count implementation whose methods are 
	SQLUnknown::AddRef (to increment the ref count to a specific instance) and SQLUnknown::Release() (to decremnt it).
	The SQLUnknown::Release() method is also responsible to delete the class when the reference count decrements to 0.
	Note that at the beginning, its reference count starts with 1.
	Because is an error prone technique to remember to AddRef or Release when necessary, the boost::auto_ptr class is 
	the best way to deal with the problem, hence a number of typedef and conveninece functions are provided.
	acs#dbSQLDriver, acs#dbSQLConnection, acs#dbSQLCommand, acs#dbSQLCursor, acs#dbSQLMetaData.
*/

namespace acs {
    
    /**
     * \brief
     * 
	 * Dummy Class to generate code 
     **/
     
   class DbExpress  { 
   public:
        
      /** Default Class constructor */
      DbExpress();
      /** Destructor */
      virtual ~DbExpress();

   private:
   
      /** Copy Constructor */
      DbExpress(const DbExpress & );
      /** Operator = */
      DbExpress &operator=(const DbExpress &);
      
   };

}

#endif //_DbExpress_H_

