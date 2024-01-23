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

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2015/11/20 11:57:18  marpas
	coding best practices application in progress
	
	Revision 1.6  2015/06/16 13:09:25  damdec
	Compilation warnings solved.
	
	Revision 1.5  2013/02/21 13:43:56  marfav
	Compiler warnings removed
	
	Revision 1.4  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.3  2008/09/09 15:42:11  marfav
	Added support to mandatory flag in descriptors
	
	Revision 1.2  2008/04/30 15:57:51  marfav
	Added temporary symbol table to manage alternatives
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#include <PCC_Base.h>
#include <PCC_Base.h>

#include <TaskTable.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(PCC_Base)


PCC_Base::PCC_Base (const PCC_ParamsPtr& theParameters) :
    TaskTableVisitor(),
	_checkPassed(true),
	_stopOnFailure(false),
	_parameters(theParameters),
    _status()
{
	// Default CTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}

PCC_Base::PCC_Base (const PCC_Base& b) :
    TaskTableVisitor(b),
	_checkPassed(b._checkPassed),
	_stopOnFailure(b._stopOnFailure),
	_parameters(b._parameters),
    _status(b._status)
{}

PCC_Base& PCC_Base::operator= (const PCC_Base& b)
{
    if (&b != this)
    {
	    _checkPassed=_checkPassed;
	    _stopOnFailure=_stopOnFailure;
	    _parameters=_parameters;
        _status=_status;
    }
    return *this;
}


PCC_Base::~PCC_Base () throw() 
{
	// DTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) // PRQA S 4631
}

void
PCC_Base::reset()
{
	_checkPassed = true;
	_status = "";
    // The connection has been created (and deleted) by the connectionGuard
	_parameters->dbKey = "";
}


bool 
PCC_Base::isCheckPassed() const
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "isCheckPassed called; returning " << _checkPassed) ;					

	return _checkPassed;
}


bool
PCC_Base::stopOnFailure() const
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "stopOnFailure called; returning " << _stopOnFailure) ;					

	return _stopOnFailure;
}


void
PCC_Base::stopOnFailure(bool yesnoFlag)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "setting _stopOnFailure to " << yesnoFlag) ;					

	_stopOnFailure = yesnoFlag;
}


const string&
PCC_Base::getStatus() const throw() // PRQA S 4120
{
	return _status;
}


//
// SYMBOL TABLE MANAGEMENT
//

bool
PCC_Base::isAlternativeInSymTable(InputAlternative const& alt, bool active) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	InputAlternative newAlt = alt;
	if (active)
	{
		newAlt.setAltActivationTime(0);
	}
	return _parameters->theSymbolTable.find(newAlt) != _parameters->theSymbolTable.end();
}


PCC_Base::SymTableParams
PCC_Base::getAlternativeValFromSymTable(InputAlternative const& alt, bool active) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	InputAlternative newAlt = alt;
	if (active)
	{
		newAlt.setAltActivationTime(0);
	}

	SymTable::const_iterator it = _parameters->theSymbolTable.find(newAlt);
	
	if ( it == _parameters->theSymbolTable.end() )
	{
		ACS_THROW (PCC_Base_Exception ("Alternative not found in symbol table")); // PRQA S 3081
	}
	
	return it->second;
}


void
PCC_Base::addAlternativeToSymTable(InputAlternative const& alt, PCC_Base::SymTableParams const& val, bool active) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
	InputAlternative newAlt = alt;
	if (active)
	{
		newAlt.setAltActivationTime(0);
	}

	(_parameters->theSymbolTable)[newAlt] = val;
}


//
// TEMPORARY SYMBOL TABLE MANAGEMENT
//

bool
PCC_Base::isAlternativeInTempSymTable(InputAlternative const& alt, bool active) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)

	InputAlternative newAlt = alt;
	if (active)
	{
		newAlt.setAltActivationTime(0);
	}
	return _parameters->tempSymbolTable.find(newAlt) != _parameters->tempSymbolTable.end();
}


PCC_Base::SymTableParams
PCC_Base::getAlternativeValFromTempSymTable(InputAlternative const& alt, bool active) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
	InputAlternative newAlt = alt;
	if (active)
	{
		newAlt.setAltActivationTime(0);
	}

	SymTable::const_iterator it = _parameters->tempSymbolTable.find(newAlt);
	
	if ( it == _parameters->tempSymbolTable.end() )
	{
		ACS_THROW (PCC_Base_Exception ("Alternative not found in temporary symbol table")); // PRQA S 3081
	}
	
	return it->second;
}


void
PCC_Base::addAlternativeToTempSymTable(InputAlternative const& alt, PCC_Base::SymTableParams const& val, bool active) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)

	InputAlternative newAlt = alt;
	if (active)
	{
		newAlt.setAltActivationTime(0);
	}
	(_parameters->tempSymbolTable)[newAlt] = val;
}

void PCC_Base::updateExpirationDate()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
	DateTime expDate(false);
	for (SymTable::iterator it = _parameters->tempSymbolTable.begin(); it != _parameters->tempSymbolTable.end(); ++it) // PRQA S 4238
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Alternative for filetype " << it->first.getFileType() <<", Active: "<< boolalpha << it->second.isActive);	
		if (!it->second.isActive)
		{
			// get as activation date the minimum between the activation dates
			if ((expDate.isFilled() && it->second.activationDate < expDate) || !expDate.isFilled())
			{
				expDate = it->second.activationDate;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Expiration Date to be used for active not passed " << expDate.getSqlString()) ;
			}
		}
	}
	if (expDate.isFilled())
	{
		// update the expiration date on active alternatives
		for (SymTable::iterator it = _parameters->tempSymbolTable.begin(); it != _parameters->tempSymbolTable.end(); ++it) // PRQA S 4238
		{
			if (it->second.isActive && !it->second.checkPassed)
			{
				if ((it->second.expirationDate.isFilled() && expDate < it->second.expirationDate) || !it->second.expirationDate.isFilled() )
				{
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Update Expiration Date for filetype " << it->first.getFileType() << ": " << expDate.getSqlString()) ;
					it->second.expirationDate = expDate;
				}
			}
		}
	}
}


void 
PCC_Base::promoteTempSymbolTable(bool mandatoryFlag, bool inputCheckStatus)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)

	// Scan each element of the tempSymbolTable
	// For each element
	// - set the mandatory flag
	// - update the symbol table

	for (SymTable::iterator it = _parameters->tempSymbolTable.begin(); it != _parameters->tempSymbolTable.end(); ++it) // PRQA S 4238
	{
		if (it->second.isActive)
		{
			bool newFlag = false;
			bool alternativeCheckStatus = it->second.checkPassed;

			if (inputCheckStatus)
			{
				// The input has passed the check... only the selected alternative
				// will be "to be downloaded", the others will be not mandatory
				// even if the input is not mandatory... the alternative that passed the check
				// shall be downloaded
				newFlag = alternativeCheckStatus;
			}
			else
			{
				// The input is not ready
				// All the alternatives are "to be downloaded" if the input is mandatory
				newFlag = mandatoryFlag;
			}

			if (isAlternativeInSymTable (it->first, true))
			{
				// If the alternative is already present in the symTable
				// update the status executing the logical or between the old
				// toBeDownloaded flag and the new one
				// in this way an alternative can be upgraded to be downloaded
				// but never downgraded to "not be downloaded"
				// The same update logic is applied to the mandatory flag field
				SymTableParams p = getAlternativeValFromSymTable (it->first, true);
				p.toBeDownloaded |= newFlag;
				p.isMandatory |= mandatoryFlag;
				// This will overwrite the "p" value in the symbol table
				addAlternativeToSymTable (it->first, p, true);
			}
			else
			{
				// The element shall be added to the symbol table
				// taking in account the new "to be downloaded flag"
				// and the mandatory flag
				it->second.toBeDownloaded = newFlag;
				it->second.isMandatory = mandatoryFlag;
				addAlternativeToSymTable (it->first, it->second, true);
			}
		}
	}

	_parameters->tempSymbolTable.clear();
}
_ACS_END_NAMESPACE

