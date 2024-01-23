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

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	


*/

#ifndef _PreInventoryObjList_H_
#define _PreInventoryObjList_H_ 

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>
#include <ProductTypeDescriptor.h>
#include <string>
#include <list>


_ACS_BEGIN_NAMESPACE(acs)


class PreInventoryObjList
{
public:

	/*! class PreInventoryObjListException declaration */
	exDECLARE_EXCEPTION(PreInventoryObjListException, exException) ; // PRQA S 2131, 2502

	/*! class EmptyInventoryListException declaration */
	exDECLARE_EXCEPTION(EmptyInventoryListException, exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Default Class constructor 
	 *    
	 */
	PreInventoryObjList(ProductTypeDescriptor*, const std::string&);

	/**
	 * \brief  Class destructor 
	 *    
	 */
	~PreInventoryObjList() throw() ;

/////////////////////////////////////////
// public methods
/////////////////////////////////////////

	/**
	 * \brief  Generate the Nominal and the Time-ordered inventory lists
	 *    
	 */
	void produceInventoryObjList();

	/**
	 * \brief  Return the Nominal inventory list
	 *    
	 */
	const std::list<std::string>& getNominalList() const throw() { return _origL0List; }

	/**
	 * \brief  Set a new Time-ordered inventory list   
	 */
	void setActualList(std::list<std::string>& newList) {_actualInventoryList = newList;};

	/**
	 * \brief  Return the Time-ordered inventory list   
	 */
	const std::list<std::string>& getActualList() const throw() { return _actualInventoryList; }

	/**
	 * \brief Write the time-ordered list on disk. The file name is
	 *		  extracted from configuration file.
	 */
	void writeInvListFile();


private:

	PreInventoryObjList(); 
	PreInventoryObjList(const PreInventoryObjList & ); 
	PreInventoryObjList &operator=(const PreInventoryObjList &);

	//build a nominal list of available Level0 product files scanning current directory
 	void getL0FileList();	

	//order the Level0 file list from earlier sensing time to the latest 
 	void timeOrderL0List();

/////////////////////////////////////////
// private attributes
/////////////////////////////////////////

	//resource containing configuration parameters
	rsResourceSet& 	_rsConf;

	ProductTypeDescriptor* _productHandler;

	std::string		_listFileName;

	//the nominal list
	std::list<std::string> _origL0List;

	//the actual list
	std::list<std::string> _actualInventoryList;

	//the list of configured file extensions
	std::vector<std::string> _configuredExt;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(PreInventoryObjList) ;

};

_ACS_END_NAMESPACE


#endif //_PreInventoryObjList_H_
