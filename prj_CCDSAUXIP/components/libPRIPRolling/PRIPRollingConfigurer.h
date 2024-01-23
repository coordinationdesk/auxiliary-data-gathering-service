// PRQA S 1050 EOF
/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    PRIPRolling Library

*/ 

#ifndef _PRIPRollingConfigurer_h_
#define _PRIPRollingConfigurer_h_

#include <rsResourceSet.h>

_ACS_BEGIN_NAMESPACE(acs)

class PRIPRollingConfigurer {
public:
	exDECLARE_EXCEPTION(exMandatoryConfigMissingException, exException);

	/** Deleted special functions */
	PRIPRollingConfigurer()                                             = delete;
	PRIPRollingConfigurer(PRIPRollingConfigurer&&) noexcept             = delete;
	PRIPRollingConfigurer& operator=(PRIPRollingConfigurer&&) noexcept  = delete;
	
	/** Special functions */
	explicit PRIPRollingConfigurer(const rsResourceSet&);
	PRIPRollingConfigurer(const PRIPRollingConfigurer&);
	PRIPRollingConfigurer& operator=(const PRIPRollingConfigurer&);
	virtual ~PRIPRollingConfigurer() noexcept;
	
	/** not mandatory configuration */
	size_t getDatabasePollingTime(bool* set = 0) const;
	size_t getRestartTime(bool* set = 0) const;
	size_t getMaxItems(bool* set = 0) const;
	bool getRemoveRecord(bool* set = 0) const;
	
	/** Logs the configuration loaded */
	void show() const;
	
public:
	static const std::string PRIP_ROLLING_DATABASE_POLLING_TIME_TAG;
	static const std::string PRIP_ROLLING_RESTART_TIME_TAG;
	static const std::string PRIP_ROLLING_MAXITEMS_TAG;
	static const std::string PRIP_ROLLING_REMOVE_RECORD_TAG;

#ifndef TEST_LIB
private:
#else
public:
#endif
	static const size_t defaultDbPollingTime;
	static const size_t defaultRestartTime;
	static const size_t defaultMaxItems;
	static const bool defaultRemoveRecord;
private:
	rsResourceSet rset_;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PRIPRollingConfigurer);
};

_ACS_END_NAMESPACE

#endif /* _PRIPRollingConfigurer_h_ */
