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

	$Prod: A.C.S. libTlOrder$

	$Id$

	$Author$

	$Log$
	Revision 2.3  2015/06/16 10:26:05  damdec
	Compilation warnings solved. New ACS log macros used.
	
	Revision 2.2  2012/06/06 16:53:03  marfav
	Code improved removing compiler warnings
	
	Revision 2.1  2007/03/22 18:07:36  fracar
	improved debugging
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/28 08:26:31  marfav
	Added OrderTypeMgr class
	
        
*/ 

#include <OrderTypeMgr.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

/*

<OrderTypeManagerRoot>
	<OrderTypeManager>
		<List_of_TT_Conversions count="2">
			<TT_Conversion>
				<Db_Type>OFFL</Db_Type>
				<TT_Type>SISTEMATIC</TT_Type>
			</TT_Conversion>
			<TT_Conversion>
				<Db_Type>RPRO</Db_Type>
				<TT_Type>REPROCESSING</TT_Type>
			</TT_Conversion>
		</List_of_TT_Conversions>
		<List_of_WSC_Conversions count="2">
			<WSC_Conversion>
				<Db_Type>OFFL</Db_Type>
				<WSC_Type>Systematic</WSC_Type>
			</WSC_Conversion>
			<WSC_Conversion>
				<Db_Type>RPRO</Db_Type>
				<WSC_Type>Reprocess</WSC_Type>
			</WSC_Conversion>
		</List_of_WSC_Conversions>
	</OrderTypeManager>
</OrderTypeManagerRoot>

*/		  

const string _namespaceKey    = "OrderTypeManager";                   // Root Level
const string _listTTConvKey   = "List_of_TT_Conversions";             // _namespaceKey
const string _listTTConvCount = "List_of_TT_Conversions.count.value"; // _namespaceKey
const string _TTConvKey       = "TT_Conversion";                      // _namespaceKey._listTTConvKey
const string _listWSConvKey   = "List_of_WSC_Conversions";             // _namespaceKey
const string _listWSConvCount = "List_of_WSC_Conversions.count.value"; // _namespaceKey
const string _WSConvKey       = "WSC_Conversion";                      // _namespaceKey._listWSConvKey
const string _dbTypeKey       = "Db_Type";                            // _namespaceKey._list<WS|TT>ConvKey._<WS|TT>ConvKey[i]
const string _ttTypeKey       = "TT_Type";                            // _namespaceKey._listTTConvKey._TTConvKey[i]
const string _wscTypeKey      = "WSC_Type";                           // _namespaceKey._listWSConvKey._WSConvKey[i]

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderTypeMgr)



OrderTypeMgr::OrderTypeMgr() :
    _wsConfigMap (),
    _taskTableMap (),
	_defaultsLoaded(false)
{

   ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)	   
   init();
	   
}

OrderTypeMgr::~OrderTypeMgr() throw() 
{
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) // PRQA S 4631   
}

OrderTypeMgr::OrderTypeMgr(const OrderTypeMgr& a) :
    _wsConfigMap (a._wsConfigMap),
    _taskTableMap (a._taskTableMap),
	_defaultsLoaded(a._defaultsLoaded)

{
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)	   
   //*this = aOrderTypeMgr;
}


OrderTypeMgr& OrderTypeMgr::operator = (const OrderTypeMgr& aOrderTypeMgr) 
{
  
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)	   

   if (this != &aOrderTypeMgr) 
   {
	 _defaultsLoaded = aOrderTypeMgr._defaultsLoaded;
	 _wsConfigMap    = aOrderTypeMgr._wsConfigMap;
	 _taskTableMap   = aOrderTypeMgr._taskTableMap;
   }
   return *this;
}


void OrderTypeMgr::loadDefaults()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)	   
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading default values") ;
	    
    _defaultsLoaded = true;

    _wsConfigMap.clear();
    _taskTableMap.clear();
    
    _wsConfigMap["OFFL"] = "Systematic";
    _wsConfigMap["RPRO"] = "Reprocess";
    _taskTableMap["OFFL"] = "SISTEMATIC";
    _taskTableMap["RPRO"] = "REPROCESSING";
}


void OrderTypeMgr::init()
{
   	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)	   

	_wsConfigMap.clear();
	_taskTableMap.clear();
	


	// Check if a configuration space is loaded or not for the translation service
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "looking for key " << _namespaceKey << " in configuration space.") ;

	
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	if (rs.hasKey(_namespaceKey)) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "key " << _namespaceKey << " found in configuration space.") ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "start reading settings") ;

		rsPushNSpace nspaceRoot (rs, _namespaceKey);
			
		// Key found let's load the values
		try
		{
			int wsEntries = 0 ;
            int ttEntries = 0 ;
			rs.getValue(_listTTConvCount, ttEntries);
			rs.getValue(_listWSConvCount, wsEntries);
			
    			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "found " << ttEntries << " TaskTable entries and " << wsEntries
						  << " WSConfig entries.") ;
				
			// loading first the TT entries 
			
			for (int idx=0; idx<ttEntries; ++idx) 
			{
				ostringstream os;
				os << _listTTConvKey << "." << _TTConvKey << "[" << idx << "]";
				rsPushNSpace nspaceEntry (rs, os.str());
				
				string dbVal ;
                string ttVal ;
				rs.getValue (_dbTypeKey, dbVal);
				rs.getValue (_ttTypeKey, ttVal);
				
    				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Adding DB to TT conversion " << dbVal << " ---> " << ttVal) ;
				
				_taskTableMap [dbVal] = ttVal;
				
			}
			
			// and then the WSConfig ones
			for (int idx=0; idx<wsEntries; ++idx) 
			{
				ostringstream os;
				os << _listWSConvKey << "." << _WSConvKey << "[" << idx << "]";
				rsPushNSpace nspaceEntry (rs, os.str());
				
				string dbVal ;
                string wsVal ;
				rs.getValue (_dbTypeKey, dbVal);
				rs.getValue (_wscTypeKey, wsVal);
				
    				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Adding DB to WSC conversion " << dbVal << " ---> " << wsVal) ;
				
				_wsConfigMap [dbVal] = wsVal;
				
			}
			
			
		}
		catch (exException& ex)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Exception caught while loading settings. Using default values.");
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			   ACS_LOG_NOTIFY_EX(ex);
		        ACS_CLASS_END_DEBUG

			loadDefaults();
			
		}
		catch (...)
		{
    			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unknown exception caught while loading settings. Using default values.") ;
				
			loadDefaults();
		}
		
	}
	else
	{
    		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "key " << _namespaceKey << " found in configuration space." ) ;

		loadDefaults();
	}

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "init exiting") ;
}


string OrderTypeMgr::getWSConfigType (const std::string& o, bool useDefaults) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "getWSConfigType (\"" << o << "\") called");


	//
	// if defaults mappings were loaded during init(),
	// return the map only if explicitely requested
	// (defaults are needed only for WSConfig back compatibility, but will harm most other missions)
	//
	if (_defaultsLoaded && !useDefaults)
	{
	    // no translation
	    return o;
	}

	// If the order type is not present in the map
	// this will return the string passed as input
	// no translation needed

	string translate = _wsConfigMap[o] ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "translate = \"" << translate << "\"");
    if (translate.empty()) { translate = o ; }

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning \"" << translate << "\"" );

	return translate ;
}

string OrderTypeMgr::getTTConfigType (const std::string& o)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "getTTConfigType (\"" << o << "\") called");

	// If the order type is not present in the map
	// this will return the string passed as input
	// no translation needed

	string result = _taskTableMap[o];
    if (result.empty()) { result = o ; }
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning \"" << result << "\"");

	return result ;
}

string OrderTypeMgr::getOrderTypeFromWS (const std::string& o, bool useDefaults) const // PRQA S 4020
{
    //
    // if defaults mappings were loaded during init(),
    // return the map only if explicitely requested
    // (defaults are needed only for WSConfig back compatibility, but will harm most other missions)
    //
    if (_defaultsLoaded && !useDefaults)
    {
	    // no translation
	    return o;
    }


    // If the order type is not present in the map
    // this will return the string passed as input
    // no translation needed
    
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "getOrderTypeFromWS (\"" << o << "\") called");
    
    
    map<string, string>::const_iterator it;
    bool found = false;
    string result;
    
    for (it = _wsConfigMap.begin(); (it != _wsConfigMap.end()) && (!found); ++it) 
    {
	    if ( (found = (it->second == o)) ) {
		    result = it->first;
        }
	    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checked item is \"" << it->second << "\" found = " << boolalpha << found ) ;
    }
    
    if (not found) { result = o ; }
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning \"" << result << "\"");

    return result ;
}

string OrderTypeMgr::getOrderTypeFromTT (const std::string& o) const
{
	// If the order type is not present in the map
	// this will return the string passed as input
	// no translation needed
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "getOrderTypeFromTT (\"" << o << "\") called");
	
	map<string, string>::const_iterator it;
	bool found = false;
	string result;
	
	for (it = _taskTableMap.begin(); (it != _taskTableMap.end()) && (!found); ++it) 
	{
		if ( (found = (it->second == o)) ) {
			result = it->first;
        }
		
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checked item is \"" << it->second << "\" found = " 
				   << boolalpha << found);
	}

    if (not found) { result = o ; }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning \"" << result << "\"");
	
	return result ;
}

_ACS_END_NAMESPACE


