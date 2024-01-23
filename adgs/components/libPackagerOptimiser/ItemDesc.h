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
 * 	$Prod: A.C.S. PackagerOptimiser ItemDesc class - HEADER FILE $
 * 
 * 	$Id$
 * 
 * 	$Author$
 *     
 * 	$Log$
 * 	Revision 2.5  2014/03/10 15:00:33  francesco.avanzi
 * 	better code practices applied
 * 	
 * 	Revision 2.4  2012/01/16 14:03:49  enrcar
 * 	EC:: The status of the entries in T_distributionItems is now stored in memory. This allows the handling of oversized items.
 * 	
 * 	Revision 2.3  2009/03/24 13:51:56  enrcar
 * 	EC:: c++rules
 * 	
 * 	Revision 2.2  2008/01/30 16:15:35  enrcar
 * 	EC:: Container extended to store also filetype and qualifierfile
 * 	
 * 	Revision 2.1  2008/01/29 14:02:18  enrcar
 * 	EC:: Container extended in order to store/retrieve the FILENAME/FILEVERSION couple for each distributionitem entry
 * 	
 *     
 */


#ifndef _ItemDesc_H_
#define _ItemDesc_H_

#include <exException.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class ItemDesc // PRQA S 2109
{
public:
	enum ItemStatus { ITEM_GOOD, ITEM_OVERSIZE, ITEM_ERROR } ;	
	std::string itemStatusToString(const ItemStatus& s) const  // PRQA S 4020
	{	/* Used only for debugging purposes */
		switch(s) {
			case ITEM_GOOD: { return "ITEM_GOOD" ; }  
			case ITEM_OVERSIZE: { return "ITEM_OVERSIZE" ; }  
			case ITEM_ERROR: { return "ITEM_ERROR" ; }  
			default: { return "UNDEFINED" ; } 
		} ;  
	}	

public:
    ItemDesc(	dbPersistent::IdType itemId, 
                unsigned long long int itemSize, 
                long double itemCreationDate, 
                const std::string& itemFileName, const std::string& itemFileVersion,
                const std::string& itemFileType, const std::string& itemQualifierFile, const ItemStatus itemStatus ) ;
    ~ItemDesc() ;

    void dumpItemDesc() const throw() ;
    long double getItemCreationDate() const throw() ;
    const std::string &getItemFileName() const throw() ;
    const std::string &getItemFileType() const throw() ;
    const std::string &getItemFileVersion() const throw() ;
    dbPersistent::IdType getItemId() const throw() ;
    unsigned long long int getItemSize() const throw() ;
    const std::string &getItemQualifierFile() const throw() ;
	ItemStatus getItemStatus() const throw() ;
	void setItemStatus(const ItemStatus) throw() ;


    ItemDesc(const ItemDesc &) ;
    ItemDesc &operator=(const  ItemDesc&) ;

private: 
	ItemDesc();	/* Unimplemented */

    dbPersistent::IdType _itemId ;
    unsigned long long int _itemSize ;
    long double _itemCreationDate ;
	std::string _itemFileName ;
    std::string _itemFileVersion ;
    std::string _itemFileType ;
    std::string _itemQualifierFile ;
	ItemStatus _itemStatus ; 


    ACS_CLASS_DECLARE_DEBUG_LEVEL(ItemDesc);

} ;

bool operator==(const ItemDesc &, const ItemDesc &) throw() ;


_ACS_END_NAMESPACE

#endif /* _ItemDesc_H_ */
