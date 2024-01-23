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
	Revision 2.2  2015/06/16 10:26:05  damdec
	Compilation warnings solved. New ACS log macros used.
	
	Revision 2.1  2007/03/22 18:07:36  fracar
	improved debugging
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/28 08:26:31  marfav
	Added OrderTypeMgr class
	
        
*/

#ifndef _OrderTypeMgr_H_
#define _OrderTypeMgr_H_

#include <exException.h>

#include <vector>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)
    
/**
 * \brief
 *  loads order type mappings from configuration and provide string mappings toward
 *  Task Table and Workstation Configuration File
 **/

class OrderTypeMgr  { 
public:

    /*! class exOrderTypeMgrException declaration */
    exDECLARE_EXCEPTION(exOrderTypeMgrException,exException) ; // PRQA S 2131, 2502

    /** Default Class constructor */
    OrderTypeMgr();
    /** Destructor */
    virtual ~OrderTypeMgr() throw() ;

    // Returns the vector containing all the processable order types
    // std::vector<std::string> getOrderTypes () const;

    // Maps the order type as retrieved by the OrdersQueue table
    // to the related string used inside the wsconfig and tasktable xml files
    std::string getWSConfigType (const std::string&, bool=false);
    std::string getTTConfigType (const std::string&);

    // Makes the reverse-search from wsconfig or tasktable order type
    // to database string values
    std::string getOrderTypeFromWS (const std::string&, bool=false) const;
    std::string getOrderTypeFromTT (const std::string&) const;



    /** Copy Constructor */
    OrderTypeMgr(const OrderTypeMgr & );
    /** Operator = */
    OrderTypeMgr &operator=(const OrderTypeMgr &);


protected:
    void init();
    void loadDefaults();

private:
    // std::vector<std::string> _orderTypes;
    std::map<std::string, std::string> _wsConfigMap;
    std::map<std::string, std::string> _taskTableMap;
    bool _defaultsLoaded;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderTypeMgr)

};

_ACS_END_NAMESPACE

#endif //_OrderTypeMgr_H_

