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
 *  Revision 5.4  2014/06/04 17:29:21  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 5.3  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:20  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.4  2012/12/03 16:56:48  marpas
 *  using new StringUtils interface
 *
 *  Revision 2.3  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.2  2012/03/27 10:52:22  micmaz
 *  work in progress
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

#include <FilePermissions.h>


#include <StringUtils.h>

#include <iostream>
#include <ostream>
#include <sstream>
#include <cstring>



_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

using std::string ;

FilePermissions::FilePermissions(const string &permissions) 
{
    init(permissions);
}

void FilePermissions::init(string const &permissions) {
    //the permissions line must be exactly of 10 characters.
    ACS_COND_THROW(permissions.length() != 10, exIllegalValueException("Wrong permissions string: \""+permissions+"\".")); // PRQA S 3081
    //the file type could be only -dcbl.
    //hereafter follows the complete list of all file types:
    //-    Regular file
    //b    Block special file
    //c    Character special file
    //d    Directory
    //l    Symbolic link
    //n    Network file
    //p    FIFO
    //s    Socket
    ACS_COND_THROW(string("-dcstbl").find(permissions[0]) == string::npos, // PRQA S 3081
        exIllegalValueException("Unknown file type: perm string is \"" + permissions + "\""));
    _type = static_cast<Type>(permissions[0]); // PRQA S 3013, 3081
    _user[read] = permissions[1] != '-';
    _user[write] = permissions[2] != '-'; // PRQA S 4400 8
    _user[execute] = permissions[3] != '-';
    _group[read] = permissions[4] != '-';
    _group[write] = permissions[5] != '-';
    _group[execute] = permissions[6] != '-';
    _other[read] = permissions[7] != '-';
    _other[write] = permissions[8] != '-';
    _other[execute] = permissions[9] != '-';
    _permissions = permissions;
}

const string &FilePermissions::str() const noexcept  // PRQA S 4120
{
    return _permissions;
}

bool FilePermissions::userCanRead()    const noexcept { return _user[read]; } // PRQA S 4120 12
bool FilePermissions::userCanWrite()   const noexcept { return _user[write]; }
bool FilePermissions::userCanExecute() const noexcept { return _user[execute]; }
bool FilePermissions::groupCanRead()    const noexcept { return _group[read]; }
bool FilePermissions::groupCanWrite()   const noexcept { return _group[write]; }
bool FilePermissions::groupCanExecute() const noexcept { return _group[execute]; }
bool FilePermissions::otherCanRead()    const noexcept { return _other[read]; }
bool FilePermissions::otherCanWrite()   const noexcept { return _other[write]; }
bool FilePermissions::otherCanExecute() const noexcept { return _other[execute]; }
bool FilePermissions::isFile() const  noexcept { return _type == file; }
bool FilePermissions::isDirectory() const noexcept { return _type == directory; }
bool FilePermissions::isLink() const noexcept { return _type == symbolicLink; }

bool FilePermissions::isDevice() const noexcept { 
    return (_type == characterDevice) || (_type == blockDevice) ; 
}

FilePermissions::Type FilePermissions::getType() const noexcept { return _type; } // PRQA S 4120

const string &FilePermissions::getPermissions() const noexcept // PRQA S 4120
{
    return _permissions;
}


_ACS_END_NESTED_NAMESPACE
