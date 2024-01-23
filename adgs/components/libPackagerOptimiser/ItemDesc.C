// PRQA S 1050 EOF
// 
// 	Copyright 1995-2014, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
// 	$Prod: A.C.S. PackagerOptimiser ItemDesc class $
// 
// 	$Id$
// 
// 	$Author$
// 
// 	$Log$
// 	Revision 2.6  2014/03/10 15:00:33  francesco.avanzi
// 	better code practices applied
// 	
// 	Revision 2.5  2013/02/07 15:35:24  enrcar
// 	EC:: Compilation warning removed
// 	
// 	Revision 2.4  2012/01/16 14:03:49  enrcar
// 	EC:: The status of the entries in T_distributionItems is now stored in memory. This allows the handling of oversized items.
// 	
// 	Revision 2.3  2009/03/24 13:51:56  enrcar
// 	EC:: c++rules
// 	
// 	Revision 2.2  2008/01/30 16:15:35  enrcar
// 	EC:: Container extended to store also filetype and qualifierfile
// 	
// 	Revision 2.1  2008/01/29 14:02:18  enrcar
// 	EC:: Container extended in order to store/retrieve the FILENAME/FILEVERSION couple for each distributionitem entry
// 	
// 


#include <ItemDesc.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ItemDesc);


//------------------------------------------------------------------------
//                                 ItemDesc
//------------------------------------------------------------------------
ItemDesc::ItemDesc(dbPersistent::IdType itemId, unsigned long long int itemSize, 
                    long double itemCreationDate,
                    const string& itemFileName, const string& itemFileVersion, 
                    const string& itemFileType, const string& itemQualifierFile, const ItemStatus itemStatus ) :
            _itemId(itemId),
            _itemSize(itemSize),
            _itemCreationDate(itemCreationDate),
            _itemFileName(itemFileName),
            _itemFileVersion(itemFileVersion),
            _itemFileType(itemFileType),
            _itemQualifierFile(itemQualifierFile),
			_itemStatus(itemStatus)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
   
   
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ;
        dumpItemDesc() ;
    ACS_CLASS_END_DEBUG;
}


//------------------------------------------------------------------------
//                                 ~ItemDesc
//------------------------------------------------------------------------
ItemDesc::~ItemDesc()
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB); // PRQA S 4631
}


//------------------------------------------------------------------------
//                                 getItemId  
//------------------------------------------------------------------------
dbPersistent::IdType ItemDesc::getItemId() const throw() 
{ 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemId " << _itemId );
    
    return _itemId ;
}


//------------------------------------------------------------------------
//                                getItemSize 
//------------------------------------------------------------------------
unsigned long long int ItemDesc::getItemSize() const throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemSize " << _itemSize );

    return _itemSize ;
}


//------------------------------------------------------------------------
//                              getItemCreationDate 
//------------------------------------------------------------------------
long double ItemDesc::getItemCreationDate() const throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemCreationDate " << _itemCreationDate);

    return _itemCreationDate ;
}


//------------------------------------------------------------------------
//                              getItemFileName 
//------------------------------------------------------------------------
const string &ItemDesc::getItemFileName() const throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemFileName: " << _itemFileName );

    return _itemFileName ;
}


//------------------------------------------------------------------------
//                              getItemFileVersion 
//------------------------------------------------------------------------
const string &ItemDesc::getItemFileVersion() const throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemFileVersion: " << _itemFileVersion );

    return _itemFileVersion ;
}


//------------------------------------------------------------------------
//                              getItemFileType 
//------------------------------------------------------------------------
const string &ItemDesc::getItemFileType() const throw()
{
   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemFileType: " << _itemFileType);
    return _itemFileType ;
}


//------------------------------------------------------------------------
//                              getItemQualifierFile 
//------------------------------------------------------------------------
const string &ItemDesc::getItemQualifierFile() const throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemQualifierFile: " << _itemQualifierFile);

    return _itemQualifierFile ;
}


//------------------------------------------------------------------------
//                              getItemStatus 
//------------------------------------------------------------------------
ItemDesc::ItemStatus ItemDesc::getItemStatus() const throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemStatus: " << int(_itemStatus) ); // PRQA S 3081

    return _itemStatus ;
}


//------------------------------------------------------------------------
//                              setItemStatus 
//------------------------------------------------------------------------
void ItemDesc::setItemStatus(const ItemDesc::ItemStatus s) throw()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "itemStatus: " << int(s)); // PRQA S 3081

	_itemStatus = s ;
}


//------------------------------------------------------------------------
//                               dumpItemDesc 
//------------------------------------------------------------------------
void ItemDesc::dumpItemDesc() const throw()
{
	/* Dump the private members */

	ACS_LOG_INFO(exDebugSignature ) ;
    ACS_LOG_INFO("\titemId: " << _itemId ) ;
    ACS_LOG_INFO("\titemSize: " << _itemSize ) ;
    ACS_LOG_INFO("\titemCreationDate: " << _itemCreationDate ) ;
    ACS_LOG_INFO("\titemFileName: " << _itemFileName ) ;
    ACS_LOG_INFO("\titemFileVersion: " << _itemFileVersion ) ;
    ACS_LOG_INFO("\titemFileType: " << _itemFileType ) ;
    ACS_LOG_INFO("\titemQualifierFile: " << _itemQualifierFile ) ;
	ACS_LOG_INFO("\titemStatus: " << int(_itemStatus) <<  " (" << itemStatusToString(_itemStatus) << ')' ) ; // PRQA S 3081
}


//------------------------------------------------------------------------
//                                ::Assignment
//------------------------------------------------------------------------
ItemDesc::ItemDesc(const ItemDesc &e) :
	_itemId (e._itemId),
	_itemSize (e._itemSize),
	_itemCreationDate (e._itemCreationDate),
	_itemFileName (e._itemFileName),
	_itemFileVersion (e._itemFileVersion),
	_itemFileType (e._itemFileType),
	_itemQualifierFile (e._itemQualifierFile),
	_itemStatus (e._itemStatus) 
{ }


//------------------------------------------------------------------------
//                                 ::Copy
//------------------------------------------------------------------------
ItemDesc& ItemDesc::operator=(const ItemDesc &e)
{	
	/* Copy c'tor */

    if (this != &e)
    {
        _itemId = e._itemId ;
        _itemSize = e._itemSize ;
        _itemCreationDate = e._itemCreationDate ;
        _itemFileName = e._itemFileName ;
        _itemFileVersion = e._itemFileVersion ;
        _itemFileType = e._itemFileType ;
        _itemQualifierFile = e._itemQualifierFile ;
        _itemStatus = e._itemStatus ;
    }

    return *this;
}


//------------------------------------------------------------------------
//                                 ::Compare
//------------------------------------------------------------------------
bool operator==(const ItemDesc &a, const ItemDesc &b) throw() 
{
   
    return ( 	(a.getItemId() == b.getItemId()) && (a.getItemSize() == b.getItemSize()) && 
    			(a.getItemCreationDate() == b.getItemCreationDate()) && // PRQA S 3270
                (a.getItemFileName() == b.getItemFileName()) && ( a.getItemFileVersion() == b.getItemFileVersion() ) &&
                (a.getItemFileType() == b.getItemFileType()) && ( a.getItemQualifierFile() == b.getItemQualifierFile() ) &&
				(a.getItemStatus() == b.getItemStatus() ) ) ;
}

_ACS_END_NAMESPACE
