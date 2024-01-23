// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.3  2013/12/09 15:34:19  giucas
	Bug fixed
	
	Revision 2.2  2013/12/09 14:01:11  giucas
	Library completely revisited.
	

*/

#ifndef _L0ProductInfo_H_
#define _L0ProductInfo_H_ 

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief  ProductField class
 */
class ProductField
{
    friend void rsResourceSet::getValue<ProductField>(const std::string &, ProductField &) const ;

public:

	ProductField() : offset(), value(), size() {};

	~ProductField() throw() {};

	ProductField(const ProductField& e) :
		offset(e.offset),
		value(e.value),
		size(e.size)
	{}		
	ProductField& operator=(const ProductField& e)
	{
		if (this != &e) 
		{
			offset	= e.offset;
			value	= e.value;
			size	= e.size;
		}
		return *this;
	}


	std::string toStr() const;

	unsigned int offset; // PRQA S 2100 3
	unsigned int value;
	unsigned int size;
};

std::ostream &operator << (std::ostream &, const ProductField &) ;
acs::exostream &operator << (acs::exostream &, const ProductField &) ; // PRQA S 2072

inline bool operator==(const ProductField &a, const ProductField &b) // PRQA S 2134
{
	return ( (a.offset == b.offset) && (a.value == b.value) && (a.size == b.size) );
}

class L0ProductInfo
{
        friend void rsResourceSet::getValue<L0ProductInfo>(const std::string &, L0ProductInfo &) const ;
public:


	/**
	 * \brief  Class constructor 
	 * 
	 * \param  The product root for extracting data from resource  
	 */
	L0ProductInfo();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	~L0ProductInfo() throw() ;

	/**
	 * \brief  Copy constructor 
	 *    
	 */ 
	L0ProductInfo(const L0ProductInfo & ); 

	/**
	 * \brief  Operator=
	 *    
	 */
	L0ProductInfo &operator=(const L0ProductInfo &);
	    
	/**
	 * \brief  Dump method
	 *    
	 */
	std::string toStr() const ;

	/**
	 * \brief  Return the ProductInfo id.
	 *    
	 */
	const std::string& getId() const throw() { return _id; }

	/**
	 * \brief  Return packet Version.
	 *    
	 */
	unsigned int getVersion() const throw() { return _ccsdsVersion;}

	/**
	 * \brief  Return Data Field Header Length.
	 *    
	 */
	unsigned int getDataFieldHeaderLength() const throw() { return _dataFieldHeaderLength; } 

	/**
	 * \brief  Return Application Data Length i.e. Source Data length.
	 *    
	 */
	unsigned int getApplicationDataLength() const throw() { return _applicationDataLength; }

	/**
	 * \brief  Return Packet Error Control Length.
	 *    
	 */
	unsigned int getPacketErrorControlLength() const throw() { return _packetErrorControlLength; }

	/**
	 * \brief  Return Application Process Id.
	 *    
	 */
	const std::vector<unsigned int>& getAPIDList() const throw(){ return _apidList;} 

	/**
	 * \brief  Return Channel Id.
	 *    
	 */
	unsigned int getVCID() const throw() { return _vcid; }

	/**
	 * \brief  Return Channel Id.
	 *    
	 */
	unsigned int getChannel() const throw() { return _channel; }

	/**
	 * \brief  Return Sensor name.
	 *    
	 */
	const std::string& getSensorName()const throw()  { return _sensorName; }

	/**
	 * \brief  Return Sensor mode.
	 *    
	 */
	const std::string& getSensorMode() const throw() { return _sensorMode; }

	/**
	 * \brief  Return true if packets are segmented false otherwise.
	 *    
	 */
	bool getSegmentedPacketsFlag() const throw() { return _segmentedPacketFlag; }

	/**
	 * \brief  Return Delta Nominal Time.
	 *    
	 */
	double getDeltaNominalTime() const throw() { return _deltaNominalTime; }

	/**
	 * \brief  Return Gap thresold value.
	 *    
	 */
	double getGapThresold() const throw(){ return _gapThresold; }

	/**
	 * \brief  Return Description.
	 *    
	 */
	const std::string &getDescription() const throw() { return _description; }

	/**
	 * \brief  Return Product Fields.
	 */
	const std::vector<ProductField> &getProductFields() const throw() {return _productFields;}

	/**
	 * \brief  Return slice parameters
	 */
	void getSliceParams(double& sliceLength, double& sliceOverlap) const throw() { sliceLength = _sliceLength ; sliceOverlap = _sliceOverlap ;} 

private:
		
/////////////////////////////////////////////		
//private attributes
/////////////////////////////////////////////
		
	std::string 				_id;
	unsigned int 				_ccsdsVersion;
	unsigned int 				_dataFieldHeaderLength;
	unsigned int 				_applicationDataLength;
	unsigned int 				_packetErrorControlLength;
	unsigned int				_channel;
	unsigned int				_vcid;
	std::string 				_sensorName;
	std::string 				_sensorMode;
	bool						_segmentedPacketFlag;
	double						_deltaNominalTime;
	double						_gapThresold;
	std::string 				_description;
	std::vector<unsigned int> 	_apidList;
	std::vector<ProductField>	_productFields;
	double 						_sliceLength;
	double 						_sliceOverlap;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(L0ProductInfo) ;

};

std::ostream &operator << (std::ostream &, const L0ProductInfo &) ;
acs::exostream &operator << (acs::exostream &, const L0ProductInfo &) ; // PRQA S 2072


template<>
inline
void rsResourceSet::getValue(const std::string &res, ProductField &v) const // PRQA S 2133, 2134
{
	Lock lock(_mutex) ;
	getValue(res+".Offset")->get(v.offset) ;
	getValue(res+".Size")->get(v.size) ;
	getValue(res+".Value")->get(v.value) ;
} 

template<>
inline
void rsResourceSet::getValue(const std::string &res, L0ProductInfo &v) const // PRQA S 2133, 2134
{
	Lock lock(_mutex) ;
	getValue(res+".Id")->get(v._id) ;
	getValue(res+".CCSDSVersion")->get(v._ccsdsVersion) ;
	getValue(res+".DataFieldHeaderLength")->get(v._dataFieldHeaderLength) ;
	getValue(res+".ApplicationDataLength")->get(v._applicationDataLength) ;
	getValue(res+".PacketErrorControlLength")->get(v._packetErrorControlLength) ;
	getValue(res+".Channel")->get(v._channel) ;
	getValue(res+".DataFieldHeaderLength")->get(v._dataFieldHeaderLength) ;
	
	getArray<unsigned int>(res+".List_of_APID.APID",v._apidList) ;
	
	getValue(res+".VCID")->get(v._vcid) ;
	getValue(res+".SensorName")->get(v._sensorName) ;
	getValue(res+".SensorMode")->get(v._sensorMode) ;
	getValue(res+".SegmentedPacketsFlag")->get(v._segmentedPacketFlag) ;
	getValue(res+".DeltaNominalTime")->get(v._deltaNominalTime) ;
	getValue(res+".GapThresold")->get(v._gapThresold) ;
	getValue(res+".Description")->get(v._description) ;	
	try {
		getArray<ProductField>(res+".List_of_Fields.Field",v._productFields) ;
		getValue(res+".Slice.Length")->get(v._sliceLength) ;	
		getValue(res+".Slice.Overlap")->get(v._sliceOverlap) ;	
	}
	catch(std::exception& e) 
	{ 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, e.what()) ;
		//not mandatory parameters 
	}
} 



_ACS_END_NAMESPACE


#endif //_L0ProductInfo_H_
