/*
	Copyright 1995-2021, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libFilenamePreInventory$
        
*/

#ifndef _S3DimsaxFilenamePreInventory_H_
#define _S3DimsaxFilenamePreInventory_H_

#include <FilenamePreInventory.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* PreInventory plugin for S3 SL_2_DIMSAX
**/

class S3DimsaxFilenamePreInventory: public FilenamePreInventory  {  // PRQA S 2109
public:

	/** Default Class constructor */
	S3DimsaxFilenamePreInventory();
	/** Destructor */
	virtual ~S3DimsaxFilenamePreInventory() ACS_NOEXCEPT ;

	/** I/F implementation */
	virtual std::string getClassId();

	ACS_DECLARE_NO_COPY_CLASS(S3DimsaxFilenamePreInventory);

protected:
	/** I/F implementation */
	virtual void parseFilename( const std::string &, const std::string&, FilenameElements & );

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S3DimsaxFilenamePreInventory)

};

_ACS_END_NAMESPACE

#endif //_S3DimsaxFilenamePreInventory_H_

