// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.14  2017/08/31 09:50:55  marfav
	The key used to get the object is now stored
	
	Revision 1.13  2017/01/16 13:25:32  marfav
	Enforcing cpp coding rules
	
	Revision 1.12  2016/05/10 14:36:17  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.11  2016/03/26 21:40:46  marpas
	using new db I/F
	
	Revision 1.10  2015/11/20 11:57:18  marpas
	coding best practices application in progress
	
	Revision 1.9  2015/06/16 13:09:25  damdec
	Compilation warnings solved.
	
	Revision 1.8  2013/11/19 18:04:11  marpas
	TaskTable has private copy ctor and operator=
	
	Revision 1.7  2013/02/21 13:43:56  marfav
	Compiler warnings removed
	
	Revision 1.6  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.5  2010/03/04 15:13:21  marfav
	Using acceptVisitor when visiting input alternatives
	
	Revision 1.4  2008/11/07 15:40:53  marfav
	Added _deltaT in parameters
	
	Revision 1.3  2008/08/04 10:16:22  marfav
	Library split in definition and implementation
	in order to support plugins management
	
	Revision 1.2  2008/04/30 15:57:51  marfav
	Added temporary symbol table to manage alternatives
	
	Revision 1.1  2008/04/29 12:45:40  marfav
	Added support for PreconditionCheckFactoryChain
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#include <ThinLayer_PCC.h>


#include <TaskTable.h>
#include <StringKey.h>

#include <AlternativeCheckerFactoryChain.h>
#include <Alternative_PCC.h>

#include <dbConnection.h>

#include <Functional.h>

#include <functional>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ThinLayer_PCC)

ThinLayer_PCC::ThinLayer_PCC (StringKey factoryKey) :
	PCC_Base(&_myParameters),
	_myParameters(),
	_myTaskTable(0)
{
	// CTOR
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Store the factory key in the internal parameters
	_myParameters._factoryKey = factoryKey;

	// This implementation of the PCC will halt on any error 
	// (Default implementation)
	stopOnFailure(true);
}

ThinLayer_PCC::~ThinLayer_PCC () throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) // PRQA S 4631 EndOfDTOR


	if (_myTaskTable)
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_myTaskTable != 0. Deallocating\n") ;					

	    delete _myTaskTable;
	}
    
    // PRQA L:EndOfDTOR
}

bool 
ThinLayer_PCC::doPreconditionCheck (TaskTable& tt, tlOrder anOrder, double deltaT, dbConnection* theConnection)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	// Do parameters setup
	_myParameters.theOrder = anOrder;
	_myParameters.deltaT = deltaT;
	_myParameters.dbKey = theConnection ? theConnection->getKey() : "" ;

	// Call the check method
	return checkTaskTable(tt);
}

void ThinLayer_PCC::reset()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)


	if (_myParameters.theSymbolTable.size())
	{

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_myParameters.theSymbolTable.size() = " << _myParameters.theSymbolTable.size() << ". Cleaning-up\n") ;					

	    _myParameters.theSymbolTable.clear();
	}

	if (_myParameters.tempSymbolTable.size())
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_myParameters.tempSymbolTable.size() = " << _myParameters.tempSymbolTable.size() << ". Cleaning-up\n") ;					

	    _myParameters.tempSymbolTable.clear();
	}


	if (_myTaskTable)
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_myTaskTable != NULL. Deallocating\n") ;					

	    delete _myTaskTable;
	}


        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "calling PCC_Base::reset\n") ;					


	PCC_Base::reset();
}


bool
ThinLayer_PCC::checkTaskTable(TaskTable& theTaskTable)
{
	// some housekeeping in case of multiple calls
	reset();


	// now, make internal copy of the input task table
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "allocating _myTaskTable\n") ;					
	_myTaskTable = theTaskTable.clone();


	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "let the dog (the visitor) out!\n") ;					

	_myTaskTable->acceptVisitor(*this);

	return isCheckPassed();
}


void
ThinLayer_PCC::getVisitedTaskTable(TaskTable& aTaskTable)
{
	if (!_myTaskTable) {
	    ACS_THROW (ThinLayer_PCC_Exception("getVisitedTaskTable called before any call to visit(TaskTable)"));
    }

	_myTaskTable->fillBase(aTaskTable) ;
}

void
ThinLayer_PCC::getSymbolTable(SymTable& aSymbolTable)
{

	aSymbolTable = _myParameters.theSymbolTable;
}

bool ThinLayer_PCC::exitVisit (TaskTable& tt)
{
	    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		    excerr << "ThinLayer_PCC::exitVisit(TaskTable&): called\n";
		    excerr << "\ttt.getVisitResult      = " << (tt.getVisitResult()?"true":"false") << "\n";
		    excerr << "\ttt.getNavigationResult = " << (tt.getNavigationResult()?"true":"false") << "\n";
		    excerr << "\tstopOnFailure          = " << (stopOnFailure()?"true":"false") << "\n";
	    ACS_CLASS_END_DEBUG
	   				


	if ((!tt.getVisitResult() || !tt.getNavigationResult()) && stopOnFailure())
	{
	    	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "stopping navigation (returning false)\n") ;					

		    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			    excerr << "ThinLayer_PCC::exitVisit(TaskTable&): dumping symbol table\n";
			    SymTable::iterator it;
			    size_t j;
			    for (j=1, it=_myParameters.theSymbolTable.begin();
				 it!=_myParameters.theSymbolTable.end(); ++j,++it)
			    {
				excerr << "\tAlternative #" << j << " " <<
					  "[" << (it->first).getFileType() << "," <<
					  (it->first).getFileClass() << "," <<
					  InputOrigin2String((it->first).getOrigin()) << "," <<
					  (it->first).getRetrievalMode() << "," <<
					  (it->first).getT0() << "," <<
					  (it->first).getT1() << "," <<
					  FileNameType2String((it->first).getFileNameType()) << "]" <<
					  ": " << (it->second.checkPassed?"FOUND!":"not found") << "\n";
			    }
	 	    ACS_CLASS_END_DEBUG


		    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			    vector<InputDescriptor> v;
			    tt.getInputList(v);
			    excerr << "ThinLayer_PCC::exitVisit(TaskTable&): retrieved " << v.size() << " inputs\n";
			    vector<InputDescriptor>::iterator it;
			    for (it=v.begin(); it != v.end(); ++it)
			    {
				excerr << "ThinLayer_PCC::exitVisit(TaskTable&): the size of input " << it - v.begin() << " is " << it->size() << "\n";
			    }
		    ACS_CLASS_END_DEBUG



	    return false;
	}
	else
	{
	    // This checker will visit explicitely here the AtExit Sequences (if any)
	    // Pools and Sequences will be scanned by the algorithm underlying the pattern
	    // 
	    // Any other element will not be visited
	    //


	    // AtExitSequences section (not mandatory)
	    vector<AtExitSequence>& s(tt);

	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "navigating AtExitSequences [" << s.size() << " total]\n") ;					

	    for (size_t i=0; i<s.size(); ++i)
	    {
	    	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "navigating AtExitSequence #" << i+1 << "\n") ;					

		    s[i].acceptVisitor (*this);
	    }



	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		    excerr << "ThinLayer_PCC::exitVisit(TaskTable&): dumping symbol table\n";
		    SymTable::iterator it;
		    size_t j;
		    for (j=1, it=_myParameters.theSymbolTable.begin();
			 it!=_myParameters.theSymbolTable.end(); ++j,++it)
		    {
			excerr << "\tAlternative #" << j << " " <<
				  "[" << (it->first).getFileType() << "," <<
				  (it->first).getFileClass() << "," <<
				  InputOrigin2String((it->first).getOrigin()) << "," <<
				  (it->first).getRetrievalMode() << "," <<
				  (it->first).getT0() << "," <<
				  (it->first).getT1() << "," <<
				  FileNameType2String((it->first).getFileNameType()) << "]" <<
				  ": " << (it->second.checkPassed?"FOUND!":"not found") << "\n";
		    }
	    ACS_CLASS_END_DEBUG


	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "continuing navigation (returning true)\n") ;					


	    return true;
	}
}



bool ThinLayer_PCC::exitVisit (Task& t)
{
	    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\tt.getVisitResult      = " << (t.getVisitResult()?"true":"false") << "\n"<<
		    "\tt.getNavigationResult = " << (t.getNavigationResult()?"true":"false") << "\n" <<
		    "\tstopOnFailure	      = " << (stopOnFailure()?"true":"false") << "\n") ;				      

	    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		    {
			vector<InputDescriptor> v;
			t.getInputList(v);
			excerr << "ThinLayer_PCC::exitVisit(Task&): retrieved " << v.size() << " inputs\n";
			vector<InputDescriptor>::iterator it;
			for (it=v.begin(); it != v.end(); ++it)
			{
			    excerr << "ThinLayer_PCC::exitVisit(Task&): the size of input " << it - v.begin() << " is " << it->size() << "\n";
			}
		    }
	    ACS_CLASS_END_DEBUG


	// we need to eliminate empty (= 0 alternative) inputs
	// they are result of failed query in the database, but could be nominal
	// if the input is optional!


	vector<InputDescriptor> v;
	t.getList(v, mem_fun_ref(&InputDescriptor::size));

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "remaining " << v.size() <<
				  " inputs out of " << t.size() << " after cleaning\n") ;					

	t.clearInputs();
	t.addInputs(v);


	if ((!t.getVisitResult() || !t.getNavigationResult()) && stopOnFailure())
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "stopping navigation (returning false)\n") ;					

	    return false;
	}
	else
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "continuing navigation (returning true)\n") ;					

	    return true;
	}
}

bool ThinLayer_PCC::enterVisit (InputDescriptor& theInputDescriptor)
{
	// Cleanup the temporary symbol table
	_myParameters.tempSymbolTable.clear();

	// if a previous input check failed, decide if stop or not navigation on this input...
		    
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "check is " <<
				      (isCheckPassed()?"passed":"NOT passed") << "; _status is [" << getStatus() << "]\n") ;					

	if (!isCheckPassed() && stopOnFailure())
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning false\n") ;					

	    return false;
	}
	else
	{

	    if ( (theInputDescriptor.getOrderType() == _myParameters.theOrder.getOrderType()) || (theInputDescriptor.getOrderType() == TaskTableElement::_alwaysOrderType) )
	    {	    
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning true\n") ;					
	        return true;
	    }
	    else
	    {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning false - wrong order type\n") ;					
	        return false;
	    }
	}
}


bool ThinLayer_PCC::visit (InputDescriptor& theInputDescriptor)
{
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "visiting InputDescriptor&\n") ;					


	{

	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "input size is [" << theInputDescriptor.size() << "]\n") ;					


		// first thing: sort alternatives ascendig by preference order...
		// don't want to waste time checking useless ones

  	        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sorting alternatives\n") ;					

		theInputDescriptor.sortAlternatives(InputDescriptor::SortByPreferenceOrder, _myParameters.theOrder.getParametersCLOB());
	}

	return true;
}


// STL Helper class
// This class searches a valid InputAlternative after navigation
class isAlternativeValid : public acs::func::unary_function<InputAlternative, bool>
{
public:
	explicit isAlternativeValid() {}
	bool operator() (const InputAlternative& a) const
	{
	    return a.getFileType() != "TO_BE_DELETED" &&
		   a.getFileClass() != "TO_BE_DELETED";
	}
};

bool ThinLayer_PCC::exitVisit (InputDescriptor& d)
{
	    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		    excerr << "ThinLayer_PCC::exitVisit(InputDescriptor&): called\n";
		    excerr << "\td.getVisitResult      = " << (d.getVisitResult()?"true":"false") << "\n";
		    excerr << "\td.getNavigationResult = " << (d.getNavigationResult()?"true":"false") << "\n";
		    excerr << "\tstopOnFailure          = " << (stopOnFailure()?"true":"false") << "\n";
	    ACS_CLASS_END_DEBUG

	updateExpirationDate();
	// first of all update the symbol table and cleanup the temporary one
	// if the input has not been navigated due to its order type
	// the temporary symbol table will be empty
	promoteTempSymbolTable(d.isMandatory(), !d.getNavigationResult());

	// if input check failed, decide if stop or not navigation on remaining inputs...
		    
	// alternative navigation failed (all of them returned true to let navigation continue)
	// or the alternative have been skipped due to its order type
	if (d.getNavigationResult()) 
	{
	    // No alternative!
	    // let's delete all those useless alternatives
	    // then it will be up to the parent to dispose of the empty input
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "all check failed... cleaning descriptor\n") ;					

	    d.clear();


	    // take into account previous status if and only if the element has been checked
	    if (d.getEnterVisitResult())
	    {
            bool cp = isCheckPassed() && !d.isMandatory();
            setCheckPassed (cp);

	        // _checkPassed &= !d.isMandatory();
	    }

	}
	else
	{
	    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "alternative navigation successful... not touching global _status\n") ;					

		// but we still need some cleanup on the alternatives....
		// if we're here, then at list one of them has to be ok
		// all the ones failed before the good one have been marked setting
		// FileType and FileClass to TO_BE_DELETED
		// erase them! (and all useless stuff after the good alternative, of course!)
		vector<InputAlternative> v;
		d.getList(v, isAlternativeValid());


		d.clear();
		d.push_back(v[0]); // only the first is good
	}



	    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "before returning, d.size = " << d.size() << "\n") ;					



	//if (d.getNavigationResult() && stopOnFailure()) // remember: if navigation failed, getNavigationResult returns false!
	if (!isCheckPassed() && stopOnFailure()) 
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning false\n") ;					

	    return false;
	}
	else
	{
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning true\n") ;					

	    return true;
	}
}
bool ThinLayer_PCC::visit (InputAlternative& a)
{
	// visiting the alternative #i
	string origin=InputOrigin2String(a.getOrigin());
		    
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "alternative [" << a.getFileType() << "] is [" << origin << "]\n") ;					

	// make the key
	StringKey theKey = StringKeyUtils::makeKey( 
		_myParameters._factoryKey.satellite(),
		_myParameters._factoryKey.mission(),
		_myParameters._factoryKey.processor(),
		_myParameters._factoryKey.version(),
		origin);
	
	tlOrder& order = getParameters()->theOrder;
	bool isSet = false;
	double actTime = a.getAltActivationTime(&isSet);
	DateTime now;
	if (isSet && actTime > now.diffDay(order.getCreationDate())*86400)
	{
	    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AlternativeActivationTime is " << actTime);
		// alternative not activated
		SymTableParams params;
		params.isActive = false;
		params.activationDate = order.getCreationDate()+(actTime/86400.);
		params.checkPassed = true;
		addAlternativeToTempSymTable(a, params, false);	
	}
	else
	{
	    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AlternativeActivationTime is " << actTime << " Time passed from order CreationDate "<< now.diffDay(order.getCreationDate()) << " - CreationDate: "<< order.getCreationDate().getSqlString());
		try
		{
			ACS_SMARTPTR<Alternative_PCC> theAlternativeChecker(AlternativeCheckerFactoryChain::instance()->newObject (theKey, getParameters()));
			if (theAlternativeChecker.get()) 
			{
				// call acceptVisitor instead of visit on order to allow enterVisit/visit/exitVisit pattern
				bool _myCheckPassed = a.acceptVisitor (*theAlternativeChecker);

				// if check successful, no need to check remaining alternatives
				// (provided alternatives are sorted in ascending order by preference!)
	    			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "alternative check result is " <<
						  (_myCheckPassed?"true":"false") << "\n") ;					

				string s = theAlternativeChecker->getStatus();

				if (!_myCheckPassed)
				{
			                string theStatus = getStatus();
				    	theStatus = theStatus + "\n" + s;
                			setStatus (theStatus);

				    // mark this alternative, to allow later identification and deletion
				    // (scarmbling both filetype and fileclass should be safe enough...
				    //  let's challenge Murphy!!!
				    a.setFileType("TO_BE_DELETED");
				    a.setFileClass("TO_BE_DELETED");
				}
				else
				{
				    // if check successful, don't visit remaining alternatives!
				    // (leave global _status to the value it had before)
	    			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning false to stop navigation\n") ;		
				    return false;
				}
			}
			else
			{
				ACS_THROW (ThinLayer_PCC_Exception("Unable to create an alternative checker"));
			}
		}
		catch (exception& ex)
		{
			ACS_THROW (ThinLayer_PCC_Exception(ex, "Exception got while checking input"));
		}
	}
	// if we reach this point, check failed... return true to continue scanning remaining alternatives
	// (can't set to falied the global _status until all alternatives are scanned... the input will do!)
	return true;
}

_ACS_END_NAMESPACE
