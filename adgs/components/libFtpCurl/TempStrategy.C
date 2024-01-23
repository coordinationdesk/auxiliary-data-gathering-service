// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2015, Advanced Computer Systems , Inc.
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
 *  Revision 1.6  2017/12/13 09:34:04  davide.tiriticco
 *  Added timestamp strategy for temporary files
 *
 *  Revision 1.5  2016/10/04 14:42:11  marfav
 *  OPMAN-196
 *  Management of basenames and dirnames now relies on libFile
 *  Class FtpDirUtility removed from the library
 *
 *  Revision 1.4  2015/04/23 17:41:50  marpas
 *  dynamic behaviour improved: isXXXNameChanged methods now can return false if there is no effective change.
 *  StingUtils pathJoin used to join dir and file names
 *
 *  Revision 1.3  2015/04/23 11:55:36  marpas
 *  interface rationalization
 *  coding best practices applied
 *  code optimized
 *
 *  Revision 1.2  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 1.1  2013/10/17 10:43:59  marpas
 *  merging files
 *
 *  Revision 5.1  2013/06/11 13:43:45  lucio.pulvirenti
 *  Added string parameter valorized with tempnamevalue (e.g. .LOCK) to CTOR.
 *  Added _tempnamevalue to IF and getTempnamevalue() method to return it.
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.3  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.2  2012/11/29 16:28:21  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.1  2012/11/27 13:26:07  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.2  2011/09/16 13:20:59  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.1  2011/07/01 16:42:11  micmaz
 *  changed the temporary file name strategy.
 *
 *
 *
 *
 */

#include <TempStrategy.h>
#include <File.h> 
#include <StringUtils.h> 
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs) 
_ACS_BEGIN_NESTED_NAMESPACE(curl, temp) 

using std::string ;

/*------------------------------------------------------------------
					AbstractTempStrategy definition
-------------------------------------------------------------------*/

/*
 * Default constructor
 */
AbstractTempStrategy::AbstractTempStrategy() {}

/*
 * Default destructor
 */
AbstractTempStrategy::~AbstractTempStrategy() throw() {} 

/*
 * Copy constructor
 */
AbstractTempStrategy::AbstractTempStrategy(const AbstractTempStrategy &) {}

/*
 * Assignment operator overload
 */
AbstractTempStrategy& AbstractTempStrategy::operator=(const AbstractTempStrategy &) { return *this ; } // no self-assignement chek needed // PRQA S 4072 

/*
 *  Returns the temporary path computed according to the defined strategy
 */
string AbstractTempStrategy::getTemporaryPath(string const& path) const {
    return StringUtils::pathJoin(getTemporaryDir(path),getTemporaryFile(path));
}

/*
 * Reset the object
 */
void AbstractTempStrategy::reset() { }



/*------------------------------------------------------------------
					TempDirStrategy definition
-------------------------------------------------------------------*/



/*
 * Constructor
 */
TempDirStrategy::TempDirStrategy(string const & tempdirvalue) :
    AbstractTempStrategy(),
    tempDir_(tempdirvalue) 
{}

/*
 * Destructor
 */
TempDirStrategy::~TempDirStrategy() throw() {}

/*
 * Returns true if this class applies the naming strategy to parent directories, false otherwise
 */
bool TempDirStrategy::isDirNameChanged() const { return not tempDir_.empty() ; }

/*
 * Returns true if this class applies the naming strategy to file names, false otherwise
 */
bool TempDirStrategy::isFileNameChanged() const { return true; }

/*
 * Applies the naming strategy (if any) to the parent directory of the input path.
 * The computation of the temporary directory name is obviously deterministic for a given path.
 */
string TempDirStrategy::getTemporaryDir(string const& path) const 
{
    return StringUtils::pathJoin(File::getDirPath(path, true),tempDir_) + "/";
}

/*
 * Applies the naming strategy (if any) to the filename of the input path.
 * The computation of the temporary filename is obviously deterministic for a given path.
 */
string TempDirStrategy::getTemporaryFile(string const& path) const 
{
   return "." + File::getFileName(path);
}

/*
 * Clones the object
 */
AbstractTempStrategy* TempDirStrategy::clone() const {
	return new TempDirStrategy(tempDir_);
}



/*------------------------------------------------------------------
					PrefixTempStrategy definition
-------------------------------------------------------------------*/


/*
 * Constructor
 */
PrefixTempStrategy::PrefixTempStrategy(const string & prefix) :
    AbstractTempStrategy(),
    prefix_(prefix) 
{}

/*
 * Destructor
 */
PrefixTempStrategy::~PrefixTempStrategy() throw() {}

/*
 * Returns always false since this class does not modify the parent directory of an input path
 */
bool PrefixTempStrategy::isDirNameChanged() const { return false ; }

/*
 * Returns always true since this class modify the filename of an input path
 */
bool PrefixTempStrategy::isFileNameChanged() const { return not prefix_.empty() ; }

/*
 * Applies the naming strategy (if any) to the parent directory of the input path
 * The computation of the temporary directory name is obviously deterministic for a given path.
 */
string PrefixTempStrategy::getTemporaryDir(string const& path) const {
    return File::getDirPath(path, true) + "/";
}

/*
 * Applies the naming strategy (if any) to the filename of the input path
 * The computation of the temporary filename is obviously deterministic for a given path.
 */
string PrefixTempStrategy::getTemporaryFile(string const& path) const 
{
    return prefix_ + File::getFileName(path);
}

/*
 * Clones the object
 */
AbstractTempStrategy* PrefixTempStrategy::clone() const {
	return new PrefixTempStrategy(prefix_);
}



/*------------------------------------------------------------------
					PostfixTempStrategy definition
-------------------------------------------------------------------*/


/*
 * Constructor
 */
PostfixTempStrategy::PostfixTempStrategy(string const & postfix) :
    AbstractTempStrategy(),
    postfix_(postfix) 
{
}

/*
 * Destructor
 */
PostfixTempStrategy::~PostfixTempStrategy() throw() {}

/*
 * Returns always false since this class does not modify the parent directory of an input path
 */
bool PostfixTempStrategy::isDirNameChanged() const { return false; }

/*
 * Returns always true since this class modify the filename of an input path
 */
bool PostfixTempStrategy::isFileNameChanged() const { return not postfix_.empty() ; }

/*
 * Applies the naming strategy (if any) to the parent directory of the input path
 * The computation of the temporary directory name is obviously deterministic for a given path.
 */
string PostfixTempStrategy::getTemporaryDir(string const& path) const 
{
    return File::getDirPath(path, true) + "/";
}

/*
 * Applies the naming strategy (if any) to the filename of the input path
 * The computation of the temporary filename is obviously deterministic for a given path.
 */
string PostfixTempStrategy::getTemporaryFile(string const& path) const 
{
    return File::getFileName(path) + postfix_;
}

/*
 * Clones the object
 */
AbstractTempStrategy* PostfixTempStrategy::clone() const {
	return new PostfixTempStrategy(postfix_);
}



/*------------------------------------------------------------------
				TimestampTempStrategy definition
-------------------------------------------------------------------*/


/*
 * Constructor
 */
TimestampTempStrategy::TimestampTempStrategy(const string & prefix) :
	AbstractTempStrategy(),
	prefix_(prefix),
	mutex_(),
	isValidCache_(false),
	referenceFilePath_(),
	tmpFileCache_()
{
	//empty
}

/*
 * Destructor
 */
TimestampTempStrategy::~TimestampTempStrategy() throw(){
	//empty
}

/*
 * Returns always false since this class does not modify the parent directory of an input path
 */
bool TimestampTempStrategy::isDirNameChanged() const {
	return false;
}

/*
 * Returns always true since this class modify the filename of an input path
 */
bool TimestampTempStrategy::isFileNameChanged() const {
	return true;
}

/*
 * Applies the naming strategy to the parent directory of the input path
 * The computation of the temporary directory name is deterministic for a given path.
 */
string TimestampTempStrategy::getTemporaryDir(string const& path) const {
	return File::getDirPath(path, true) + "/";
}

/*
 * Applies the naming strategy to the filename of the input path. The computation of the temporary filename
 * is NOT deterministic for a given path due to the use of timestamps. Thus, for design choice, this function
 * caches the last computed value associated to a given input path and, until a new path is provided, it will
 * always returns this value. If for a given path an updated value is needed, then the reset() function should
 * be used to invalidate caches.
 */
string TimestampTempStrategy::getTemporaryFile(string const& path) const { 
	Lock lock(mutex_);
	if (!isValidCache_ || referenceFilePath_ != path) {
		DateTime now;
		referenceFilePath_ = path;
		tmpFileCache_ = (prefix_ + File::getFileName(StringUtils::pathNormalize(path)) + "_" + now.timeString(0, true, '_'));
		isValidCache_ = true;
	}

	return tmpFileCache_;
}

/*
 * Clones the object
 */
AbstractTempStrategy* TimestampTempStrategy::clone() const {
	Lock lock(mutex_);
	TimestampTempStrategy* cloned = new TimestampTempStrategy(prefix_);
	cloned->isValidCache_ = isValidCache_;
	cloned->referenceFilePath_ = referenceFilePath_;
	cloned->tmpFileCache_ = tmpFileCache_;
	
	return cloned;
}

/*
 * Invalidate caches for a given instance
 */
void TimestampTempStrategy::reset() {
	Lock lock(mutex_);
	isValidCache_ = false;
	referenceFilePath_ = "";
	tmpFileCache_ = "";
}



/*------------------------------------------------------------------
					NoTempStrategy definition
-------------------------------------------------------------------*/


/*
 * Constructor
 */
NoTempStrategy::NoTempStrategy() :
    AbstractTempStrategy() 
{}

/*
 * Destructor
 */
NoTempStrategy::~NoTempStrategy() throw() {}

/*
 * Returns always false since this class does not modify the parent directory of an input path
 */
bool NoTempStrategy::isDirNameChanged() const { return false; }

/*
 * Returns always false since this class does not modify the filename of an input path
 */
bool NoTempStrategy::isFileNameChanged() const {  return false; }

/*
 * 
 */
string NoTempStrategy::getTemporaryDir(string const& path) const {
    return File::getDirPath(path, true) + "/";
}

/*
 * Applies the naming strategy to the parent directory of the input path
 * The computation of the temporary directory name is deterministic for a given path.
 */
string NoTempStrategy::getTemporaryFile(string const& path) const {
    return File::getFileName(path) ;
}

/*
 * Applies the naming strategy (if any) to the filename of the input path
 * The computation of the temporary filename is obviously deterministic for a given path.
 */
AbstractTempStrategy* NoTempStrategy::clone() const {
	return new NoTempStrategy();
}

_ACS_END_NESTED_NAMESPACE

_ACS_END_NAMESPACE
