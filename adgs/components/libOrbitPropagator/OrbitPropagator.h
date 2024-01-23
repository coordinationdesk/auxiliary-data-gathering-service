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
    Revision 2.11  2017/12/21 08:23:52  ricfer
    Added new version of initPropagFromFile (IERS B bulletin passed as input).

    Revision 2.10  2016/03/18 15:36:27  marpas
    I/F robustness improved

    Revision 2.9  2016/03/18 11:45:35  marpas
    interface robustness improved

    Revision 2.8  2014/03/27 12:46:46  marpas
    coding best practices applied
    interface rationalization
    warnings fixed (qa & g++)

    Revision 2.7  2014/03/20 11:16:58  marpas
    best coding practices applied
    warnings fixed (qa & g++)


*/

#ifndef _OrbitPropagator_H_
#define _OrbitPropagator_H_ 


#include <acs_c++config.hpp>
#include <exException.h>

#include <ProcessingTime.h>

#include <AbsOrbitInfo.h>
#include <RelOrbitInfo.h>
#include <StateVector.h>
#include <ExtraInfo.h>
#include <KeplerianElement.h>


_ACS_BEGIN_NAMESPACE(acs)

	
class OrbitPropagator 
{
public:

	/*! class OrbitPropagatorException declaration */
	exDECLARE_EXCEPTION(OrbitPropagatorException, exException) ; // PRQA S 2131, 2502

	/*! class OrbitPropagatorLibraryInitialized declaration */
	exDECLARE_EXCEPTION(OrbitPropagatorLibraryInitialized, exException) ; // PRQA S 2131, 2502


	/*! class OrbitPropagatorLibraryInvalidInterval declaration */
	exDECLARE_EXCEPTION(OrbitPropagatorLibraryInvalidInterval, exException) ; // PRQA S 2131, 2502

	/*! class NoFileReturnedByQuery declaration */
	exDECLARE_EXCEPTION(NoFileReturnedByQuery,exException) ;	 // PRQA S 2131, 2502	

	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	OrbitPropagator();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~OrbitPropagator() throw();

/**
	 * \brief Initialize the object with info read from a configuration file
	 *	      A OrbitPropagatorLibraryInitialized is thrown if the library
	 *		  has just been initialized.
	 * \param The configuration filename
	 * \param The Orbit Scenario filename
	 * \param The IERSB filename
	 * \param The Time Model
	 * \param The initialization requested start time
	 * \param The initialization requested stop time
	 * \param The initialization propagation model
	 * \param The initialization time mode
	 * \param The initialization time reference
	 */
	virtual void initPropagFromFile(const std::string&,
					const std::string&,
					const std::string&, 
					const PropagFormat::TimeModel&, 
					double&, 
					double&,
					int = static_cast<int>(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL), // PRQA S 3081
					PropagFormat::TimeInitMode = PropagFormat::OP_SEL_TIME,
					PropagFormat::TimeReference = PropagFormat::OP_TIME_TAI)=0;
	
	/**
	 * \brief Initialize the object with info read from a configuration file
	 *	      A OrbitPropagatorLibraryInitialized is thrown if the library
	 *		  has just been initialized.
	 * \param The configuration filename
	 * \param The Orbit Scenario filename
	 * \param The Time Model
	 * \param The initialization requested start time
	 * \param The initialization requested stop time
	 * \param The initialization propagation model
	 * \param The initialization time mode
	 * \param The initialization time reference
	 */
	virtual void initPropagFromFile(const std::string&,
					const std::string&, 
					const PropagFormat::TimeModel&, 
					double&, 
					double&,
					int = static_cast<int>(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL), // PRQA S 3081
					PropagFormat::TimeInitMode = PropagFormat::OP_SEL_TIME,
					PropagFormat::TimeReference = PropagFormat::OP_TIME_TAI)=0;


	/**
	 * \brief Initialize the object given a Time Model and a time interval
	 *	      A OrbitPropagatorLibraryInitialized is thrown if the library
	 *		  has just been initialized.
	 * \param The Time Model
	 * \param The initialization requested start time
	 * \param The initialization requested stop time
	 * \param The initialization propagation model
	 * \param The initialization time mode
	 * \param The initialization time reference
	 */
	virtual void initPropagFromFile(const PropagFormat::TimeModel&, 
					double&, 
					double&,
					int = static_cast<int>(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL), // PRQA S 3081
					PropagFormat::TimeInitMode = PropagFormat::OP_SEL_TIME,
					PropagFormat::TimeReference = PropagFormat::OP_TIME_TAI)=0;

	 /**
	 * \brief Initialize Orbit Scenario File for a specific time interval, downloading file from database
	 * \param The time interval time0 and time1 expressed in double format
	 * \retval The OrbitScenario File name
	 */
	virtual std::string initOrbitScenario(double, double)=0;

	 /**
	 * \brief Initialize Orbit Scenario File for a specific file name
	 * \param The Orbit Scenario filename
	 */
	virtual void initOrbitScenario(const std::string&)=0;

	/**
	 * \brief Initialize the object given a Time Model and a time interval.
	 *		  The configuration file corresponding to the given Time Model is downloaded
	 *		  from the database. It could not cover completely the requested time interval time1-time0.
	 *		  The file that covers the biggest portion of time interval time1-time0 is retrieved.
	 *		  A getValidationInterval method call is needed in order to retrieve the validation interval value.
	 * \param The TimeModel
	 * \param The initialization requested start time
	 * \param The initialization requested stop time
	 * \param The initialization propagation model
	 * \param The initialization time mode
	 * \param The initialization time reference
	 */
	virtual void initConditioned(const PropagFormat::TimeModel&, 
					 double&, 
					 double&, 
					 int = static_cast<int>(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL), // PRQA S 3081
					 PropagFormat::TimeInitMode = PropagFormat::OP_SEL_TIME,
					 PropagFormat::TimeReference = PropagFormat::OP_TIME_TAI)=0;


	/**
	 * \brief Return the validation time interval as Start and Stop time.
	 *		  A OrbitPropagatorLibraryInvalidInterval exception is thrown if the
	 *		  initialization has not been performed.
	 * \param The validation start time
	 * \param The validation stop time
	 */
	virtual void getValidationInterval(long double&, long double&)=0;

	/**
	 * \brief Return the validation time interval for propagation routine as Start and Stop time.
	 *		  A OrbitPropagatorLibraryInvalidInterval exception is thrown if the
	 *		  initialization has not been performed.
	 * \param The validation start time
	 * \param The validation stop time
	 */
	virtual void getPropagationValidationInterval(long double&, long double&)=0;

	/**
	 * \brief Set the Satellite ID value.By default the Satellite ID value is PropagFormat::OP_SAT_DEFAULT.
	 * \param PropagFormat::SatelliteID enum value
	 * \param Mission (default is 1)
	 */
	virtual void setSatelliteID(const PropagFormat::SatelliteID&, int=1)=0;

	/**
	 * \brief Set the Satellite ID value.
	 * \param SatelliteID in string format (i.e. Cryosat for CRYOSAT or Envisat for ENVISAT)
	 * \param Mission (default is 1)
	 */
	virtual void setSatelliteID(const std::string&, int=1)=0;

	/**
	 * \brief Return the TimeModel used to initialize the Library.
	 */
	virtual PropagFormat::TimeModel getTimeModel() const =0;

	/**
	 * \brief Return the File Type associated to TimeModel passed as argument
	 * \param The input TimeModel
	 */
	virtual std::string getFileType(PropagFormat::TimeModel) const =0;

	/**
	 * \brief Return the reference system associated to TimeModel passed as argument
	 * \param The input TimeModel
	 */
	virtual PropagFormat::RefCoordinateFrames getRefSystem(const PropagFormat::TimeModel&)=0;


	/**
	 * \brief Return the list of all configured Time Models
	 */
	virtual const std::vector<PropagFormat::TimeModel> &getAllModels() const = 0;



	/**
	 * \brief Return the File Type associated to TimeModel passed as argument
	 *		  
	 */
	virtual std::string getOrbitScenarioFileType() const =0;

	/**
	 * \brief Return the name of the file used to initialize the Library.
	 */
	virtual std::string getFileName() const =0;

	/**
	 * \brief Propag Extra information
	 * \param ExtraInfo The output extra information
	 */
	virtual void propagExtraInfo(ExtraInfo&)=0;

	/**
	 * \brief Convert a processing time in orbit absolute time
	 * \param ProcessingTime input time in processing format.
	 * \param AbsOrbitInfo the output object reference
	 */
	virtual void timeToAbsOrbit(const ProcessingTime&, AbsOrbitInfo&)=0;

	/**
	 * \brief Convert an orbit absolute time into processing time.
	 * \param AbsOrbitInfo input orbit absolute time
	 * \param ProcessingTime the output object reference
	 */
	virtual void absOrbitToTime(const AbsOrbitInfo&, ProcessingTime&)=0;

	/**
	 * \brief Calculates the relative orbit information related to a specific absolute orbit number.
	 * \param The input absolute orbit number
	 * \param The output relative orbit information
	 */
	virtual void relOrbitFromAbs(const long int&, RelOrbitInfo&)=0;

	/**
	 * \brief Calculates the absolute orbit information related to a specific relative orbit information.
	 *		  This method fills the phase and extraInfo fields for the input RelOrbitInfo object and
	 *		  returns also the absolute orbit number in output. 
	 * \param The input relative orbit information
	 * \param The output absolute orbit number
	 */
	virtual void absFromRelOrbit(RelOrbitInfo&, long int&)=0;

	/**
	 * \brief Calculates the relative orbit information related to the first orbit of a phase.
	 * \param The output relative orbit information. <b>This object must have the phase
	 *		  field set as it will be used in input.</b> 
	 * \param The output absolute orbit number		  
	 */
	virtual void relOrbitFromPhase(RelOrbitInfo&,long int&)=0;

	/**
	 * \brief Simulate orbit propagation over a complete orbit, performing an accurate prediction of
	 *		  osculating Cartesian state vectors for a requested time 
	 * \param <b>The procTime field must be set as used for input.</b>The position, velocity
	 *		  and acceleration are filled into the object for output.
	 */
	virtual void propag(StateVector&)=0;


	/**
	 * \brief Transforms a cartesian state vector between different reference frames.
	 *
	 * \param The input StateVector object.
	 * \param The output StateVector object.<b>It must have the refSystem field value set</b>.
	 */
	virtual void changeCartesianRef(const StateVector&, StateVector&)=0;


	/**
	 * \brief Make a coversion from keplerian to cartesian coordinates
	 *		  The acceleration coordinates are not set in output StateVector object.
	 *
	 * \param The input KeplerianElement object.
	 * \param The output StateVector object.
	 */
	virtual void convert(const KeplerianElement&, StateVector&)=0;

	/**
	 * \brief Make a coversion from cartesian to keplerian coordinates
	 *		  The acceleration coordinates are not set in output StateVector object.
	 * \param The input StateVector object.
	 * \param The output KeplerianElement object.
	 */
	virtual void convert(const StateVector&, KeplerianElement&)=0;

	/*
	 * \brief Return the list of SVs (as a std::vector)
	 * \param The output Vector of StateVector objects.
	 */
	virtual void getSVs(std::vector<StateVector>& stateVec)=0 ;
	
private:

	OrbitPropagator(const OrbitPropagator & ); // not implemented 
	OrbitPropagator &operator=(const OrbitPropagator &); // not implemented
};
_ACS_END_NAMESPACE

#endif //_OrbitPropagator_H_
