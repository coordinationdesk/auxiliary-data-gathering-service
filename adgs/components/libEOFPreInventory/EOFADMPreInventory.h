// PRQA S 1050 EOF
/*
	Copyright 1995-2015, Advanced Computer Systems , Inc.
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
	Revision 1.12  2015/07/09 16:25:43  matteo.airoldi
	Versioning in the ADM context has been fixed
	XSLT is now used to change HDR and EEF contents
	
	Revision 1.11  2015/07/02 14:50:39  francesco.avanzi
	HDR file are now taken in account without extenction
	
	Revision 1.10  2015/06/18 11:18:20  francesco.avanzi
	useless dependency removed (ACSTimeConverter)
	
	Revision 1.9  2015/06/12 13:10:51  francesco.avanzi
	now taking in account _context.TL situation
	minor issues fixed
	
	Revision 1.8  2015/06/12 10:43:05  francesco.avanzi
	 conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.7  2015/06/04 08:15:04  francesco.avanzi
	hasBaseline tag removed in configuration, code rearranged
	
	Revision 1.6  2015/06/03 13:38:54  francesco.avanzi
	qualityFile Removed, Qualified field-filling Reimplemented
	
	Revision 1.5  2015/06/03 12:27:46  francesco.avanzi
	conf/Test/libEOFPreInventory.so.xml
	
	Revision 1.4  2015/05/29 16:10:35  francesco.avanzi
	all noted issues previous
	
	Revision 1.3  2015/05/29 09:29:19  francesco.avanzi
	minor issues fixed
	
	Revision 1.2  2015/05/29 07:59:17  francesco.avanzi
	now ADMAPFPreInventory takes in account Baselines and ParentFiles
	
	Revision 1.1  2015/05/27 15:10:06  francesco.avanzi
	Class Added
	
	Revision 1.1.1.1  2015/03/04 09:04:25  francesco.avanzi
	Import libEOFADMPreInventory
	
	
	
        
*/

#ifndef _EOFADMPreInventory_H_
#define _EOFADMPreInventory_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <EOFPreInventory.h>
#include <PreInventoryBase.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * PreInventory plugin for EOF(Earth Observation File) products
 **/

class EOFADMPreInventory : public EOFPreInventory // PRQA S 2109, 2153
{ 
public:

    exDECLARE_EXCEPTION(exEOFADMPreInventoryException, exException) ; // Base EOFADMPreInventory Exception. // PRQA S 2131, 2502

    EOFADMPreInventory();
    virtual ~EOFADMPreInventory() throw() ;
    virtual std::string getClassId() { return "EOFADMPreInventory"; } //PRQA S 2131
    
    bool hasBaseline() const throw(){ return not baseline_.empty() ; }
    bool hasQuality() const throw(){ return hasQuality_; }
    bool inputFileToBeRemoved() const throw() {return removeInputFile_; }
    bool doVersioning() const throw() {return doVersioning_; }
    
    bool needsPair()const throw(){ return not dsName_.empty() ; }
    
    bool isToBeRejected( dbConnection &conn ) ;
    virtual bool overrideVersionHandling( bool &doVersioningByPlugin ) { doVersioningByPlugin = false; return false; }//PRQA S 2131

    void checkForBlAndPf()  ;

    //classe di Test CPPUnit
#ifdef CLASS_UNDER_TEST
    friend class EOFADMPreInventoryTest;
#endif //CLASS_UNDER_TEST

protected:

    virtual void computeMetadata( const std::string& pathFileName, const std::string& fileType, Metadata& metadata, dbConnection &);

    virtual void extractFurther(rsResourceSet &) ;
    virtual void addFurtherNodes(std::vector<std::string> &) ;

    virtual std::string getUniqueId();

    virtual std::string doPackaging( Metadata& metadata, const std::string &, const std::string &, const std::string &, dbConnection & );

private:
    
    void tLFileNameVersioningQuery(dbConnection &conn)  ;

    void generateXSLT (const std::string& newfilename, const std::string& newfileversion, std::string& xsltText) const;

private:    
    EOFADMPreInventory(const EOFADMPreInventory&); //not implemented
    EOFADMPreInventory& operator=(const EOFADMPreInventory&); //not implemented

private:
	
    ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFADMPreInventory)
    std::string  baseline_ ;
    std::string dsName_ ;
    std::string relFile_ ;
    bool hasQuality_ ;
    std::string fileVersion_ ;	
    std::string fileBaseName_ ;
    std::string fileExt_ ;
    bool removeInputFile_ ;
    bool doVersioning_;
};

_ACS_END_NAMESPACE

#endif //_EOFADMPreInventory_H_

