// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer WS Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2015/11/19 15:44:28  marpas
	coding best practices and rule applied (partially)
	
	Revision 2.5  2013/03/11 16:27:09  marfav
	Compiler warning reduced
	
	Revision 2.4  2013/03/11 16:24:49  marfav
	Compiler warning reduced
	
	Revision 2.3  2007/03/13 14:27:00  fracar
	fixed bug in OrderTypeTranslation
	
	Revision 2.2  2006/04/21 12:55:43  marfav
	Log level managment logic improved
	
	Revision 2.1  2006/04/21 10:22:06  marfav
	Standard output and standard error indipendent log levels support added
	
	Revision 2.0  2006/02/28 10:10:54  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/08/31 11:18:26  marfav
	WSConfig message fixed. Some ordertypes were reported always as disabled.
	
	Revision 1.2  2005/06/28 08:31:42  marfav
	Multiple WSConfig schemas supported
	OrderType translation supported by libTlOrder manager
	
	Revision 1.1  2004/12/23 17:35:01  marfav
	Importing WSConfigMessage
	

*/


#include <WSConfigMessage.h>
#include <OrderTypeMgr.h>

#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {

// key used when dumping object to stream
const string _statusKey = "Status";
const string _confFileKey = "Config_File";
const string _defConfVerKey = "Default_Config_Version";
const string _messageKey = "Message";
const string _prcArrayKey = "List_of_Processors";         // to be used with no namespace
const string _prcKey      = "Processor";       // to be used inside namespace _prcArrayKey
const string _prcNameKey      = "Processor_Name";  // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcVersionKey   = "Version";         // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcLogLevelKey  = "Log_Level";       // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcOutLogLevelKey   = "Stdout_Log_Level";        // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcErrLogLevelKey   = "Stderr_Log_Level";        // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcTaskTableKey = "Task_Table";      // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcOrdersKey    = "Orders";      // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcSystematicKey    = "Systematic";      // to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcNRTKey       = "NRT";         // to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcReprocessKey = "Reprocess";       // to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcTrblshootKey = "Trblshoot";       // to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcSUBSKey  = "SUBS";        // to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _enabledString = "enabled";
const string _disabledString = "disabled";

// APF-411
const string _posixExitCode = "Posix_Exit_Code";

// values used in legacy mode for retrieve settings
const string _typeSystematic  = "OFFL";
const string _typeNRT     = "NRT";       
const string _typeReprocess   = "RPRO"; 
const string _typeTrblshoot   = "Trblshoot"; 
const string _typeSUBS        = "SUBS";      

}


// class WSConfigMessageElement Implementation

WSConfigMessageElement::WSConfigMessageElement ():
    WSConfigElement(),
	message(),
	status(),
	defConfigVersion(),
	defConfigFile()
{
}

WSConfigMessageElement::WSConfigMessageElement (const WSConfigElement& e):
    WSConfigElement(e),
	message(),
	status(),
	defConfigVersion(),
	defConfigFile()
{
}


WSConfigMessageElement::WSConfigMessageElement (const WSConfigMessageElement& e):
    WSConfigElement(e),
	message(e.message),
	status(e.status),
	defConfigVersion(e.defConfigVersion),
	defConfigFile(e.defConfigFile)
{
}


WSConfigMessageElement&
WSConfigMessageElement::operator = (const WSConfigMessageElement &e)
{
	if (this != &e)
	{
		WSConfigElement::operator= (e) ;
		message = e.message;
		status = e.status;
		defConfigVersion = e.defConfigVersion;
		defConfigFile = e.defConfigFile;
	}
	return *this;
}

bool
operator == (const WSConfigMessageElement& a, const WSConfigMessageElement& b)
{
    const WSConfigElement &aa = a ;
    const WSConfigElement &bb = b ;
   
    return (( aa == bb ) &&
	        (a.message           == b.message) &&
	        (a.status            == b.status) &&
	        (a.defConfigVersion  == b.defConfigVersion) &&
	        (a.defConfigFile     == b.defConfigFile));
}


string
WSConfigMessageElement::statusType2string (statusType status) // PRQA S 4020
{
	switch (status)
	{
		case OK: { return "OK"; }
		case DISABLED: { return "DISABLED"; }
		case NO_TT: { return "NO_TT"; }
		case BAD_TT: { return "BAD_TT"; }
		default: { return "UNKNOWN"; }
	}
}


// Class WSConfigMessage Implementation

WSConfigMessage::WSConfigMessage (const WSConfig& w) :
	_legacyMode(),
	_wsMessages()
{
	_wsMessages.clear();
	for ( vector<WSConfigElement>::const_iterator it = w.begin(); it != w.end(); ++it)
	{
		_wsMessages.push_back (WSConfigMessageElement(*it)); // PRQA S 3081
	}
	_legacyMode = w.isLegacy();
}

ostream & operator << (ostream &os, const WSConfigMessage &m) 
{
    m.writeTo(os) ;
    return os ; // PRQA S 4028
}


void WSConfigMessage::writeTo (std::ostream& o) const
{
	XMLOstream xo(o);

	XMLOstream::Tag roottag (xo, "Proc_Table", "", "");

	ostringstream os;
	os << "count=\"" << size() << "\"";

	XMLOstream::Tag procarrtag (xo, _prcArrayKey, "", os.str());


	//{ XMLOstream::Tag t(_theOstream, _defs._inputMandatoryKey, desc.isMandatory() ? "Yes" : "No", "", true); } // PRQA S 3380

    for (size_t i=0; i<_wsMessages.size(); i++)
    {
	    const WSConfigMessageElement& ec = _wsMessages[i];

		XMLOstream::Tag proctag (xo, _prcKey, "");

		{ XMLOstream::Tag t(xo, _prcNameKey , ec.name , "", true); } // PRQA S 3380
		{ XMLOstream::Tag t(xo, _prcVersionKey , ec.version , "", true); } // PRQA S 3380
		{ XMLOstream::Tag t(xo, _prcLogLevelKey , ec.logLevel , "", true); } // PRQA S 3380
		
	    if (ec.outLogLevel != "")
	    {
			{ XMLOstream::Tag t(xo, _prcOutLogLevelKey , ec.outLogLevel , "", true); } // PRQA S 3380
			{ XMLOstream::Tag t(xo, _prcErrLogLevelKey , ec.errLogLevel , "", true); } // PRQA S 3380
	    }

		{ XMLOstream::Tag t(xo, _prcTaskTableKey , ec.taskTable , "", true); } // PRQA S 3380

		// orcOrdersKeyTag
		{
			XMLOstream::Tag procorderstag (xo, _prcOrdersKey, "");

			OrderTypeMgr otm;
			string s;

			if (_legacyMode) 
			{
		
				map <string, bool> ots = ec.orderTypeStatus;	

				{ XMLOstream::Tag t(xo, _prcSystematicKey , (ots[_typeSystematic] ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcNRTKey        , (ots[_typeNRT]        ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcReprocessKey  , (ots[_typeReprocess]  ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcTrblshootKey  , (ots[_typeTrblshoot]  ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcSUBSKey       , (ots[_typeSUBS]       ? _enabledString : _disabledString) , "", true); } // PRQA S 3380

			}
			else
			{

			    map <string, bool> orderStatusMap = ec.orderTypeStatus;
			    map <string, bool>::iterator it;
			    for (it = orderStatusMap.begin(); it != orderStatusMap.end(); ++it) 
			    {
				    s = (it->second ? _enabledString : _disabledString); // PRQA S 3380
					{ XMLOstream::Tag t(xo, otm.getWSConfigType(it->first) , s , "", true); } // PRQA S 3380
			    }
			}
		}

		{ XMLOstream::Tag t(xo, _confFileKey   , ec.defConfigFile , "", true); } // PRQA S 3380
		{ XMLOstream::Tag t(xo, _defConfVerKey , ec.defConfigVersion , "", true); } // PRQA S 3380
		{ XMLOstream::Tag t(xo, _posixExitCode , (ec.hasPosixExitCode ? "true":"false") , "", true); } // PRQA S 3380

		{ XMLOstream::Tag t(xo, _statusKey     , ec.statusType2string(ec.status) , "", true); } // PRQA S 3380
		{ XMLOstream::Tag t(xo, _messageKey    , ec.message , "", true); } // PRQA S 3380

    }
}

const WSConfigMessageElement& WSConfigMessage::operator [] (const int& s) const
{
	return _wsMessages[s];
}
const WSConfigMessageElement& WSConfigMessage::at (const int & s) const
{
	return _wsMessages.at(s);
}

size_t WSConfigMessage::size() const
{
	return _wsMessages.size();
}

void WSConfigMessage::clear()
{
	_wsMessages.clear();
}
void WSConfigMessage::push_back(WSConfigMessageElement & t)
{
	_wsMessages.push_back(t);
}
WSConfigMessage::operator const std::vector<WSConfigMessageElement>& () const
{
	return _wsMessages;
}
WSConfigMessage::operator std::vector<WSConfigMessageElement>& () 
{
	return _wsMessages;
}
std::vector<WSConfigMessageElement>::const_iterator WSConfigMessage::begin() const
{
	return _wsMessages.begin();
}
std::vector<WSConfigMessageElement>::const_iterator WSConfigMessage::end() const
{
	return _wsMessages.end();
}
std::vector<WSConfigMessageElement>::iterator WSConfigMessage::begin() 
{
	return _wsMessages.begin();
}
std::vector<WSConfigMessageElement>::iterator WSConfigMessage::end() 
{
	return _wsMessages.end();
}

_ACS_END_NAMESPACE
