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
 *  Revision 1.6  2017/12/13 11:32:13  davide.tiriticco
 *  Added default value for prefix in timestamp strategy
 *
 *  Revision 1.5  2017/12/13 09:34:04  davide.tiriticco
 *  Added timestamp strategy for temporary files
 *
 *  Revision 1.4  2015/05/04 13:58:48  marfav
 *  Use of default strategy values fixed
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
 *  Revision 5.2  2013/06/18 12:50:08  marpas
 *  qa warnings
 *
 *  Revision 5.1  2013/06/11 13:43:45  lucio.pulvirenti
 *  Added string parameter valorized with tempnamevalue (e.g. .LOCK) to CTOR.
 *  Added _tempnamevalue to IF and getTempnamevalue() method to return it.
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.2  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.1  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.0  2011/10/18 14:43:49  marpas
 *  new statistics adopted
 *
 *  Revision 1.3  2011/09/29 16:46:53  marpas
 *  rules fixing
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

#ifndef _TempStrategy_H_
#define _TempStrategy_H_

#include <acs_c++config.hpp>
#include <string>
#include <ThreadSafe.h>

_ACS_BEGIN_NAMESPACE(acs) 
_ACS_BEGIN_NESTED_NAMESPACE(curl, temp) 

/**
 * \brief This is the base class for all temporary strategies
 */
class AbstractTempStrategy { 
public:
	/**
	 * \brief Default constructor
	 */
    AbstractTempStrategy() = default ;
    
    AbstractTempStrategy(const AbstractTempStrategy & ) = delete ;
    AbstractTempStrategy & operator=(const AbstractTempStrategy & ) = delete ;

    /**
     * \brief Destructor
     */
    virtual ~AbstractTempStrategy() = default ;
	
	/**
	 * \brief Returns true if this class applies the naming strategy to parent directories, false otherwise
	 */
    virtual bool isDirNameChanged() const = 0;
    
	/**
	 * \brief Returns true if this class applies the naming strategy to file names, false otherwise
	 */
    virtual bool isFileNameChanged() const = 0;
    
	/**
	 * \brief Applies the naming strategy (if any) to the parent directory of the input path
	 */
    virtual std::string getTemporaryDir(std::string const& path) const = 0;
 
	/**
	 * \brief Applies the naming strategy (if any) to the filename of the input path
	 */
    virtual std::string getTemporaryFile(std::string const& path) const = 0;

	/**
	 * \brief Clones the object
	 */
    virtual AbstractTempStrategy* clone() const = 0;
    
	/**
	 * \brief Returns the temporary path computed according to the defined strategy
	 */
    virtual std::string getTemporaryPath(std::string const& path) const;
    
	/**
	 * \brief Reset the object
	 */
	virtual void reset();
	
};



/**
 * \brief This class implements the strategy that appends to the parent directory the input suffix.
 * 		  The computation of the temporary directory name is obviously deterministic for a given path.
 */
class TempDirStrategy : public AbstractTempStrategy { // PRQA S 2109
public:
	/**
	 * \brief Constructor
	 */
    explicit TempDirStrategy(std::string const &tempdirvalue = "_TMP_") ;

	/**
	 * \brief Destructor
	 */
    ~TempDirStrategy() override = default ;

	/**
	 * \brief Returns true if this class applies the naming strategy to parent directories, false otherwise
	 */
    bool isDirNameChanged() const override ;
    
	/**
	 * \brief Returns true if this class applies the naming strategy to file names, false otherwise
	 */
    bool isFileNameChanged() const override ;
    
 	/**
	 * \brief Applies the naming strategy (if any) to the parent directory of the input path
	 * 		  The computation of the temporary directory name is obviously deterministic for a given path.
	 */   
    std::string getTemporaryDir(std::string const& path) const override ;
    
	/**
	 * \brief Applies the naming strategy (if any) to the filename of the input path
	 * 		  The computation of the temporary filename is obviously deterministic for a given path.
	 */
    std::string getTemporaryFile(std::string const& path) const override ;
    
 	/**
	 * \brief Clones the object
	 */   
	AbstractTempStrategy* clone() const override ;
	
private:
    std::string tempDir_ ; 
};


/**
 * \brief This class implements the strategy that adds a prefix to the filename of the input path.
 */
class PrefixTempStrategy : public AbstractTempStrategy { // PRQA S 2109
public:

	/**
	 * \brief Constructor
	 */
    explicit PrefixTempStrategy(std::string const &prefix = ".");

	/**
	 * \brief Destructor
	 */
    ~PrefixTempStrategy() override = default ;

	/**
	 * \brief Returns always false since this class does not modify the parent directory of an input path
	 */
    bool isDirNameChanged() const override ;

	/**
	 * \brief Returns always true since this class modify the filename of an input path
	 */
    bool isFileNameChanged() const override ;

 	/**
	 * \brief Applies the naming strategy (if any) to the parent directory of the input path
	 * 		  The computation of the temporary directory name is obviously deterministic for a given path.
	 */ 
    std::string getTemporaryDir(std::string const& path) const override ;

	/**
	 * \brief Applies the naming strategy (if any) to the filename of the input path
	 * 		  The computation of the temporary filename is obviously deterministic for a given path.
	 */
    std::string getTemporaryFile(std::string const& path) const override ;

 	/**
	 * \brief Clones the object
	 */ 
	AbstractTempStrategy* clone() const override ;
	
private:
    std::string prefix_ ; 
};


/**
 * \brief This class implements the strategy that adds a suffix to the filename of the input path.
 */
class PostfixTempStrategy : public AbstractTempStrategy { // PRQA S 2109
public:

	/**
	 * \brief Constructor
	 */ 
    explicit PostfixTempStrategy(std::string const &postfix = ".lock");

	/**
	 * \brief Destructor
	 */
    ~PostfixTempStrategy() override = default ;

	/**
	 * \brief Returns always false since this class does not modify the parent directory of an input path
	 */
    bool isDirNameChanged() const override ;
    
	/**
	 * \brief Returns always true since this class modify the filename of an input path
	 */
    bool isFileNameChanged() const override ;

 	/**
	 * \brief Applies the naming strategy (if any) to the parent directory of the input path
	 * 		  The computation of the temporary directory name is obviously deterministic for a given path.
	 */     
    std::string getTemporaryDir(std::string const& path) const override ;

	/**
	 * \brief Applies the naming strategy (if any) to the filename of the input path
	 * 		  The computation of the temporary filename is obviously deterministic for a given path.
	 */    
    std::string getTemporaryFile(std::string const& path) const override ;
    
 	/**
	 * \brief Clones the object
	 */ 
	AbstractTempStrategy* clone() const override ;

private:
    std::string postfix_ ; 
};


/**
 * \brief This class implements the strategy that adds a prefix and a timestamp (HH_MM_SS) as a suffix to the filename of the input path.
 */
class TimestampTempStrategy : public AbstractTempStrategy { // PRQA S 2109
public:

	/**
	 * \brief Constructor
	 */ 
    explicit TimestampTempStrategy(std::string const & = ".");

	/**
	 * \brief Destructor
	 */
    ~TimestampTempStrategy() override = default ;

	/**
	 * \brief Returns always false since this class does not modify the parent directory of an input path
	 */    
    bool isDirNameChanged() const override ;

	/**
	 * \brief Returns always true since this class modify the filename of an input path
	 */
    bool isFileNameChanged() const override ;

 	/**
	 * \brief Applies the naming strategy to the parent directory of the input path
	 * 		  The computation of the temporary directory name is deterministic for a given path.
	 */ 
    std::string getTemporaryDir(std::string const& path) const override ;

	/**
	 * \brief Applies the naming strategy to the filename of the input path. The computation of the temporary filename
	 *		  is NOT deterministic for a given path due to the use of timestamps. Thus, for design choice, this function
	 *		  caches the last computed value associated to a given input path and, until a new path is provided, it will
	 *		  always returns this value. If for a given path an updated value is needed, then the reset() function should
	 *		  be used to invalidate caches.
	 */
    std::string getTemporaryFile(std::string const& path) const override ;
    
 	/**
	 * \brief Clones the object
	 */ 
	AbstractTempStrategy* clone() const override ;
	
 	/**
	 * \brief Invalidate caches for a given instance
	 */ 
	void reset() override ;
    
private:
    std::string prefix_ ;
    Mutex mutex_ = {} ;
    mutable bool isValidCache_ = false ;
	mutable std::string referenceFilePath_ = "" ;
	mutable std::string tmpFileCache_= "" ;
};

/**
 * \brief This class implements the no-op strategy.
 */
class NoTempStrategy : public AbstractTempStrategy { // PRQA S 2109
public:

	/**
	 * \brief Constructor
	 */ 
    NoTempStrategy() = default ;

	/**
	 * \brief Destructor
	 */
    ~NoTempStrategy() override = default ;

	/**
	 * \brief Returns always false since this class does not modify the parent directory of an input path
	 */ 
    bool isDirNameChanged() const override ;

	/**
	 * \brief Returns always false since this class does not modify the filename of an input path
	 */ 
    bool isFileNameChanged() const override ;

 	/**
	 * \brief Applies the naming strategy to the parent directory of the input path
	 * 		  The computation of the temporary directory name is deterministic for a given path.
	 */ 
    std::string getTemporaryDir(std::string const& path) const override ;

	/**
	 * \brief Applies the naming strategy (if any) to the filename of the input path
	 * 		  The computation of the temporary filename is obviously deterministic for a given path.
	 */
    std::string getTemporaryFile(std::string const& path) const override ;

 	/**
	 * \brief Clones the object
	 */ 
	AbstractTempStrategy* clone() const override ;
};


_ACS_END_NESTED_NAMESPACE

_ACS_END_NAMESPACE

#endif /* _TempStrategy_H_ */
