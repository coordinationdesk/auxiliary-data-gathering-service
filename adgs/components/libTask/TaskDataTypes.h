// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _TASKDATATYPES_H_
#define _TASKDATATYPES_H_

#include <acs_c++config.hpp>
#include <string>
	

_ACS_BEGIN_NAMESPACE(acs)

/**
 * Here are defined some common types used in different
 * task table element definition
 * The types are defined as enumeratives and are 
 * supported by converting functions (to and/or from string)
 */ 	
	
enum InputOrigin
{
	InputOriginDB,
	InputOriginPROC,
	InputOriginLOG,
	InputOriginUNKNOWN
};
std::string InputOrigin2String(InputOrigin o);
InputOrigin String2InputOrigin (const std::string&);

enum OutputDest 
{
	OutputDestDB, 
	OutputDestPROC,
	OutputDestDBPROC,
	OutputDestUNKNOWN
};
std::string OutputDest2String(OutputDest d);
OutputDest String2OutputDest (const std::string&);

enum FileNameType
{
	FileNameTypePHYSICAL,
	FileNameTypeLOGICAL,
	FileNameTypeSTEM,
	FileNameTypeDIRECTORY,
	FileNameTypeREGEXP,
	FileNameTypeUNKNOWN
};
std::string FileNameType2String(FileNameType d);
FileNameType  String2FileNameType (const std::string& s);

enum ExitStatus
{
	ExitStatusOK,
	ExitStatusERROR,
	ExitStatusABORT,
	ExitStatusUNKNOWN
};
std::string ExitStatus2String(ExitStatus s);
ExitStatus  String2ExitStatus (const std::string& s);


_ACS_END_NAMESPACE

#endif //_TASKDATATYPES_H_

