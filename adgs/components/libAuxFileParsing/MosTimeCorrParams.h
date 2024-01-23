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
	Revision 1.2  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.1  2009/07/13 13:27:35  marant
	Class MosTimeCorrParams added
	



*/

#ifndef _MosTimeCorrParams_H_
#define _MosTimeCorrParams_H_ 

#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class holds reference UTC-SBT-ClockStep parameters used for UTC-SBT Time conversions
 */	
class MosTimeCorrParams : public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	MosTimeCorrParams();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~MosTimeCorrParams() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	MosTimeCorrParams(const MosTimeCorrParams & ); 

	/**
	 * \brief  Operator= 
	 */
	MosTimeCorrParams &operator=(const MosTimeCorrParams &);

    //Time Correlation info
	unsigned long time_code; // PRQA S 2100 3
    long double reference_j50;
    unsigned long clock_step_length;

    //Auxiliary L0 Time Info
    unsigned long l0_line_number; // PRQA S 2100 3
    unsigned long l0_time_code;
    long double l0_reference_j50;
    
    double AcrossTrackBias;

	/**
	 * \brief  Return a dump of object state
	 */
	std::string dump() const ;


private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MosTimeCorrParams) ;

};//end MosTimeCorrParams class

_ACS_END_NAMESPACE

#endif //_MosTimeCorrParams_H_
