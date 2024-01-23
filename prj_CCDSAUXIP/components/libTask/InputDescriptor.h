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

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#ifndef _INPUTDESCRIPTOR_H_
#define _INPUTDESCRIPTOR_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <TaskTableVisitable.h>
#include <InputAlternative.h>          

#include <vector>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief This class is the representation of an input descriptor
 *        This class allows implementation of alternate file type for processor inputs.
 *        It stores internally the list of all possible alternatives, which have to be intended as mutually exclusive.
 */

class InputList : public TaskTableVisitable
{
public:
	InputList();
	InputList(const InputList&);
	virtual ~InputList() throw() {} // PRQA S 2131
	
	/**
	 * Operator =
	 * Used by copy ctor
	 */
	InputList& operator = (const InputList&);


	/**
	 *  Implementation for the virtual interface TaskTableVisitable
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);

	virtual bool isContainer() const {return true;} // PRQA S 2131

private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(InputList) ;
};

class InputDescriptor : public TaskTableVisitable
{

public:
	InputDescriptor();
	InputDescriptor(const InputDescriptor& d);
	virtual ~InputDescriptor() throw() {} // PRQA S 2131

	/**
	 *  Methods for setting and getting the Mandatory flag of the input
	 */
	bool isSetMandatory() const;
	bool isMandatory() const;
	void isMandatory(bool val) ;

	/**
	 * Methods for setting and getting the OrderType for the input
	 * The inputs are included in the processing environment only if
	 * the OrderType is the one specified in the order
	 * ALWAYS means all the order types
	 */
	bool isSetOrderType() const;
	std::string getOrderType() const;
	void setOrderType(const std::string& aType);

	/**
	 *  Methods for setting and getting the Id of the input
	 */
	bool isSetId() const;
	std::string getId() const;
	void setId(const std::string& anId) ;
	void resetId() ;

	/**
	 *  Methods for setting and getting the Ref of the input
	 */
	bool isSetRef() const;
	std::string getRef() const;
	void setRef(const std::string& aRef) ;
	void resetRef();

	/**
	 * returns true if the descriptor contains data coming from DB
	 * that has to be downloaded
	 */ 
	bool needsDownload() const;

	/**
	 * Methods for sorting the alternatives composing the descriptor
	 * Different SortTypes can be selected... 
	 */
	enum SortType {DefaultSort, SortByPreferenceOrder, SortByFileType} ;
	virtual void sortAlternatives(InputDescriptor::SortType type = InputDescriptor::DefaultSort, const std::string& orderClob = "");

	// overload standard push_back method, to allow keeping sequence sorted
	void push_back(const InputAlternative& a);

	/**
	 *  Operators
	 *  The alternatives are sorted using this operators and STL algos
	 *  See the implementation for details about the meaning of the operators
	 */
	InputDescriptor& operator = (const InputDescriptor& d);
	bool operator == (const InputDescriptor& d) const; // PRQA S 2070 2
	bool operator < (const InputDescriptor& d) const;



	// This method will complete a partial InputAlternative object
	// (i.e. the empty alternative picked up from a TaskTable's task)
	// using the pathnames and the related time intervals 
	// found in the database and stored il local paths
	void fillAlternative (InputAlternative& theAlternative) const;

	// This method will return the InputAlternative (if any)
	// related to a given fileType
	// It will throw exception if the fileType is not found in the alternatives
	InputAlternative const& getAlternative(const std::string& fileType) const;



	/**
	 *  Dumps the content of the Descriptor
	 *  used mainly for debug purposes
	 */
	void dump() const;

	/**
	 *  Implementation for the virtual interface TaskTableElement
	 */
	virtual TaskTableVisitable* clone() const; // PRQA S 2502
	virtual bool acceptVisit (TaskTableVisitor&);
	virtual bool acceptEnterVisit (TaskTableVisitor&);
	virtual bool acceptExitVisit (TaskTableVisitor&);


private: // attributes
	std::string _OrderType;
	std::string _Id;
	std::string _Ref;
	bool _IsMandatory;

	bool _OrderTypeSet;
	bool _IdSet;
	bool _RefSet;
	bool _IsMandatorySet;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(InputDescriptor) ;
};

_ACS_END_NAMESPACE

#endif //_INPUTDESCRIPTOR_H_

