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
	Revision 2.2  2013/12/09 14:01:10  giucas
	Library completely revisited.
	
	Revision 2.1  2006/04/10 17:13:19  davcas
	Re-implemented method getProductIdByApidandVcid
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.4  2004/10/21 13:23:43  davcas
	Implemented method getName using CosmoProductFileName corresonding method
	
	Revision 1.3  2004/09/02 16:24:59  giucas
	Implemented getNameByApidVcidClass and init methods
	
	Revision 1.2  2004/08/06 14:09:52  giucas
	Added getNameByApidVcidClass method, still to be implemented.
	
	Revision 1.1  2004/08/06 11:48:13  giucas
	First release.
	


*/

#ifndef _CosmoL0ProductTypeDescriptor_H_
#define _CosmoL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)

class CosmoL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	CosmoL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit CosmoL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~CosmoL0ProductTypeDescriptor() throw();

	/**
	 * \brief Return a Product file name 
	 * \param Product name root
	 * \param Start/Stop dates in JD2000 (double) format
	 * \param Mission number 
	 * \param Image Segment File number
	 * \param File Extension 
	 */
	void getName(const std::string& root, // PRQA S 4211
				 double startDate, 
				 double stopDate, 
				 unsigned int mission,
				 unsigned int isf_id,
				 const std::string& ext, 
				 std::string& result)
    {
	    ACS_COND_THROW(!getSatFlag(), L0ProductTypeDescriptorException("Satellite name not set") ); // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,mission,isf_id,ext,result);
    }

private:

	/**
	 * \brief  Copy constructor
	 */
	CosmoL0ProductTypeDescriptor(const CosmoL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	CosmoL0ProductTypeDescriptor &operator=(const CosmoL0ProductTypeDescriptor &);


};


_ACS_END_NAMESPACE

#endif //_CosmoL0ProductTypeDescriptor_H_
