// PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. Safe Wrapper Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.11  2014/02/06 12:59:55  enrcar
 	EC:: ProductTypeId() made public
 	
 	Revision 1.10  2013/11/14 14:44:40  marpas
 	coding best practices applied,
 	interface robustness and performances improved
 	
 	Revision 1.9  2013/10/02 13:05:54  giucas
 	rsResourceSet used as reference
 	
 	Revision 1.8  2011/09/12 10:44:57  aleber
 	keyword inline placed outside class scope
 	
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

#ifndef _ManifestBuilderBase_H_
#define _ManifestBuilderBase_H_ 

#include <exStream.h>
#include <AcquisitionContainerBase.h>
#include <PlatformContainerBase.h>
#include <OrbitRefContainerBase.h>
#include <ProcessingContainerBase.h>
#include <QualityContainerBase.h>
#include <FrameSetContainerBase.h>
#include <ObjectContainerBase.h>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class ManifestBuilderBase // PRQA S 2109
{
public:

	/*! class ManifestBuilderBaseException declaration */
	exDECLARE_EXCEPTION(ManifestBuilderBaseException,exException); // PRQA S 2131, 2502

	/*
	 *\param satellite platform name, mission id, instrument name, level number in string format
	 */			
	explicit ManifestBuilderBase(const std::string& safeDir = "",
				const std::string& mission = "",
				const std::string& satellite = "",
				const std::string& sensor = "",
				const std::string& sensorMode = "",
				const std::string& level = "",
				const std::string& productClass = "",
				const std::string& polarisation = "",
				bool validateFlag = true); 
    virtual ~ManifestBuilderBase() throw() ;

 	/**                                                                            
	 * \brief  writes the Manifest file according to the information provided through the
	 * Container objects and according to the Manifest configuration corresponding to the provided
	 * satellite, platform, sensor , sensorMode etc. information 
	 * \param IN: platformContainer = pointer to object of class PlatformContainer
	 */
	virtual void writeManifest() = 0;

 	/**                                                                            
	 * \brief  validates (if required) the Manifest content and closes the Manifest file 
	 * \param IN: validateFlag = if TRUE validation of Manifest is performrd
	 */
	void setValidateFlag(bool validateFlag) { _validateFlag = validateFlag; }
	bool getValidateFlag() const throw() { return _validateFlag; }

 	/**                                                                            
	 * \brief  assign the pointer to an object of class AcquisitionContainer (provided as argument)
	 *  to the private base class member "_acquisitionContainer"
	 * \param IN: acquisitionContainer = pointer to object of class AcquisitionContainer
	 */
  	void setAcquisition(AcquisitionContainerBase* v) throw() { _acquisitionContainer = v; }
	const AcquisitionContainerBase*	acquisitionContainer() const throw() { return _acquisitionContainer ; }
 	/**                                                                            
	 * \brief  assign the pointer to an object of class PlatformContainer (provided as argument)
	 *  to the private base class member "_platformContainer"
	 * \param IN: platformContainer = pointer to object of class PlatformContainer
	 */
  	void setPlatform(PlatformContainerBase* v) throw() { _platformContainer = v; }
	const PlatformContainerBase * platformContainer() const throw() { return _platformContainer ; }


	/**                                                                            
	 * \brief  assign the pointer to an object of class OrbitRefContainer (provided as argument)
	 *  to the private base class member "_orbitRefContainer"
	 * \param IN: orbitRefContainer = pointer to object of class OrbitRefContainer
	 */
 	void setOrbitRef(OrbitRefContainerBase* v) throw() { _orbitRefContainer = v; }
 	const OrbitRefContainerBase* orbitRefContainer() const throw() { return _orbitRefContainer ; }

	/**                                                                            
	 * \brief  assign the pointer to an object of class ProcessingContainer (provided as argument)
	 *  to the private base class member "_processingContainer"
	 * \param IN: ProcessingContainer = pointer to object of class ProcessingContainer
	 */
 	void setProcessing(ProcessingContainerBase* v) throw() { _processingContainer = v; }
	const ProcessingContainerBase* processingContainer() const throw() { return _processingContainer ; }

	/**                                                                            
	 * \brief  assign the pointer to an object of class FrameSetContainer (provided as argument)
	 *  to the private base class member "_frameSetContainer"
	 * \param IN: frameSetContainer = pointer to object of class FrameSetContainer
	 */
 	void setFrameSet(FrameSetContainerBase* v) throw() { _frameSetContainer = v; }
	const FrameSetContainerBase* frameSetContainer() const throw() { return _frameSetContainer ; }

	/**                                                                            
	 * \brief  assign the pointer to a vector of objects of class QualityContainer (provided as argument reference)
	 *  to the private base class member "_qualityContainer"
	 * \param IN: qualityContainer = pointer to a vector of object of class QualityContainer
	 */
 	void setQuality(std::vector<QualityContainerBase>& v)  { _qualityContainer = v; };
	const std::vector<QualityContainerBase> &qualityContainer() const throw() { return _qualityContainer ; }

	/**
 	 * \brief  dumps the content of the ManifestBuilderBase class (including content of objects of classes Containers)
	 */
	virtual std::string toStr() const ;
	/**
	 * \brief  get the value of an element (with id = listElementId) from a "ListOf" tag of a resourceSet
	 * \param IN: id = name of the "ListOf" tag 
	 * \param IN: listElementId = number of the element of the list
	 *    
	 */
	static std::string getValueFromList(const std::string& listName, int listElementId) 
       { return ObjectContainerBase:: getValueFromList(listName, listElementId) ; }                                                                                  
	static std::string getValueFromList(const std::string& listName, size_t listElementId) 
       { return ObjectContainerBase:: getValueFromList(listName, listElementId) ; }                                                                                  
	static std::string getValueFromList(const std::string& listName, unsigned int listElementId) 
       { return ObjectContainerBase:: getValueFromList(listName, listElementId) ; }                                                                                  

	/**
	 * \brief  get the value of the tag <Tag> of an element (with id = listElementId) from a "ListOf" tag of a resourceSet
	 * \param IN: id = name of the "ListOf" tag 
	 * \param IN: listElementId = number of the element of the list
	 * \param IN: tag = name of the tag whose value is required
	 *    
	 */
	static std::string getValueFromList(const std::string& listName, int listElementId, const std::string& tag)                                                                                  
       { return ObjectContainerBase::getValueFromList(listName, listElementId, tag) ; }                                                                                  
	static std::string getValueFromList(const std::string& listName, size_t listElementId, const std::string& tag)                                                                                  
       { return ObjectContainerBase::getValueFromList(listName, listElementId, tag) ; }                                                                                  
	static std::string getValueFromList(const std::string& listName, unsigned int listElementId, const std::string& tag)                                                                                  
       { return ObjectContainerBase::getValueFromList(listName, listElementId, tag) ; }                                                                                  

	/**
	 * \brief  get the value of a "tag" (e.g. "SatelliteName")  from a "ListOf" of a resourceSet,
	 *  correponding to a given "Id" (e.g. "S1A") as for the following example of resource file:
	*  <ListOfMissions count="2">
	*  	    <Mission>
	*  		    <Id>S1A</Id>
	*  		    <MissionName>SENTINEL-1</MissionName>			    
	*  		    <SatelliteName>A</SatelliteName>				    
	*  		    <NameForSchema>sentinel-1</NameForSchema>
	*  		    <NssdcId></NssdcId>
	*  	    <Mission>
	*  		    <Id>S1B</Id>
	*  		    <MissionName>SENTINEL-1</MissionName>			    
	*  		    <SatelliteName>B</SatelliteName>				    
	*  		    <NameForSchema>sentinel-1</NameForSchema>
	*  		    <NssdcId></NssdcId>
	*  	    </Mission>
	*  </ListOfMissions>
	 * \param IN: id = name of the "ListOf" tag (e.g. "ListOfMissions") 
	 * \param IN: listElementName = Name of the element of "ListOf" tag (e.g. "Mission")
	 * \param IN: elementRefId = Value of the element of "ListOf" tag (e.g. "S1A")
	 * \param IN: tag = Name of the tag whose value is requested (e.g. "SatelliteName")
	 *    
	 */
	static std::string getValueFromList(const std::string& listName, 
					                   const std::string& listElementName, 
					                   const std::string& elementRefId, 
					                   const std::string& tag)
     { return  ObjectContainerBase::getValueFromList(listName, 
                                                     listElementName,
                                                     elementRefId,
                                                     tag) ; }

    const std::string &productTypeId() const throw() { return _productTypeId ; }

protected:


    const std::string &safeDir() const throw() { return _safeDir ; }
    const std::string &mission() const throw() { return _mission ; }
    const std::string &satellite() const throw() { return _satellite ; }
    const std::string &sensor() const throw() { return _sensor ; }
    const std::string &sensorMode() const throw() { return _sensorMode ; }
    const std::string &level() const throw() { return _level ; }
    const std::string &productClass() const throw() { return _productClass ; }
    const std::string &polarisation() const throw() { return _polarisation ; }
    size_t numOfObjects() const throw() { return _numOfObjects ; }
    void setNumOfObjects(size_t v) throw() { _numOfObjects = v ; }
    void setProductTypeId(const std::string &v) { _productTypeId = v ; }

private:

	ManifestBuilderBase(const ManifestBuilderBase & ); 
	ManifestBuilderBase &operator=(const ManifestBuilderBase &) ;


private:

	AcquisitionContainerBase*	_acquisitionContainer;
	PlatformContainerBase*		_platformContainer;
 	OrbitRefContainerBase*		_orbitRefContainer;	      
	ProcessingContainerBase*	_processingContainer;	      
	FrameSetContainerBase*		_frameSetContainer;	      
	std::vector<QualityContainerBase> _qualityContainer;

	std::string _safeDir ;
	std::string _mission;                
	std::string _satellite;              
	std::string _sensor;                 
	std::string _sensorMode;                 
	std::string _level;              
	std::string _productClass;       
	std::string _polarisation;               

	bool _validateFlag ;

	size_t _numOfObjects;
	std::string _productTypeId;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ManifestBuilderBase)

};


std::ostream &operator << (std::ostream &, const ManifestBuilderBase &) ;
acs::exostream &operator << (acs::exostream &, const ManifestBuilderBase &) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif //_ManifestBuilderBase_H_
