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
    enum Type {
        file            = '-', 
        directory       = 'd', 
        characterDevice = 'c', 
        blockDevice     = 'b', 
        symbolicLink    = 'l', 
        socket          = 's', 
        sticky          = 't'
    } ;

    enum Permissions {
        read = 0, 
        write = 1, 
        execute = 2
    } ;

public:
    FilePermissions();
    explicit FilePermissions(std::string const &);
    FilePermissions(FilePermissions const& );
    FilePermissions& operator=(FilePermissions const& );
    ~FilePermissions() throw();
    const std::string &str() const throw() ;
    void init(std::string const &);

    bool userCanRead() const throw() ;
    bool userCanWrite() const throw() ;
    bool userCanExecute() const throw() ;
    bool groupCanRead() const throw() ;
    bool groupCanWrite() const throw() ;
    bool groupCanExecute() const throw() ;
    bool otherCanRead() const throw() ;
    bool otherCanWrite() const throw() ;
    bool otherCanExecute() const throw() ;
    bool isFile() const throw() ;
    bool isDirectory() const throw() ;
    bool isLink() const throw() ;
    bool isDevice() const throw() ;
    Type getType() const throw() ;
    const std::string &getPermissions() const throw() ;
private:
    Type _type;
    bool _user[3]; // PRQA S 4403 3
    bool _group[3];
    bool _other[3];
    std::string _permissions;
};


_ACS_END_NESTED_NAMESPACE

#endif /* _FilePermissions_H_ */
