// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	This is the Inventory Object "Tutto Er Cucuzzaro" that will evolve to include all subsequent modifications.

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.8  2016/07/04 15:15:44  damdec
	CSGACQ-82: new getFields interface adopted.
	
	Revision 5.7  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.6  2016/04/15 10:13:44  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
	Revision 5.5  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.4  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.3  2015/03/13 17:51:19  marfav
	S2PDGS-1137 Added support to bulk extra parameters and quality value
	
	Revision 5.2  2015/02/26 16:50:00  marfav
	APF-256 baseline associated to TEC objects with external relation table
	
	Revision 5.1  2015/02/20 16:23:53  marfav
	Fixing export object in rs virtual operation signature
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:58  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.2  2013/09/26 19:32:52  marpas
	qa rules
	coding best practices
	compilation warnings removed
	using libException 5.x interface
	
	Revision 3.1  2013/09/24 17:36:10  marpas
	introducing libException 5.x interface
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.12  2013/03/29 19:51:29  marpas
	removing html related functions
	
	Revision 2.11  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.9  2012/06/15 09:54:30  marpas
	removing warning related to virtual method hiding
	
	Revision 2.8  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.7  2011/03/30 16:59:32  marpas
	tree management added
	
	Revision 2.6  2011/03/25 14:11:50  marpas
	tree management in progress
	
	Revision 2.5  2011/03/02 18:47:14  marpas
	serial 8 support
	
	Revision 2.4  2009/07/14 13:14:22  marfav
	Added workaround for --replacefile sites management
	to be fixed
	
	Revision 2.3  2009/06/12 09:27:50  marpas
	removeAllSitesLinks method added
	
	Revision 2.2  2008/10/20 14:05:46  marpas
	work in progress
	
	Revision 2.1  2007/03/16 14:00:19  marpas
	work in progress
	
		

*/


#ifndef _dbInventoryObjectTEC_H_
#define _dbInventoryObjectTEC_H_

#include <acs_c++config.hpp>
#include <dbInventoryObjectWithGapsComments.h>
#include <dbInventoryAddParamsQuery.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class dbCryosatFactory;
class dbInventoryMDSQuery;
class dbInventoryMDS;
class dbInventorySiteQuery;
class dbInventorySite;
class dbInventoryAddParams;


//
//
//
// class dbInventoryObjectTEC
//
//
//
class dbInventoryObjectTEC : public dbInventoryObjectWithGapsComments // PRQA S 2109, 2153
{
public:
	friend class dbCryosatFactory; // PRQA S 2107
	//
	// Methods
	//


	virtual ~dbInventoryObjectTEC() throw() ;
	dbInventoryObjectTEC &operator=(const dbInventoryObjectTEC &) ; // operator =

	// inherited from dbPersistent
	virtual dbPersistent *clone() const ; // PRQA S 2502
	virtual std::string className() const ;
	static  std::string staticClassName() ;
	virtual void reset() ; // initialize the class
	virtual IdType save(dbConnection &)  ; // save the data in database
    virtual IdType update(dbConnection &)  ; // update an existing instance in database
	virtual bool load(dbConnection &, IdType id=0)  ; // load the class from the database. Return true if obect exist
	virtual IdType remove(dbConnection &)  ; // remove the class from the database // PRQA S 2502

	virtual void exportObj(rsResourceSet &s)  const; // export the class to stream
	virtual void importObj(rsResourceSet &s)  ; // import the class from a stream
	virtual void exportXML(XMLOstream &xst) ; // export the class to XML stream

	int loadRow(dbConnection &, const dbRow &, int offset=0) ;

	static std::string getFields() ;
	static std::string getFields(dbConnection&) ;
	static std::deque<std::string> getFieldArray(dbConnection & conn) ;


	// Methods used to access the comments
	unsigned long MDSsNum(dbConnection &)	;
	dbInventoryMDS * MDS(unsigned long ind, dbConnection &) ; 
	void addMDS(int mdsid, int count, dbConnection &) ; 

	// Methods used to access the comments
	unsigned long	SitesNum(dbConnection &)	;
	dbInventorySite * Site(unsigned long ind, dbConnection &) ; 
	void addSite(int sid, dbConnection &) ; 
	void removeAllSitesLink(dbConnection &) ;

	// Methods used to access the additional params
	void setAddParams(const std::map<std::string, std::string>&, dbConnection &) ; 
	IdType getAdditionalParamsTimelinessId(const std::string& sessionKey, dbConnection &conn) const ;


	// output to stream
	virtual void writeTo(std::ostream &) const ;
	virtual void writeTo(exostream &) const ;

    // baseline management
    void addBaselineId (IdType bid);
    void removeBaselineLinks (dbConnection &);
    const std::vector<IdType>& getBaselineIds();
    void resetBaselines();
    
    // additional parameters clob table
    void setExtraInfo (const std::string&);
    const std::string& getExtraInfo (bool* = 0) const;
    void unsetExtraInfo ();


protected:
	dbInventoryObjectTEC() ;
	dbInventoryObjectTEC(const dbInventoryObjectTEC &) ;


private:

	unsigned long loadMDSs(dbConnection &) ; 
	void saveMDSs(dbConnection &) ;
	void deleteMDSs2Save() throw() ;

	unsigned long loadSites(dbConnection &) ; 
	void saveSites(dbConnection &) ;
	void deleteSites2Save() throw() ;

	bool hasAddParams (dbConnection& conn) ;
	unsigned long loadAddParams(dbConnection &) ; 
	void updateAddParams(dbConnection &) ;
	void saveAddParams(dbConnection &) ;
	void deleteAddParams2Save() throw() ;
    void clearAddParams (dbConnection &);

    void updateBaselines(dbConnection &);
    void saveBaselines(dbConnection &);
    void loadBaselines(dbConnection &);

    bool hasExtraInfo (dbConnection &);
    void updateExtraInfo (dbConnection &);
    void saveExtraInfo (dbConnection &);
    void loadExtraInfo (dbConnection &);
    void clearExtraInfo (dbConnection &);

private:
	bool _MDSsNumSet ; 
	int _MDSsNum ; 
	dbInventoryMDSQuery *_inventoryMDSs ; 
	bool _MDSsLoaded ; 
	
	bool _SitesNumSet ; 
	int _SitesNum ; 
	dbInventorySiteQuery *_inventorySites ; 
	bool _SitesLoaded ; 
	dbInventoryAddParamsQuery *_inventoryAddParams ; 

	std::vector<dbInventoryMDS *> _MDSs2Save ;
	std::vector<dbInventorySite *> _Sites2Save ;
	dbInventoryAddParams * _addParams2Save ;
	bool _addParamsSet ;
			
    std::vector<IdType> _baselineIds;
    bool _baselineModified;

    std::string _extraInfo;
    bool _extraInfoSet;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObjectTEC);
} ;


_ACS_END_NAMESPACE


#endif // _dbInventoryObjectTEC_H_
