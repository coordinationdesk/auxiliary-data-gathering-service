// PRQA S 1050 EOF
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
	Revision 1.9  2014/09/10 09:31:27  lucio.pulvirenti
	S1PDGS-31530: New format of Sea ICE products in NetCDF format managed. New exMacros used.
	
	Revision 1.8  2014/09/09 17:08:48  lucio.pulvirenti
	S1PDGS-31530: work in progress.
	
	Revision 1.7  2013/04/29 16:29:27  marpas
	base class interface changed
	
	Revision 1.6  2013/02/22 14:37:07  lucio.pulvirenti
	dbConnection parameter added to prepareMetadata,getSatelliteIdFromSatelliteName,computeMetadata methods.
	
	Revision 1.5  2012/12/07 17:09:26  lucio.pulvirenti
	doPackaging method IF changed in the base class.
	
	Revision 1.4  2012/10/29 13:24:08  giufor
	implemented getTypeNetCDF method
	
	Revision 1.3  2012/10/25 13:53:11  giufor
	implemented readNETCDFFile method
	
	Revision 1.2  2012/10/23 16:38:35  giufor
	implemented getDeltaValidities, getFileNameRegexps, getValidityTime methods
	
	Revision 1.1.1.1  2012/10/22 10:25:47  giufor
	Import libNETCDFPreInventory
	
        
*/

#ifndef _NETCDFPreInventory_H_
#define _NETCDFPreInventory_H_



#include <exException.h>
#include <PreInventoryBase.h>
#include <Metadata.h>



namespace acs {

	/**
	* \brief
	* PreInventory plugin for NETCDF products
	**/

	class NETCDFPreInventory : public PreInventoryBase
	{ 
		public:

			/** NETCDFPreInventory Exceptions */
			exDECLARE_EXCEPTION(exNETCDFPreInventoryException, exException) ; // Base NETCDFPreInventory Exception.
			exDECLARE_EXCEPTION(exNETCDFPreInventoryCriticalException, exNETCDFPreInventoryException) ; // Critical Exception.


			/** Default Class constructor */
			NETCDFPreInventory();
			/** Destructor */
			virtual ~NETCDFPreInventory() throw();

			virtual std::string getClassId();


		private:
			 // copy constructor and operator= defined but not implemented
			/** Copy Constructor */
			NETCDFPreInventory(const NETCDFPreInventory & );
			/** Operator = */
			NETCDFPreInventory &operator=(const NETCDFPreInventory &);
			
			enum NetCDFType { NONE, ICE, NEWICE, WAVE };
			
			struct NETCDFStruct
                        {
                                NETCDFStruct(): CreationDate(""), Area(""), SouthernLat("0"), NorthernLat("0"), WesternLon("0"), EasternLon("0"), 
						StartDate(""), StartTime(""), StopDate(""), StopTime(""), Institution(""), OperationalStatus("") {}

                                std::string CreationDate;
				std::string Area;
				std::string SouthernLat;
				std::string NorthernLat;
				std::string WesternLon;
				std::string EasternLon;
				std::string StartDate;
				std::string StartTime;
				std::string StopDate;
				std::string StopTime;
				std::string Institution;
				std::string OperationalStatus;
                        };
			
			
			
			//extracts information from NetCDF file 
			void readNETCDFFile( const std::string& pathFileName, NETCDFStruct& netcdfs);
			
			std::string getUniqueId();
			//std::string getParametersFile();
			std::string getFormattedVersion( const std::string& version);
			
			Metadata prepareMetadata( const std::string& fileType, const NETCDFStruct& netcdfs, dbConnection &);

			
			int getSatelliteIdFromSatelliteName( const std::string& satName, dbConnection &);
			void getDeltaValidities( int& iceDelta, int& waveDelta);
			void getFileNameRegexps( std::string& regExpICE, std::string& regExpNewICE, std::string& regExpWAVE);
			void getValidityTime( std::string& validityStart, std::string& validityStop);
			void getTypeNetCDF( const std::string& fileName );
			
			std::string getNetCDFTypeStr( NetCDFType );
			

		protected:

            virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection & );
            virtual void updateProductAndMetadata( const std::string& newVersion, Metadata& metadata);
 			/**
			 * \brief  Do nothing.
			 * \param Metadata obj
			 * \param File full path on disk of file to inventory
			 * \param File type
			 * \param Empty string; if valorized exception is thrown
			 * \retval The full path on disk to inventory
			 */    	
    		virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection &  );

		private:
                        //classe di Test CPPUnit
#ifdef CLASS_UNDER_TEST
                        friend class NETCDFPreInventoryTest;
#endif //CLASS_UNDER_TEST

		private:
			std::string _currentFileName;
			std::pair<NetCDFType, std::string> _netCDFcontentId;
			static const size_t _ICERegExpValStartInxDefault = 2;
			static const size_t _NewICERegExpValStartInxDefault = 2;
			static const size_t _WAVERegExpValStartInxDefault = 1;
		
			ACS_CLASS_DECLARE_DEBUG_LEVEL(NETCDFPreInventory);

	};

}

#endif //_NETCDFPreInventory_H_

