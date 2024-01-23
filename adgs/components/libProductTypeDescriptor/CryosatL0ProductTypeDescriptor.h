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
	Revision 2.1  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.1  2004/08/02 16:04:06  giucas
	Imported source.
	


*/

#ifndef _CryosatL0ProductTypeDescriptor_H_
#define _CryosatL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <L0ProductTypeDescriptor.h>
#include <ProductFileName.h>
#include <ProductFileNameFactory.h>


_ACS_BEGIN_NAMESPACE(acs)

class CryosatL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	CryosatL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit CryosatL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~CryosatL0ProductTypeDescriptor() throw();


	/**
	 * \brief Return a Product file name given a Product name,
	 *		  start/stop dates in string format and the file extension
	 */
	void getName(const std::string& root, // PRQA S 4211
				 const std::string& startDate, 
				 const std::string& stopDate, 
				 const std::string& ext, 
				 std::string& result)
    {
	    ACS_COND_THROW(!getSatFlag(), L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,ext,result);
    }

	/**
	 * \brief Return a Product file name given a Product name root,
	 *		  start/stop dates in UTC double format and the file extension
	 */
	void getName(const std::string& root,  // PRQA S 4211
				 double startDate, 
				 double stopDate, 
				 const std::string& ext, 
				 std::string& result)
    {
	    ACS_COND_THROW(!getSatFlag(), L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,ext,result);
    }

private:

	/**
	 * \brief  Copy constructor
	 */
	CryosatL0ProductTypeDescriptor(const CryosatL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	CryosatL0ProductTypeDescriptor &operator=(const CryosatL0ProductTypeDescriptor &);

};

	
_ACS_END_NAMESPACE


#endif //_CryosatL0ProductTypeDescriptor_H_
