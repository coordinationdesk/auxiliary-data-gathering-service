// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	
	

*/

#ifndef _EarthCAREWithVersProductFileName_H_
#define _EarthCAREWithVersProductFileName_H_ 

#include <ProductFileName.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class EarthCAREWithVersProductFileName : public ProductFileName
{
public:

	/**
	 * \brief  Default Class constructor
	 */	
	EarthCAREWithVersProductFileName();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~EarthCAREWithVersProductFileName() throw();

/////////////////////////////////////////////		
//public methods
/////////////////////////////////////////////

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getNamesById(const std::string&, std::vector<std::string>&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getNamesById(const std::string&, const std::string&, const std::string&, std::vector<std::string>&);


	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, std::vector<std::string>&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, const std::string&, const std::string&, std::vector<std::string>&);


	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, std::vector<std::string>&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, const std::string&, const std::string&, std::vector<std::string>&);

	/**
	 * \brief  Return true if the string in input represent a valid EarthCARE product (version in name), false otherwise. 
	 */
	virtual bool isValidName(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual bool isValidExtension(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual bool isValidRoot(const std::string&);


	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual bool isValidL0Name(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual bool isValidL0Root(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual bool isValidL1Name(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual bool isValidL1Root(const std::string&);



	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getName(const std::string&, const std::string&, const std::string&, const std::string&, std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getName(const std::string&, double, double, const std::string&, std::string&);

	/**
	 * \brief Not implemented: throw exception  
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
						);

	/**
	 * \brief Not implemented: throw exception  
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
						);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getProductIDFromRoot(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getProductIDFromName(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getDefaultFileNameVersion();

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getMissionID();

///////////////////////////////////////////////////////////
// useful methods to extract info from complete file names
///////////////////////////////////////////////////////////

	/**
	 * \brief  Return File Name Version.
	 * \param  The complete product file name
	 */
	virtual std::string getVersion(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getFileClass(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getFileClassFromRoot(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual long double getStartTime(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual long double getStopTime(const std::string&);

	/**
	 * \brief  Return a product file name with the new version.
	 * \param  The complete product file name
	 * \param  The new file name version
	 */
	virtual std::string nameWVersion(const std::string&,const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual std::string getOriginatorFromRoot(const std::string&);

	/**
	 * \brief Not implemented: throw exception  
	 */
	virtual void getName(const std::string&, // PRQA S 2131
						 double,
						 double,
						 unsigned int, 
						 unsigned int,
						 const std::string&,
						 std::string&
						) {}

private:
	/**
	 * \brief  Copy constructor
	 */
	EarthCAREWithVersProductFileName(const EarthCAREWithVersProductFileName & ); 


	/**
	 * \brief  Operator=
	 */
	EarthCAREWithVersProductFileName &operator=(const EarthCAREWithVersProductFileName &);



/////////////////////////////////////////////		
//           private methods
/////////////////////////////////////////////

	bool checkNameValidity(const std::string&, size_t & pos);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EarthCAREWithVersProductFileName) ;
};

_ACS_END_NAMESPACE


#endif //_EarthCAREWithVersProductFileName_H_
