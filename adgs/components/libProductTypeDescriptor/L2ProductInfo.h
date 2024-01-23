// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
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

#ifndef _L2ProductInfo_H_
#define _L2ProductInfo_H_

#include <L1ProductInfo.h>

_ACS_BEGIN_NAMESPACE(acs)

class L2ProductInfo : public L1ProductInfo // PRQA S 2153
{
public:
	/**
	 * \brief  Class constructor
	 *
	 * \param  The product root for extracting data from resource
	 */
	explicit L2ProductInfo(const std::string&);

	/**
	 * \brief  Class destructor
	 *
	 */
	virtual ~L2ProductInfo() throw();

	/**
	 * \brief  Copy constructor
	 *
	 */
	L2ProductInfo(const L2ProductInfo & );

	/**
	 * \brief  Operator=
	 *
	 */
	L2ProductInfo &operator=(const L2ProductInfo &);
private:
    L2ProductInfo() ; // not implemented
};

_ACS_END_NAMESPACE


#endif // _L2ProductInfo_H_

