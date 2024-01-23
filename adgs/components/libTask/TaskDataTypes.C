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

#include <TaskDataTypes.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;



string InputOrigin2String(InputOrigin o) // PRQA S 4020
{                           	
    switch (o)
    {
	case InputOriginDB:   { return "DB"; }
	case InputOriginPROC: { return "PROC"; }
	case InputOriginLOG:  { return "LOG"; }
	default: { return "UNKNOWN"; }
    }
}

InputOrigin String2InputOrigin (const string& s) // PRQA S 4020
{
	if (s == "DB") {
		return InputOriginDB;
    }
	else if (s == "PROC") {
		return InputOriginPROC;
    }
	else if (s == "LOG") {
		return InputOriginLOG;
    }
	else {
		return InputOriginUNKNOWN;
    }
}



string OutputDest2String(OutputDest d) // PRQA S 4020
{
	switch (d)
	{
	case OutputDestDB:  { return "DB"; }
	case OutputDestPROC: { return "PROC"; }
	case OutputDestDBPROC: { return "DBPROC"; }
	default: { return "UNKNOWN"; }
	}
}

OutputDest String2OutputDest (const string& s) // PRQA S 4020
{
	if (s == "DB") {
		return OutputDestDB;
    }
	else if (s == "PROC") {
		return OutputDestPROC;
    }
	else if (s == "DBPROC") {
		return OutputDestDBPROC;
    }
	else {
		return OutputDestUNKNOWN;
    }
}



string FileNameType2String (FileNameType d) // PRQA S 4020
{
	switch (d)
	{
	case FileNameTypeLOGICAL: { return "Logical"; }
	case FileNameTypeSTEM: { return "Stem"; }
	case FileNameTypePHYSICAL: { return "Physical"; }
	case FileNameTypeDIRECTORY: { return "Directory"; }
	case FileNameTypeREGEXP: { return "Regexp"; }
	default: { return "UNKNOWN"; }
	}
}

FileNameType String2FileNameType (const string& s) // PRQA S 4020
{
	if (s == "Logical") {
		return FileNameTypeLOGICAL;
    }
	else if (s == "Physical") {
		return FileNameTypePHYSICAL;
    }
	else if (s == "Stem") {
		return FileNameTypeSTEM;
    }
	else if (s == "Directory") {
		return FileNameTypeDIRECTORY;
    }
	else if (s == "Regexp") {
		return FileNameTypeREGEXP;
    }
	else {
		return FileNameTypeUNKNOWN;
    }
}

string ExitStatus2String (ExitStatus s) // PRQA S 4020

{
	switch (s)
	{
	case ExitStatusOK: { return "OK"; }
	case ExitStatusERROR: { return "ERROR"; }
	case ExitStatusABORT: { return "ABORT"; }
	default: { return "UNKNOWN"; }
	}
}


ExitStatus String2ExitStatus (const string& s) // PRQA S 4020
{
	if (s == "OK") {
		return ExitStatusOK;
    }
	else if (s == "ERROR") {
		return ExitStatusERROR;
    }
	else if (s == "ABORT") {
		return ExitStatusABORT;
    }
	else {
		return ExitStatusUNKNOWN;
    }
}


_ACS_END_NAMESPACE
