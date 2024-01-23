// PRQA S 1050 EOF
/*

	Copyright 2007-2013, Advanced Computer Systems , Inc.
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
 	Revision 1.2  2013/11/14 14:44:40  marpas
 	coding best practices applied,
 	interface robustness and performances improved
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	

*/

#include <FrameSetContainerBase.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


TileContainer::TileContainer(double cloudVote,  int column, int row) :
    _cloudVote(cloudVote),
    _column(column),
    _row(row)
{
}

TileContainer::~TileContainer() throw() 
{
}

// copy constructor
TileContainer::TileContainer(const TileContainer & r) :
    _cloudVote(r._cloudVote),
    _column(r._column),
    _row(r._row)
{
}

// operator =
TileContainer&  TileContainer::operator=(const TileContainer & e)
{
	if (this != &e) {
       _cloudVote = e._cloudVote;
       _column = e._column;
       _row = e._row;
	}
	return *this ;
}

string TileContainer::toStr() const
{                                                                                                           
    ostringstream os ;
    os << "TileContainer class members: " << '\n'
	   << "\tCloudVote = " << _cloudVote << '\n'  
	   << "\tColumn = " << _column << '\n'  
	   << "\tRow = " << _row ;
    return os.str() ;
}                                                                                                           
  
                                                                                                              
std::ostream &operator << (std::ostream &os, const TileContainer &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const TileContainer &v) // PRQA S 4222
{
    return os << v.toStr() ;
}
  
  
                                                                                                            

FrameContainer::FrameContainer(
	                           const string& centerCoord,
	                           const string& footprint,		 
	                           int number,		   		 
	                           int orbit,		   		 
	                           int track,		   		 
	                           const string& time
                              ) :
    _centerCoord(centerCoord),
    _footprint(footprint),
    _number(number),
    _orbit(orbit),
	_track(track),
	_time(time),
	_timeJd50(),
	_urLat(),
	_urLon(),
	_ulLat(),
	_ulLon(),
	_llLat(),
	_llLon(),
	_lrLat(),
	_lrLon(),
	_tile()
{
}

FrameContainer::~FrameContainer() throw() {}


// copy constructor
FrameContainer::FrameContainer(const FrameContainer & e) :
    _centerCoord(e._centerCoord),
    _footprint(e._footprint),
    _number(e._number),
    _orbit(e._orbit),
	_track(e._track),
	_time(e._time),
	_timeJd50(e._timeJd50),
	_urLat(e._urLat),
	_urLon(e._urLon),
	_ulLat(e._ulLat),
	_ulLon(e._ulLon),
	_llLat(e._llLat),
	_llLon(e._llLon),
	_lrLat(e._lrLat),
	_lrLon(e._lrLon),
	_tile(e._tile)

{
}

// operator =
FrameContainer&  FrameContainer::operator=(const FrameContainer & e)
{
	if (this != &e) {
        _centerCoord = e._centerCoord ;
        _footprint = e._footprint ;
        _number = e._number ;
        _orbit = e._orbit ;
	    _track = e._track ;
	    _time = e._time ;
	    _timeJd50 = e._timeJd50 ;
	    _urLat = e._urLat ;
	    _urLon = e._urLon ;
	    _ulLat = e._ulLat ;
	    _ulLon = e._ulLon ;
	    _llLat = e._llLat ;
	    _llLon = e._llLon ;
	    _lrLat = e._lrLat ;
	    _lrLon = e._lrLon ;
	    _tile = e._tile ; 
	}
	return *this ;
}

	    		    
void FrameContainer::setCorner(double urLat,
			                   double urLon,
			                   double ulLat,
			                   double ulLon,
			                   double llLat,
			                   double llLon,
			                   double lrLat,
			                   double lrLon)
			       
{
	_urLat = urLat;
	_urLon = urLon;
	_ulLat = ulLat;
	_ulLon = ulLon;
	_llLat = llLat;
	_llLon = llLon;
	_lrLat = lrLat;
	_lrLon = lrLon;
}


string FrameContainer::toStr() const
{                                                                                                           
    ostringstream os ;
	os << "FrameContainer class members: " << '\n' 
	   << "\tCenterCoord = " << _centerCoord << '\n'
	   << "\tFootprint = " << _footprint << '\n'
	   << "\tNumber = " << _number << '\n'
	   << "\tOrbit = " << _orbit << '\n'
	   << "\tTrack = " << _track << '\n' 
	   << "\tTime = " << _time ;
						     	     
	for(vector<TileContainer>::const_iterator tit = _tile.begin(); tit != _tile.end(); ++tit) {	     

		os << '\n' << *tit ;			     	     

	}							     	     
    
    return os.str() ;
}                                                                                                           
                                                                                                            
                                                                                                              
std::ostream &operator << (std::ostream &os, const FrameContainer &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const FrameContainer &v) // PRQA S 4222
{
    return os << v.toStr() ;
}
  
  

ACS_CLASS_DEFINE_DEBUG_LEVEL(FrameSetContainerBase)


FrameSetContainerBase::FrameSetContainerBase(
	double cloudVoteFloor,
	double cloudVoteCeiling
) :
	_cloudVoteFloor(cloudVoteFloor),
	_cloudVoteCeiling(cloudVoteCeiling),
	_frame(),
	_footprint()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

FrameSetContainerBase::~FrameSetContainerBase() throw() {}

// copy constructor
FrameSetContainerBase::FrameSetContainerBase(const FrameSetContainerBase & e) :
	_cloudVoteFloor(e._cloudVoteFloor),
	_cloudVoteCeiling(e._cloudVoteCeiling),
	_frame(e._frame),
	_footprint(e._footprint)
{
}

// operator =
FrameSetContainerBase&  FrameSetContainerBase::operator=(const FrameSetContainerBase & e)
{
	if (this != &e) {
		  _cloudVoteFloor = e._cloudVoteFloor;
		  _cloudVoteCeiling = e._cloudVoteCeiling;
		  _footprint = e._footprint;
		  _frame = e._frame;
	}
	return *this ;
}

// void FrameSetContainerBase::sortFramesByTimeJd50()                                                                                    
// {                                                                                                           
// 
//  	sort(_frame.begin(), _frame.end(), not2(FrameContainer()));
// 
// }                                                                                                           
                                                                                                            
// added with CCN safe 1.3
void FrameSetContainerBase::buildFootprint()
{

	size_t numOfFrame = _frame.size();

	// the footprint coordinates are expressed as a GML string (considering that it
	// is a GML LinearString type, therefore a list of pairs lat/lon (where lat and lon are separated
	// by a comma) separated by a blank; the first and last point must coincide; according to SAFE
	// specifications, the points are listed in counter-clockwise order

	// UR corner of the bounding polygon (where right and left, upper and lower 
	// are referred to the orbit orientation) corresponds to UR corner of last frame
	// (namely frame with more recent acquisition time)
	ostringstream gmlCoord;
 	gmlCoord << _frame[numOfFrame - 1].urLat() << "," <<  _frame[numOfFrame - 1].urLon() << " "; // PRQA S 3084 2
 	gmlCoord << _frame[numOfFrame - 1].ulLat() << "," <<  _frame[numOfFrame - 1].ulLon() << " ";
  	for (int frameNum = numOfFrame - 1; frameNum >= 0; --frameNum) {
	
		gmlCoord << _frame[frameNum].llLat() << "," <<  _frame[frameNum].llLon() << " ";
	
	}		
	
  	for (size_t frameNum = 0; frameNum < numOfFrame; ++frameNum) {
	
		gmlCoord << _frame[frameNum].lrLat() << "," <<  _frame[frameNum].lrLon() << " ";
	
	}		

	// according to GML standard, first and last point must coincide		
 	gmlCoord << _frame[numOfFrame - 1].urLat() << "," <<  _frame[numOfFrame - 1].urLon(); // PRQA S 3084
	
	_footprint = gmlCoord.str();
	
}


string FrameSetContainerBase::toStr() const
{                                                                                                           

	ostringstream os ;
    os << "FrameSetContainerBase class members: " << '\n'
	   << "\tCloudVoteFloor = " << _cloudVoteFloor << '\n'
	   << "\tCloudVoteCeiling = " << _cloudVoteCeiling ;

	for(vector<FrameContainer>::const_iterator fit = _frame.begin(); fit != _frame.end(); ++fit) {	     

		os << '\n' << *fit ;				     	     

	}							     	     
    return os.str() ;
}                                                                                                           


std::ostream &operator << (std::ostream &os, const FrameSetContainerBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const FrameSetContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}
  
  

_ACS_END_NAMESPACE


