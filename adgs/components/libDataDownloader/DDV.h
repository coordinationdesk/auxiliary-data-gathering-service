// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2015, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: DataDownloader VERSIONING $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 *	$Log$
 *	Revision 1.3  2013/09/30 12:51:48  marpas
 *	minor changes
 *	
 *	Revision 1.2  2013/09/30 12:45:55  marpas
 *	fixing version file
 *	
 *	Revision 1.1.1.1  2013/05/16 09:35:23  enrcar
 *	Imported source
 *	
 *	
 *
 */

 
#ifndef _DDV_H_
#define _DDV_H_

namespace acs {

// support class for versioning

class DDV {
public:
	DDV() throw() ;
	~DDV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	DDV(const DDV &);
	DDV &operator=(const DDV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _DDV_H_



