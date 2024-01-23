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
 *  Revision 5.0  2013/06/06 18:51:20  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1  2012/09/14 16:31:32  micmaz
 *  S2GPP-611
 *  Development of Distribution Formatter framework for Export
 *
 *
 */

#ifndef _SingleNodeFtpFormatter_H_
#define _SingleNodeFtpFormatter_H_

#include <acs_c++config.hpp>
#include <DataFormatter.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider) ;

class SingleNodeFtpFormatter: public DataFormatter {
public:
    SingleNodeFtpFormatter();
    virtual ~SingleNodeFtpFormatter()throw();
    virtual void generateDeliverySlip();
private:
    SingleNodeFtpFormatter(const SingleNodeFtpFormatter &) ; // not implemented
    SingleNodeFtpFormatter& operator= (const SingleNodeFtpFormatter &) ; // not implemented};
} ;

_ACS_END_NESTED_NAMESPACE

#endif /* _SingleNodeFtpFormatter_H_ */
