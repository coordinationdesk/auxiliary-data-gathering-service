// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA PDS Db Library $

*/

#ifndef _FileTypeRecognition_H_
#define _FileTypeRecognition_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbPersistent.h>
#include <map>
#include <vector>
#include <string>


class dbConnection;

_ACS_BEGIN_NESTED_NAMESPACE(acs, db)
	
class FileTypeRecognition { // PRQA S 2109
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

	FileTypeRecognition() ;
	~FileTypeRecognition() throw() ;

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
	FileTypeRecognition(const FileTypeRecognition &) ;
	FileTypeRecognition &operator=(const FileTypeRecognition &) ; 
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

	ACS_CLASS_DECLARE_DEBUG_LEVEL(FileTypeRecognition) ;
} ;

std::ostream &operator << (std::ostream &,FileTypeRecognition::ExtensionId) ; 
acs::exostream &operator << (acs::exostream &,FileTypeRecognition::ExtensionId) ; // PRQA S 2072


_ACS_END_NESTED_NAMESPACE


#endif // _FileTypeRecognitionRecognition_H_
