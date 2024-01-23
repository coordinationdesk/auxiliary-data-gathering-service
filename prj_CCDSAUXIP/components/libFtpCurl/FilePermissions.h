// PRQA S 1050 EOF
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
 *  Revision 5.2  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.2  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.1  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.0  2011/10/18 14:43:50  marpas
 *  new statistics adopted
 *
 *  Revision 1.4  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.3  2011/09/08 09:28:23  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.2  2011/04/29 17:20:41  micmaz
 *  check style changes
 *
 *  Revision 1.1  2011/01/13 10:57:28  micmaz
 *  added mput() and mget() method in order to open only one connection if you must trasfer more than one file to/from the same host.
 *
 *
 *
 */

#ifndef _FilePermissions_H_
#define _FilePermissions_H_

#include <acs_c++config.hpp>
#include <string>


_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

class FilePermissions { // PRQA S 2109
public:
    enum Type { // NOSONAR - old enum
        file            = '-', 
        directory       = 'd', 
        characterDevice = 'c', 
        blockDevice     = 'b', 
        symbolicLink    = 'l', 
        socket          = 's', 
        sticky          = 't'
    } ;

    enum Permissions { // NOSONAR - old enum
        read = 0, 
        write = 1, 
        execute = 2
    } ;

    FilePermissions() = default ;
    explicit FilePermissions(std::string const &);
    FilePermissions(FilePermissions const& ) = default ;
    FilePermissions& operator=(FilePermissions const& ) = default ;
    ~FilePermissions() noexcept = default ;
    const std::string &str() const noexcept ;
    void init(std::string const &);

    bool userCanRead() const noexcept ;
    bool userCanWrite() const noexcept ;
    bool userCanExecute() const noexcept ;
    bool groupCanRead() const noexcept ;
    bool groupCanWrite() const noexcept ;
    bool groupCanExecute() const noexcept ;
    bool otherCanRead() const noexcept ;
    bool otherCanWrite() const noexcept ;
    bool otherCanExecute() const noexcept ;
    bool isFile() const noexcept ;
    bool isDirectory() const noexcept ;
    bool isLink() const noexcept ;
    bool isDevice() const noexcept ;
    Type getType() const noexcept ;
    const std::string &getPermissions() const noexcept ;
private:
    Type _type = {} ;
    bool _user[3] = {} ; // NOSONAR - array 
    bool _group[3] = {} ; // NOSONAR - array 
    bool _other[3] = {} ; // NOSONAR - array 
    std::string _permissions = "" ;
};


_ACS_END_NESTED_NAMESPACE

#endif /* _FilePermissions_H_ */
