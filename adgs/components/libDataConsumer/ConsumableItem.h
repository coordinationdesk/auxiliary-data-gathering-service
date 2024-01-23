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
 *  Revision 5.6  2016/03/22 16:36:03  francesco.avanzi
 *  S2PDGS-1434: get and set EarlyFinalRemoval implemented, in order to pass bool forward up to Curl.C
 *
 *  Revision 5.5  2014/06/24 10:58:34  marpas
 *  useless macros eviction in progress
 *
 *  Revision 5.4  2014/06/18 13:42:44  lucio.pulvirenti
 *  S2PDGS-726: StopException defined to discriminate stop.
 *
 *  Revision 5.3  2014/02/14 13:54:46  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.2  2013/07/22 13:18:15  marpas
 *  types and best practices fixed
 *
 *  Revision 5.1  2013/07/15 16:48:06  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.7  2013/04/05 09:47:15  marpas
 *  libDCUtility interface change
 *  flow control changed
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.6  2012/03/30 12:50:27  micmaz
 *  work in progress
 *
 *  Revision 1.5  2012/03/13 13:19:56  marpas
 *  abstract signature fixed
 *
 *  Revision 1.4  2012/02/07 10:25:56  micmaz
 *  removed warnings.
 *
 *  Revision 1.3  2011/04/15 15:33:20  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/11 12:24:53  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:14:43  micmaz
 *  work in progress...
 *
 *
 *
 *
 */

#ifndef _ConsumableItem_H_
#define _ConsumableItem_H_

#include <exException.h>
#include <acs_c++config.hpp>
#include <string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dc)


class ConsumableItem { // PRQA S 2109
public:
		
	exDECLARE_EXCEPTION(StopException,exException); // PRQA S 2131, 2502

    ConsumableItem();
    virtual ~ConsumableItem() throw();
    bool getEarlyFinalRemoval() const ;
private:
    ConsumableItem(const ConsumableItem & ) ;
    ConsumableItem & operator= (const ConsumableItem & ) ;
public:
    virtual std::string id() const =0;
    virtual std::string groupId() const=0;
    virtual std::string str() const=0;
    virtual void doWork()=0;
};

_ACS_END_NESTED_NAMESPACE

#endif /* _ConsumableItem_H_ */
