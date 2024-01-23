/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.7  2017/12/21 13:14:42  davide.tiriticco
 *  Refactoring copy constructor and added resetStartegyState functionality
 *
 *  Revision 5.6  2017/12/13 09:34:04  davide.tiriticco
 *  Added timestamp strategy for temporary files
 *
 *  Revision 5.5  2015/04/23 17:39:45  marpas
 *  isChangeStrategy method changed into hasTemporaryTargetName
 *  debug improved
 *
 *  Revision 5.4  2015/04/23 11:56:27  marpas
 *  removing useless boost pointers
 *  coding best practices applied
 *
 *  Revision 5.3  2014/02/13 19:14:24  marpas
 *  potential crash when referring static const members fixed
 *
 *  Revision 5.2  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/06/11 14:28:05  lucio.pulvirenti
 *  Static const strings names with strategy value changed according to the new content of t_circulationtempnames table.
 *  Added string parameter to CTORs (default empty string) to pass tempnamevalue (e.g. .LOCK).
 *  Added tempnamevalue_ member to IF.
 *  Debug macro fixed.
 *
 *  Revision 5.0  2013/06/06 18:08:22  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.7  2013/05/29 10:53:59  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.6  2013/04/15 09:12:35  marpas
 *  coding best practices applied
 *  interfaces rationalization and simplification
 *
 *  Revision 2.5  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.4  2013/01/25 13:06:46  marpas
 *  using pattern::Singleton from libException
 *
 *  Revision 2.3  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/03/28 15:22:24  micmaz
 *  work in progress
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:52  marpas
 *  new statistics adopted
 *
 *  Revision 1.5  2011/09/29 16:46:53  marpas
 *  rules fixing
 *
 *  Revision 1.4  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.3  2011/08/29 10:24:29  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *
 *  Revision 1.2  2011/08/26 15:52:05  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *  IN PROGRESS - issue S1PDGS-1808: Improve the CirculationCurl bytes transfer
 *  http://jira.acsys.it/browse/S1PDGS-1808
 *
 *  Revision 1.1  2011/07/01 16:42:12  micmaz
 *  changed the temporary file name strategy.
 *
 *
 *
 *
 */

#ifndef _TemporaryFileService_H_
#define _TemporaryFileService_H_

#include<TempStrategy.h>
#include<exMacros.h>
#include<exStream.h>
#include<iostream>
#include<memory>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) 
class RemoteSite;
_ACS_BEGIN_NAMESPACE(temp) 

enum TempRenameStrategy { // NOSONAR - old enum
    noTemp = 0, prefix, postfix, tempDir, tstamp, unknownStrategy
};

class TemporaryFileService {

public:
    static const std::string PREFIX_NAME_STRATEGY;
    static const std::string SUFFIX_NAME_STRATEGY;
    static const std::string TEMP_DIR_NAME_STRATEGY;
    static const std::string TIMESTAMP_NAME_STRATEGY;
    static const std::string NO_NAME_STRATEGY;

    static const std::string PREFIX ;
    static const std::string SUFFIX ;
    static const std::string TEMPDIR ;
    static const std::string TSTAMP;

    static const std::string &get_DEFAULT_TEMP_NAME_STRATEGY() ;
    
    TemporaryFileService() ;
    explicit TemporaryFileService(TempRenameStrategy strategy, const std::string & = "");
    explicit TemporaryFileService(const std::string &strategyName, const std::string & = "");
    ~TemporaryFileService() = default ;
    TemporaryFileService& operator=(TemporaryFileService const& other);
    TemporaryFileService(TemporaryFileService const& other);

    std::string getTemporaryDir(std::string const& path) const ;
    std::string getTemporaryFile(std::string const& path) const ;
    std::string getTemporaryPath(std::string const& path) const ;
    void resetStrategyState();
    bool hasTemporaryTargetName() const ;
    TempRenameStrategy getStrategy() const ;

    static TempRenameStrategy makeDefaultStrategy();
    static TempRenameStrategy makeStrategy(std::string const& strategyName);


private:
    void init();
    void setRemotePath(bool isTemporaryDir, bool isTemporaryFile, RemoteSite* site) const;

    std::unique_ptr<AbstractTempStrategy> handle_ = {} ;

    TempRenameStrategy strategy_ = prefix ;
	std::string tempnamevalue_ = "" ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(TemporaryFileService) ;
};

std::ostream &operator<< (std::ostream &, TempRenameStrategy) ;
acs::exostream &operator<< (acs::exostream &, TempRenameStrategy) ;

_ACS_END_NAMESPACE

_ACS_END_NESTED_NAMESPACE


#endif /* _TemporaryFileService_H_ */
