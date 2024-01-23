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
	Revision 2.1  2013/12/09 14:01:10  giucas
	Library completely revisited.
	
*/

#ifndef _BWSProductInfo_H_
#define _BWSProductInfo_H_

#include <acs_c++config.hpp>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class BWSProductInfo // PRQA S 2109
{
public:


	/**
	 * \brief  Class constructor
	 *
	 * \param  The product root for extracting data from resource
	 */
	explicit BWSProductInfo(const std::string&);

	/**
	 * \brief  Class destructor
	 *
	 */
	~BWSProductInfo() throw();

	/**
	 * \brief  Copy constructor
	 *
	 */
	BWSProductInfo(const BWSProductInfo & );

	/**
	 * \brief  Operator=
	 *
	 */
	BWSProductInfo &operator=(const BWSProductInfo &);

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
	const std::string &getSensorName()  const throw() { return _sensorName; }

	/**
	 * \brief  Return Sensor mode.
	 *
	 */
	const std::string &getSensorMode()  const throw() { return _sensorMode; }

private:
    BWSProductInfo() ; // not implemented
private:

	std::string 			_id;
	std::string 			_sensorName;
	std::string 			_sensorMode;

};
_ACS_END_NAMESPACE

#endif //_BWSProductInfo_H_
