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
	Revision 2.3  2014/02/14 08:46:29  giucas
	New log macros adopted
	
	
*/

#ifndef _ProductHeader_H_
#define _ProductHeader_H_ 

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <XMLIstream.h>
#include <XMLOstream.h>
#include <exException.h>
#include <string>
#include <vector>
#include <fstream>

_ACS_BEGIN_NAMESPACE(acs)

class ProductHeader
{
public:

	/*! class ProductHeaderException declaration */
	exDECLARE_EXCEPTION(ProductHeaderException,exException) ;


	/**
	 * \brief  Default Class constructor. No initialization is done.
	 *		   <b>Clients MUST call initXXX methods</b>
	 *    
	 */
	ProductHeader();
	/**
	 * \brief  Class constructor used to automatically initialize all data.
	 *		   <b>the rsResourceSet argument MUST cointain all needed information</b>
	 *    	   <b>the default init() methods are executed to initialize the object</b>
	 */
	ProductHeader(const rsResourceSet&);

	/**
	 * \brief  Class destructor 
	 *    
	 */
    ~ProductHeader() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	ProductHeader(const ProductHeader & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	ProductHeader &operator=(const ProductHeader &) ;

	/**
	 * \brief  Initialize MPH (Main Product Header) part 
	 * \param  rsResourceSet containing the initialization information
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values
	 */
	void initMPH(const rsResourceSet&, std::string="MPH" );

	/**
	 * \brief  Initialize SPH (Specific Product Header) part
	 * \param  rsResourceSet containing the initialization information  
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values  
	 */
	void initSPH(const rsResourceSet&, std::string="SPH" );

	/**
	 * \brief  Initialize DSD (Data Set Descriptor) part
	 * \param  rsResourceSet containing the initialization information
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values   
	 */
	void initDSD(const rsResourceSet& ,u_int = 1, std::string="DSD");

	/**
	 * \brief  Initialize Spare DSD i.e. a std::string of 279 blank character terminated by a '\n' char.
	 *    
	 */
	void initSpareDSD(u_int = 1);

	/**
	 * \brief  Initialize XML Header MPH part
	 * \param  rsResourceSet containing the initialization information 
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values    
	 */
	void initXmlMPH(const rsResourceSet&, std::string="XmlMPH");

	/**
	 * \brief  Initialize XML Header SPH part
	 * \param  rsResourceSet containing the initialization information
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values
	 */
	void initXmlSPH(const rsResourceSet&, std::string="XmlSPH");

	/**
	 * \brief  Initialize XML Header MDH part
	 * \param  rsResourceSet containing the initialization information
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values      
	 */
	void initXmlMDH(const rsResourceSet&, std::string="XmlMDH");

	/**
	 * \brief  Initialize XML Header Fixed Header part
	 * \param  rsResourceSet containing the initialization information
	 * \param  A std::string identifying the root in the rsResourceSet used to retrieve field values      
	 */
	void initXmlFixedHeader(const rsResourceSet&, std::string="XmlFixedHeader");

	/**
	 * \brief  Initialize XML Header DSD part
	 * \param  rsResourceSet containing the initialization information
	 * \param  The number of DSDs. Default is 1.
	 * \param  The string identifying the root in the rsResourceSet used to retrieve field values
	 */
	void initXmlDSD(const rsResourceSet&, u_int = 1, std::string="XmlDSD");


//////////////////////////
// MPH Fields set methods
//////////////////////////		
	/**
	 * \brief  Set a specific MPH field with a std::string value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const std::string& );

	/**
	 * \brief  Set a specific MPH field with a float value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const float& );

	/**
	 * \brief  Set a specific MPH field with a int value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const int& );

	/**
	 * \brief  Set a specific MPH field with a long int value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const long int& );

	/**
	 * \brief  Set a specific MPH field with a unsigned long int value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const unsigned long int& );

	/**
	 * \brief  Set a specific MPH field with a long long int value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const long long int& );

	/**
	 * \brief  Set a specific MPH field with an unsigned long long int value
	 * \param MPH field name
	 * \param MPH field value   
	 */
	void setMPHField(const std::string& , const unsigned long long int& );

//////////////////////////
// SPH Fields set methods
//////////////////////////		

	/**
	 * \brief Set a specific SPH field with a std::string value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const std::string& );

	/**
	 * \brief  Set a specific SPH field with a float value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const float& );

	/**
	 * \brief  Set a specific SPH field with a int value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const int& );

	/**
	 * \brief  Set a specific SPH field with a long long int value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const long long int& );

	/**
	 * \brief  Set a specific SPH field with an unsigned long long int value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const unsigned long long int& );		

	/**
	 * \brief  Set a specific SPH field with a long int value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const long int& );

	/**
	 * \brief  Set a specific SPH field with an unsigned long int value
	 * \param SPH field name
	 * \param SPH field value   
	 */
	void setSPHField(const std::string& , const unsigned long int& );		

//////////////////////////
// DSD Fields set methods
//////////////////////////		

	/**
	 * \brief  Set a specific DSD field with a std::string value
	 * \param The DSD field name
	 * \param The DSD field value   
	 * \param The number of DSD(default is 1)
	 */
	void setDSDField(const std::string& , const std::string& ,u_int =1);

	/**
	 * \brief  Set a specific DSD field with a int value
	 * \param The DSD field name
	 * \param The DSD field value   
	 * \param The number of DSD(default is 1)
	 */
	void setDSDField(const std::string& , const int& ,u_int =1);

	/**
	 * \brief  Set a specific DSD field with a long long int value
	 * \param The DSD field name
	 * \param The DSD field value   
	 * \param The number of DSD(default is 1)
	 */
	void setDSDField(const std::string& , const long long int& ,u_int =1);

	/**
	 * \brief  Set a specific DSD field with an unsigned long long int value
	 * \param The DSD field name
	 * \param The DSD field value   
	 * \param The number of DSD(default is 1)
	 */
	void setDSDField(const std::string& , const unsigned long long int& ,u_int =1);

	/**
	 * \brief  Set a specific DSD field with a long int value
	 * \param The DSD field name
	 * \param The DSD field value   
	 * \param The number of DSD(default is 1)
	 */
	void setDSDField(const std::string& , const long int& ,u_int =1);

	/**
	 * \brief  Set a specific DSD field with an unsigned long int value
	 * \param The DSD field name
	 * \param The DSD field value   
	 * \param The number of DSD(default is 1)
	 */
	void setDSDField(const std::string& , const unsigned long int& ,u_int =1);

///////////////////////////
// MPH Fields get methods
///////////////////////////

	/**
	 * \brief  Get a specific MPH field value as a std::ostringstream
	 * \param std::string MPH field name
	 *		  std::ostringstream Return field value   
	 */
	void getMPHField(const std::string& , std::ostringstream& );

	/**
	 * \brief  Get a specific MPH field value as a std::string
	 * \param std::string MPH field name
	 *		  std::string Return field value   
	 */
	void getMPHField(const std::string& , std::string& );

	/**
	 * \brief  Get a specific MPH field value as a float
	 * \param std::string MPH field name
	 *		  float Return field value   
	 */
	void getMPHField(const std::string& , float& );

	/**
	 * \brief  Get a specific MPH field value as a int
	 * \param std::string MPH field name
	 *		  int Return field value   
	 */
	void getMPHField(const std::string& , int& );

	/**
	 * \brief  Get a specific MPH field value as a int
	 * \param MPH field name
	 *		  long int Return field value   
	 */
	void getMPHField(const std::string& , long int& );

	/**
	 * \brief  Get a specific MPH field value as a int
	 * \param MPH field name
	 *		  unsigned long int Return field value   
	 */
	void getMPHField(const std::string& , unsigned long int& );

	/**
	 * \brief  Get a specific MPH field value as a long long int
	 * \param std::string MPH field name
	 *		  long long int Return field value   
	 */
	void getMPHField(const std::string& , long long int& );

	/**
	 * \brief  Get a specific MPH field value as a unsigned long long int
	 * \param std::string MPH field name
	 *		  unsigned long long int Return field value   
	 */
	void getMPHField(const std::string& , unsigned long long int& );

///////////////////////////
// SPH Fields get methods
///////////////////////////

	/**
	 * \brief  Get a specific SPH field value as a std::ostringstream
	 * \param std::string SPH field name
	 *		  std::ostringstream Return field value   
	 */
	void getSPHField(const std::string& , std::ostringstream& );

	/**
	 * \brief  Get a specific SPH field value as a std::string
	 * \param std::string SPH field name
	 *		  std::string Return field value   
	 */
	void getSPHField(const std::string& , std::string& );

	/**
	 * \brief  Get a specific SPH field value as a float
	 * \param std::string SPH field name
	 *		  float Return field value   
	 */
	void getSPHField(const std::string& , float& );

	/**
	 * \brief  Get a specific SPH field value as a int
	 * \param std::string SPH field name
	 *		  int Return field value   
	 */
	void getSPHField(const std::string& , int& );

	/**
	 * \brief  Get a specific SPH field value as a int
	 * \param  string SPH field name
	 *		   long int return field value   
	 */
	void getSPHField(const std::string& , long int& );

	/**
	 * \brief  Get a specific SPH field value as a int
	 * \param  string SPH field name
	 *		   unsigned long int return field value   
	 */
	void getSPHField(const std::string& , unsigned long int& );

	/**
	 * \brief  Get a specific SPH field value as a long long int
	 * \param std::string SPH field name
	 *		  long long int Return field value   
	 */
	void getSPHField(const std::string& , long long int& );

	/**
	 * \brief  Get a specific SPH field value as an unsigned long long int
	 * \param std::string SPH field name
	 *		  unsigned long long int Return field value   
	 */
	void getSPHField(const std::string& , unsigned long long int& );			

///////////////////////////
// DSD Fields get methods
///////////////////////////

	/**
	 * \brief  Get a specific DSD field value as a std::ostringstream
	 * \param std::string DSD field name
	 *		  std::ostringstream Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , std::ostringstream& ,u_int =1);

	/**
	 * \brief  Get a specific DSD field value as a std::string
	 * \param std::string DSD field name
	 *		  std::string Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , std::string& ,u_int =1);

	/**
	 * \brief  Get a specific DSD field value as a int
	 * \param std::string DSD field name
	 *		  int Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , int& ,u_int =1);

	/**
	 * \brief  Get a specific DSD field value as a int
	 * \param std::string DSD field name
	 *		  long int Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , long int& ,u_int =1);

	/**
	 * \brief  Get a specific DSD field value as a int
	 * \param std::string DSD field name
	 *		  unsigned long int Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , unsigned long int& ,u_int =1);


	/**
	 * \brief  Get a specific DSD field value as a long long int
	 * \param std::string DSD field name
	 *		  long long int Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , long long int& ,u_int =1);

	/**
	 * \brief  Get a specific DSD field value as a unsigned long long int
	 * \param std::string DSD field name
	 *		  unsigned long long int Return field value   
	 *		  int The number of DSD(default is 1)
	 */
	void getDSDField(const std::string& , unsigned long long int& ,u_int =1);


///////////////////////////////////////
// XML Fixed Header Fields set methods
///////////////////////////////////////		

	/**
	 * \brief  Set a specific XML Fixed Header field to a std::string value
	 * \param std::string XML Fixed Header field name
	 *		  std::string XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const std::string& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML Fixed Header field to a float value
	 * \param std::string XML Fixed Header field name
	 *		  float XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const float& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML Fixed Header field to a int value
	 * \param std::string XML Fixed Header field name
	 *		  int XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML Fixed Header field to a long long int value
	 * \param std::string XML Fixed Header field name
	 *		  long int XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML Fixed Header field to a unsigned long long int value
	 * \param XML Fixed Header field name
	 *		  unsigned long int XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const unsigned long int& ,const unsigned int& );


	/**
	 * \brief  Set a specific XML Fixed Header field to a long long int value
	 * \param XML Fixed Header field name
	 *		  long long int XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const long long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML Fixed Header field to a unsigned long long int value
	 * \param std::string XML Fixed Header field name
	 *		  unsigned long long int XML Fixed Header field value   
	 *		  unsigned int The XML Fixed Header field number
	 */
	void setXmlFixedHeaderField(const std::string& , const unsigned long long int& ,const unsigned int& );

//////////////////////////////
// XML MPH Fields set methods
//////////////////////////////		

	/**
	 * \brief  Set a specific XML MPH field to a std::string value
	 * \param std::string XML MPH field name
	 *		  std::string XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const std::string& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MPH field to a float value
	 * \param std::string XML MPH field name
	 *		  float XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const float& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MPH field to a int value
	 * \param std::string XML MPH field name
	 *		  int XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MPH field to a long long int value
	 * \param std::string XML MPH field name
	 *		  long int XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MPH field to a unsigned long long int value
	 * \param std::string XML MPH field name
	 *		  unsigned long int XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const unsigned long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MPH field to a long long int value
	 * \param std::string XML MPH field name
	 *		  long long int XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const long long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MPH field to a unsigned long long int value
	 * \param std::string XML MPH field name
	 *		  unsigned long long int XML MPH field value   
	 *		  unsigned int The XML MPH field number
	 */
	void setXmlMPHField(const std::string& , const unsigned long long int& ,const unsigned int& );

//////////////////////////////
// XML SPH Fields set methods
//////////////////////////////		

	/**
	 * \brief  Set a specific XML SPH field to a std::string value
	 * \param std::string XML SPH field name
	 *		  std::string XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const std::string& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML SPH field to a float value
	 * \param std::string XML SPH field name
	 *		  float XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const float& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML SPH field to a int value
	 * \param std::string XML SPH field name
	 *		  int XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML SPH field to a long long int value
	 * \param std::string XML SPH field name
	 *		  long int XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML SPH field to a unsigned long long int value
	 * \param std::string XML SPH field name
	 *		  unsigned long int XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const unsigned long int& ,const unsigned int& );


	/**
	 * \brief  Set a specific XML SPH field to a long long int value
	 * \param std::string XML SPH field name
	 *		  long long int XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const long long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML SPH field to a unsigned long long int value
	 * \param std::string XML SPH field name
	 *		  unsigned long long int XML SPH field value   
	 *		  unsigned int The XML SPH field number
	 */
	void setXmlSPHField(const std::string& , const unsigned long long int& ,const unsigned int& );


//////////////////////////////
// XML MDH Fields set methods
//////////////////////////////		

	/**
	 * \brief  Set a specific XML MDH field to a std::string value
	 * \param std::string XML MDH field name
	 *		  std::string XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const std::string& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MDH field to a float value
	 * \param std::string XML MDH field name
	 *		  float XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const float& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MDH field to a int value
	 * \param std::string XML MDH field name
	 *		  int XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MDH field to a long long int value
	 * \param std::string XML MDH field name
	 *		  long int XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MDH field to a unsigned long long int value
	 * \param std::string XML MDH field name
	 *		  unsigned long int XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const unsigned long int& ,const unsigned int& );


	/**
	 * \brief  Set a specific XML MDH field to a long long int value
	 * \param std::string XML MDH field name
	 *		  long long int XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const long long int& ,const unsigned int& );

	/**
	 * \brief  Set a specific XML MDH field to a unsigned long long int value
	 * \param std::string XML MDH field name
	 *		  unsigned long long int XML MDH field value   
	 *		  unsigned int The XML MDH field number
	 */
	void setXmlMDHField(const std::string& , const unsigned long long int& ,const unsigned int& );


//////////////////////////////
// XML DSD Fields set methods
//////////////////////////////		

	/**
	 * \brief  Set a specific XML DSD field with a std::string value
	 * \param The XML DSD field name
	 * \param The XML DSD field value
	 * \param The XML DSD field number
	 * \param The number of XML DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const std::string&,const unsigned int&  ,u_int =1);

	/**
	 * \brief  Set a specific XML DSD field with a std::string value
	 * \param The XML DSD field name
	 * \param The XML DSD field value
	 * \param The XML DSD field number
	 * \param The number of XML DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const float&,const unsigned int&  ,u_int =1);

	/**
	 * \brief  Set a specific XML DSD field with a int value
	 * \param The XML DSD field name
	 * \param The XML DSD field value   
	 * \param The XML DSD field number
	 * \param The number of DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const int&,const unsigned int&  ,u_int =1);

	/**
	 * \brief  Set a specific XML DSD field with a long long int value
	 * \param The XML DSD field name
	 * \param The XML DSD field value   
	 * \param The XML DSD field number
	 * \param The number of XML DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const long long int&,const unsigned int&  ,u_int =1);

	/**
	 * \brief  Set a specific XML DSD field with an unsigned long long int value
	 * \param The XML DSD field name
	 * \param The XML DSD field value   
	 * \param The XML DSD field number
	 * \param The number of XML DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const unsigned long long int&,const unsigned int&  ,u_int =1);

	/**
	 * \brief  Set a specific XML DSD field with a long int value
	 * \param The XML DSD field name
	 * \param The XML DSD field value   
	 * \param The XML DSD field number
	 * \param The XML number of DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const long int&,const unsigned int&  ,u_int =1);

	/**
	 * \brief  Set a specific XML DSD field with an unsigned long int value
	 * \param The XML DSD field name
	 * \param The XML DSD field value   
	 * \param The XML DSD field number
	 * \param The number of XML DSD(default is 1)
	 */
	void setXmlDSDField(const std::string& , const unsigned long int&,const unsigned int&  ,u_int =1);


	/**
	 * \brief  Get the list of all MPH field names
	 * \retval vector<std::string> The list of all MPH field names
	 */
	std::vector<std::string> getMPHFieldsList();

	/**
	 * \brief  Get the list of all SPH field names
	 * \retval vector<std::string> The list of all SPH field names
	 */
	std::vector<std::string> getSPHFieldsList();

	/**
	 * \brief  Get the list of all DSD field names
	 * \retval vector<std::string> The list of all DSD field names
	 */
	std::vector<std::string> getDSDFieldsList();

	/**
	 * \brief  Get the list of all XML MPH field names
	 * \retval vector<std::string> The list of all XML MPH field names
	 */
	std::vector<std::string> getXmlMPHFieldsList();

	/**
	 * \brief  Get the list of all XML SPH field names
	 * \retval vector<std::string> The list of all XML SPH field names
	 */
	std::vector<std::string> getXmlSPHFieldsList();

	/**
	 * \brief  Get the list of all XML MDH field names
	 * \retval vector<std::string> The list of all XML MDH field names
	 */
	std::vector<std::string> getXmlMDHFieldsList();

	/**
	 * \brief  Get the list of all XML Fixed Header field names
	 * \retval vector<std::string> The list of all XML Fixed Header field names
	 */
	std::vector<std::string> getXmlFixedHeaderFieldsList();

	/**
	 * \brief  Read a Product Header file and load data in private structures
	 *    
	 */
	void read(const std::string& );

	/**
	 * \brief  Read an XML file, loading data in private structures. Only the
	 * 		   XML sections that have been initialized will be read from file.
	 */
	void readXML(const std::string& );

	/**
	 * \brief  Read an XML file starting from a given offset within the file. 
	 *         Only the XML sections that have been initialized will be read from file.
	 */
	void readXML(const std::string&, const int&);

	/**
	 * \brief  Copy to target ProductHeader object all the elements specified
	 *		   in the vector<int> specification. This vector holds int values
	 *		   with the following meaning:
	 *			0 means "copy MPH"
	 *			1 means "copy SPH"
	 *    		2 means "copy 1st DSD"
	 *			3 means "copy 2nd DSD"
	 *			and so on.
	 * \param ProductHeader target
	 *		  vector<int> Copy specification.
	 */
	void copy(ProductHeader &, const std::vector<int>&);

	/**
	 * \brief  This method writes the Product Header on ascii file specified
	 *		   by the first argument if second argument is not specified.
	 *		   Otherwise it writes the Product Header on ascii file specified
	 *		   by the first argument and then appends to it the content of the
	 *		   binary file specified by second argument.
	 *		   A ProductHeaderException exception is thrown if the 1st argument file exists.
	 *
	 * \param std::string The ascii file name where writing the Product header
	 *		  std::string A binary file name which content will be appended to the
	 *				 ascii file after Product Header writing.    
	 */
	void connectBinToPH(const std::string& , std::string = "");

	/**
	 * \brief  This method writes the Product Header on the head of the binary file 
	 *		   specified by the method argument.
	 *		   <b>This file must be a not empty binary file.</b>
	 *		   This method expects there is enough space left on the head of the 
	 *		   binary file for writing the Product Header. If there's not space 
	 *		   it is thrown a ProductHeaderException.
	 *		   <b>The MPH fields TOT_SIZE and DSD_SIZE value must be set otherwise an exception is thrown.</b>
	 *
	 * \param std::string The binary file name where writing the Product header on its head
	 *
	 */
	void writePH(const std::string& );

	/**
	 * \brief  This method writes the Product Header on the head of the binary file 
	 *		   specified by the method argument. This file can be "empty".
	 *		   This method does not make any check about free space on the head of file.
	 *
	 * \param std::string The binary file name where writing the Product header on its head
	 *
	 */
	void overwritePH(const std::string& );

	/**
	 * \brief This method is used to write the XML Product Header information into xmlFile passed in input. 
	 *		  If this file exists it is thrown an exception otherwise the file is created and XML generated. 
	 *		   
	 *
	 * \param std::string The file name where writing the XML Product header on its head
	 * \param std::string The XML file encoding type
	 *
	 */
	void writeXmlPH(const std::string&, std::string const & encoding="UTF-8" );

	/**
	 * \brief This method is used to write the XML Product Header information into xmlFile passed in input,
	 *        at a given offset. The default offset value is 0.
	 *		   
	 *
	 * \param std::string The file name where writing the XML Product header on its head
	 *		  int The offset to which write the XML header within the file
	 * \param std::string The XML file encoding type
	 */
	void overwriteXmlPH(const std::string&, int=0, std::string const & encoding="UTF-8");

	/**
	 * \brief This method is used to write the XML Product Header information at the end of binFile passed in input.
	 *		  If the file does't exist it is created.
	 *		   
	 *
	 * \param std::string The file name where writing the XML Product header on its head
	 * \param std::string The XML file encoding type
	 *
	 */
	void connectXmlToBin(const std::string& , std::string const & encoding="UTF-8");

	/**
	 * \brief Return the number of DSD initialized 
	 *		  
	 * \retval int Number of DSD initialized
	 *
	 */
	int getNumOfDSD();

	/**
	 * \brief Return the number of spare DSD defined
	 *		  
	 */
	int getNumOfSpares(){ if(_dsdSpareData.size()!=0) 
							return _dsdSpareData.size();
						  else return 0;
						};

	/**
	 * \brief Return the size of the Product Header (MPH+SPH+DSDs) 
	 *		  
	 */
	int getTotalHeaderSize()
	{ return ( getMPHSize() + getSPHSize() + getNumOfDSD() * getDSDSize() + getNumOfSpares() * getDSDSize() );};

	/**
	 * \brief Return the Product Header MPH size
	 *		  
	 */
	int getMPHSize() { if(_mphData.size() != 0) 
						return getSize(_mphData);
						else return 0;
					 };

	/**
	 * \brief Return the Product Header SPH size
	 *		  
	 */
	int getSPHSize(){ if(_sphData.size() != 0)
						return getSize(_sphData);
					  else return 0;
					};

	/**
	 * \brief Return the Product Header DSD size, that is
	 * the sizes of a single DSD
	 *		  
	 */
	int getDSDSize(){ if(_dsdData.size() != 0)
						return getSize(_dsdData[0]);
					  else return 0;
					};

	/**
	 * \brief Set the value of the XML Schema server URL attribute in XML Header
	 */
	void setXmlSchemaServerUrl(const std::string& s){_xmlSchemaServerUrl = s;};		


protected:

	// list of permitted field value
	struct  fieldPermittedValuesList
	{
		fieldPermittedValuesList();
		~fieldPermittedValuesList();
		unsigned int 		numberOfFieldValuesPermitted;
		std::vector<std::string> permittedValue;

		// copy constructor
		fieldPermittedValuesList(const fieldPermittedValuesList & ) ;

		// operator =
		fieldPermittedValuesList &operator=(const fieldPermittedValuesList & ); 

	} ;	  


	// this structure contains for each field value constraint these data: constraint index, constraint
	// value and the list of forced positions in field value of the same constraint
	struct fieldValueConstraint 
	{
		fieldValueConstraint();
		~fieldValueConstraint();

		std::string					fieldValueConstraintContent;
		unsigned int 				numberOfFieldValueConstraintPositions;
		std::vector<unsigned int> 	infoOnFieldValueConstraintPosition;

		// copy constructor
		fieldValueConstraint(const fieldValueConstraint & ) ;

		// operator = 
		fieldValueConstraint &operator=(const fieldValueConstraint & ); 


	} ;



	//this structure contains the list of constraints on writing a specific field value 
	struct fieldValueConstraintsList 
	{
		fieldValueConstraintsList();
		~fieldValueConstraintsList();
		unsigned int 						numberOfFieldValueConstraints;
		std::vector<fieldValueConstraint>	valueConstraint;

		// copy constructor 
		fieldValueConstraintsList(const fieldValueConstraintsList & ) ;

		// operator =
		fieldValueConstraintsList &operator=(const fieldValueConstraintsList & ); 

	} ;	


	// main structure
	struct  PHdata 
	{
		PHdata();
		~PHdata();

		std::string 				fieldName;
		std::string 				fieldValue;
		fieldPermittedValuesList	listOfFieldPermittedValues;	
		std::string					fieldValueFormat;
		fieldValueConstraintsList	listOfFieldValueConstraints;
		u_int 						fieldValueSize;
		std::string 				unitsFormat;
		std::string 				quotationMark;

		std::string dump (const std::string& tab="")
		{
			std::ostringstream o ; 
			o 	<< tab << "fieldName: \"" << fieldName << "\"\n" 
				<< tab << "fieldValue: \"" << fieldValue << "\"\n" 
				/*<< "fieldPermittedValuesList: \"" << fieldPermittedValuesList << "\"\n" */
				<< tab << "fieldValueFormat: \"" << fieldValueFormat << "\"\n" 
				/*<< "listOfFieldValueConstraints: \"" << listOfFieldValueConstraints << "\"\n" */
				<< tab << "fieldValueSize: " << fieldValueSize << "\n" 
				<< tab << "unitsFormat: \"" << unitsFormat << "\"\n" 
				<< tab << "quotationMark: \"" << quotationMark << "\"\n" ;
			return o.str() ;
		}

		/* copy constructor */
		PHdata(const PHdata & ); 

		/* operator = */
		PHdata &operator=(const PHdata &);

	} ;

	void load(const std::string& , std::vector<PHdata>& );

	void readNetCDFParams(const std::string& fileName) ;

	void readMPH(std::ifstream& );
	void readSPH(std::ifstream& );
	void readDSD(std::ifstream& );


	//flags used during "read" phase in order to establish if the various components have been read or not
	bool _mphRead;
	bool _sphRead;
	bool _dsdRead;

	unsigned int _numberOfDSD;
	unsigned int _totSize;

	// vectors holding infos loaded from resources
	std::vector<PHdata> 				_mphData;
	std::vector<PHdata> 				_sphData;
	std::vector< std::vector<PHdata> > 	_dsdData;
	std::vector<std::string> 			_dsdSpareData;

	// default DSD info used in "read" operation for appending DSDs 
	std::vector<PHdata> 	_dsdTemplate;


private:


	// data structure for XML info
	struct  XmlPHData 
	{
		XmlPHData();
		~XmlPHData();

		std::string 			tagType;
		u_int 					fieldNumber;
		PHdata 					data;		
		std::string 			tagName;
		u_int 					childrenNumber;
		std::vector<XmlPHData> 	children;

		//copy constructor
		XmlPHData(const XmlPHData & ); 

		// operator = 
		XmlPHData &operator=(const XmlPHData &);

	} ;

	std::string trim_right ( const std::string& source , const std::string & t = " " );
	//std::wstring trim_right ( const std::wstring & source ,const std::wstring & t = L" " );
	std::string trim_left ( const std::string& source ,const std::string & t = " " );
	//std::wstring trim_left ( const std::wstring & source ,const std::wstring & t = L" " );
	std::string trim ( const std::string& source ,const std::string & t = " " );
	//std::wstring trim ( const std::wstring & source ,const std::wstring & t = L" " );


	template<typename T> T fromString(const std::string& s) 
	{
	  std::string str = trim(s);
	  if(str == "")
		return 0;
	  std::istringstream is(str);
	  T t;
	  is >> t;
	  return t;
	}

	template<typename T> std::string toString(const T& t) 
	{
	  std::ostringstream s;
	  s.precision(6);
	  s.setf(std::ios::fixed, std::ios::floatfield);
	  s << t; 
	  return s.str();
	}



	// load default info for Product Header 
	void init(const rsResourceSet& ,const std::string&,std::vector<PHdata>& );
	// load default info for XML parts of Product Header
	void initXML(const rsResourceSet&,const std::string& ,std::vector<XmlPHData>&,unsigned int);

	void setField(const std::string& , const std::string& ,std::vector<PHdata>& );
	void setField(const std::string& , const float& ,std::vector<PHdata>& );
	void setField(const std::string& , const int& ,std::vector<PHdata>& );
	void setField(const std::string& , const long long int& ,std::vector<PHdata>& );
	void setField(const std::string& , const unsigned long long int& ,std::vector<PHdata>& );
	void setField(const std::string& , const long int& ,std::vector<PHdata>& );
	void setField(const std::string& , const unsigned long int& ,std::vector<PHdata>& );

	bool setXmlField(const std::string& , const std::string& ,std::vector<XmlPHData>&, const unsigned int& );
	bool setXmlField(const std::string& , const float& ,std::vector<XmlPHData>&, const unsigned int& );
	bool setXmlField(const std::string& , const int& ,std::vector<XmlPHData>&, const unsigned int& );
	bool setXmlField(const std::string& , const long long int& ,std::vector<XmlPHData>&, const unsigned int& );
	bool setXmlField(const std::string& , const long int& ,std::vector<XmlPHData>&, const unsigned int& );
	bool setXmlField(const std::string& , const unsigned long long int& ,std::vector<XmlPHData>&, const unsigned int& );
	bool setXmlField(const std::string& , const unsigned long int& ,std::vector<XmlPHData>&, const unsigned int& );

	void write(const std::vector<PHdata>& , std::ofstream& );
	void writeXML(std::ofstream&, std::string const & encoding );

	//std::string writeXMLData(vector<XmlPHData>& );
	void writeXMLData(std::vector<XmlPHData>&, XMLOstream&);

	bool checkFieldValueConstraints(const std::string& , const PHdata& );

	std::string getFieldValue(const std::string& , const std::vector<PHdata>& );


	void loadXmlFixedHeaderData(const std::string&,const rsResourceSet&,std::vector<XmlPHData>&,int count);
	void loadXmlMPHData(const std::string&,const rsResourceSet&,std::vector<XmlPHData>&,int count);
	void loadXmlSPHData(const std::string&,const rsResourceSet&,std::vector<XmlPHData>&,int count);
	void loadXmlMDHData(const std::string&,const rsResourceSet&,std::vector<XmlPHData>&,int count);
	void loadXmlDSDData(const std::string&,const rsResourceSet&,std::vector< std::vector<XmlPHData> >&,int count);
	void loadXmlDSDSingleData(const std::string&,const rsResourceSet&,std::vector<XmlPHData>&,int count, const unsigned int&);

	void checkDataInizialization();
	void checkXMLDataInizialization();

	void retrieveFieldNames(std::vector<XmlPHData>&, std::vector<std::string>&);

	int getSize(const std::vector<PHdata>&);

	bool isXmlFixedHeaderInitialized(){return(_xmlFixedHeaderData.size()>0);};
	bool isXmlMPHInitialized(){return(_xmlMphData.size()>0);};
	bool isXmlSPHInitialized(){return(_xmlSphData.size()>0);};
	bool isXmlDSDInitialized(){return(_xmlDsdData.size()>0);};
	bool isXmlMDHInitialized(){return(_xmlMdhData.size()>0);};



// private members
private:


	/* std::string separator of NAME from VALUE in PH file 
	 * This value is read from MPH resource during its init phase
	 */
	std::string  _separator;


	std::string  							_xmlSchemaServerUrl;
	std::vector<XmlPHData> 					_xmlMphData;
	std::vector<XmlPHData> 					_xmlSphData;
	std::vector<XmlPHData> 					_xmlMdhData;
	std::vector<XmlPHData> 					_xmlFixedHeaderData;
	std::vector< std::vector<XmlPHData> > 	_xmlDsdData;

	std::vector<XmlPHData>  _xmlDsdTemplate;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProductHeader) ;

};

_ACS_END_NAMESPACE

#endif //_ProductHeader_H_
