//PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. SafeWrapper Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.9  2013/10/02 13:05:54  giucas
 	rsResourceSet used as reference
 	
 	Revision 1.8  2012/02/13 09:24:44  lucio.pulvirenti
 	warning on deprecated exDEFINE_EXCEPTION_MACRO() removed
 	
 	Revision 1.7  2011/09/09 16:19:27  aleber
 	minor error on computation of productTypeId fixed
 	
 	Revision 1.6  2011/09/07 17:19:16  aleber
 	writing of Manifest processing section implemented
 	
 	Revision 1.5  2011/08/01 10:20:53  aleber
 	AcquisitionContainer class integrated
 	
 	Revision 1.4  2011/07/18 16:15:21  aleber
 	method getValuefor List() completed
 	
 	Revision 1.3  2011/07/11 17:51:57  aleber
 	handling of schema repository completed
 	
 	Revision 1.2  2011/07/08 17:54:48  aleber
 	Writing of DataObjectContainer completed (but for checksum)
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	

*/

#include <ConfigurationSingleton.h>
#include <ManifestBuilderBase.h>
#include <Filterables.h>           


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ManifestBuilderBase)

ManifestBuilderBase::ManifestBuilderBase(const string& safeDir,
					 const string& mission,
					 const string& satellite,
					 const string& sensor,
					 const string& sensorMode,
					 const string& level,
					 const string& productClass,
					 const string& polarisation,
					 bool validateFlag): 
 	_acquisitionContainer(0),
 	_platformContainer(0),
 	_orbitRefContainer(0),	      
 	_processingContainer(0),	      
	_frameSetContainer(0),
    _qualityContainer(0),
	_safeDir(safeDir),
	_mission(mission),
	_satellite(satellite),
	_sensor(sensor),
	_sensorMode(sensorMode),
	_level(level),
	_productClass(productClass),
	_polarisation(polarisation),
	_validateFlag(validateFlag),
    _numOfObjects(),
    _productTypeId()
{

	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;


	// build Product Type Id, based on provided arguments
	if (!mission.empty()) {
		_productTypeId += mission;
    }
	if (!satellite.empty()) {
		_productTypeId += "_" + satellite;
    }
	if (!sensor.empty()) {
		_productTypeId += "_" + sensor;
    }
	if (!sensorMode.empty()) {
		_productTypeId += "_" + sensorMode;
    }
	if (!level.empty()) {
		_productTypeId += "_" + level;
    }
	if (!productClass.empty()) {
		_productTypeId += "_" + productClass;		 
    }
	if (!polarisation.empty()) {
		_productTypeId += "_" + polarisation;
    }
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Object Safe created with following data members:") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\t product directory = " << safeDir) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\t mission = " << mission) ;
}

ManifestBuilderBase::~ManifestBuilderBase() throw() 
{

}

string ManifestBuilderBase::toStr() const
{
 	ostringstream msg;
	msg << "\nManifestBuilderBase content: " << '\n'
            << "\tPRIVATE DATA MEMBERS:" <<'\n'
            << "\t\tproduct directory\t= " << _safeDir << '\n'
            << "\t\tmission\t\t\t= " << _mission << '\n'
            << "\t\tsatellite\t\t= " <<  _sensor << '\n'
            << "\t\tsensorMode\t\t= " << _sensorMode << '\n'
            << "\t\tlevel\t\t\t= " << _level << '\n'
            << "\t\tproductClass\t\t= " << _productClass << '\n'
            << "\t\tpolarisation\t\t" << _polarisation << '\n'
            << "\t\tProduct Type Id\t\t" << _productTypeId << '\n'
            << "\t\tnumber of Data Objects\t\t" << _numOfObjects << '\n'
            << *_platformContainer << '\n' 
	        << *_orbitRefContainer << '\n' 
            << *_processingContainer << '\n' 
            << *_frameSetContainer << '\n' 
            << *_acquisitionContainer;
 	return msg.str() ;                                                                                                 
	
}

                                                                                                            
std::ostream &operator << (std::ostream &os, const ManifestBuilderBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const ManifestBuilderBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}

_ACS_END_NAMESPACE
