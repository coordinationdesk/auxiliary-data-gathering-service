// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA  - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.8  2018/07/18 15:10:51  marfav
	Syntax of const in-class definition changed in C++11 and now is supported
	
	Revision 1.7  2014/04/17 12:39:04  marpas
	work in progress
	
	Revision 1.6  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.5  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.4  2012/07/26 12:51:51  damdec
	writeXmlStream method made virtual
	
	Revision 1.3  2012/06/29 11:05:30  damdec
	readXmlStream method added.
	
	Revision 1.2  2012/04/17 16:00:16  damdec
	Safe namespace and srsName passed as parameters to the constructor.
	
	Revision 1.1  2012/03/16 15:00:51  damdec
	FrameSet class added.
	
        
*/
#ifndef _FrameSet_H_
#define _FrameSet_H_

#include <exException.h>
#include <WrappedMetadata.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;


struct Tile // PRQA S 2173, 2175
{
	int row; // PRQA S 2100 3
	int column;
	double cloudVote;
    Tile() : row(), column(), cloudVote() 
    {}
};


/**
* \brief
* Class mapping the content of SAFE FrameSetType
**/

class FrameSet : public WrappedMetadata { // PRQA S 2109
public:

	/** FrameSet Exceptions */
	exDECLARE_EXCEPTION(exFrameSetException, exException) ; // Base FrameSet Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exFrameSetCriticalException, exFrameSetException) ; // Critical Exception. // PRQA S 2153


	/** Class constructors (write mode) */
	FrameSet(const std::string & safeNamespace, 
             const std::string & srsName, 
             const std::vector<Tile> &tile, 
             const std::vector<std::pair<double, double> > & footprint, 
             unsigned int orbit = 0, 
             unsigned int track = 0, 
	         unsigned int number = 0, 
             const std::pair<double, double> &center = std::pair<double, double>(), 
             const std::string &time="",
             const std::string& safePrefix="sentinel-safe:");
	
    FrameSet(const std::string & safeNamespace, 
             const std::string & srsName, 
             const std::vector<Tile> &tile, 
             const std::vector<std::pair<double, double> > & footprint, 
             double floor_value, 
             double ceil_value, 
             unsigned int orbit = 0, 
             unsigned int track = 0,  
             unsigned int number = 0, 
             const std::pair<double, double> &center = std::pair<double, double>(), 
             const std::string &time="",
             const std::string& safePrefix="sentinel-safe:");
	
	/* Class constructors (read mode)  */
	explicit FrameSet(const rsResourceSet & rs);
	
	/** Destructor */
	virtual ~FrameSet() throw() ;


	/** Copy Constructor */
	FrameSet(const FrameSet & );
	/** Operator = */
	FrameSet &operator=(const FrameSet &);
	
	const std::vector< std::pair<double, double> > &getFootprint() const throw() {return _footprint;};
	
protected:
	
	virtual void writeXmlStream() ;
	virtual void readXmlStream(const rsResourceSet & rs);
private:
    FrameSet() ; // not implemented
public:
#if __cplusplus >= 201103L
    // Syntax changed in C++11 specifications
    static constexpr double DefaultFloor = 0.0 ; // PRQA S 2100 2
    static constexpr double DefaultCeil = 10.0;
#else
    static const double DefaultFloor = 0.0 ; // PRQA S 2100 2
    static const double DefaultCeil = 10.0;
#endif

private:
	std::string _srsName;
	double _floor;
	double _ceil;
	bool _isSetCloudSystem;
	unsigned int _orbit;
	unsigned int _track;
	unsigned int _number;
	std::pair<double, double> _center;
	std::vector< std::pair<double, double> > _footprint;
	std::string _time;
	
	std::vector<Tile> _tile; 
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FrameSet)

};

_ACS_END_NAMESPACE

#endif //_FrameSet_H_

