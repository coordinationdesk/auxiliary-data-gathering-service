// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/10/20 09:00:51  davide.tiriticco
	Added doc comments
	
	Revision 1.2  2017/10/20 08:37:45  davide.tiriticco
	Header guard moved on top
	
	Revision 1.1  2017/10/20 08:29:52  davide.tiriticco
	First add
	
	
*/

#ifndef _NoMergeRegXOrderCreator_H_
#define _NoMergeRegXOrderCreator_H_

#include <NoMergeOrderCreator.h>
#include <RegExp.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief This class implements the REG-X order generation algorithm
 */
class NoMergeRegXOrderCreator : public NoMergeOrderCreator { // PRQA S 2109, 2153

public:

	/**
	 * \brief Constructor
	 */
	NoMergeRegXOrderCreator(const std::string& type, 
				const std::string& algo, 
				const std::string& algoSuffix, 
				unsigned int satId, 
				dbConnection&);

	/**
	 * \brief Destructor
	 */
	virtual ~NoMergeRegXOrderCreator() throw();

	/**
	 * \brief Returns the algorithm key used in configuration file
	 */
	virtual std::string getAlgoKeyString() const;
	
	/**
	 * \brief Returns the regexp currently used by the algorithm.
	 */
	inline RegExp getRegExp() const { return _regex; }
	
	/**
	 * \brief Returns the algorithm suffix (e.g., REG-X-NT --> NT)
	 */
	inline std::string getAlgoSuffix() const { return _algoSuffix; }
	
protected:

	/**
	 * \brief Adds the temporary order in the orders vector to be created if and only if it matches the input regex
	 */
	virtual void appendTmpOrder(double maxTime, const TemporaryOrder &order, std::vector<TemporaryOrder> &orders, dbConnection &conn) const;

	/**
	 * \brief Loads the duplicate allowed flag from configuration file
	 */
	virtual void loadDuplicatesAllowed(const std::string& dbKey);

	/**
	 * \brief Loads from configuration the regular expression to be used
	 */	
	virtual void loadRegex(const std::string& dbKey);
		
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(NoMergeRegXOrderCreator)

	/**
	 * \brief Default constructor. Defined but not implemented
	 */	
	NoMergeRegXOrderCreator();

	/**
	 * \brief Copy constructor. Defined but not implemented.
	 */
	NoMergeRegXOrderCreator(const NoMergeRegXOrderCreator&);

	/**
	 * \brief Assignment operator overload. Defined but not implemented
	 */
	NoMergeRegXOrderCreator& operator=(const NoMergeRegXOrderCreator&);
	
private:

	std::string _algoSuffix;
	RegExp _regex;
};

_ACS_END_NAMESPACE

#endif /* _NoMergeRegxOrderCreator_H_ */
