// PRQA S 1050 EOF
/*
 *

 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S.  $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.1  2013/11/04 13:56:31  marpas
 *  coding best practices applied
 *  robustness improved
 *  performances improved
 *  compilation & qa warnings removed
 *
 *  Revision 5.0  2013/06/06 18:51:19  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.2  2012/11/05 13:38:54  micmaz
 *  all data provider configuration parameters are now optional
 *
 *  Revision 1.1  2012/09/14 16:31:32  micmaz
 *  S2GPP-611
 *  Development of Distribution Formatter framework for Export
 *
 *
 */

#ifndef _DataFormatter_H_
#define _DataFormatter_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider) ;

class DataFormatter {
public:
    DataFormatter();
    virtual ~DataFormatter() throw ();
    virtual void generateDeliverySlip()=0;
private:
    DataFormatter(const DataFormatter &) ; // not implemented
    DataFormatter& operator= (const DataFormatter &) ; // not implemented};
};


_ACS_END_NESTED_NAMESPACE

#endif /* _DataFormatter_H_ */
