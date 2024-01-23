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
	Revision 2.2  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2011/09/22 07:33:43  giucas
	Added S1OBMemConfParams
	


*/

#ifndef _S1OBMemConfParams_H_
#define _S1OBMemConfParams_H_ 

#include <exException.h>
#include <AuxFileParser.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)


/**
 * This class holds parmaeters extracted from the S1 OnBoard Memory Configuration Update file
 */	
class S1OBMemConfParams : public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	S1OBMemConfParams();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~S1OBMemConfParams() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	S1OBMemConfParams(const S1OBMemConfParams & ); 

	/**
	 * \brief  Operator= 
	 */
	S1OBMemConfParams &operator=(const S1OBMemConfParams &);

	/**
	 * \brief PacketStore information: key is PacketStore (i.e. PS-0), and value is pair <VC-ID,PSType> (i.e. VCID-0, Emergency)
	 */
	std::map<std::string,std::pair<std::string,std::string> >	_psInfo ; // PRQA S 2100

	/**
	 * \brief VCID information: key is VCID as in the data (i.e. 1-relative, VCID-0 is VCID 1 in data), and value is the PSType
	 */
	std::map<unsigned int,std::string>	_vcidDataInfo ; // PRQA S 2100

	/**
	 * \brief  Return a dump of object state
	 */
	std::string dump() const;


private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1OBMemConfParams) ;

};//end S1OBMemConfParams class

_ACS_END_NAMESPACE

#endif //_S1OBMemConfParams_H_
