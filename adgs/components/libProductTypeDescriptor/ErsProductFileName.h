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
	Revision 1.5  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 1.4  2012/03/12 10:19:18  giucas
	Some compile warnings fixed
	
	Revision 1.3  2008/08/22 11:39:56  giucas
	Methods for L1 implemented. Note that for L0 the Naming Convention is ESA NAMING CONVENTION while for L1 is ENVISAT NAMING CONVENTION
	
	Revision 1.2  2008/07/25 16:38:20  giucas
	Fixed erroneous call
	
	Revision 1.1  2008/07/22 13:26:25  giucas
	First issue
	

*/

#ifndef _ErsProductFileName_H_
#define _ErsProductFileName_H_ 

#include <acs_c++config.hpp>
#include <ProductFileName.h>
#include <rsResourceSet.h>
#include <exException.h>
#include <ESAProductFileName.h>

_ACS_BEGIN_NAMESPACE(acs)

class ErsProductFileName : public ProductFileName
{
public:

	/**
	 * \brief  Default Class constructor. Using this constructor the class
	 *		   data are read from Configuration Space
	 */	
	ErsProductFileName();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsProductFileName() throw();


/////////////////////////////////////////////		
//public methods
/////////////////////////////////////////////

	/**
	 * \brief Return a list of Product Names given a Product Id  
	 */
	virtual void getNamesById(const std::string&, std::vector<std::string>&){} // PRQA S 2131

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product Id  
	 */
	virtual void getNamesById(const std::string&, const std::string&, const std::string&, std::vector<std::string>&){} // PRQA S 2131


	/**
	 * \brief Return a list of Product Names given a Mission Id  and a Product Id
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, std::vector<std::string>&){} // PRQA S 2131

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Mission Id and a Product Id 
	 */
	virtual void getNamesByMission(const std::string&, const std::string&, const std::string&, const std::string&, std::vector<std::string>&){} // PRQA S 2131


	/**
	 * \brief Return a list of Product Names given a Product file extension and a Product Id 
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, std::vector<std::string>&){} // PRQA S 2131

	/**
	 * \brief Return a list of Product Names including Sensing Start and Stop times
	 *        given a Product file extension and a Product Id  
	 */
	virtual void getNamesByExtension(const std::string&, const std::string&, const std::string&, const std::string&, std::vector<std::string>&){} // PRQA S 2131

	/**
	 * \brief  Return true if the string in input represent a valid L0 or L1 Product name, false otherwise. 
	 */
	virtual bool isValidName(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid file extension, false otherwise. 
	 */
	virtual bool isValidExtension(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid L0 or L1 Root name, false otherwise. 
	 */
	virtual bool isValidRoot(const std::string&);


	/**
	 * \brief  Return true if the string in input represent a valid L0 Product name (ESA NAMING CONVENTION), false otherwise. 
	 */
	virtual bool isValidL0Name(const std::string& );

	/**
	 * \brief  Return true if the string in input represent a valid L0 Root name (ESA NAMING CONVENTION), false otherwise. 
	 */
	virtual bool isValidL0Root(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid L1 Product name (ENVISAT NAMING CONVENTION), false otherwise. 
	 */
	virtual bool isValidL1Name(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid L1 Root name (ENVISAT NAMING CONVENTION), false otherwise. 
	 */
	virtual bool isValidL1Root(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid BWS Product name, false otherwise. 
	 */
	virtual bool isValidBWSName(const std::string&);

	/**
	 * \brief  Return true if the string in input represent a valid BWS Root name, false otherwise. 
	 */
	virtual bool isValidBWSRoot(const std::string&);


	/**
	 * \brief Return a Product file name given a Product name root AA_BBBB_CCCCCCCCCC,
	 *		  start/stop dates in string format (yyyymmddThhnnss) and the file extension
	 */
	virtual void getName(const std::string&, const std::string&, const std::string&, const std::string&, std::string&){} // PRQA S 2131


	/**
	 * \brief Return a Product file name given a Product name root AA_BBBB_CCCCCCCCCC,
	 *		  start/stop dates in UTC double format and the file extension
	 */
	virtual void getName(const std::string&, double, double, const std::string&, std::string&){} // PRQA S 2131


	/**
	 * \brief Return a Product file name given a Product name root AA_BBBB_CCCCCCCCCC
	 *		  and start/stop dates in string format (yyyymmddThhnnss)
	 * \param The filename root
	 * \param The Start Time in string format
	 * \param The Stop Time in string format
	 * \param The Phase <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The Cycle <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The Relative orbit <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The Absolute orbit <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The counter <b> ( NOT USED FOR CRYOSAT )</b>
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
						);

	/**
	 * \brief Return a Product file name given a Product name root AA_BBBB_CCCCCCCCCC
	 *		  and start/stop dates in UTC double format
	 * \param The filename root
	 * \param The Start Time in MJD2000 format
	 * \param The Stop Time in MJD2000 format
	 * \param The Phase <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The Cycle <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The Relative orbit <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The Absolute orbit <b> ( NOT USED FOR CRYOSAT )</b>
	 * \param The counter <b> ( NOT USED FOR CRYOSAT )</b>
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
						);

	/**
	 * \brief  Return the Product Id given a 'short' Product File Name Root AA_BBBB_CCCCCCCCCC.EEE.
	 *		   If the file name is not a valid Level0 Product File Name Root an exception is thrown.
	 */
	virtual std::string getProductIDFromRoot(const std::string&){return"";} // PRQA S 2131

	/**
	 * \brief  Return the Product Id given a 'complete' Product File Name.
	 *		   If the file name is not a valid Level0 Product File Name an exception is thrown.
	 */
	virtual std::string getProductIDFromName(const std::string&){return"";} // PRQA S 2131

	/**
	 * \brief  Return Level0 Default File Name Version.
	 */
	virtual std::string getDefaultFileNameVersion(){return "";} // PRQA S 2131

	/**
	 * \brief  Return the Mission ID.
	 */
	virtual std::string getMissionID(){return "";} // PRQA S 2131

///////////////////////////////////////////////////////////
// useful methods to extract info from complete file names
///////////////////////////////////////////////////////////

	/**
	 * \brief  Return File Name Version.
	 * \param  The complete product file name
	 */
	virtual std::string getVersion(const std::string&){return"";} // PRQA S 2131

	/**
	 * \brief  Return File Name Class.
	 * \param  The complete product file name
	 */
	virtual std::string getFileClass(const std::string&){return"";} // PRQA S 2131

	/**
	 * \brief  Return File Name Class.
	 * \param  The product file name root
	 */
	virtual std::string getFileClassFromRoot(const std::string&){return"";} // PRQA S 2131

	/**
	 * \brief  Return File Name Validity Start Time as a MJD2000.
	 * \param  The complete product file name
	 */
	virtual long double getStartTime(const std::string&){return 0.0;} // PRQA S 2131

	/**
	 * \brief  Return File Name Validity Stop Time as a MJD2000.
	 * \param  The complete product file name
	 */
	virtual long double getStopTime(const std::string&){return 0.0;} // PRQA S 2131

	/**
	 * \brief  Return a product file name with the new version.
	 * \param  The complete product file name
	 * \param  The new file name version
	 */
	virtual std::string nameWVersion(const std::string&,const std::string&){return "";} // PRQA S 2131

	/**
	 * \brief  Return OriginatorId <b> ONLY FOR ENVISAT </b>.
	 *        <b> The File Name Class format depends on specific implementation </b>
	 * \param  The product file name root
	 */
	virtual std::string getOriginatorFromRoot(const std::string&){return "";} // PRQA S 2131

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
	virtual void getName(const std::string&,// PRQA S 2131
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
	ErsProductFileName(const ErsProductFileName & ); 


	/**
	 * \brief  Operator=
	 */
	ErsProductFileName &operator=(const ErsProductFileName &);


/////////////////////////////////////////////		
//		private methods
/////////////////////////////////////////////

	//load data from configuration space
	void init();

	//given start and stop times return the difference in seconds
	std::string getDuration(const std::string&, const std::string&);

	// return a root for file name including product id, Processing Stage and originatorId
	std::string getRoot(const std::string& fClass, const std::string& prodId, const std::string& originatorId) { return prodId + fClass + originatorId; } ;

	bool checkNameValidity(const std::string&, std::vector<std::string>&);
	bool checkRootValidity(const std::string&, std::vector<std::string>&);

/////////////////////////////////////////////		
//		private attributes
/////////////////////////////////////////////

	rsResourceSet&				_rsConf;
	std::string 				_missionId;
	std::string				_fileNameVersion;
	unsigned int 				_rootSize;
	unsigned int 				_fileSize;

	//the list of permitted originators
	std::vector<std::string> 	_permittedOriginators;
	//the list of permitted file extensions
	std::vector<std::string> 	_permittedExtensions;
	//the list of permitted file classes
	std::vector<std::string> 	_permittedClasses;		
	//valid L0 file names (roots)
	std::vector<std::string>	_validL0FileNames;
	//valid L1 file names (roots)
	std::vector<std::string>	_validL1FileNames;
	//valid BWS file names (roots)
	std::vector<std::string>	_validBWSFileNames;
	//valid file names
	std::vector<std::string>	_validFileNames;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ErsProductFileName) ;
};

_ACS_END_NAMESPACE

#endif //_ErsProductFileName_H_
