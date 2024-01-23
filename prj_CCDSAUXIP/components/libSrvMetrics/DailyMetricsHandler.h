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

#ifndef _DailyMetricsHandler_H_
#define _DailyMetricsHandler_H_

#include <exMacros.h>
#include <exException.h>
#include <Filterables.h>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>
#include <dbConnection.h>

_ACS_BEGIN_NAMESPACE(acs)

class BaseMetricsHandler;
class MetricsHandlerFactory;

class DailyMetricsHandler : public BaseMetricsHandler {
public:
	friend class MetricsHandlerFactory ; // PRQA S 2107

	exDECLARE_EXCEPTION(DailyMetricsHandlerException, exException);

	virtual ~DailyMetricsHandler() noexcept = default;
	
	void computeAndStore(dbConnection& conn, const DateTime& ref = DateTime());
	
	/* Setters */
	inline void setOriginToPublicationMetricsEnabled(bool v) { originToPublicationMetricsEnabled_ = v; }
	inline void setSubmissionToCompletionMetricsEnabled(bool v) { submissionToCompletionMetricsEnabled_ = v; }
	inline void setArchivedMetricsEnabled(bool v) { submissionToCompletionMetricsEnabled_ = v; }
	inline void setProdTypeArchivedMetricsEnabled(bool v) { prodTypeArchivedMetricsEnabled_ = v; }
	inline void setDownloadMetricsEnabled(bool v) { submissionToCompletionMetricsEnabled_ = v; }
	inline void setRetrievedMetricsEnabled(bool v) { submissionToCompletionMetricsEnabled_ = v; }
	inline void setIncludeMetricTypeInArchivedMetrics(bool v) { includeMetricTypeInArchivedMetrics_ = v; }
	
	/* Getters */
	inline bool getOriginToPublicationMetricsEnabled() const { return originToPublicationMetricsEnabled_; }
	inline bool getSubmissionToCompletionMetricsEnabled() const { return submissionToCompletionMetricsEnabled_; }
	inline bool getArchivedMetricsEnabled() const { return submissionToCompletionMetricsEnabled_; }
	inline bool getProdTypeArchivedMetricsEnabled() const { return prodTypeArchivedMetricsEnabled_; }
	inline bool getDownloadMetricsEnabled() const { return submissionToCompletionMetricsEnabled_; }
	inline bool getRetrievedMetricsEnabled() const { return submissionToCompletionMetricsEnabled_; }
	inline bool getIncludeMetricTypeInArchivedMetrics() const { return includeMetricTypeInArchivedMetrics_; }
	
private:
	/**
	 * OriginToPublication metrics wrapper
	 */
	typedef struct OriginToPublicationMetrics {
		OriginToPublicationMetrics() :
			prodType(),
			platformShortName(),
			platformSerialId(),
			min(0.0),
			max(0.0),
			avg(0.0)
		{ }
		
		OriginToPublicationMetrics(const OriginToPublicationMetrics& m) :
			prodType(m.prodType),
			platformShortName(m.platformShortName),
			platformSerialId(m.platformSerialId),
			min(m.min),
			max(m.max),
			avg(m.avg)
		{ }
		
		OriginToPublicationMetrics& operator=(const OriginToPublicationMetrics& m) {
			if (this != &m) {
				prodType = m.prodType;
				platformShortName = m.platformShortName;
				platformSerialId = m.platformSerialId;
				min = m.min;
				max = m.max;
				avg = m.avg;
			}
			
			return *this;
		}
		
		std::string prodType;
		std::string platformShortName;
		std::string platformSerialId;
		float min;
		float max;
		float avg;
	} OriginToPublicationMetrics;
	
	/**
	 * SubmissionToCompletion metrics wrapper
	 */
	typedef struct SubmissionToCompletionMetrics {
		SubmissionToCompletionMetrics() :
			min(0.0),
			max(0.0),
			avg(0.0)
		{ }
		
		SubmissionToCompletionMetrics(const SubmissionToCompletionMetrics& m) :
			min(m.min),
			max(m.max),
			avg(m.avg)
		{ }
		
		SubmissionToCompletionMetrics& operator=(const SubmissionToCompletionMetrics& m) {
			if (this != &m) {
				min = m.min;
				max = m.max;
				avg = m.avg;
			}
			
			return *this;
		}

		float min;
		float max;
		float avg;
	} SubmissionToCompletionMetrics;

	/**
	 * Archived metrics wrapper
	 */
	typedef struct ArchivedMetrics {
		ArchivedMetrics() :
			prodType(),
			platformShortName(),
			platformSerialId(),
			size(0),
			count(0),
			prodTypeSet(true)
		{ }
		
		ArchivedMetrics(const ArchivedMetrics& m) :
			prodType(m.prodType),
			platformShortName(m.platformShortName),
			platformSerialId(m.platformSerialId),
			size(m.size),
			count(m.count),
			prodTypeSet(m.prodTypeSet)
		{ }
		
		ArchivedMetrics& operator=(const ArchivedMetrics& m) {
			if (this != &m) {
				prodType = m.prodType;
				platformShortName = m.platformShortName;
				platformSerialId = m.platformSerialId;
				size = m.size;
				count = m.count;
				prodTypeSet = m.prodTypeSet;
			}
			
			return *this;
		}
		
		std::string prodType;
		std::string platformShortName;
		std::string platformSerialId;
		unsigned long long size;
		unsigned long long count;
		bool prodTypeSet;
	} ArchivedMetrics;
	
	/**
	 * Retrieved metrics wrapper
	 */
	typedef struct RetrievedMetrics {
		RetrievedMetrics() :
			prodType(),
			platformShortName(),
			platformSerialId(),
			size(0),
			completed(0),
			failed(0)
		{ }
		
		RetrievedMetrics(const RetrievedMetrics& m) :
			prodType(m.prodType),
			platformShortName(m.platformShortName),
			platformSerialId(m.platformSerialId),
			size(m.size),
			completed(m.completed),
			failed(m.failed)
		{ }
		
		RetrievedMetrics& operator=(const RetrievedMetrics& m) {
			if (this != &m) {
				prodType = m.prodType;
				platformShortName = m.platformShortName;
				platformSerialId = m.platformSerialId;
				size = m.size;
				completed = m.completed;
				failed = m.failed;
			}
			
			return *this;
		}
		
		std::string prodType;
		std::string platformShortName;
		std::string platformSerialId;
		unsigned long long size;
		unsigned long long completed;
		unsigned long long failed;
	} RetrievedMetrics;
	
	/**
	 * Download metrics wrapper
	 */
	typedef struct DownloadMetrics {
		DownloadMetrics() :
			prodType(),
			platformShortName(),
			platformSerialId(),
			serviceAlias(),
			size(0),
			completed(0),
			failed(0)
		{ }
		
		DownloadMetrics(const DownloadMetrics& m) :
			prodType(m.prodType),
			platformShortName(m.platformShortName),
			platformSerialId(m.platformSerialId),
			serviceAlias(m.serviceAlias),
			size(m.size),
			completed(m.completed),
			failed(m.failed)
		{ }
		
		DownloadMetrics& operator=(const DownloadMetrics& m) {
			if (this != &m) {
				prodType = m.prodType;
				platformShortName = m.platformShortName;
				platformSerialId = m.platformSerialId;
				serviceAlias = m.serviceAlias;
				size = m.size;
				completed = m.completed;
				failed = m.failed;
			}
			
			return *this;
		}
		
		std::string prodType;
		std::string platformShortName;
		std::string platformSerialId;
		std::string serviceAlias;
		unsigned long long size;
		unsigned long long completed;
		unsigned long long failed;
	} DownloadMetrics;

protected:
	DailyMetricsHandler();

private:
	/** Used to compute OriginToPublications daily metrics using ref as reference time */
	void loadOriginToPublicationDailyMetrics(const DateTime& ref, std::vector<OriginToPublicationMetrics>& res, dbConnection& conn);
	/** Used to compute SubmissionToCompletion daily metrics using ref as reference time */
	void loadSubmissionToCompletionDailyMetrics(const DateTime& ref, std::vector<SubmissionToCompletionMetrics>& res, dbConnection& conn);
	/** Used to compute Archived and Retrieved metrics */
	void loadArchivedAndRetrievedMetrics(std::vector<ArchivedMetrics>& archres, std::vector<RetrievedMetrics>& retres, dbConnection& conn); 
	/** Used to compute Archived metrics independently by the product type */
	void loadProdTypeArchivedMetrics(std::vector<ArchivedMetrics>& archres, dbConnection& conn);
	/** Used to compute Download metrics */ 
	void loadDownloadMetrics(std::vector<DownloadMetrics>& res, dbConnection& conn); 
	/** Stores OriginToPublications metrics into database */
	void storeOriginToPublicationDailyMetrics(const DateTime& ref, const std::vector<OriginToPublicationMetrics>& res, dbConnection& conn);
	/** Stores SubmissionToCompletion metrics into database */
	void storeSubmissionToCompletionDailyMetrics(const DateTime& ref, const std::vector<SubmissionToCompletionMetrics>& res, dbConnection& conn);
	/** Stores Archived metrics into database */
	void storeArchivedMetrics(const DateTime& ref, const std::vector<ArchivedMetrics>& met, dbConnection& conn);
	/** Stores Retrieved metrics into database */
	void storeRetrievedMetrics(const DateTime& ref, const std::vector<RetrievedMetrics>& met, dbConnection& conn);
	/** Stores Download metrics into database */
	void storeDownloadMetrics(const DateTime& ref, const std::vector<DownloadMetrics>& met, dbConnection& conn);
	/** Locks the input db table */
	void lockDbTable(const std::string& table, const std::string& mode, dbConnection& conn);
	/** Updates the counter metrics identified by key and returns the number of affected rows */
	size_t updateCounterMetric(const std::string& key, const DateTime& ref, unsigned long long val, dbConnection&); 
	/** Inserts the counter metric identified by key into database */
	void insertCounterMetric(const std::string& key, const DateTime& ref, unsigned long long val, dbConnection&);
private:
	DailyMetricsHandler(const DailyMetricsHandler&)               = delete;
	DailyMetricsHandler(const DailyMetricsHandler&&)              = delete;
	DailyMetricsHandler& operator=(const DailyMetricsHandler&)    = delete;
	DailyMetricsHandler& operator=(const DailyMetricsHandler&&)   = delete;
	
private:
	bool originToPublicationMetricsEnabled_;
	bool submissionToCompletionMetricsEnabled_;
	bool downloadMetricsEnabled_;
	bool archivedMetricsEnabled_;
	bool prodTypeArchivedMetricsEnabled_;
	bool retrievedMetricsEnabled_;
	bool includeMetricTypeInArchivedMetrics_;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(DailyMetricsHandler);
};

_ACS_END_NAMESPACE

#endif /* _DailyMetricsHandler_H_ */
