// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2016/07/13 09:24:54  marfav
	CSGACQ-86 using SQLString instead of free SQL code
	
	Revision 2.3  2014/02/05 15:51:51  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after being removed by mistake
	
	Revision 2.2  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	Revision 2.1  2013/02/04 15:40:17  marpas
	disseminationpackingalgo_id used for t_temporarydistribution
	
	Revision 2.0  2013/01/21 17:52:32  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.9  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.8  2012/03/08 11:18:27  marpas
	refactoring
	
	Revision 1.7  2012/02/29 15:17:45  chicas
	Added getAlgoKeyString and extended getconf to bool and vector string
	
	Revision 1.6  2009/06/05 09:02:49  crivig
	added cvrg algos
	
	Revision 1.5  2009/03/24 09:41:40  crivig
	modified to handle new records in t_distributionitems
	
	Revision 1.4  2008/02/01 14:27:15  crivig
	qualifyreport algo added
	
	Revision 1.3  2007/03/29 09:42:27  crivig
	*** empty log message ***
	
	Revision 1.2  2007/03/15 15:04:15  crivig
	coding and compiling...
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	

*/



#ifndef _DistributorCreator_H_
#define _DistributorCreator_H_ 

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <string>
#include <vector>
#include <map>
#include <DateTime.h>
#include <dbPersistent.h>
#include <ConfigurationSingleton.h>


_ACS_BEGIN_NAMESPACE(acs)
class DateTime ;
class dbConnectionPool ;
class dbConnection ;

class DistributorCreator // PRQA S 2109
{		
public:
	exDECLARE_EXCEPTION(DistributorException,dbException) ; // PRQA S 2131, 2153, 2502

public:
	enum SummaryType {
			DistributionIdSuccesfullyCreated,
			TemporaryDistributionIdHasProblem
			 } ;
public:

	explicit DistributorCreator(const std::string &algo);
	virtual ~DistributorCreator() throw() ;
	const std::string &algo() const throw() ;

	virtual void createDistribution(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const = 0 ;

	virtual bool simulation() const ;

	static void emitSummary() ;

#ifdef CLASS_UNDER_TEST
public:
#else
protected:
#endif
    struct  TemporaryInfo // PRQA S 2173, 2175
    {
        TemporaryInfo() : 
            id(), userid(),mediatype(), filename(), 
            fileversion(), format(), duedate(), 
            timestamp(), cartid(), subreq(), qualifierFile(), 
            policy(), dissempkalgoid() {}

        ~TemporaryInfo() throw() {} 

        TemporaryInfo(const TemporaryInfo &r) :
            id(r.id), 
            userid(r.userid),
            mediatype(r.mediatype), 
            filename(r.filename), 
            fileversion(r.fileversion), 
            format(r.format), 
            duedate(r.duedate), 
            timestamp(r.timestamp), 
            cartid(r.cartid), 
            subreq(r.subreq), 
            qualifierFile(r.qualifierFile), 
            policy(r.policy),
            dissempkalgoid(r.dissempkalgoid) 
        {}

        TemporaryInfo& operator=(const TemporaryInfo &r)
        {
            if ( this != &r ) {
                id = r.id ; 
                userid = r.userid ;
                mediatype = r.mediatype ; 
                filename = r.filename ; 
                fileversion = r.fileversion ; 
                format = r.format ; 
                duedate = r.duedate ; 
                timestamp = r.timestamp ; 
                cartid = r.cartid ; 
                subreq = r.subreq ; 
                qualifierFile = r.qualifierFile ; 
                policy = r.policy ; 
                dissempkalgoid = r.dissempkalgoid ; 
            }
            return *this ;
        } 
        // PRQA S 2100 L1
		dbPersistent::IdType id;
		std::string userid;
		std::string mediatype;
		std::string filename;
		std::string fileversion;
		std::string format;
        DateTime duedate;
		DateTime timestamp;
		long long cartid;
		int subreq;
		std::string qualifierFile;
		int policy;
        int dissempkalgoid ;
        // PRQA L:L1
    } ;

	void sendInDistribution(const std::vector<TemporaryInfo> &, dbConnection &)  const ;
	void queryFromTempDist(const std::string & sqlCommand, const std::string &whyQuery, std::vector <TemporaryInfo> & TmpInfo, dbConnection &)  const ;
	void deleteFromTempDist(const std::vector<dbPersistent::IdType> &id, dbConnection &) const;
    dbPersistent::IdType send1InDistribution(const TemporaryInfo &tmpInfo, dbConnection &) const ;

    void getConf(const std::string &keyEnd, std::string &, off_t &,const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, double &,const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, std::string &, const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, bool &, const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, std::vector<std::string> &, const std::string &subparam) const ;

	virtual std::string buildKey(const std::string &keyEnd, const std::string &subparam="") const ;
	virtual std::string getAlgoKeyString() const ;
	static void appendSummary(SummaryType, dbPersistent::IdType) ;

private:
	// declared but not implemented 
	DistributorCreator();
	DistributorCreator(const DistributorCreator&);
	DistributorCreator & operator=(const DistributorCreator&);	
protected:
	std::string _algo ; // PRQA S 2101 2

private:
	static std::map<SummaryType, std::vector<dbPersistent::IdType> > _summary ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributorCreator);
};


_ACS_END_NAMESPACE

#endif /* _DistributorCreator_H_ */
