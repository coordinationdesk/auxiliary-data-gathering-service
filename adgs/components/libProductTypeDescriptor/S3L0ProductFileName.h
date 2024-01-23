// PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. S3 Product File name helper class $
	
	$Id$

	$Author$
	
	$Log$
	Revision 2.3  2013/12/19 09:49:52  giucas
	S3PDGS-1721 : Naming convention aligned to [CFi-32.1] ls1r3C
	
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.1  2011/11/16 14:27:45  giucas
	Added S3L0ProductFileName
	

*/

#ifndef _S3L0ProductFileName_H_
#define _S3L0ProductFileName_H_ 

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class S3L0ProductFileName 
{
public:

	/*! class S3L0ProductFileNameException declaration */
	exDECLARE_EXCEPTION(S3L0ProductFileNameException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor.
	 *    
	 */	
	S3L0ProductFileName();

	/**
	 * \brief  Class destructor 
	 *    
	 */
	~S3L0ProductFileName() throw();


/////////////////////////////////////////////		
//public methods
/////////////////////////////////////////////


	/**
	 * \brief Set the mission field 
	 * <b> The mission field </b>  
	 */
	void setMission(const std::string& mission);

	/**
	 * \brief Set the file type field 
	 * <b> The file type field </b>  
	 */
	void setFileType(const std::string& fileType);

	/**
	 * \brief Set the file validity
	 * <b> The file sensing start and stop times </b>  
	 */
	void setValidity(double startTime, double stopTime);

	/**
	 * \brief Set the information for instance id
	 * <b> The information for instance id field </b>  
	 */
	void setInstanceId(int duration, int cycle, int relOrbit, int frameAlongTrack=0);

	/**
	 * \brief Set the file version
	 * <b> The file version field </b>  
	 */
	void setExtension(const std::string& ext);

	/**
	 * \brief build and retrun the complete file name
	 */
	std::string getName() const;


private:

	S3L0ProductFileName(const S3L0ProductFileName&) ;
	S3L0ProductFileName& operator=(const S3L0ProductFileName&) ;

	std::string		_mission ;
	std::string		_fileType ;
	std::string		_startTime ;
	std::string		_stopTime ;
	std::string		_currentTime ;
	std::string		_instanceId ;
	std::string		_ext ;
	std::string		_siteCentre ;
	std::string		_platform ;
	std::string		_timeliness ;
	std::string		_usage ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3L0ProductFileName) ;

};


_ACS_END_NAMESPACE


#endif //_S3L0ProductFileName_H_
