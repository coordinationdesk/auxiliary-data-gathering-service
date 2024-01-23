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
	Revision 1.3  2014/02/13 11:03:11  giucas
	New log macros adopted
	
	Revision 1.2  2007/07/19 12:42:42  marpas
	GOCE work in progress
	
	Revision 1.1  2007/05/14 12:40:31  giucas
	First issue
	

*/

#ifndef _AnxInfo_H_
#define _AnxInfo_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)

class OrbitPropagator;
	
class AnxInfo // PRQA S 2109
{
public:


	/**
	 * \brief  Class constructor 
	 * \param satelliteName The Satellite Name
	 * \param mission The mission number 
	 */	
	explicit AnxInfo(const std::string& satelliteName, int mission=1);


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~AnxInfo() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	AnxInfo(const AnxInfo & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	AnxInfo &operator=(const AnxInfo &);

	/**
	 * \brief  Return a vector containing all the ANX times covering the interval [start,stop]
	 *    
	 */
	void get(double start, double stop, std::vector<double>&, std::vector<int>& orbits);

	/**
	 * \brief  Set the PropagModel used for initialisation. Default is 
	 *  PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL + PropagFormat::OP_PROPAG_MODEL_AUTO + PropagFormat::OP_PROPAG_MODEL_DOUBLE  
	 */
	void setPropagModel(int m){_propagModel = m;};

	double anx(int orbit) ;
private:
    AnxInfo() ; // not implemented
private:

	int			_propagModel;
	std::string _satelliteName ;
	int			_mission ;
	std::map<int,double> _orb2Anx ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AnxInfo) ;

};
_ACS_END_NAMESPACE

#endif //_AnxInfo_H_
