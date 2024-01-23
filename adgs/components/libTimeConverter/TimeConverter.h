// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

    $Log$
    Revision 2.4  2014/04/08 13:05:49  marpas
    coding best practices applied
    interface rationalization in progress.

    Revision 2.3  2014/03/18 12:08:34  marpas
    Coding best practices applied
    warnings removed (g++ & qa)
    interface rationalization
 
    
*/

#ifndef _TimeConverter_H_
#define _TimeConverter_H_ 


#include <acs_c++config.hpp>
#include <exException.h>
#include <TransportTime.h>
#include <ProcessingTime.h>
#include <AsciiTime.h>
#include <OnBoardTime.h>
#include <TimeFormat.h>


_ACS_BEGIN_NAMESPACE(acs)

	
class TimeConverter 
{
public:


	/*! class TimeConverterException declaration */
	exDECLARE_EXCEPTION(TimeConverterException,exException) ; // PRQA S 2131, 2502

	/*! class TimeConverterInitException declaration */
	exDECLARE_EXCEPTION(TimeConverterInitException,exException) ; // PRQA S 2131, 2502


	/*! class TimeConverterLibraryInvalidInterval declaration */
	exDECLARE_EXCEPTION(TimeConverterLibraryInvalidInterval,exException) ; // PRQA S 2131, 2502


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	TimeConverter();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~TimeConverter() throw() ;	    

	/**
	 * \brief Initialize the object with info read from a configuration file
	 *	      A TimeConverterInitException Exception is thrown if the library
	 *		  has been initialized yet.
	 * \param The configuration filename
	 * \param The Time Model
	 * \param The time interval time0 and time1 expressed in double format
	 */
	virtual void initFromFile(const std::string&, TimeFormat::TimeModel,long double, long double)=0;

	/**
	 * \brief Initialize the object given a Time Model and a time interval.
	 *		  The configuration file corresponding to the given Time Model is downloaded
	 *		  from the database. It must cover completely the requested time interval time1-time0.
	 *		  In case there is more than one file that satisfies this condition, the one with the
	 *		  latest version is retrieved.
	 *		  A getValidationInterval method call is needed in order to retrieve the validation interval value.
	 * \param The TimeModel
	 * \param The time interval time0 and time1 expressed in double format
	 */
	virtual void initFromFile(TimeFormat::TimeModel, long double, long double)=0;

	/**
	 * \brief Initialize the object given a Time Model and a time interval.
	 *		  The configuration file corresponding to the given Time Model is downloaded
	 *		  from the database. It could not cover completely the requested time interval time1-time0.
	 *		  The file that covers the biggest portion of time interval time1-time0 is retrieved.
	 *		  A getValidationInterval method call is needed in order to retrieve the validation interval value.
	 * \param The TimeModel
	 *		  The time interval time0 and time1 expressed in double format
	 */
	virtual void initConditioned(TimeFormat::TimeModel, long double, long double)=0;


	/**
	 * \brief Initialize the object for On Board Time Conversion.
	 *		  The UTC/SBT Time Conversion File which Validity Start Time is nearer to the given Start Time is downloaded
	 *		  from the database, is parsed and the needed information retrieved and stored for
	 *		  further conversions.
	 * \param The time interval Start Time and Stop Time expressed in double format
	 */
	virtual void initOBCTimeConversion(long double, long double)=0;

	/**
	 * \brief Initialize the object for On Board Time Conversion with the UTC/SBT Time Conversion File
	 *		  given in input.
	 * \param The UTC/SBT Time Conversion File name
	 */
	virtual void initOBCTimeConversion(const std::string&)=0;

	/**
	 * \brief Initialize the object for On Board Time Conversion searching the UTC/SBT Time Conversion File
	 *		  in the path specified in input. 
	 * \param The dir where finding the UTC/SBT Time Conversion File
	 * \param The Reference UTC time read from file
	 * \param The Reference SBT time read from file
	 * \param The SBT clock step read from file
	 */
	virtual void initOBCTimeConversion(const std::string&, 
					                  long double&,
					                 std::vector<unsigned long int>&,
					                 unsigned long int&)=0;

	/**
	 * \brief Return the validation time interval as Start and Stop time.
	 * \param double start and stop time
	 *		  
	 */
	virtual void getValidationInterval(long double&, long double&)=0;

	/**
	 * \brief Set the Satellite ID value.By default the Satellite ID value is TimeFormat::TC_SAT_DEFAULT.
	 * \param TimeFormat::SatelliteID enum value
	 * \param Mission (default is 1)		  
	 */
	virtual void setSatelliteID(TimeFormat::SatelliteID, int=1) = 0 ;

	/**
	 * \brief Set the Satellite ID value.
	 * \param SatelliteID in string format (i.e. Cryosat for CRYOSAT or Envisat for ENVISAT)
	 * \param Mission (default is 1)		  
	 */
	virtual void setSatelliteID(const std::string&, int=1) = 0;

	/**
	 * \brief Return the TimeModel used to initialize the Library.
	 *		  
	 */
	virtual TimeFormat::TimeModel getTimeModel() const =0;

	/**
	 * \brief Return the File Type associated to TimeModel passed as argument
	 *		  
	 */
	virtual std::string getFileType(TimeFormat::TimeModel) const =0;

	/**
	 * \brief Retrieves the leap second information(if any) in the initialized time range.
	 * \param Input AsciiTime object for UTC time before leap. This object must have the reference field set 
	 * 	      to TimeFormat::TC_TIME_UTC and the Format field set as input parameters.
	 * \param Input AsciiTime object for UTC time after leap. This object must have the reference field set 
	 * 	      to TimeFormat::TC_TIME_UTC and the Format field set as input parameters.
	 * \param A long int reference value specifying a flag for leap second presence within
	 *		  time initialisation data. It will be set to:		  
	 *		  -1 (Negative Leap Second) : a leap second has been removed
	 *		  0 : no leap second within initialization data
	 *		  +1 (Positive Leap Second) : a leap second has been added
	 */
	virtual void getLeapSecondInfo(AsciiTime&,AsciiTime&,long int&)=0;

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
	virtual ssize_t getNumberOfLeapSecondsAtDate(const AsciiTime&)=0 ;

	/**
	 * \brief Return the name of the file used to initialize the Library.
	 *		  
	 */
	virtual std::string getFileName() const =0;

	/**
	 * \brief Convert a given time in TransportTime format to an output time in
	 *		  TransportTime format.
		 	  <b>The input TransportTime object must contain value, reference and format informations.
	 *		  The output TransportTime object must contain reference and format informations.</b>
	 * \param The input TransportTime object.
	 *		  The output TransportTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output TransportTime object
	 */
	virtual TransportTime& convert(const TransportTime&, TransportTime&)=0;

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
	virtual ProcessingTime& convert(const TransportTime&, ProcessingTime&)=0;

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
	virtual AsciiTime& convert(const TransportTime&, AsciiTime&)=0;

	/**
	 * \brief Convert a given time in ProcessingTime format to an output time in
	 *		  TransportTime format.
		 	  <b>The input ProcessingTime object must contain value, reference and format informations.
	 *		  The output TransportTime object must contain reference and format informations.</b>
	 * \param The input ProcessingTime object.
	 *		  The output TransportTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output TransportTime object
	 */
	virtual TransportTime& convert(const ProcessingTime&, TransportTime&)=0;

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
	virtual ProcessingTime& convert(const ProcessingTime&, ProcessingTime&)=0;

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
	virtual AsciiTime& convert(const ProcessingTime&, AsciiTime&)=0;

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
	virtual TransportTime& convert(const AsciiTime&, TransportTime&)=0;

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
	virtual ProcessingTime& convert(const AsciiTime&, ProcessingTime&)=0;

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
	virtual AsciiTime& convert(const AsciiTime&, AsciiTime&)=0;

	/**
	 * \brief Convert a given time in OnBoardTime format to an output time in
	 *		  ProcessingTime format.
	 *		  <b>The input OnBoardTime object must contain counter informations.
	 *		  The output ProcessingTime object must contain reference and format informations.</b>
	 * \param The input OnBoardTime object.
	 * \param The output ProcessingTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output ProcessingTime object
	 */
	virtual ProcessingTime& convert(const OnBoardTime&, ProcessingTime&)=0;

	/**
	 * \brief Convert a given time in ProcessingTime format to an output time in
	 *		  OnBoardTime format.
	 * \param The input ProcessingTime object.
	 * \param The output OnBoardTime object.
	 * \param The pid of the process/thread using the object
	 * \retval A reference to the output OnBoardTime object
	 */
	virtual OnBoardTime& convert(const ProcessingTime&, OnBoardTime&)=0;


private:


	TimeConverter(const TimeConverter & ); // not implemented
	TimeConverter &operator=(const TimeConverter &); // not implemented


	ACS_CLASS_DECLARE_DEBUG_LEVEL(TimeConverter) ;

};
_ACS_END_NAMESPACE

#endif //_TimeConverter_H_
