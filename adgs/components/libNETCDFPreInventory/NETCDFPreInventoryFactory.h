/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libNETCDFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/10/22 10:25:47  giufor
	Import libNETCDFPreInventory
	
	
        
*/

#ifndef _NETCDFPreInventoryFactory_H_
#define _NETCDFPreInventoryFactory_H_

#include <PreInventoryFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


namespace acs {

	/**
	* \brief
	* NETCDF PreInventory builder 
	**/

	class NETCDFPreInventoryFactory : StringKeyFactory<PreInventoryBase>
	{
	 
		public:

			/** NETCDFPreInventoryFactory Exceptions */
			exDECLARE_EXCEPTION(exNETCDFPreInventoryFactoryException, exException) ; // Base NETCDFPreInventoryFactory Exception.
			exDECLARE_EXCEPTION(exNETCDFPreInventoryFactoryCriticalException, exNETCDFPreInventoryFactoryException) ; // Critical Exception.


			/** Default Class constructor */
			NETCDFPreInventoryFactory();
			/** Destructor */
			virtual ~NETCDFPreInventoryFactory() throw() ;

			static acs::PreInventoryBase* buildObject();

		private:  // copy constructor and operator= defined but not implemented
			/** Copy Constructor */
			NETCDFPreInventoryFactory(const NETCDFPreInventoryFactory & );
			/** Operator = */
			NETCDFPreInventoryFactory &operator=(const NETCDFPreInventoryFactory &);
	

		protected:

		private:
			ACS_CLASS_DECLARE_DEBUG_LEVEL(NETCDFPreInventoryFactory);

	};

}

#endif //_NETCDFPreInventoryFactory_H_

