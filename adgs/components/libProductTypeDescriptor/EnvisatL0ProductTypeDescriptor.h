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
	Revision 2.1  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.1  2004/08/02 16:04:06  giucas
	Imported source.
	


*/

#ifndef _EnvisatL0ProductTypeDescriptor_H_
#define _EnvisatL0ProductTypeDescriptor_H_ 

#include <acs_c++config.hpp>
#include <L0ProductTypeDescriptor.h>


_ACS_BEGIN_NAMESPACE(acs)

class EnvisatL0ProductTypeDescriptor : public L0ProductTypeDescriptor // PRQA S 2153
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from database using Configuration Space
	 */	
	EnvisatL0ProductTypeDescriptor();

	/**
	 * \brief  Class constructor
	 * \param  Satellite Name
	 */	
	explicit EnvisatL0ProductTypeDescriptor(const std::string&);


	/**
	 * \brief  Class destructor 
	 */
	virtual ~EnvisatL0ProductTypeDescriptor() throw();


//////////////////////////
//ENVISAT Specific Methods		
//////////////////////////

	/**
	 * \brief Return a Product file name.
	 *        <b> The root and dates formats depend on specific implementation </b>
	 * \param The filename root
	 * \param The Start Time in string format
	 * \param The Stop Time in string format
	 * \param The Phase
	 * \param The Cycle
	 * \param The Relative orbit
	 * \param The Absolute orbit
	 * \param The counter
	 * \param The file extension
	 * \param The result file name
	 */
	void getName(const std::string& root, // PRQA S 4211
				 const std::string& startDate,
				 const std::string& stopDate,
				 long int phase, 
				 long int cycle,
				 long int relOrb,
				 long int absOrb,
				 int counter,
				 const std::string& ext,
				 std::string& result
				)
    {
	    ACS_COND_THROW(!getSatFlag() , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,phase,cycle,relOrb,absOrb,counter,ext,result);
    }

	/**
	 * \brief Return a Product file name.
	 *        <b> The root and dates formats depend on specific implementation </b>
	 * \param The filename root
	 * \param The Start Time in MJD2000 format
	 * \param The Stop Time in MJD2000 format
	 * \param The Phase
	 * \param The Cycle
	 * \param The Relative orbit
	 * \param The Absolute orbit
	 * \param The counter
	 * \param The file extension
	 * \param The result file name
	 */
	void getName(const std::string& root, // PRQA S 4211
				 double startDate,
				 double stopDate,
				 long int phase, 
				 long int cycle,
				 long int relOrb,
				 long int absOrb,
				 int counter,
				 const std::string& ext,
				 std::string& result
				)
    {
	    ACS_COND_THROW(!getSatFlag() , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	    getFileNamesHandler()->getName(root,startDate,stopDate,phase,cycle,relOrb,absOrb,counter,ext,result);
    }

private:

	/**
	 * \brief  Copy constructor
	 */
	EnvisatL0ProductTypeDescriptor(const EnvisatL0ProductTypeDescriptor & ); 


	/**
	 * \brief  Operator=
	 */
	EnvisatL0ProductTypeDescriptor &operator=(const EnvisatL0ProductTypeDescriptor &);

};
	

_ACS_END_NAMESPACE

#endif //_EnvisatL0ProductTypeDescriptor_H_
