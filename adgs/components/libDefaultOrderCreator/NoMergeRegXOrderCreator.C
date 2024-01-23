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
	Revision 1.5  2018/03/12 14:54:30  davide.tiriticco
	Fixed bug in algorithm key. Added tmporders removal in case of unmatch with regexp
	
	Revision 1.4  2017/10/26 12:28:25  davide.tiriticco
	Fixed bug on duplicate order removal
	
	Revision 1.3  2017/10/20 09:30:57  davide.tiriticco
	qac++ analisys
	
	Revision 1.2  2017/10/20 09:00:51  davide.tiriticco
	Added doc comments
	
	Revision 1.1  2017/10/20 08:29:52  davide.tiriticco
	First add
	
	
*/

#include <NoMergeRegXOrderCreator.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(NoMergeRegXOrderCreator)

/*
 * Constructor
 */
NoMergeRegXOrderCreator::NoMergeRegXOrderCreator(const std::string& type, const std::string& algo, const std::string& algoSuffix, unsigned int satId, dbConnection& conn) :
	NoMergeOrderCreator(type, algo + algoSuffix, satId, conn, false),
	_algoSuffix(algoSuffix),
	_regex()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);
	
	setAgingPolicyOnTmpOrder(true);
	
	const std::string& dbKey(conn.getKey());
	loadAllowedQualified(dbKey); // recall it here because need the algoKeyString
	loadDuplicatesAllowed(dbKey);
	loadRegex(dbKey);
}

/*
 * Destructor
 */
NoMergeRegXOrderCreator::~NoMergeRegXOrderCreator() throw() { }

/*
 * Adds the temporary order in the orders vector to be created if and only if it matches the input regex
 */
void NoMergeRegXOrderCreator::appendTmpOrder(double maxTime, const TemporaryOrder &order, std::vector<TemporaryOrder> &orders, dbConnection &conn) const {// PRQA S 4214, 4020		
	bool remove = false;
	if (!isQualifiedAllowed(order)) {
		remove = true;
		ACS_LOG_WARNING("The temporary order id " << order.id()
			<< " will be deleted because it refers to file \"" << order.fileName() << "\""
 			<< " currently marked as not qualified");		
	}
	
	else if (!dupAllowed() && order.isDup()) {
		remove = true;
		ACS_LOG_WARNING("The temporary order id " << order.id()
			<< " will be deleted because it refers to file \"" << order.fileName() << "\""
 			<< " currently marked as DUP");
	}
	
	else if (!_regex.match(order.fileName())) {
		remove = true;
		ACS_LOG_INFO("The temporary order id " << order.id() << " that referes to file \"" << order.fileName() << "\""
			" does not match the regular expression \"" << _regex.getReg() << "\" and will be deleted.");			
	}

	if (remove) {
		try {
			if (!simulation()) {
				order.removeFromDb(conn);
			}
			if (simulation()) {
				ACS_LOG_WARNING("above deletion has been simulated");
			}
	 	} catch (std::exception &x) {
			ACS_LOG_ERROR("Cannot remove tmp order id: " << order.id() << " because: "
				<< x.what() // not all the stack
				<< "Processing continue: will retry later");
		}
		return; // no more processing on this one.
	}
	
	if (order.validityFlag()) {
		std::ostringstream msg; 
		msg << "[tmpid: " << order.id() << "] ";
		maxTime = getMaxWaitingTime("") / 86400.0; // PRQA S 4400
		try {
			maxTime = getMaxWaitingTime("",order.processorName()) / 86400.0; // PRQA S 4400
			msg << algo() << ": Using waiting time for processor \"" << order.processorName() << "\" " << maxTime << " [sec]";
		} catch (std::exception &x) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, x << "This exception has been ignored: using default") ;
			msg << algo() << ": Using default waiting time for processor \"" << order.processorName() << "\" " << maxTime << " [sec]";
		}

		bool append = false;

		if (!getAgingPolicyOnTmpOrder()) { // as usual by default argument value
			if( order.isInventoryExpired(maxTime)) { append = true; }
		}
		else { // new tmp order tstamp aging means _agingOnTmpTStamp == true
			if( order.isTmpOrderExpired(maxTime)) { append = true; }
		}

		if (append) {
			orders.push_back(order);
			ACS_LOG_INFO( msg.str() ); // print the message only if it is a candidate
		}


	} else {	

		ACS_LOG_WARNING("The temporary order id " << order.id()
			<< " cannot be processed because it refers to file \"" << order.fileName() << "\"" 
			<< " currently invalid");
		appendSummary(TemporaryOrderIdHasProblem,order.id());
	}
}

/*
 * Loads the duplicate allowed flag from configuration file
 */
void NoMergeRegXOrderCreator::loadDuplicatesAllowed(const std::string& dbKey) {
	bool nodup(false);
	std::string confKey = buildKey(_algoSuffix + ".NoDup");
	ConfigurationSingleton::instance()->get().getValue(confKey, nodup) ;// PRQA S 4412
   	
   	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded NoDup \"" + ((nodup) ? (std::string("true")) : (std::string("false"))) + "\" from configuration."); // PRQA S 3081, 3380, 3385
   	
   	setDupAllowed(!nodup);
}

/*
 * Loads the regular expression to be used from configuration file
 */
void NoMergeRegXOrderCreator::loadRegex(const std::string& dbKey) {
	std::string regex;
	std::string confKey = buildKey(_algoSuffix + ".Regex");
	ConfigurationSingleton::instance()->get().getValue(confKey, regex) ;// PRQA S 4412
   	
   	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded regexp \"" + regex + "\" from configuration.");
   	
   	_regex.setReg(regex);
}

/*
 * Returns the algorithm key used in configuration file
 */
std::string NoMergeRegXOrderCreator::getAlgoKeyString() const { // PRQA S 4020
	std::string prefix("REG-X-");
	if (StringUtils::equalsNoCase(StringUtils::substring(algo(), 0, prefix.length()), prefix)) {
		return "Reg-X-";
	}

	return "";
}


_ACS_END_NAMESPACE
