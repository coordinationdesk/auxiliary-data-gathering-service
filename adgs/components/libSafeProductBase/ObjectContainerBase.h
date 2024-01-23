// PRQA S 1050 EOF
/*

	Copyright 2006-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. Safe Product Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.8  2013/11/14 14:44:40  marpas
 	coding best practices applied,
 	interface robustness and performances improved
 	
 	Revision 1.7  2013/10/02 13:05:54  giucas
 	rsResourceSet used as reference
 	
 	Revision 1.6  2011/08/01 10:21:35  aleber
 	destructor method made virtual
 	
 	Revision 1.5  2011/07/11 17:51:57  aleber
 	handling of schema repository completed
 	
 	Revision 1.4  2011/07/08 17:54:48  aleber
 	Writing of DataObjectContainer completed (but for checksum)
 	
 	Revision 1.3  2011/07/07 16:04:08  aleber
 	Object configuration correctly extracted from SafeProduct.xml
 	
 	Revision 1.2  2011/07/06 17:52:50  aleber
 	method addNewItem implemented
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	
*/

#ifndef _ObjectContainerBase_H_
#define _ObjectContainerBase_H_ 

#include <exStream.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class ObjectContainerBase // PRQA S 2109
{
public:

	/*! class ManifestBuilderBaseException declaration */
	exDECLARE_EXCEPTION(ObjectContainerBaseException,exException); // PRQA S 2131, 2502

	/**
	 * \brief  Class constructor
	 *    
	 */
	explicit ObjectContainerBase(const std::string& mission = "",
				const std::string& satellite = "",
				const std::string& sensor = "",
				const std::string& sensorMode = "",
				const std::string& level = "0",
				const std::string& productClass = "S",
				const std::string& polarisation = "S"); 

	/**
	 * \brief  Class destructor 
	 *    
	 */
    virtual ~ObjectContainerBase() throw() ;

	/**
	 * \brief  Return a vector containing the complete list of Ids 
	 * (Data Object ids, Metadata Object ids, Schema ids etc) configured for that object.
	 * A generic Object id can have several attribute associated to it; one attribute is mandatory 
	 * ("filename" representing the name of the Object file )and is associated by default;  
	 * others (e.g. "mimeType) are optional and can be associated by calling method addNewItem()
	 *    
	 */
	virtual std::vector<std::string> getIds() const ;

	/**
	 * \brief  set the value of the attribute "filename" associated to the Object id 
	 * \param IN: id = Object id (e.g. "MeasurementData")
	 * \param IN: filename = name of the Object file 
	 *    
	 */
	virtual void setFilename(const std::string& id, const std::string& filename);

	/**
	 * \brief  get the value of the provided attribute associated to the provided Object id 
	 * \param IN: id = Object id (e.g. "MeasurementData")
	 * \param IN: attribute = name of the attribute to get
	 *    
	 */
	std::string getAttribute(const std::string& id, const std::string& attribute) const ;                                                                                    

	virtual std::string toStr() const ;                                                                                    


	/**
	 * \brief  get a value from a "ListOf" tag of a resourceSet
	 * \param IN: id = name of the "ListOf" tag 
	 * \param IN: listElementId = number of the element of the list
	 *    
	 */
	static std::string getValueFromList(const std::string& , int );                                                                                    
	static std::string getValueFromList(const std::string& , size_t );                                                                                    
	static std::string getValueFromList(const std::string& , unsigned int );                                                                                    


	static std::string getValueFromList(const std::string& listName, int , const std::string& tag);                                                                                   
	static std::string getValueFromList(const std::string& listName, size_t , const std::string& tag);                                                                                   
	static std::string getValueFromList(const std::string& listName, unsigned int , const std::string& tag);                                                                                   

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
	 * \param IN: elementRefId = Name of the element of "ListOf" tag (e.g. "Mission")
	 * \param IN: listElementName = Name of the reference Id (e.g. "S1A")
	 * \param IN: tag = Name of the tag whose value is requested (e.g. "SatelliteName")
	 *    
	 */
	static std::string getValueFromList(const std::string& listName, 
					                   const std::string& listElementName, 
					                   const std::string& elementRefId, 
					                   const std::string& tag);                                                                                    

protected:
	/**
	 * \brief  Add a new attribute (e.g. mimeType) to the Object id 
	 * \param IN: id = Object id (e.g. "MeasurementData"
	 * \param IN: key = name of the new attribute (e.g. "mimeType")
	 *    
	 */
	virtual void addNewItem(const std::string& id, const std::string& key);

	/**
	 * \brief  builds a string representing the root string of a set of resourceSet tags 
	 * (when the root is a "ListOf" tag)
	 * \param IN: id = name of the "ListOf" root 
	 * \param IN: listElementId = number of the element of the list
	 *    
	 */
	std::string getRootFromList(const std::string& listName, int listElementId);                                                                                    

	/**
	 * \brief  builds a string representing the root string of a set of resourceSet tags 
	 * (when the root is a "ListOf" tag)
	 * \param IN: id = name of the "ListOf" root 
	 * \param IN: listElementId = number of the element of the list
	 *    
	 */
	static std::string getTagFromList(const std::string& listName, int listElementId);                                                                                    


    const std::string &productTypeId() const throw() { return _productTypeId ; }
    void setProductTypeId(const std::string &v) { _productTypeId = v ; }
    void setidMap(std::map< std::string, std::map<std::string, std::string> > &idMap) throw() { _idMap = idMap ; }
	std::map< std::string, std::map<std::string, std::string> > & idMap() throw() { return _idMap ; } // PRQA S 4024
    const std::map< std::string, std::map<std::string, std::string> > & idMap() const throw() { return _idMap ; }
    const std::string &mission() const throw() { return _mission ; }
    const std::string &satellite() const throw() { return _satellite ; }
    const std::string &sensor() const throw() { return _sensor ; }
    const std::string &sensorMode() const throw() { return _sensorMode ; }
    const std::string &level() const throw() { return _level ; }
    const std::string &productClass() const throw() { return _productClass ; }
	
private:
	ObjectContainerBase(const ObjectContainerBase & ); 
	ObjectContainerBase &operator=(const ObjectContainerBase &) ;


private:
	std::string _mission;                
	std::string _satellite;              
	std::string _sensor;                 
	std::string _sensorMode;                 
	std::string _level;              
	std::string _productClass;       
	std::string _polarisation;               
	std::string _productTypeId;
	std::map< std::string, std::map<std::string, std::string> >  _idMap;				

	size_t _numOfObject;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(ObjectContainerBase)

};

std::ostream &operator << (std::ostream &, const ObjectContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const ObjectContainerBase &) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif //_ObjectContainerBase_H_
