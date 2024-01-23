// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2014, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod: A.C.S. PackagerOptimiser Library $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 * 	$Log$
 * 	Revision 2.12  2014/06/25 16:24:12  marpas
 * 	coding best practices applied
 * 	
 * 	Revision 2.11  2014/03/10 15:00:33  francesco.avanzi
 * 	better code practices applied
 * 	
 * 	Revision 2.10  2013/02/07 15:18:59  enrcar
 * 	EC::
 * 	Updated to handle the macro PDS_OLD_SCHEMA and the new schemas. Connections via connection-pool.
 * 	
 * 	Revision 2.9  2012/01/16 14:13:23  enrcar
 * 	EC:: Modified in order to keep the information of oversized items. This allows the creation of packages in status ERROR
 * 	
 * 	Revision 2.8  2011/03/14 14:46:04  enrcar
 * 	EC:: optimiseForSpace bug found: unable to select any list if the first product exceeded media capacity (this resulted in no package created). Fixed.
 * 	Several methods now produce a BOOL in order to check if a proper value is returned
 * 	
 * 	Revision 2.7  2011/03/03 12:30:08  marpas
 * 	ihash_map changed into map
 * 	
 * 	Revision 2.6  2010/06/09 08:21:14  enrcar
 * 	EC:: An iterator was erased  in a for cycle and the loop condition was based on the iterator itself. Potentially dangerous. Fixed.
 * 	
 * 	Revision 2.5  2009/07/13 13:13:31  enrcar
 * 	EC:: c++rules
 * 	
 * 	Revision 2.4  2009/03/24 14:22:04  enrcar
 * 	EC:: c++rules
 * 	
 * 	Revision 2.3  2008/04/22 15:03:15  enrcar
 * 	Transactions shortened. Ref. CRY-284
 * 	
 * 	Revision 2.2  2008/01/31 09:57:25  enrcar
 * 	EC:: getPriorityListFirstItemCheckReport, optimiseForNoRuleCheckReport, checkIfItemIsAReport, getReportIterator : Methods added.
 * 	fileVersion, fileType, qualifierFile handled to be stored into ItemDesc's new interface (REPORT HANDLING)
 * 	
 * 
 */
   


#ifndef _PackagerOptimiser_H_
#define _PackagerOptimiser_H_


#include <string>

#include <exException.h>
#include <rsResourceSet.h>
#include <Thread.h>
#include <dbDistributionItemQuery.h>
#include <dbPersistent.h>
#include <DateTime.h>
#include <ItemDesc.h>
#include <ItemPartition.h>
#include <map>

// PRQA S 1020, 1031 2
#define PACKAGEROPT_ARGS const std::string&, const int&, const std::string&, const int&, const unsigned long long int&, const unsigned long long int&, \
	const DateTime&, const DateTime&, const PackagerOptimiser::TypeReportFormats&
/* const string &mediaType, const int &mediaFormatId, const string &mediaFormat, const int& packingAlgoId, const unsigned long long int& mintr, const unsigned long long int& maxtr,  
	const DateTime& delay, const DateTime& stabilityTime, const TypeReportFormats& reportFormats */

_ACS_BEGIN_NAMESPACE(acs)

class PackagerOptimiser
{


public:	
  
	/*! class PackagerOptimiserException declaration */
	exDECLARE_EXCEPTION(PackagerOptimiserException,exException) ; // PRQA S 2131, 2502
  
	enum PackageStatus { PKG_ELIGIBLE, PKG_TRANSIENT, PKG_ERROR } ;	
	std::string packageStatusToString(const PackageStatus& s) const // PRQA S 4020
	{	/* Used as actual status on DB queries (!) */
		switch(s) {
			case PKG_ELIGIBLE: { return "ELIGIBLE" ; }
			case PKG_TRANSIENT: { return "TRANSIENT" ; } 
			case PKG_ERROR: { return "ERROR" ; } 
			default: { return "UNDEFINED" ;  }
		} ;  
	}	
    struct ItemIdFilenFilev // PRQA S 2171, 2173, 2175
    {
        ItemIdFilenFilev(dbPersistent::IdType id, const std::string &fn, const std::string &fv ):id_(id), fn_(fn), fv_(fv){}
        ~ItemIdFilenFilev() throw() {}
        ItemIdFilenFilev(const ItemIdFilenFilev &i):id_(i.id_), fn_(i.fn_), fv_(i.fv_){}
        ItemIdFilenFilev& operator=(const ItemIdFilenFilev &) ;
    
    public:
        dbPersistent::IdType id_ ; // PRQA S 2100 3
        std::string fn_ ;
        std::string fv_ ;
    private:
        ItemIdFilenFilev(); //not implemented
    };
    
  
    /* Returned by configuration file -- Key is each distributionformat that allow reports, Value is the list of report filetypes */
    typedef std::map< std::string, std::vector< std::string > > TypeReportFormats ;
  
  	/* Key is the packageId; Values are a pair of the package status and all items associated to the packageid */
    typedef std::map< dbPersistent::IdType, std::pair < PackageStatus, std::vector<ItemIdFilenFilev> > > TypeToDoList ;

    
    struct ItemPartitionHash
    {
         int operator()(ItemPartition &element) const ;
    };

    struct eqItemPartition
    {
	    bool operator()(const ItemPartition &s1, const ItemPartition &s2) const ;
    };

    struct lessItemPartition
    {
	    bool operator()(const ItemPartition &s1, const ItemPartition &s2) const ;
    };


    // Used as Priority list (key is: DueDate):
    typedef std::multimap<long double, ItemDesc> TypePriorityList ; // i.e. TYPE(double(DateTime::JD50)), TYPE(ItemDesc)


    // Used as User list. THIS IS THE MAIN TABLE: 
    typedef std::map<ItemPartition, TypePriorityList*, lessItemPartition> TypeUserList ; 
	

    PackagerOptimiser(PACKAGEROPT_ARGS);
	virtual ~PackagerOptimiser() throw() ;
  
	const std::string &getParameter_mediaType() const throw() ;
	int getParameter_mediaFormatId() const throw() ;
	const std::string &getParameter_mediaFormat() const throw() ;
	int getParameter_packingAlgoId() const throw() ;
	unsigned long long int getParameter_mintr() const throw() ;
	unsigned long long int getParameter_maxtr() const throw() ;
	const DateTime &getParameter_delay() const throw() ;
	const DateTime &getParameter_stabilityTime() const throw() ;
	bool getStopStatus() const throw() ;
    void stop();

    void dumpMainUserList();    // Debug use only
    void eraseMainList() ;
    void makeMainList() ;
    bool processToDoList() ;

	virtual void doOptimise()=0; // pure virtual
  
  	/* Static method */
 	static void getFormatReportMap( TypeReportFormats & ) ;


protected:

    // **** DO NOT CHANGE FOLLOWING ORDER ******
    std::string _mediaType ; // PRQA S 2101 11
    int _mediaFormatId ;
    std::string _mediaFormat ;
	int _packingAlgoId ;
    unsigned long long int _mintr;
    unsigned long long int  _maxtr ;
    DateTime _delay ;
    DateTime _stabilityTime ;
    TypeReportFormats _reportFormats ;
    TypeUserList _listOfUsers ;
    TypeToDoList _listOfTodos ;
    // *****************************************

    void optimiseForNoRule( TypeUserList& userList, TypePriorityList& outList) ;
    void optimiseForNoRuleCheckReport( TypeUserList& userList, TypePriorityList& outList) ;
    void optimiseForSpace(TypeUserList& userList, unsigned long long int mintr, unsigned long long int maxtr, TypePriorityList& outList) ;
    void optimiseForTresholdOnDate(TypeUserList& userList, unsigned long long int maxtr, const DateTime &delay, TypePriorityList& outList) ;
    void performTablesUpdate(TypePriorityList& inList) ; 
   
    ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagerOptimiser); // PRQA S 2101


private:
	PackagerOptimiser() ;	/* unimplemented */
    
    bool checkIfItemIsAReport(const ItemDesc&) ;
    bool cmpDateToTreshold(long double, const DateTime &delay) ;
    bool cmpDateToTreshold(const DateTime &date, const DateTime &delay) ;
    void dumpUserList(const TypeUserList &userList);   // Debug use only
    void dumpPriorityList(const TypePriorityList &priorityList);   // Debug use only
    void eraseEmptyUsers(TypeUserList& userList) ;
    void eraseLists(TypeUserList& userList) ;
	bool eraseUserFromUserList(TypeUserList& userList, ItemPartition& itemPartition) ;
	bool evalLargestDueDate(const TypeUserList& userList, long double& date) ;
	bool evalLargestCreationDatePrList(const TypePriorityList &priorityList, long double& date);
	bool evalSmallestDueDate(TypeUserList& userList, long double& date) ;
	bool evalSumSpace(const TypePriorityList &priorityList, unsigned long long int& sum);
	bool evalSumSpaceAfterTreshold(const TypePriorityList &priorityList, unsigned long long int trs, unsigned long long int& sum);        
	bool evalSumSpaceBeforeTreshold(const TypePriorityList &priorityList, unsigned long long int trs, unsigned long long int& sum);        
	bool evalLargestCreationDateBeforeTreshold(const TypePriorityList &priorityList, unsigned long long int trs, long double& date);        
	bool evalSumSpaceAndLargestCreationDateBelowTreshold(const TypePriorityList&, unsigned long long int trs, unsigned long long int& sum, long double& date) ;              		// OUT: largest date
    void getPriorityList(TypePriorityList& priorityList, TypePriorityList& outList) ;
    void getPriorityListFirstItem(TypePriorityList& priorityList, TypePriorityList& outList) ;
    void getPriorityListFirstItemCheckReport(TypePriorityList& priorityList, TypePriorityList& outList) ;
    void getPriorityListTillSize(TypePriorityList& priorityList, unsigned long long int trsize, TypePriorityList& outList);
   	TypePriorityList::iterator getReportIterator(TypePriorityList&, const ItemDesc&) ;
    void makeList(TypeUserList& userList) ;
	PackageStatus processStatusOfItems(const TypePriorityList& inList) const ;

    rsResourceSet _rs;
	bool _stop ;	
    
	PackagerOptimiser(const PackagerOptimiser &) ;
	PackagerOptimiser &operator=(const PackagerOptimiser &) ;

} ;

inline bool operator==(  const PackagerOptimiser::ItemIdFilenFilev &a, PackagerOptimiser::ItemIdFilenFilev &b ) throw()  // PRQA S 2134
{ 
    return (a.id_ == b.id_) && 
           (a.fv_ == b.fv_) && 
           (a.fn_ == b.fn_) ; 
}

_ACS_END_NAMESPACE

#endif  /* _PackagerOptimiser_H_ */
