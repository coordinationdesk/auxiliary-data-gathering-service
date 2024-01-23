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
	Revision 2.1  2013/12/09 14:01:11  giucas
	Library completely revisited.
	

*/

#include <L0ProductInfo.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(L0ProductInfo) ;

L0ProductInfo::L0ProductInfo() :
	_id(), 					
	_ccsdsVersion(),			
	_dataFieldHeaderLength(),	
	_applicationDataLength(),	
	_packetErrorControlLength(),
	_channel(),
	_vcid(),					
	_sensorName(),			
	_sensorMode(),				
	_segmentedPacketFlag(),	
	_deltaNominalTime(),		
	_gapThresold(),			
	_description(),			
	_apidList(),				
	_productFields(),			
	_sliceLength(),			
	_sliceOverlap()		
{
}

L0ProductInfo::~L0ProductInfo() throw()
{
}

L0ProductInfo::L0ProductInfo(const L0ProductInfo & e) :
	_id(e._id), 					
	_ccsdsVersion(e._ccsdsVersion),			
	_dataFieldHeaderLength(e._dataFieldHeaderLength),	
	_applicationDataLength(e._applicationDataLength),	
	_packetErrorControlLength(e._packetErrorControlLength),
	_channel(e._channel),
	_vcid(e._vcid),					
	_sensorName(e._sensorName),			
	_sensorMode(e._sensorMode),				
	_segmentedPacketFlag(e._segmentedPacketFlag),	
	_deltaNominalTime(e._deltaNominalTime),		
	_gapThresold(e._gapThresold),			
	_description(e._description),	
	_apidList(e._apidList),				
	_productFields(e._productFields),
	_sliceLength(e._sliceLength),
	_sliceOverlap(e._sliceOverlap)	
{
}

L0ProductInfo&  L0ProductInfo::operator=(const L0ProductInfo & e)
{
	if (this != &e) {	
		_id 						= e._id;
		_ccsdsVersion				= e._ccsdsVersion;
		_dataFieldHeaderLength		= e._dataFieldHeaderLength;
		_applicationDataLength		= e._applicationDataLength;
		_packetErrorControlLength	= e._packetErrorControlLength;
		_channel					= e._channel;
		_apidList					= e._apidList;
		_vcid						= e._vcid;
		_sensorName					= e._sensorName;
		_sensorMode					= e._sensorMode;
		_segmentedPacketFlag		= e._segmentedPacketFlag;
		_deltaNominalTime			= e._deltaNominalTime;
		_gapThresold				= e._gapThresold;
		_description				= e._description;
		_productFields				= e._productFields;
		_sliceLength				= e._sliceLength;
		_sliceOverlap				= e._sliceOverlap;
	}
	return *this ;
}

string L0ProductInfo::toStr() const
{
	ostringstream msg;
	msg << "_id=" << _id << '\n'
		<< "_ccsdsVersion=" << _ccsdsVersion << '\n'
		<< "_dataFieldHeaderLength=" << _dataFieldHeaderLength << '\n'
		<< "_applicationDataLength=" << _applicationDataLength << '\n'
		<< "_packetErrorControlLength=" << _packetErrorControlLength << '\n'
		<< "_vcid=" << _vcid << '\n'
		<< "_sensorName=" << _sensorName << '\n'
		<< "_sensorMode=" << _sensorMode << '\n'
		<< "_segmentedPacketFlag=" << boolalpha << _segmentedPacketFlag << '\n'
		<< "_deltaNominalTime=" << _deltaNominalTime << '\n'
		<< "_gapThresold=" << _gapThresold << '\n'
		<< "_description=" << _description << '\n'
		<< "_sliceLength=" << _sliceLength << '\n'
		<< "_sliceLength=" << _sliceLength << '\n' ;
	for(size_t i=0; i<_apidList.size(); i++)
	{
		msg << "APID[" << i << "]=" << _apidList[i] << '\n' ;
	}	
	for(size_t i=0; i<_productFields.size(); i++)
	{
		msg << _productFields[i] << '\n' ;
	}	
	return msg.str();
}

std::ostream &operator << (std::ostream &os, const L0ProductInfo &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const L0ProductInfo &v) // PRQA S 4222
{
    return os << v.toStr() ;
}


string ProductField::toStr() const
{
	ostringstream msg;
	msg << "ProductField.Offset = " << offset << ", "
		<< "ProductField.Size   = " << size << ", "
		<< "ProductField.Value  = " << value ;
	return msg.str();
}

std::ostream &operator << (std::ostream &os, const ProductField &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const ProductField &v) // PRQA S 4222
{
    return os << v.toStr() ;
}

_ACS_END_NAMESPACE
