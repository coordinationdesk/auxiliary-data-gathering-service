// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
*/

#ifndef _L1ProductInfo_H_
#define _L1ProductInfo_H_

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class L1ProductInfo // PRQA S 2109
{
public:


	/**
	 * \brief  Class constructor
	 *
	 * \param  The product root for extracting data from resource
	 */
	explicit L1ProductInfo(const std::string&);

	/**
	 * \brief  Class destructor
	 *
	 */
	virtual ~L1ProductInfo() throw();

	/**
	 * \brief  Copy constructor
	 *
	 */
	L1ProductInfo(const L1ProductInfo & );

	/**
	 * \brief  Operator=
	 *
	 */
	L1ProductInfo &operator=(const L1ProductInfo &);

/////////////////////////////////////////////
//public methods
/////////////////////////////////////////////

	/**
	 * \brief  Return the ProductInfo id.
	 *
	 */
	const std::string &getId() const throw() { return _id; }

	/**
	 * \brief  Return Sensor name.
	 *
	 */
	const std::string &getSensorName() const throw() { return _sensorName; } 

	/**
	 * \brief  Return Sensor mode.
	 *
	 */
	const std::string &getSensorMode() const throw() { return _sensorMode; } // PRQA S 4211

private:
    L1ProductInfo() ; // not implemented
private:

	std::string 			_id;
	std::string 			_sensorName;
	std::string 			_sensorMode;

};

_ACS_END_NAMESPACE


#endif //_L1ProductInfo_H_
