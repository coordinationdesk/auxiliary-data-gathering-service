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
	Revision 2.3  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.2  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.1  2007/07/05 09:10:43  giucas
	Added getOriginatorFromRoot
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.7  2004/10/21 15:32:04  envisat
	Added method getName for Cosmo CDF files
	
	Revision 1.6  2004/09/01 10:30:42  giucas
	Added nameWVersionForQuery method
	
	Revision 1.5  2004/05/06 17:06:45  envisat
	Added class BWSProductTypeDescriptor
	
	Revision 1.4  2004/02/03 14:05:59  marfav
	Added support for Level2 products
	
	Revision 1.3  2003/10/23 08:04:08  giucas
	Added isValidName and isValidRoot methods for L0 and L1 products
	
	Revision 1.2  2003/10/13 15:36:14  giucas
	Added method getFileClassFromRoot
	
	Revision 1.1  2003/09/09 13:13:14  giucas
	Created from L0FileNames class
	
	


*/

#ifndef _ProductFileName_H_
#define _ProductFileName_H_ 

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class ProductFileName 
{
public:

	/*! class ProductFileNameException declaration */
	exDECLARE_EXCEPTION(ProductFileNameException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor.
	 *    
	 */	
	ProductFileName();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~ProductFileName() throw();


/////////////////////////////////////////////		
//public methods
/////////////////////////////////////////////


	/**
	 * \brief Return a list of Product Names given a Product Id  
	 * <b> The names formats depend on specific implementation </b>  
	 */
	virtual void getNamesById(const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product Id  
	 * <b> The names, dates and extension formats depend on specific implementation </b>  
	 */
	virtual void getNamesById(const std::string&, const std::string&, const std::string&, std::vector<std::string>&)=0;


	/**
	 * \brief Return a list of Product Names given a Mission Id and a ProductID  
	 * <b> The names formats depend on specific implementation </b>  
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Mission Id  and a ProductID
	 * <b> The names formats depend on specific implementation </b>  
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, const std::string&, const std::string&, std::vector<std::string>&)=0;


	/**
	 * \brief Return a list of Product Names given a Product file extension and a ProductID  
	 * <b> The names and extension formats depend on specific implementation </b>  
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product file extension and a ProductID  
	 * <b> The root, dates and extension formats depend on specific implementation </b>  
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, const std::string&, const std::string&, std::vector<std::string>&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L0 or L1 Product name, false otherwise. 
	 * <b> The file name format depends on specific implementation </b>  
	 */
	virtual bool isValidName(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L0 or L1 Root name, false otherwise. 
	 * <b> The root format depends on specific implementation </b>  
	 */
	virtual bool isValidRoot(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid file extension, false otherwise. 
	 *    
	 */
	virtual bool isValidExtension(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L0 Product name, false otherwise. 
	 * <b> The file name format depends on specific implementation </b>  
	 */
	virtual bool isValidL0Name(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L0 Root name, false otherwise. 
	 * <b> The root format depends on specific implementation </b>  
	 */
	virtual bool isValidL0Root(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L1 Product name, false otherwise. 
	 * <b> The file name format depends on specific implementation </b>  
	 */
	virtual bool isValidL1Name(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L1 Root name, false otherwise. 
	 * <b> The root format depends on specific implementation </b>  
	 */
	virtual bool isValidL1Root(const std::string&)=0;

	/**
	 * \brief  Return true if the string in input represent a valid L2 Product name, false otherwise.
	 * <b> The file name format depends on specific implementation </b>
	 */
	virtual bool isValidL2Name(const std::string&) {return false;} // PRQA S 2131

	/**
	 * \brief  Return true if the string in input represent a valid L2 Root name, false otherwise.
	 * <b> The root format depends on specific implementation </b>
	 */
	virtual bool isValidL2Root(const std::string&) {return false;} // PRQA S 2131

	/**
	 * \brief  Return true if the string in input represent a valid BWS Product name, false otherwise. 
	 * <b> The file name format depends on specific implementation </b>  
	 */
	virtual bool isValidBWSName(const std::string&) {return false;} // PRQA S 2131

	/**
	 * \brief  Return true if the string in input represent a valid BWS Root name, false otherwise. 
	 * <b> The root format depends on specific implementation </b>  
	 */
	virtual bool isValidBWSRoot(const std::string&) {return false;} // PRQA S 2131

	/**
	 * \brief Return a Product file name 
	 * \param The filename root. <b>Its format depends on specific implementation</b>
	 * \param The Start Time in string format<b>(depends on specific implementation)</b>
	 * \param The Stop Time in string format<b>(depends on specific implementation)</b>
	 * \param The file extension
	 * \param The result file name
	 */
	virtual void getName(const std::string&, const std::string&, const std::string&, const std::string&, std::string&)=0;


	/**
	 * \brief Return a Product file name 
	 * \param The filename root. <b>Its format depends on specific implementation</b>
	 * \param The Start Time in MJD2000 format
	 * \param The Stop Time in MJD2000 format
	 * \param The file extension
	 * \param The result file name
	 */
	virtual void getName(const std::string&, double, double, const std::string&, std::string&)=0;


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
	virtual void getName(const std::string&,
						 const std::string&,
						 const std::string&,
						 long int, 
						 long int,
						 long int,
						 long int,
						 int,
						 const std::string&,
						 std::string&
						)=0;

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
	virtual void getName(const std::string&,
						 double,
						 double,
						 long int, 
						 long int,
						 long int,
						 long int,
						 int,
						 const std::string&,
						 std::string&
						)=0;

	/**
	 * \brief Return a COSMO CDF Product file name 
	 * \param The filename root
	 * \param The Start Time in MJD2000 format
	 * \param The Stop Time in MJD2000 format
	 * \param The Mission number
	 * \param The Image Segment number
	 * \param The file extension
	 * \param The result file name
	 */
	virtual void getName(const std::string&, // PRQA S 2131
						double,
						double,
						unsigned int, 
						unsigned int,
						const std::string&,
						std::string&
						) {};

	/**
	 * \brief  Return the Product Id given a 'short' Product File Name Root+Extension.
	 *		   If the file name is not a valid Level0 Product File Name Root an exception is thrown.
	 * <b> The root and extension formats depend on specific implementation </b>  
	 */
	virtual std::string getProductIDFromRoot(const std::string&)=0;

	/**
	 * \brief  Return the Product Id given a 'complete' Product File Name.
	 *		   If the file name is not a valid Level0 Product File Name an exception is thrown.
	 * <b> The Product File Name depends on specific implementation </b>  
	 */
	virtual std::string getProductIDFromName(const std::string&)=0;

	/**
	 * \brief  Return Default File Name Version.
	 * <b> The File Name Version depends on specific implementation </b> 
	 */
	virtual std::string getDefaultFileNameVersion()=0;

	/**
	 * \brief  Return the Mission ID.
	 *    
	 */
	virtual std::string getMissionID()=0;

///////////////////////////////////////////////////////////
// useful methods to extract info from complete file names
///////////////////////////////////////////////////////////

	/**
	 * \brief  Return File Name Version.
	 *        <b> The File Name Version format depends on specific implementation </b>
	 * \param  The complete product file name
	 */
	virtual std::string getVersion(const std::string&)=0;

	/**
	 * \brief  Return File Name Class.
	 *        <b> The File Name Class format depends on specific implementation </b>
	 * \param  The complete product file name
	 */
	virtual std::string getFileClass(const std::string&)=0;

	/**
	 * \brief  Return File Name Class.
	 *        <b> The File Name Class format depends on specific implementation </b>
	 * \param  The product file name root
	 */
	virtual std::string getFileClassFromRoot(const std::string&)=0;

	/**
	 * \brief  Return File Name Validity Start Time as a MJD2000.
	 * \param  The complete product file name
	 */
	virtual long double getStartTime(const std::string&)=0;

	/**
	 * \brief  Return File Name Validity Stop Time as a MJD2000.
	 * \param  The complete product file name
	 */
	virtual long double getStopTime(const std::string&)=0;

	/**
	 * \brief  Return a product file name with the new version.
	 * \param  The complete product file name
	 * \param  The new file name version
	 */
	virtual std::string nameWVersion(const std::string&,const std::string&)=0;

	/**
	 * \brief  Return OriginatorId <b> ONLY FOR ENVISAT </b>.
	 *        <b> The File Name Class format depends on specific implementation </b>
	 * \param  The product file name root
	 */
	virtual std::string getOriginatorFromRoot(const std::string&)=0;
private:
   	ProductFileName(const ProductFileName &); // not implemented
   	ProductFileName& operator= (const ProductFileName &); // not implemented

};


_ACS_END_NAMESPACE


#endif //_ProductFileName_H_
