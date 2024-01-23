// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 7.2  2018/01/17 10:43:19  marfav
	PDSEV-44 multiple levels of virtual filetypes managed
	
	Revision 7.1  2017/11/14 17:40:29  marpas
	doc improved
	
	Revision 7.0  2017/05/31 17:32:58  nicvac
	OPMAN-260: Need PDS DB schema PDS_12-04.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/27 12:20:55  marpas
	isDAT, isHDR, isSNG methods implemented
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.2  2014/06/16 15:49:59  marpas
	fnameBelong2RFtype method added: returns the REAL ftype in case the given one is virtual - empty string signals no match
	
	Revision 4.1  2013/12/03 19:07:11  marpas
	guessMtdFileNameWExtension method added
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:23  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.18  2013/06/19 17:24:47  marpas
	adopting libException 5.x standards and removing qa warnings
	
	Revision 2.17  2013/04/29 15:42:32  marpas
	dbConnection in interfaces enforced
	
	Revision 2.16  2013/04/10 14:10:30  enrcar
	EC:: Connection key handled. Method getTraits modified in order not to load tables from DB if a filetype is already mapped
	
	Revision 2.15  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.14  2013/02/20 17:09:08  marpas
	inserter operator for enum added
	
	Revision 2.13  2013/02/12 12:38:53  marpas
	interfaces improved over qa rules
	
	Revision 2.12  2012/12/03 10:43:04  lucio.pulvirenti
	ExtensionInfo struct added to return extension information globally from checkFNameExtension method.
	checkFNameExtension IF changed.
	
	Revision 2.11  2012/11/30 14:36:06  lucio.pulvirenti
	 ExtensionId enum added to identify extension configured on db for a given file type.
	checkFNameExtension: ExtensionId output parameter added to identify the t_filetypes column
	corresponding to the input extension.
	
	Revision 2.10  2012/11/30 11:11:20  marpas
	added FTName2FTypeId, FTypeId2FTName methods only in new schema
	added checkFNameExtension always valid
	
	Revision 2.9  2012/11/19 22:51:15  marpas
	compile with PDS_OLD_SCHEMA to use old pds schema.
	dbReceptionRule and related Query class introduced
	
	Revision 2.8  2012/11/16 13:01:13  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.7.4.1  2012/10/24 16:41:44  marpas
	new branch
	
	Revision 2.7  2012/03/26 16:49:55  marpas
	published static method getExpectedIndataSize(const ftype_traits &)
	
	Revision 2.6  2012/03/26 16:22:40  marpas
	getTraits method added
	
	Revision 2.5  2012/03/26 16:14:55  marpas
	rationalizing data size for import: new methods added
	
	Revision 2.4  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2012/01/30 16:16:26  marpas
	thread safety implemented
	
	Revision 2.2  2011/02/10 13:37:29  crivig
	connection passed as optional parameter
	
	Revision 2.1  2009/01/30 14:28:57  marpas
	new schema for virtual file types implemented
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.4  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.3  2003/07/11 20:47:31  paoscu
	exException::clone method implemented,
	order by clause implemented in distribution*query classes and inventoryquery
	test improved and added
	
	Revision 1.2  2002/11/29 10:34:02  paoscu
	Two problems fixed.
	
	Revision 1.1  2002/11/26 16:25:26  paoscu
	Imported.
	
	Revision 1.5  2002/11/13 17:24:08  danalt
	aligned with dbPersistent
		

*/

#ifndef _dbFileTypeRecognition_H_
#define _dbFileTypeRecognition_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbPersistent.h>
#include <map>
#include <vector>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)
	
class dbConnection;
class dbFileTypeRecognition { // PRQA S 2109
public:


    struct ftype_traits { // PRQA S 2173, 2175
    
        ftype_traits() :
            id_filetype(),
            mngsngext(),
            sngext(),
            hdrext(),
            datext(),
            header_separator()
        {}
            
	    ~ftype_traits() throw() {}
    
        ftype_traits(const ftype_traits &r) :
            id_filetype(r.id_filetype),
            mngsngext(r.mngsngext),
            sngext(r.sngext),
            hdrext(r.hdrext),
            datext(r.datext),
            header_separator(r.header_separator)
        {}

        ftype_traits& operator= (const ftype_traits &r)
        {
            if (this != &r) {
                id_filetype = r.id_filetype ;
                mngsngext = r.mngsngext ;
                sngext = r.sngext ;
                hdrext = r.hdrext ;
                datext = r.datext ;
                header_separator = r.header_separator ;
            }
            return *this ;
        }
        
		std::string str() const {
			std::ostringstream o ; 
			o	<< "ftype_traits{" 
				<< "  separate header: " << std::boolalpha << header_separator
				<< "  manage single ext: " << std::boolalpha << mngsngext
				<< "  single ext: " << sngext
				<< "  header ext: " << hdrext
				<< "  data ext: " << datext
				<< "  }" ; 
			return o.str() ;
		}
		
        // PRQA S 2100 L1
        dbPersistent::IdType id_filetype ;
        bool mngsngext ;
        std::string sngext ;
        std::string hdrext ;
        std::string datext ;
        bool header_separator ;
        // PRQA L:L1
    } ;	/* END: struct ftype_traits */

	// KEY: The filetype. VALUE: The traits
	typedef std::map<std::string, ftype_traits> FtypeTraitsType ;	

	// KEY: The filetype. VALUE: A pair containing the regexp and all the associated filetypes
	typedef std::map<std::string, std::pair <std::string, std::vector<std::string> > > FtypesCompType ;

    /*!
		Extension corresponding to a given filetype from t_filetypes columns 
    */
	enum ExtensionId {
		UNDEFINED,
		SNG,		// sngext
		HDR,		// hdrext
		DAT			// datext
	};

	
    /*!
		It collects information on extension corresponding to a given filetype from t_filetypes columns 
    */
	struct ExtensionInfo { // PRQA S 2173, 2175
		
        ExtensionInfo() :
            extId(UNDEFINED),
            mngSngExt(false),
            expectedExtNr(0),
            otherExt()
        {}
            
	    ~ExtensionInfo() throw() {}
    
        ExtensionInfo(const ExtensionInfo &r) :  
            extId(r.extId),
            mngSngExt(r.mngSngExt),
            expectedExtNr(r.expectedExtNr),
            otherExt(r.otherExt)
        {}

        ExtensionInfo& operator= (const ExtensionInfo &r)
        {
            if (this != &r) {
                extId = r.extId ;
                mngSngExt = r.mngSngExt ;
                expectedExtNr = r.expectedExtNr ;
                otherExt = r.otherExt ;
            }
            return *this ;
        }
        // PRQA S 2100 L2
        
        ExtensionId extId;		// extension identifier (e.g. SNG, HDR, etc.)
        bool mngSngExt;			// true if sngext must be saved when archiving the file, false otherwise
        int expectedExtNr;		// number of expected extension for that file type (e.g. if sngext is 1, otherwise is 2)
        std::string otherExt ;	// second extension string if extension is either hdrext (datext) or datext (hdrext). If sngext not valorized.
		// PRQA L:L2
	} ;
	
	/*! class NotFound declaration */
	exDECLARE_EXCEPTION(NotFound,exException) ; // PRQA S 2131, 2502

	dbFileTypeRecognition() ;
	~dbFileTypeRecognition() throw() ;

	std::string fname2RealFType(std::string const &, dbConnection &) const ; 
	bool fnameBelong2Ftype(std::string const &ftype, std::string const & fname, dbConnection &) const ; 
    // return empty string if not belongs to the ftype, else return the REAL ftype (if the given one is virtual)
	std::string fnameBelong2RFtype(std::string const &ftype, std::string const & fname, dbConnection &) const ; 
	std::string getRegExp(const std::string &ftype,dbConnection &) const ;
	std::string getRealRegExp(const std::string &ftype,dbConnection &) const ;
    
    /*- returns true if it is a real filetype - regular expression not empty 
    */
    bool isRealFType(const std::string &ftype, dbConnection &c) const { return not getRealRegExp(ftype,c).empty() ; }
    
    // 0 if ftype not present, else 1 if a single file expected 2 if a couple.
    ftype_traits getTraits(const std::string &ftype,dbConnection &) const ;
    /*- check whether the filetype desc is a descendant of a virtual filetype anc
     * returns true also if desc and anc are the same file type
     *
     */
    bool relationship(const std::string &desc,  const std::string &anc, dbConnection &) const ;

    static dbPersistent::IdType FTName2FTypeId(const std::string &,dbConnection &) ;
    static std::string FTypeId2FTName(dbPersistent::IdType,dbConnection &) ;
   

    /*!
        \returns true if the given filename has one of the correct extensions for the ftype
		extInfo returns extension info information from t_filetypes table.
    */
    static bool checkFNameExtension(const std::string &ftype, 
                                   const std::string &fname,
								   ExtensionInfo & extInfo,
                                   dbConnection &) ;
    
    static bool isSNG(const std::string &ftype, const std::string &fname,dbConnection &, bool ignoreExtCase = true) ;
    static bool isHDR(const std::string &ftype, const std::string &fname,dbConnection &, bool ignoreExtCase = true) ;
    static bool isDAT(const std::string &ftype, const std::string &fname,dbConnection &, bool ignoreExtCase = true) ;

    /*!
        \returns the actual metadata file name given a filename, a filetype and ftype_traits
    */
    std::string guessMtdFileNameWExtension(const std::string &ftype, 
                                           const std::string &fname,
                                           dbConnection &) const ;

	// Returns the list of the real filtypes composing the given one
	std::vector<std::string> getRealFTypesList (const std::string& ftype, dbConnection& conn) const;

private:
	// Returns the list of the filtypes composing the given one, including virtual if requested
	// The passed vector will be erased
	void internalGetFTypesList (const std::string& ftype, dbConnection& conn, std::vector<std::string>& theList, bool alsoVirtual) const;

private:
	//declared but not implemented to prevent their use
	dbFileTypeRecognition(const dbFileTypeRecognition &) ;
	dbFileTypeRecognition &operator=(const dbFileTypeRecognition &) ; 
	static void loadMap(dbConnection &) ;
	static bool loadMapIfNeeded(const std::string&, dbConnection &) ;
	//std::string &regexp(std::string &, const std::string &ftype, const std::string& connKey) const ;

private:
    static ThreadSafe _mutex ;
	
	// Key: connection key. Value: the time of the last update
	static std::map<std::string, time_t> _lastUpdateMap ;
	
	// Key: connection key. Value: the map FtypesCompType
	static std::map<std::string, FtypesCompType> _ftypesMap ;

	// Key: connection key. Value: the map FtypeTraitsType
	static std::map<std::string, FtypeTraitsType> _ftypeTraitsMap ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbFileTypeRecognition) ;
} ;

std::ostream &operator << (std::ostream &,dbFileTypeRecognition::ExtensionId) ; 
acs::exostream &operator << (acs::exostream &,dbFileTypeRecognition::ExtensionId) ; // PRQA S 2072


_ACS_END_NAMESPACE


#endif // _dbFileTypeRecognitionRecognition_H_
