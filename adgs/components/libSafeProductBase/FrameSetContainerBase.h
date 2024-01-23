// PRQA S 1050 EOF
/*

	Copyright 2007-2013, Advanced Computer Systems , Inc.
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
 	Revision 1.3  2013/11/14 14:44:40  marpas
 	coding best practices applied,
 	interface robustness and performances improved
 	
 	Revision 1.2  2012/02/20 14:03:37  marpas
 	no useless base class
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	 	

*/

#ifndef _FrameSetContainerBase_H_
#define _FrameSetContainerBase_H_ 

#include <exStream.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class TileContainer
{

public:

	explicit TileContainer(double cloudVote = 0.,
		     	  int column = 0,
		     	  int row = 0);
                  
	TileContainer(const TileContainer&);
	TileContainer &operator=(const TileContainer&);
	~TileContainer() throw() ;

	void setCloudVote(double v) throw() { _cloudVote = v ; }
	void setColumn(int v) throw() { _column = v ; }
	void setRow( int v ) throw() { _row = v ; }

	double cloudVote() const throw() { return _cloudVote; }
	int column() const throw() { return _column; }
	int row() const throw() { return _row; }

	std::string toStr() const ;

private:

    double 	_cloudVote;
    int	_column;
    int	_row;
};

std::ostream &operator << (std::ostream &, const TileContainer &) ;
acs::exostream &operator << (acs::exostream &, const TileContainer &) ; // PRQA S 2072
	

class FrameContainer
{

public:
    static const int DEFAULT_ORBIT = -1;
    static const int DEFAULT_NUMBER = -1;
    static const int DEFAULT_TRACK = -1;

	explicit FrameContainer(const std::string& centerCoord = "",
		           const std::string& footprint = "",
		      	   int number = DEFAULT_NUMBER,
		      	   int orbit = DEFAULT_ORBIT,
		      	   int track = DEFAULT_TRACK,
		      	   const std::string& time = "");
	~FrameContainer() throw() ;
	FrameContainer(const FrameContainer&);
	FrameContainer &operator=(const FrameContainer&);

	void setCenterCoord(const std::string& v) { _centerCoord = v; }	
	void setFootprint(const std::string& v) { _footprint = v; } 
	void setNumber(int v) throw() { _number = v; }
	void setOrbit(int v) throw() { _orbit = v; }
	void setTrack(int v) throw() { _track = v; }
	void setTime(long double timeJd50);
	void setTime(const std::string& v) { _time = v; }
	void setCorner(double urLat,
			       double urLon,
			       double ulLat,
			       double ulLon,
			       double llLat,
			       double llLon,
			       double lrLat,
			       double lrLon);

	void addTile(const TileContainer& tile) { _tile.push_back(tile); } 		

	const std::string& centerCoord() const throw() { return _centerCoord; }
	const std::string& footprint() const throw() { return _footprint; }
	int number() const throw() { return _number; }
	int orbit() const throw() { return _orbit; }
	int track() const throw() { return _track; }
	const std::string& time() const throw() { return _time; } // PRQA S 2502
	long double timeJd50() const throw() { return _timeJd50; }
	double urLat() const throw() { return _urLat; }
	double urLon() const throw() { return _urLon; } 
	double ulLat() const throw() { return _ulLat; } 
	double ulLon() const throw() { return _ulLon; } 
	double llLat() const throw() { return _llLat; } 
	double llLon() const throw() { return _llLon; } 
	double lrLat() const throw() { return _lrLat; } 
	double lrLon() const throw() { return _lrLon; } 
	inline const std::vector<TileContainer> &tile() const throw() { return _tile; }	

	std::string toStr() const ;

private:

    std::string _centerCoord;	// 
    std::string _footprint;		// 
    int _number;
    int _orbit;
	int _track;
	std::string _time;
	long double _timeJd50;
	double _urLat;
	double _urLon;
	double _ulLat;
	double _ulLon;
	double _llLat;
	double _llLon;
	double _lrLat;
	double _lrLon;
	std::vector<TileContainer>	_tile;
};

std::ostream &operator << (std::ostream &, const FrameContainer &) ;
acs::exostream &operator << (acs::exostream &, const FrameContainer &) ; // PRQA S 2072
	




class FrameSetContainerBase
{
public:
#if __cplusplus >= 201103L
    // Syntax changed in C++11 specifications
    static constexpr double DEFAULT_MIN_CLOUD_VOTE = 0.;  // PRQA S 2100 2
    static constexpr double DEFAULT_MAX_CLOUD_VOTE = 10.; 
#else
    static const double DEFAULT_MIN_CLOUD_VOTE = 0.;  // PRQA S 2100 2
    static const double DEFAULT_MAX_CLOUD_VOTE = 10.; 
#endif

	/**
	 * \brief  Class constructor
	 *    
	 */
	explicit FrameSetContainerBase(double cloudVoteFloor = DEFAULT_MIN_CLOUD_VOTE,
			  double cloudVoteCeiling = DEFAULT_MAX_CLOUD_VOTE);

	/**
	 * \brief  Class destructor 
	 *    
	 */
    ~FrameSetContainerBase() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	FrameSetContainerBase(const FrameSetContainerBase & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	FrameSetContainerBase &operator=(const FrameSetContainerBase &) ;

	void setCloudVoteFloor(double v) throw() { _cloudVoteFloor = v ; }
	void setCloudVoteCeiling(double v) throw() { _cloudVoteCeiling = v ; }
	void addFrame(const FrameContainer & frame) { _frame.push_back(frame); }
	void setFootprint(const std::string& v) { _footprint = v; }
	void sortFramesByTimeJd50();

	void buildFootprint();

	double cloudVoteFloor() const throw() { return _cloudVoteFloor; }
	double cloudVoteCeiling() const throw() { return _cloudVoteCeiling; }
	const std::vector<FrameContainer> &frame() const throw() { return _frame; }
	const std::string& footprint()const throw() { return _footprint; }

	std::string toStr() const ;

private:

	double _cloudVoteFloor;
	double _cloudVoteCeiling;
	std::vector<FrameContainer>	_frame;
	std::string _footprint;	// Bounding polygon of the product;
				// the bounding polygon includes all frames;
				// it is a GML string (formatted according to gml::linearRing type, namely
				// as a list of tuples (lat and lon separated by a comma)
				// each representing one corner of the Bounding polygon
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FrameSetContainerBase)

};


std::ostream &operator << (std::ostream &, const FrameSetContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const FrameSetContainerBase &) ; // PRQA S 2072


_ACS_END_NAMESPACE

#endif //_FrameSetContainerBase_H_
