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
	Revision 2.11  2015/11/19 15:44:28  marpas
	coding best practices and rule applied (partially)
	
	Revision 2.10  2015/06/17 10:53:45  damdec
	Compilation warnings removed.
	
	Revision 2.9  2013/03/11 16:24:49  marfav
	Compiler warning reduced
	
	Revision 2.8  2012/12/05 11:43:46  chicas
	removing compiler warnings
	
	Revision 2.7  2012/02/13 17:10:24  marpas
	removing compiler warnings
	
	Revision 2.6  2007/03/22 18:10:38  fracar
	handling default for OrderTypeManager
	
	Revision 2.5  2007/03/13 15:44:04  fracar
	added DEBUG capability
	
	Revision 2.4  2007/03/13 14:27:00  fracar
	fixed bug in OrderTypeTranslation
	
	Revision 2.3  2006/04/21 12:55:43  marfav
	Log level managment logic improved
	
	Revision 2.2  2006/04/21 12:17:05  marfav
	Added get methods for Stdout and Stderr log levels
	
	Revision 2.1  2006/04/21 10:22:06  marfav
	Standard output and standard error indipendent log levels support added
	
	Revision 2.0  2006/02/28 10:10:54  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.21  2005/09/02 15:10:54  marfav
	Write on xml file using the new format allows only DB related order types values
	
	Revision 1.20  2005/06/28 08:31:42  marfav
	Multiple WSConfig schemas supported
	OrderType translation supported by libTlOrder manager
	
	Revision 1.19  2005/06/10 13:51:07  fracar
	added empty constructor
	
	Revision 1.18  2004/12/23 09:48:00  marfav
	Added support for using ostream in operator >>
	
	Revision 1.17  2004/12/03 18:05:07  marfav
	Header fixed
	
	Revision 1.16  2004/09/16 12:43:02  marpas
	XMLResourceHandler include removed
	
	Revision 1.15  2004/04/26 12:37:13  marfav
	Exception notification improved
	
	Revision 1.14  2004/04/16 17:40:37  marfav
	Exception handling improved
	Messaging improved
	
	Revision 1.13  2004/04/06 15:42:01  marfav
	Does not use ACE mutex anymore
	
	Revision 1.12  2004/04/05 13:38:34  marfav
	using exStream.h
	
	Revision 1.11  2004/03/12 09:11:13  marfav
	Added support for merging WSConfiguration files
	
	Revision 1.10  2003/09/22 11:04:53  fracar
	operator = now thread safe
	
	Revision 1.9  2003/08/01 10:45:19  marfav
	Check for configuration variations on reload added
	
	Revision 1.8  2003/07/29 10:42:30  marfav
	A rsResourceSet bug fixed. Code alignment.
	
	Revision 1.7  2003/07/28 14:58:08  marfav
	Copy-CTOR and operator= implemented
	
	Revision 1.6  2003/07/10 14:03:50  marfav
	clone method added to exceptions
	
	Revision 1.5  2003/04/18 16:16:30  fracar
	align to new compiler version
	
	Revision 1.4  2003/04/18 12:50:57  marfav
	Using namespaces removed from .h
	
	Revision 1.3  2003/02/18 09:11:28  fracar
	improved comments
	
	Revision 1.2  2003/02/04 14:14:38  fracar
	improved log messaging
	
	Revision 1.1.1.1  2003/01/31 16:03:28  marfav
	Import libWSConfig
	
	
	

*/

#include <tlwscV.h>
#include <WSConfig.h>

#include <XMLIstream.h>
#include <exException.h>
#include <rsPushNSpace.h>
#include <exStream.h>
#include <OrderTypeMgr.h>

#include <sstream>
#include <algorithm>
#include <functional>

#include <iostream>
#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

#ifndef RESOURCE_SET_BUG_FIXED
#warning Resource Set relies on count attribute for arrays
#endif

namespace {
tlwscV version;

const string _prcCountKey	= "List_of_Processors.count.value";	// to be used with no namespace
const string _prcArrayKey	= "List_of_Processors";			// to be used with no namespace
const string _prcKey		= "Processor";	     // to be used inside namespace _prcArrayKey
const string _prcNameKey		= "Processor_Name";  // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcVersionKey	= "Version";	     // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcLogLevelKey	= "Log_Level";	     // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcOutLogLevelKey	= "Stdout_Log_Level";// to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcErrLogLevelKey	= "Stderr_Log_Level";// to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcTaskTableKey	= "Task_Table";	     // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcOrdersKey	= "Orders";	     // to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcSystematicKey	= "Systematic";	// to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcNRTKey		= "NRT";	// to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcReprocessKey	= "Reprocess";	// to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcTrblshootKey	= "Trblshoot";	// to be used inside namespace _prcArrayKey._prcKey[i].Orders
const string _prcSUBSKey		= "SUBS";	// to be used inside namespace _prcArrayKey._prcKey[i].Orders

// variables for the new WSConfig xml schema
const string _prcOrderCountKey = "List_of_Orders.count.value"; //to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcOrderArrayKey = "List_of_Orders"; //to be used inside namespace _prcArrayKey._prcKey[i]
const string _prcOrderItemKey  = "Order"; //to be used inside namespace _prcArrayKey._prcKey[i]._procOrderArrayKey
const string _orderItemType    = "Order_Type"; //to be used inside namespace _prcArrayKey._prcKey[i]._procOrderArrayKey.procOrderItemKey[i]
const string _orderItemStatus   = "Status"; ; //to be used inside namespace _prcArrayKey._prcKey[i]._procOrderArrayKey.procOrderItemKey[i]

// APF-411
const string _posixExitCodeKey = "Posix_Exit_Code"; //to be used inside namespace _prcArrayKey._prcKey[i] - optional key default true

// next two string are boolean values for the tags inside the Orders section 
const string _disabledString = "disabled";
const string _enabledString = "enabled";

}

WSConfigElement::WSConfigElement() :
	name (),
	version (),
	logLevel (),
	outLogLevel (),
	errLogLevel (),
	taskTable (),
	orderTypeStatus (),
	hasPosixExitCode (true)
{}

WSConfigElement::~WSConfigElement() throw()
{}


WSConfigElement::WSConfigElement(const WSConfigElement& e) :
	name (e.name),
	version (e.version),
	logLevel (e.logLevel),
	outLogLevel (e.outLogLevel),
	errLogLevel (e.errLogLevel),
	taskTable (e.taskTable),
	orderTypeStatus (e.orderTypeStatus),
	hasPosixExitCode (e.hasPosixExitCode)

{
}

WSConfigElement&
WSConfigElement::operator = (const WSConfigElement& e)
{
    if (this != &e)
    {
		name             = e.name;
		version          = e.version;
		logLevel         = e.logLevel;
		outLogLevel      = e.outLogLevel;
		errLogLevel      = e.errLogLevel;
		taskTable        = e.taskTable;
		orderTypeStatus  = e.orderTypeStatus;
		hasPosixExitCode = e.hasPosixExitCode;
	}
    return *this;
}

bool
operator == (const WSConfigElement& a, const WSConfigElement &b)
{
    return ((a.name             == b.name)        &&
	        (a.version          == b.version)     &&
	        (a.logLevel         == b.logLevel)    &&
	        (a.outLogLevel      == b.outLogLevel) &&
	        (a.errLogLevel      == b.errLogLevel) &&
	        (a.taskTable        == b.taskTable)   &&
	        (a.orderTypeStatus  == b.orderTypeStatus) &&
			(a.hasPosixExitCode == b.hasPosixExitCode) );
}




//
// class WSConfig
//

ACS_CLASS_DEFINE_DEBUG_LEVEL(WSConfig)


WSConfig::WSConfig () : 
	_path(),
	_hasChanged(false),
	_legacyMode(false),
	_mutex(),
	_wsElements()
{


	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
}




WSConfig::WSConfig (const string& PathName) : 
	_path(),
	_hasChanged(false),
	_legacyMode(false),
	_mutex(),
	_wsElements()

{

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ctor(" << PathName << " ) called");
    load (PathName);
}


WSConfig::WSConfig (const WSConfig& aWSConfig) :
	_path(),
	_hasChanged(false),
	_legacyMode(false),
	_mutex(),
	_wsElements()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)

    *this = aWSConfig;
}


WSConfig&
WSConfig::operator = (const WSConfig& r)
{
    if (this != &r)
    {
	    acs::ThreadSafe::Lock guard(_mutex); // don't modify nothing during the copy
	    acs::ThreadSafe::Lock guard_source(r._mutex); // don't let source object be modified during the copy

	    _path       = r._path;
	    _hasChanged = r._hasChanged;
	    _legacyMode = r._legacyMode;
		_wsElements = r._wsElements;
// 	    clear();
// 	    copy(r.begin(), r.end(), back_inserter(*this));
    }

    return *this;
}

bool
WSConfig::hasChanged() const
{
    acs::ThreadSafe::Lock guard(_mutex);
    return _hasChanged;
}

void
WSConfig::load (const string& PathName) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)


    OrderTypeMgr otm;
    rsResourceSet _res;

    try
    {
	    XMLIstream  xmlIstream(PathName);
	    xmlIstream >> _res;
	    _res.extendBoolean(_disabledString, _enabledString);
    }
    catch (const exception& x)
    {
	    ACS_THROW(WSConfigException(x, "Cannot load WS configuration file " + PathName)) ; // PRQA S 3081
    }

    vector<WSConfigElement> v;

    {
	rsResourceSet& rs(_res); // lazyness in keystroking :-))))

	if(!rs.hasKey(_prcCountKey) )    //nothing to load
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "no " << _prcCountKey << " key found" );
	    return;
	}

	int procs = 0 ;
    int prcCount = 0 ;
	rs.getValue(_prcCountKey, prcCount);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, prcCount << " entries found");

	if (0 == prcCount) // no processors
	{
	    return;
	}


#ifdef RESOURCE_SET_BUG_FIXED
	try
	{
	    procs=rs.getArraySize(_prcArrayKey);
	}
	catch(exception& ex)
	{
	    ACS_LOG_NOTIFY_EX(ex);
	    return;
	}

	if (prcCount != procs)
	{
	    ACS_LOG_ERROR("Processor count mismatch [count=" << prcCount << ", size=" << procs << "] in WS Configuration " << _path);
	    return;
	}
#else
	procs = prcCount;
#endif // RESOURCE_SET_BUG_FIXED

	if (0 == procs)
	{
	    return;
	}


	{
	    rsPushNSpace nspace(rs,_prcArrayKey);

	    for(int i=0; i<procs; i++)
	    {
		    try
		    {
		        ostringstream os;
		        os << _prcKey << "["<<i<<"]";
		        string prefix=os.str();
		        rsPushNSpace nspace_inner(rs,prefix);

		        WSConfigElement e;

		        rs.getValue(_prcNameKey, e.name);
		        rs.getValue(_prcVersionKey, e.version);

		        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading [" <<
				                  e.name << "," << e.version << "]");

		        rs.getValue(_prcLogLevelKey, e.logLevel);
		        if (rs.hasKey (_prcOutLogLevelKey) || rs.hasKey (_prcErrLogLevelKey) )
		        {
			    rs.getValue(_prcOutLogLevelKey, e.outLogLevel);
			    rs.getValue(_prcErrLogLevelKey, e.errLogLevel);
		        }

		        rs.getValue(_prcTaskTableKey, e.taskTable);

				// APF-411
				rs.getValue (_posixExitCodeKey, e.hasPosixExitCode, false);

		        if (rs.hasKey(_prcOrderArrayKey)) 
		        {
			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "non-legacy processor");

			    // The WSConfig is compliant to the new schema
			    _legacyMode = false;
			    int typesCount = 0;
			    rs.getValue (_prcOrderCountKey, typesCount);

			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, typesCount << " mappings found");
			    for (int j=0; j<typesCount; ++j) 
			    {
			        ostringstream tos;
			        tos << _prcOrderArrayKey << "." << _prcOrderItemKey << "[" << j << "]";
			        rsPushNSpace nspace2(rs, tos.str());
			        string orderType;
			        rs.getValue(_orderItemType, orderType);
			        string dbOrderType = otm.getOrderTypeFromWS(orderType);
			        rs.getValue(_orderItemStatus, e.orderTypeStatus[dbOrderType]);
			    }
		        }
		        else
		        {
			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "legacy processor");


			    // WSConfig Cryosat-like
			    _legacyMode = true;	
			    rsPushNSpace nspace_inner2(rs, _prcOrdersKey);

			    string dbOrderType;

			    dbOrderType = otm.getOrderTypeFromWS (_prcSystematicKey, true);
			    rs.getValue(_prcSystematicKey, e.orderTypeStatus[dbOrderType]);

			    dbOrderType = otm.getOrderTypeFromWS (_prcNRTKey, true);
			    rs.getValue(_prcNRTKey, e.orderTypeStatus[dbOrderType]);

			    dbOrderType = otm.getOrderTypeFromWS (_prcReprocessKey, true);
			    rs.getValue(_prcReprocessKey, e.orderTypeStatus[dbOrderType]);

			    dbOrderType = otm.getOrderTypeFromWS (_prcTrblshootKey, true);
			    rs.getValue(_prcTrblshootKey, e.orderTypeStatus[dbOrderType]);

			    dbOrderType = otm.getOrderTypeFromWS (_prcSUBSKey, true);
			    rs.getValue(_prcSUBSKey, e.orderTypeStatus[dbOrderType]);
		        }

		        v.push_back(e);
		    }
		    catch (exception& ex)
		    {
		        ACS_LOG_NOTIFY_EX(ex);
		        ACS_LOG_WARNING("WSConfig: exception caugth while loading [" << PathName <<"]. Skipping processor definition #" << i) ;
		        continue;
		    }
	    }//for(int i=0; i<procs; i++)
	}
    }


    // now we can safely modify internal status
    //
    acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
    if ( (size() != v.size()) || !equal(begin(), end(), v.begin()) )
    {
		_wsElements = v;
// 	 clear();
// 	 copy(v.begin(), v.end(), back_inserter(*this));
	 _path=PathName;
	 _hasChanged=true;
    }
    else
    {
	 _hasChanged = false;
    }
}

void
WSConfig::load ()
{
    load (_path);
}


void
WSConfig::getProcessors(vector<pair<string, string> >& ProcsAndVers) const
{
    acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock

  ProcsAndVers.clear();

  // enumerate couples processor+version
  {
    for(size_t i=0; i<size(); i++)
    {
	ProcsAndVers.push_back(make_pair((*this)[i].name, (*this)[i].version));
    }//for(size_t i=0; i<procs; i++)
  }
}


void
WSConfig::getOrders(const string& aProc, const string& aVersion, vector<string>& orders) const
{
    acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock

    orders.clear();


    // search for the requested couple processor+version
    for(size_t i=0; i<size(); i++)
    {
	if ((*this)[i].name == aProc)
	{
	    // let's check the version
	    if ((*this)[i].version == aVersion)
	    {
		// processor+version found:
		// let's get the order types
		map <string, bool> theMap = (*this)[i].orderTypeStatus;
		map <string, bool>::iterator it;
		for (it = theMap.begin(); it!=theMap.end(); ++it) 
		{
		    if (it->second) 
		    {
			    orders.push_back(it->first);
		    }
		}
	    }
	}
    }
}

bool
WSConfig::getProcessor(const std::string& aProc, const std::string& aVersion, WSConfigElement& e) const // PRQA S 4020
{
  acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock


  // search for the requested couple processor+version
  {
    for(size_t i=0; i<size(); i++)
    {
	if ((*this)[i].name == aProc)
	{
	    // let's check the version
	    if ((*this)[i].version == aVersion)
	    {
		// FOUND!
		e = (*this)[i];
		return true;
	    }
	}
    }
  }

  // if we reach this point, we didn't find anything, so...
  return false;
}


bool
WSConfig::hasPosixExitCode(const string& aProc, const string& aVersion, bool& b) const
{
	string s;
	bool retval = getProcKey(aProc, aVersion, _posixExitCodeKey, s);
	// ugly conversion bool - string - bool 
	b = ( s == "true" );
	return retval;
}




bool
WSConfig::getTaskTable(const string& aProc, const string& aVersion, string& tt) const
{
  return getProcKey(aProc, aVersion, _prcTaskTableKey, tt);
}


bool
WSConfig::getLogLevel(const string& aProc, const string& aVersion, string& ll) const
{
  return getProcKey(aProc, aVersion, _prcLogLevelKey, ll);
}

bool
WSConfig::getOutLogLevel(const string& aProc, const string& aVersion, string& ll) const
{
  return getProcKey(aProc, aVersion, _prcOutLogLevelKey, ll);
}

bool
WSConfig::getErrLogLevel(const string& aProc, const string& aVersion, string& ll) const
{
  return getProcKey(aProc, aVersion, _prcErrLogLevelKey, ll);
}


bool
WSConfig::isOrderProcessable(const string& aProc, const string& aVersion, const string& order) // PRQA S 4020
{
    acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock


    // search for the requested couple processor+version
    size_t mysize = _wsElements.size() ;
    for(size_t i=0; i<mysize; ++i)
    {
	    if ( (_wsElements[i].name == aProc) && (_wsElements[i].version == aVersion))
	    {
	        // processor+version found:
	        // let's check the order type
	        if (_wsElements[i].orderTypeStatus.find(order) != _wsElements[i].orderTypeStatus.end() ) 
	        {
		        // The order type is present in the map
		        return ( _wsElements[i].orderTypeStatus[order] );
	        }
	        else
	        {
		        // No orderType defined... so the orderType is not processable
		        // Returning now just to avoid the loop completion
		        return false;
	        }
	    }
    }//for(size_t i=0; i<procs; i++)

    // if we reach this point, we didn't find anything, so...
    return false;
}


ostream & operator << (ostream &os, const WSConfig &ws) 
{
    ws.toStream(os) ;
    return os ; // PRQA S 4028
}

void WSConfig::toStream (ostream& o) const
{
    acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
    OrderTypeMgr otm;


	XMLOstream xo(o);

	XMLOstream::Tag roottag (xo, "Proc_Table", "", "");

	ostringstream os;
	os << "count=\"" << size() << "\"";

	XMLOstream::Tag procarrtag (xo, _prcArrayKey, "", os.str());

    for (size_t i=0; i<size(); i++)
    {
 
	    const WSConfigElement& ec = (*this)[i];

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
		{ XMLOstream::Tag t(xo, _posixExitCodeKey , (ec.hasPosixExitCode ? "true":"false") , "", true); } // PRQA S 3380

	    {
	        string s;

	        if (_legacyMode) 
	        {
				map <string, bool> ots = ec.orderTypeStatus;	

				XMLOstream::Tag procorderstag (xo, _prcOrdersKey, "");

				{ XMLOstream::Tag t(xo, _prcSystematicKey , (ots[otm.getOrderTypeFromWS(_prcSystematicKey, true)] ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcNRTKey        , (ots[otm.getOrderTypeFromWS(_prcNRTKey,        true)] ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcReprocessKey  , (ots[otm.getOrderTypeFromWS(_prcReprocessKey,  true)] ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcTrblshootKey  , (ots[otm.getOrderTypeFromWS(_prcTrblshootKey,  true)] ? _enabledString : _disabledString) , "", true); } // PRQA S 3380
				{ XMLOstream::Tag t(xo, _prcSUBSKey       , (ots[otm.getOrderTypeFromWS(_prcSUBSKey,       true)] ? _enabledString : _disabledString) , "", true); } // PRQA S 3380

			}
			else
			{

				map <string, bool> orderStatusMap = ec.orderTypeStatus;
				map <string, bool>::iterator it;
				ostringstream os;
				os << " count=\"" << orderStatusMap.size() << "\"";

				XMLOstream::Tag procarrtag (xo, _prcOrderArrayKey, "", os.str());


				for (it = orderStatusMap.begin(); it != orderStatusMap.end(); ++it) 
				{
					XMLOstream::Tag procorderstag (xo, _prcOrderItemKey, "");

					{ XMLOstream::Tag t(xo, _orderItemType , otm.getWSConfigType(it->first) , "", true); } // PRQA S 3380
					{ XMLOstream::Tag t(xo, _orderItemStatus , (it->second ? _enabledString : _disabledString) , "", true); } // PRQA S 3380

		    	}
	        }
	    }
    }
}

string
WSConfig::getPath() const
{
  acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock

  return _path;
}

bool 
WSConfig::isLegacy() const
{
	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
	return _legacyMode;
}
	
void 
WSConfig::setLegacy (bool l)
{
	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
	_legacyMode = l;
}

bool
WSConfig::getProcKey(const string& aProc, const string& aVersion, const string& aKey, string& s) const // PRQA S 4020
{
  acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock


  // search for the requested couple processor+version
  {
    for(size_t i=0; i<size(); i++)
    {
	if ((*this)[i].name == aProc)
	{
	  // let's check the version
	  if ((*this)[i].version == aVersion)
	  {
	    // FOUND! let's load the key

	      if (aKey == _prcLogLevelKey)
	      {
		    s = (*this)[i].logLevel;
	      }
	      else if (aKey == _prcOutLogLevelKey)
	      {
		    s = (*this)[i].outLogLevel;
	      }
	      else if (aKey == _prcErrLogLevelKey)
	      {
		    s = (*this)[i].errLogLevel;
	      }
	      else if (aKey == _prcTaskTableKey)
	      {
		    s = (*this)[i].taskTable;
	      }
	      else if (aKey == _posixExitCodeKey)
	      {
		    s = ((*this)[i].hasPosixExitCode ? "true":"false");
	      }
	      else { 
            return false;
          }
	      return true;
	  }
	}
    }
  }

  // if we reach this point, we didn't find anything, so...
  return false;
}
const WSConfigElement& WSConfig::operator [] (const int& s) const
{
	return _wsElements[s];
}
const WSConfigElement& WSConfig::at (const int & s) const
{
	return _wsElements.at(s);
}
std::vector<WSConfigElement> WSConfig::getWSConfigElements () const
{
	return _wsElements;
}

size_t WSConfig::size() const
{
	return _wsElements.size();
}

void WSConfig::clear()
{
	_wsElements.clear();
}
void WSConfig::push_back(WSConfigElement & t)
{
	_wsElements.push_back(t);
}
std::vector<WSConfigElement>::iterator WSConfig::erase(std::vector<WSConfigElement>::const_iterator position)
{
	return _wsElements.erase(position);
}
std::vector<WSConfigElement>::iterator WSConfig::erase(std::vector<WSConfigElement>::const_iterator firstPos, std::vector<WSConfigElement>::const_iterator lastPos)
{
	return _wsElements.erase(firstPos, lastPos);
}
std::vector<WSConfigElement>::iterator WSConfig::erase(std::vector<WSConfigElement>::iterator position)
{
	return _wsElements.erase(position);
}
std::vector<WSConfigElement>::iterator WSConfig::erase(std::vector<WSConfigElement>::iterator firstPos, std::vector<WSConfigElement>::iterator lastPos)
{
	return _wsElements.erase(firstPos, lastPos);
}
WSConfig::operator const std::vector<WSConfigElement>& () const
{
	return _wsElements;
}
WSConfig::operator std::vector<WSConfigElement>& () 
{
	return _wsElements;
}
std::vector<WSConfigElement>::const_iterator WSConfig::begin() const
{
	return _wsElements.begin();
}
std::vector<WSConfigElement>::const_iterator WSConfig::end() const
{
	return _wsElements.end();
}
std::vector<WSConfigElement>::iterator WSConfig::begin() 
{
	return _wsElements.begin();
}
std::vector<WSConfigElement>::iterator WSConfig::end() 
{
	return _wsElements.end();
}

_ACS_END_NAMESPACE
