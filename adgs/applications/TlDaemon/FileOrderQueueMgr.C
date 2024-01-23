/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2015/05/13 11:05:21  damdec
	Compilation warning fixed.
	
	Revision 2.3  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.2  2013/02/26 16:36:23  marfav
	Some compiler warning removed
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.11  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.10  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.9  2003/07/18 14:50:45  marfav
	tlDaemon configuration space fixed
	
	Revision 1.8  2003/06/06 10:26:20  marfav
	 no log
	
	Revision 1.6  2003/04/18 15:56:56  fracar
	aligning to new compiler version
	
	Revision 1.5  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.4  2003/02/04 16:45:18  marfav
	Added registerDownloadStart method
	
	Revision 1.3  2003/02/04 09:50:04  marfav
	Using ConfigurationSingleton instead of parent config
	

*/



#include <FileOrderQueueMgr.h>

#include <tlDaemon.h>

#include <rsResourceSet.h>
#include <File.h>

#include <iterator>
#include <iostream>
#include <algorithm>
#include <ConfigurationSingleton.h>

#include <exStream.h>

using namespace std;
using namespace acs;



static const char *_orderFileKey = "TlDaemon.TLD_FileOrderMgr.Path";

static const char field_separator = ';';




ACS_CLASS_DEFINE_DEBUG_LEVEL(FileOrderQueueMgr)


FileOrderQueueMgr::FileOrderQueueMgr(tlDaemon& aDaemon) :
    OrderQueueMgr(aDaemon),
    _path()
    
{
}


bool
FileOrderQueueMgr::registerProcessingStart(const tlOrder& anOrder)
{
  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "FileOrderQueueMgr: registering start processing for order id#[" << anOrder.getOrderId());

  updateOrderStatus(anOrder, OrderStatusPROCESSING);

  return true;
}

bool
FileOrderQueueMgr::registerDownloadStart(const tlOrder& anOrder)
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "registering start download for order id#[" << anOrder.getOrderId());


  updateOrderStatus(anOrder, OrderStatusDOWNLOADING);

  return true;
}


bool
FileOrderQueueMgr::registerProcessingStop(const tlOrder& anOrder, tlOrderStatus status)
{
  ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "FileOrderQueueMgr: registering stop processing for order id#[" << anOrder.getOrderId());

  updateOrderStatus(anOrder, status);

  return true;
}


bool
FileOrderQueueMgr::updateOrderStatus(const tlOrder& anOrder, tlOrderStatus status, bool)
{
  ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "FileOrderQueueMgr: updating order id#[" <<
              anOrder.getOrderId() <<
	      "] status to " << 
	      OrderStatus2String(status) );


  return true;
}

void
FileOrderQueueMgr::init()
{

  // Retrieving configuration
  const rsResourceSet& rs = ConfigurationSingleton::instance() ->get();


  if (!rs.hasKey(_orderFileKey) )    //nothing to load
  {
    excerr << "FileOrderQueueMgr: cannot find " << _orderFileKey << " in the configuration file\n";
  }
  else
  {
    rs.getValue(_orderFileKey, _path);

    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB,  "FileOrderQueueMgr: order file is " << _path);
  }
}

tlOrder
FileOrderQueueMgr::line2order(string line)
{
  ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "FileOrderQueueMgr: parsing line " << line );

  tlOrder o;

  string::size_type i = line.find(field_separator);
  string sub = line.substr(0, i);
  string rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setOrderId(atoi(sub.c_str())) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setPriority(atoi(sub.c_str())) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setStatus(sub) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setProcessorName(sub) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setProcessorVersion(sub) ;


  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setProcessorConfiguration(sub) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setTroubleshooting(sub == "Y" ? true : false) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  sub[sub.find('T')] = ' ';
  o.setStartTime(sub) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  sub[sub.find('T')] = ' ';
  o.setStopTime(sub) ;

  i = rem.find(field_separator);
  sub = line.substr(0, i);
  rem = line.substr(i+1);
  line = rem;

  ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "\tread substring [" << sub << "] remainder is [" << rem << "]");

  o.setProcessorId(sub) ;

  sub = line;
  o.setOrderType(sub) ;

  return o;
}


void
FileOrderQueueMgr::retrieveOrders(vector<tlOrder>& orders)
{
  orders.clear();

  if (File::exists(_path))
  {
    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "FileOrderQueueMgr: retrieving orders from file " << _path);

    ifstream in(_path.c_str());

    transform(istream_iterator<string>(in),
	      istream_iterator<string>(),
	      back_inserter(orders), line2order);
    {
      string new_name = _path+".processed";
      File::rename(_path, new_name);
    }
  }
  else
  {
    excerr << "FileOrderQueueMgr: ERROR not existing file " << _path << "\n";
  }
}
