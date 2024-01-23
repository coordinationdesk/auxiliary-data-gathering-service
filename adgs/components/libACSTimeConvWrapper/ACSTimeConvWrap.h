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
  
   $Author$
  
   @(#) A.C.S.: $Id$

   $Log$
   Revision 2.6  2014/04/14 14:01:19  marpas
   using new libACSTimeConverter 2.14
   interface and code rationalization

   Revision 2.5  2014/04/08 12:29:12  marpas
   connection pool is now got from application.
   In case app does not exists or still has no pool, a default pool is asked - this could compromise multidatabase application.

   Revision 2.4  2014/02/03 15:31:04  giucas
   New macros adopted

   Revision 2.3  2013/03/12 17:54:18  marpas
   dbConnectionPool is mandatory in file actions

   Revision 2.2  2007/11/30 14:13:08  giucas
   Checking the mission when retrieving the configuration

   Revision 2.1  2007/01/10 11:22:57  giucas
   Updated for IF changes (CFI 3.5 porting)

   Revision 2.0  2006/03/01 16:39:23  serfol
   Exception 2.0 I/F adapted

   Revision 1.14  2006/01/31 14:03:34  seralb
   Added AuxDownloaderSingleton

   Revision 1.13  2005/11/21 11:12:53  giucas
   setSatellite methods changed.Implemented initOBTTimeConversion and getFileType methods

   Revision 1.12  2005/11/03 13:44:31  seralb
   Modify setSatelliteId and initOBTConverter

   Revision 1.11  2005/09/16 11:44:47  seralb
   Now parser on orbit data instead of OBT class

   Revision 1.10  2005/09/12 14:49:24  seralb
   Added debug

   Revision 1.9  2005/07/08 11:37:41  seralb
   initOBCTimeConversion method now multisatellite

   Revision 1.8  2005/07/06 15:06:49  seralb
   Added methods

   Revision 1.7  2005/07/04 09:14:17  seralb
   Removed some methods

   Revision 1.6  2005/04/22 14:50:59  giucas
   Moved includes from .h to .C

   Revision 1.5  2005/04/21 10:44:18  giucas
   Removed dependencies from ACE. Using ThreadSafe class instead.

 
*/


#ifndef _ACSTimeConvWrap_H_
#define _ACSTimeConvWrap_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <TimeConverter.h>
#include <ACSTimeConv.h>
#include <OBTConverter.h>

_ACS_BEGIN_NAMESPACE(acs)

class AuxDownloader ;	
class dbConnectionPool ;
class ACSTimeConvWrap : public TimeConverter // PRQA S 2109
{
public:		

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	ACSTimeConvWrap();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~ACSTimeConvWrap() throw() ;


	/**
	 * \brief Method not implemented
	 */
	virtual void initFromFile(const std::string&,TimeFormat::TimeModel, long double, long double);

	/**
	 * \brief Method not implemented
	 */
	virtual void initFromFile(TimeFormat::TimeModel, long double, long double);

	/**
	 * \brief Method not implemented
	 */
	virtual void initConditioned(TimeFormat::TimeModel, long double, long double);		

	/**
	 * \brief Method not implemented
	 */
	virtual void initOBCTimeConversion(long double, long double);

	/**
	 * \brief set the parameters to perform the OBCTime conversion
	 * \param the OBCTime filename
	 */
	virtual void initOBCTimeConversion(const std::string&);

	/**
	 * \brief Method not implemented
	 */
	virtual void initOBCTimeConversion(const std::string&, long double &, std::vector<unsigned long int>&, unsigned long int&);

	/**
	 * \brief Method not implemented
	 */
	virtual void getValidationInterval(long double& start, long double& stop);

	/**
	 * \brief Method not implemented
	 */
	virtual void setSatelliteID(TimeFormat::SatelliteID, int=1);

	/**
	 * \brief set the satellite name
	 * \param the satellite name ane the mission id
	 */
	virtual void setSatelliteID(const std::string&, int=1);

	/**
	 * \brief Method not implemented
	 */
	virtual TimeFormat::TimeModel getTimeModel() const { return _configuredTimeModel; } // PRQA S 2131

	/**
	 * \brief Method not implemented
	 */
	virtual std::string getFileType(TimeFormat::TimeModel) const ;

	/**
	 * \brief Method not implemented
	 */
	virtual void getLeapSecondInfo(AsciiTime&,AsciiTime&,long int&);

	/**
	 * \brief Retrieves the number of leap second (defined as delta, TAI-UTC difference) at a specific date
	 *        This information can be used as a base for several other computations, e.g. the GPS leap seconds
	 *        or the leap seconds relative to a different offset (e.g. relative to the 2000-01-01 time reference)
	 *        Example:
	 *        On 2012-07-01 00:00:00 delta = 35
	 *        On 2012-06-30 23:59:59 delta = 34
	 *        (Since in 2012 a leap second was added on 2012-06-30 23:59:60)
	 * \param The date used to perform the computation, as double
     *
	 */
	virtual ssize_t getNumberOfLeapSecondsAtDate(const AsciiTime&) ; // FIXME IT SHOULD BE =0 ;

	/**
	 * \brief Method not implemented
	 */
	virtual std::string getFileName() const { return _fileName; } // PRQA S 2131


	/**
	 * \brief Convert a given time in TransportTime format to an output time in
	 *		  TransportTime format.
	 *		  <b>The input TransportTime object must contain value, reference and format informations.
	 *		  The output TransportTime object must contain reference and format informations.</b>
	 * \param The input TransportTime object.
	 *		  The output TransportTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output TransportTime object
	 */
	virtual TransportTime& convert(const TransportTime&, TransportTime&);

	/**
	 * \brief Convert a given time in TransportTime format to an output time in
	 *		  ProcessingTime format.
	 *		  <b>The input TransportTime object must contain value, reference and format informations.
	 *		  The output ProcessingTime object must contain reference and format informations.</b>
	 * \param The input TransportTime object.
	 *		  The output ProcessingTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output ProcessingTime object
	 */
	virtual ProcessingTime& convert(const TransportTime&, ProcessingTime&);

	/**
	 * \brief Convert a given time in TransportTime format to an output time in
	 *		  AsciiTime format.
	 *		  <b>The input TransportTime object must contain value, reference and format informations.
	 *		  The output AsciiTime object must contain reference and format informations.</b>
	 * \param The input TransportTime object.
	 *		  The output AsciiTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output AsciiTime object
	 */
	virtual AsciiTime& convert(const TransportTime&, AsciiTime&);

	/**
	 * \brief Convert a given time in ProcessingTime format to an output time in
	 *		  TransportTime format.
	 *		  <b>The input ProcessingTime object must contain value, reference and format informations.
	 *		  The output TransportTime object must contain reference and format informations.</b>
	 * \param The input ProcessingTime object.
	 *		  The output TransportTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output TransportTime object
	 */
	virtual TransportTime& convert(const ProcessingTime&, TransportTime&);

	/**
	 * \brief Convert a given time in ProcessingTime format to an output time in
	 *		  ProcessingTime format.
	 *		  <b>The input ProcessingTime object must contain value, reference and format informations.
	 *		  The output ProcessingTime object must contain reference and format informations.</b>
	 * \param The input ProcessingTime object.
	 *		  The output ProcessingTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output ProcessingTime object
	 */
	virtual ProcessingTime& convert(const ProcessingTime&, ProcessingTime&);

	/**
	 * \brief Convert a given time in ProcessingTime format to an output time in
	 *		  AsciiTime format.
	 *		  <b>The input ProcessingTime object must contain value, reference and format informations.
	 *		  The output AsciiTime object must contain reference and format informations.</b>
	 * \param The input ProcessingTime object.
	 *		  The output AsciiTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output AsciiTime object
	 */
	virtual AsciiTime& convert(const ProcessingTime&, AsciiTime&);

	/**
	 * \brief Convert a given time in AsciiTime format to an output time in
	 *		  TransportTime format.
	 *		  <b>The input AsciiTime object must contain value, reference and format informations.
	 *		  The output TransportTime object must contain reference and format informations.</b>
	 * \param The input AsciiTime object.
	 *		  The output TransportTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output TransportTime object
	 */
	virtual TransportTime& convert(const AsciiTime&, TransportTime&);

	/**
	 * \brief Convert a given time in AsciiTime format to an output time in
	 *		  ProcessingTime format.
	 *		  <b>The input AsciiTime object must contain value, reference and format informations.
	 *		  The output ProcessingTime object must contain reference and format informations.</b>
	 * \param The input AsciiTime object.
	 *		  The output ProcessingTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output ProcessingTime object
	 */
	virtual ProcessingTime& convert(const AsciiTime&, ProcessingTime&);

	/**
	 * \brief Convert a given time in AsciiTime format to an output time in
	 *		  AsciiTime format.
	 *		  <b>The input AsciiTime object must contain value, reference and format informations.
	 *		  The output AsciiTime object must contain reference and format informations.</b>
	 * \param The input AsciiTime object.
	 *		  The output AsciiTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output AsciiTime object
	 */
	virtual AsciiTime& convert(const AsciiTime&, AsciiTime&);


	/**
	 * \brief Convert a given time in OnBoardTime format to an output time in
	 *		  ProcessingTime format.
	 *		  <b>The input OnBoardTime object must contain counter and time informations.
	 *		  The output ProcessingTime object must contain reference and format informations.</b>
	 * \param The input OnBoardTime object.
	 *		  The output ProcessingTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output ProcessingTime object
	 */
	virtual ProcessingTime& convert(const OnBoardTime&, ProcessingTime&);

	/**
	 * \brief Convert a given time in ProcessingTime format to an output time in
	 *		  OnBoardTime format.
	 * \param The input ProcessingTime object.
	 * \param The output OnBoardTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output OnBoardTime object
	 */
	virtual OnBoardTime& convert(const ProcessingTime&, OnBoardTime&);


private:
	ACSTimeConvWrap(const ACSTimeConvWrap & ); 	 // not implemented	
	ACSTimeConvWrap &operator=(const ACSTimeConvWrap &); // not implemented

//////////////////////
//methods
//////////////////////

	ACSTimeConv::TimeFormatDefines convertTransportEnum(TimeFormat::TransportTimeFormat );
	ACSTimeConv::TimeFormatDefines convertProcessingEnum(TimeFormat::ProcessingTimeFormat );
	ACSTimeConv::TimeFormatDefines convertAsciiEnum(TimeFormat::AsciiTimeFormat);
	ACSTimeConv::TimeReferenceDefines convertReferenceEnum(TimeFormat::TimeReference);

	void loadConfiguration(TimeFormat::SatelliteID, int mission);

    // this asks to the app the pool - implies a dependency
    static dbConnectionPool &getPool() ;
//////////////////////
//attributes
//////////////////////
private:
	std::string				_fileName;
	std::string				_satelliteName;
	TimeFormat::TimeModel	_configuredTimeModel;
	OBTConverter*			_obtConverter;	
	OBTFileParser*			_parser;
	acs::ThreadSafe			_token;		

	ACSTimeConv				_acsTimeConv;

	AuxDownloader* 			_downloader;
	std::string				_onBoardTimeFileType;
	std::map<TimeFormat::TimeModel,std::string>	_confFileTypes;
	long int 		_satelliteID;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACSTimeConvWrap) ;

};

_ACS_END_NAMESPACE


#endif //_ACSTimeConvWrap_H_
