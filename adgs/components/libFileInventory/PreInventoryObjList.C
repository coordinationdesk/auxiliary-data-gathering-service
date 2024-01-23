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

	$Prod: 

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/07/15 16:31:31  marpas
	compilation warnings fixed
	
	Revision 5.0  2013/06/20 07:48:55  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
*/

#include "PreInventoryObjList.h"

#include <Dir.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>

#include <algorithm>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PreInventoryObjList)

/*
 * Class constructors
 */

PreInventoryObjList::PreInventoryObjList(ProductTypeDescriptor* ptd, const string& fileName) :
	_rsConf(ConfigurationSingleton::instance()->get()),
	_productHandler(ptd),
	_listFileName(fileName),
    _origL0List(),
    _actualInventoryList(),
    _configuredExt()
    
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	//load from Inventory Configuration Space the list of recognized file extensions
	unsigned int extCount = 0;
	string extension="";
	_rsConf.getValue("Inventory.List_Of_Extensions.count.value",extCount);
	for(unsigned int i=0; i<extCount; i++)
	{
		ostringstream num;
		num << i;
		_rsConf.getValue("Inventory.List_Of_Extensions.Extension["+num.str()+"]",extension);
		_configuredExt.push_back(extension);
		extension="";
	}
}


/*
 * Class destructor
 */
PreInventoryObjList::~PreInventoryObjList() throw() {}


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////


//Build the Nominal Inventory List and the Time-Ordered Inventory List
void PreInventoryObjList::produceInventoryObjList()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called...") ;
	
	getL0FileList();
	
	timeOrderL0List();
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executed") ;
}

void PreInventoryObjList::writeInvListFile()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called... _listFileName: " << _listFileName) ;
	
	ofstream invFile;
	invFile.open(_listFileName.c_str());
	if (!invFile)
	{
		ACS_THROW(exFileOpenException("Error opening output file " + _listFileName)) ; // PRQA S 3081
	}
	
	for(list<string>::iterator it=_actualInventoryList.begin(); // PRQA S 4238
		it != _actualInventoryList.end();
		it++
		)
	{
		invFile << (*it) << "\n";
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, (*it) + " written on InventoryListFile") ;
	}
	invFile.close();
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed ") ;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////
 
/*
 * build a list of available product files scanning current directory
 */
void PreInventoryObjList::getL0FileList()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called...") ;
	Dir dir(".");
	for(unsigned int i=0; i < dir.size(); i++)
	{
		string currentFileName = dir[i];
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Processing directory entry: " << currentFileName) ;
		
		if(_productHandler->isValidName(currentFileName))
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Adding file " + currentFileName + " to NOMINAL list") ;
			//check if file extension is recognized
			vector<string>::iterator it = find(_configuredExt.begin(),
											   _configuredExt.end(),
											   File::getExt(currentFileName)
											  );
			//file extension RECOGNIZED : remove it and rename the file
			if( it != _configuredExt.end())
			{
				_origL0List.push_back(File::removeExt(currentFileName));
				int res = rename(currentFileName.c_str(), File::removeExt(currentFileName).c_str() );
				if (res != 0 ) 
				{
    				ACS_THROW( // PRQA S 3081
                        PreInventoryObjListException(
                            "Cannot rename file " + currentFileName + " in " + File::removeExt(currentFileName)
                                                    ));
				}
			}
			else //file extension NOT RECOGNIZED : consider the full filename
			{
				_origL0List.push_back(currentFileName);
			}
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Entry: " << currentFileName << " skipped") ;
		}
	
	}
	
	//check if the _origL0List is empty: in this case throw an exception
	if(_origL0List.size() == 0)
	{
		ACS_THROW( // PRQA S 3081
            EmptyInventoryListException(
                "No Files found to inventory. The list is empty.")) ;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed") ;
	
}

/*
 * order the Level0 file list from earlier sensing time to the latest 
 */
void PreInventoryObjList::timeOrderL0List()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called... _origL0List.size(): " << _origL0List.size()) ;
	
	_origL0List.sort();

	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "after sort _origL0List.size(): " << _origL0List.size()) ;

	vector<string> orderedProductIds = _productHandler->getOrderedProductIds();
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "orderedProductIds size: " << orderedProductIds.size()) ;

	list<string>::iterator origIt;
	
    size_t opsize = orderedProductIds.size() ;
	for (size_t i=0; i < opsize; ++i)
	{
		origIt = _origL0List.begin();
		while ( origIt != _origL0List.end() )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Matching: !" << (*origIt) << "! and !" << orderedProductIds[i] << "!" ) ;

			if ((*origIt).find(orderedProductIds[i]) != string::npos) {
				_actualInventoryList.push_back(*origIt);
            }
			origIt++;
		}
	}
	
	_origL0List.clear() ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed _actualInventoryList.size(): " << _actualInventoryList.size()) ;
}


_ACS_END_NAMESPACE
