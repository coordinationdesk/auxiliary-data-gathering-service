// PRQA S 1050 EOF
/*

	Copyright 2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Metrics Library $


*/

#ifndef _BaseMetricsHandler_H_
#define _BaseMetricsHandler_H_

#include <dbConnection.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

class BaseMetricsHandler {
public:
	BaseMetricsHandler(const BaseMetricsHandler&)               = delete;
	BaseMetricsHandler(const BaseMetricsHandler&&)              = delete;
	BaseMetricsHandler& operator=(const BaseMetricsHandler&)    = delete;
	BaseMetricsHandler& operator=(const BaseMetricsHandler&&)   = delete;
	
	virtual ~BaseMetricsHandler() noexcept               = default;
	
	
	inline void setMaxInsertBlock(size_t block) { maxInsertBlock_ = block; }
	
	/** 
	 * \brief This functions implementes the procedure to compute the metrics and store them on database.
	 * It must be implemented by derived classes.
	 */
	virtual void computeAndStore(dbConnection& conn, const DateTime& ref = DateTime()) = 0;
	
protected:
	BaseMetricsHandler();
	
	static const size_t defaultMaxInsertBlock = 30;
	
	size_t maxInsertBlock_;
	
};

_ACS_END_NAMESPACE

#endif /* _BaseMetricsHandler_H_ */
