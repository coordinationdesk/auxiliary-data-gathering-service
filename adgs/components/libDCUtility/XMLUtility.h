// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2012, Advanced Computer Systems , Inc.
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
 *  Revision 5.0  2013/06/06 18:10:25  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.8  2013/04/29 17:20:37  marpas
 *  removing useless code
 *
 *  Revision 1.7  2013/04/29 12:49:42  marpas
 *  useless include removed
 *
 *  Revision 1.6  2013/04/03 17:02:44  marpas
 *  coding best practices applied
 *  interfaces rationalization
 *
 *  Revision 1.5  2012/12/21 15:27:08  micmaz
 *  work in progress
 *
 *  Revision 1.4  2012/11/21 11:01:36  marpas
 *  smart persistent i/f changed
 *
 *  Revision 1.3  2012/11/08 16:53:11  micmaz
 *  correct errors in parsing xml.
 *
 *  Revision 1.2  2012/11/08 11:41:15  micmaz
 *  reworked xml parser
 *
 *  Revision 1.1  2012/10/31 18:36:07  micmaz
 *  added xml utility
 *
 *
 */

#ifndef XMLUTILITY_H_
#define XMLUTILITY_H_

#include <ConfigurationProperty.h>

#include <exMacros.h>
#include <vector>
#include <ostream>


_ACS_BEGIN_NAMESPACE(acs)
class rsResourceSet;
_ACS_END_NAMESPACE

_ACS_BEGIN_NESTED_NAMESPACE(acs, utility)

class XMLUtility { // PRQA S 2109
public:
    static void readFromXML(ConfigurationProperty &, rsResourceSet const& );
    static void writeToXML(std::string const& rootTagName, std::vector<ConfigurationProperty> const& properties, std::ostream& xml);
private:
    XMLUtility();
    ~XMLUtility() ;
    XMLUtility(const XMLUtility &);
    XMLUtility& operator= (const XMLUtility &);
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLUtility) ;
};



_ACS_END_NESTED_NAMESPACE

#endif /* XMLUTILITY_H_ */
