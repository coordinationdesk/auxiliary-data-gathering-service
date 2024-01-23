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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.6  2013/04/22 10:59:49  marpas
 *  NameIdAssociator and derived are now using singleton template pattern
 *
 *  Revision 1.5  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.4  2012/02/17 11:20:32  marpas
 *  namespaces fixed
 *  tests dependencies fixed
 *
 *  Revision 1.3  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.2  2011/08/29 17:12:04  micmaz
 *  synchronized NameIdAssociator creation.
 *
 *  Revision 1.1  2011/06/23 15:49:20  micmaz
 *  work in progress
 *
 *
 *
 *
 */

#include "CirculationMode.h"


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

std::string const CirculationMode::implicitMode("IMPLICIT");
std::string const CirculationMode::explicitMode("EXPLICIT");
std::string const CirculationMode::dynamicMode("DYNAMIC");

CirculationMode::~CirculationMode() throw() {
    //empty.
}

CirculationMode::CirculationMode(const std::string &key) :
    NameIdAssociator<short>("T_CIRCULATIONMODES", "ID", "CIRCULATIONMODE", key),
    ParamConstructibleSingletonMap<CirculationMode, std::string>()
{
    NameIdAssociator<short>::load() ;
}


_ACS_END_NESTED_NAMESPACE;

