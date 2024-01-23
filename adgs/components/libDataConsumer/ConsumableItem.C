// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2016, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.2  2014/06/24 10:58:34  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.1  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 20:55:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.3  2013/04/05 09:47:15  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.2  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:14:43  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#include <ConsumableItem.h>
#include <DataConsumerConfigurer.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)

ConsumableItem::ConsumableItem()
{
    //empty
}

ConsumableItem::~ConsumableItem() throw()
{
    // empty
}

bool getEarlyFinalRemoval() 
{
    return conf::dcConfigurerSingleton::instance()->getEarlyFinalRemoval() ; // should I remove existing file before or after the transfer
}


_ACS_END_NESTED_NAMESPACE 
