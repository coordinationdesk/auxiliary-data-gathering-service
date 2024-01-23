/*

	Copyright 2011, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 1.6  2011/09/12 12:12:29  marpas
	write() signature changed - still compatible with previous version.
	Now it can overwrite an existing file
	
	Revision 1.5  2011/09/12 09:39:50  marpas
	size() method fixed
	
	Revision 1.4  2011/09/12 09:03:16  marpas
	size method added
	
	Revision 1.3  2011/09/07 15:57:31  marpas
	interface improved: constness and throw declaration where needed and perfo for methods returning structures
	
	Revision 1.2  2011/09/02 08:40:04  giucas
	Added dump method for L0PDataFile
	
	Revision 1.1  2011/09/02 07:49:43  giucas
	Imported
	

*/

#ifndef _L0POrder_H_
#define _L0POrder_H_ 

#include <acs_c++config.hpp>
#include <DateTime.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief Helper class used to read/write the L0 Processing Order XML file.
 *		  This file is intented as the interface between the Ingestion function (DirectDataCapture)
 *		  and the processing infrastructure in order to make available raw data files for the L0 Processing.
 */
class L0POrder
{

public:

	/**
	 * \brief Container class used to store information for each L0P Data file
	 */
	class  L0PDataFile
	{
		public:

			/**
			 * \brief  Default Class constructor.
			 */
			L0PDataFile();

			/**
			 * \brief  Class destructor 
			 */
			~L0PDataFile() throw() ;

			/**
			 * \brief  Class copy constructor 
			 *    
			 */
			L0PDataFile(const L0PDataFile & ); 

			/**
			 * \brief  Operator= 
			 */
			L0PDataFile &operator=(const L0PDataFile &);

			/**
			 * \brief Return the file name
			 */
			const std::string & getFileName() const throw();

			/**
			 * \brief Set the file name
			 * \param fileName The file name
			 */
			void setFileName(const std::string& fileName) ;

			/**
			 * \brief Return the file Validity start
			 */
			const DateTime &getValidityStart() const throw() ;

			/**
			 * \brief Set the file Validity start
			 * \param valStart The Validity start
			 */
			void setValidityStart(const DateTime& valStart) ;

			/**
			 * \brief Return the file Validity stop
			 */
			const DateTime &getValidityStop() const throw() ;


			/**
			 * \brief Set the file Validity stop
			 * \param valStop The Validity stop
			 */
			void setValidityStop(const DateTime& valStop) ;

			/**
			 * \brief Return the orbit number
			 */
			unsigned int getOrbit() const throw() ;

			/**
			 * \brief Set the orbit number
			 * \param orbit The orbit number
			 */
			void setOrbit(unsigned int orbit) throw() ;

			/**
			 * \brief Return the file type
			 */
			const std::string &getFileType() const throw() ;

			/**
			 * \brief Set the file type
			 * \param fileType The file type
			 */
			void setFileType(const std::string& fileType) ;

			/**
			 * \brief Return the sensor id number
			 */
			unsigned int getSensorId() const throw() ;

			/**
			 * \brief Set the sensor id number
			 * \param sId The sensor id number as into the database
			 */
			void setSensorId(unsigned int sId) throw() ;

			/**
			 * \brief Return the acquisition station id number
			 */
			unsigned int getAcqStationId() const throw() ;

			/**
			 * \brief Set the acquisition station id number
			 * \param aId The acquisition station id number as into the database
			 */
			void setAcqStationId(unsigned int aId) throw() ;

			/**
			 * \brief Return the processing station id number
			 */
			unsigned int getProcStationId() const throw() ;

			/**
			 * \brief Set the processing station id number
			 * \param pId The processing station id number as into the database
			 */
			void setProcStationId(unsigned int pId) throw() ;

			/**
			 * \brief Return the satellite id number
			 */
			unsigned int getSatelliteId() const throw() ;

			/**
			 * \brief Set the satellite id number
			 * \param satId The satellite id number as into the database
			 */
			void setSatelliteId(unsigned int satId) throw() ;

			/**
			 * \brief Return the mission number
			 */
			unsigned int getMission() const throw() ;

			/**
			 * \brief Set the mission number
			 * \param mission The mission number
			 */
			void setMission(unsigned int mission) throw() ;

			/**
			 * \brief Dump classs content
			 * \retval A string specifying attributes content
			 */
			std::string dump() ;

		private :

			std::string 	_fileName;
			DateTime 		_startTime;
			DateTime 		_stopTime;
			unsigned int	_orbit ;
			std::string 	_fileType;
			unsigned int	_sensorId ;
			unsigned int	_acqStationId ;
			unsigned int	_procStationId ;
			unsigned int	_satelliteId ;
			unsigned int	_mission ;
	} ;


public:

	/**
	 * \brief  Default Class constructor.
	 */
	L0POrder();

	/**
	 * \brief  Class destructor 
	 */
    ~L0POrder() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	L0POrder(const L0POrder & ); 

	/**
	 * \brief  Operator= 
	 */
	L0POrder &operator=(const L0POrder &) ;

	/**
	 * \brief Used to reset the internal data for previously added/read data files
	 */
	void clear() ;

	/**
	 * \brief number of data file contained
	 */
    size_t size() const throw() ;

//////////////////////////////////////////////////////////////////
// 							read/get methods
//////////////////////////////////////////////////////////////////

	/**
	 * \brief Read the L0 Processing  order XML file
	 * \param fileName The file full path
	 * \retval The number of L0P data files read
	 */
	unsigned int read(const std::string& fileName) ;


	/**
	 * \brief Return the data file information inside L0PDataFile containers
	 * \param result The vector of L0PDataFile containers filled
	 */
	void getDataFiles(std::vector<L0POrder::L0PDataFile>& result) const ;

//////////////////////////////////////////////////////////////////
// 							set/write methods
//////////////////////////////////////////////////////////////////

	/**
	 * \brief Add a single file information to the L0POrder
	 * \param dataFile The container class filled to be added
	 */
	void addDataFile(const L0POrder::L0PDataFile& dataFile) ;

	/**
	 * \brief Write the L0 Processing order file on the specified path
	 * \param fileName The L0 Processing order file full path
	 */
	void write(const std::string& fileName, bool overwrite = false) const ;

private:

	void getTime(const std::string& valTime, DateTime& dt) ;

	std::vector<L0POrder::L0PDataFile>	_dataFiles ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(L0POrder) ;

};

_ACS_END_NAMESPACE

#endif //_L0POrder_H_
