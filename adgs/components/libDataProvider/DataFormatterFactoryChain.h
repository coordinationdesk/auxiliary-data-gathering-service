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
 *  $Prod: A.C.S. Data Provider Library $
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
 *  Revision 1.2  2013/01/25 13:34:27  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 1.1  2012/09/14 16:31:32  micmaz
 *  S2GPP-611
 *  Development of Distribution Formatter framework for Export
 *
 *
 */

#ifndef _DataFormatterFactoryChain_h_
#define _DataFormatterFactoryChain_h_

#include <acs_c++config.hpp>
#include <DataFormatterFactory.h>
#include <DataFormatter.h>
#include <Singleton.hpp>
#include<exMacros.h>
#include<vector>
#include<string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)

class DataFormatterFactoryChain  { // PRQA S 2109   
    friend DataFormatterFactoryChain *pattern::Singleton<DataFormatterFactoryChain>::instance() ; // PRQA S 2107
public:
    virtual ~DataFormatterFactoryChain() throw();

    // registers a new factory
    void registerFactory(DataFormatterFactory* factory);
    // unregisters a new factory
    void unregisterFactory(DataFormatterFactory* factory) throw() ;

    DataFormatter* newFormatter(std::string const& algo);
    std::string capabilities() const;
protected:
    DataFormatterFactoryChain();

private:
    // declared but not implemented
    DataFormatterFactoryChain(DataFormatterFactoryChain const& );
    DataFormatterFactoryChain& operator=(DataFormatterFactoryChain const& );

private:
    std::vector<DataFormatterFactory*> _factories;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(DataFormatterFactoryChain) ;
};

typedef pattern::Singleton<DataFormatterFactoryChain> DataFormatterProvider;


_ACS_END_NESTED_NAMESPACE

#endif /* _DataFormatterFactoryChain_h_ */
