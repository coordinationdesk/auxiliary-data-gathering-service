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

	$Prod: A.C.S. libRollingAlgo$

 */

#include <ParametersBasedSelector.h>

#include <Filterables.h>
#include <UtilXml.h>

#include <boost/algorithm/string.hpp> // PRQA S 1013

#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ParametersBasedSelector)


ParametersBasedSelector::ParametersBasedSelector(const InvPolicy & policy):
InvPolicyCommonSelector(policy)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ParametersBasedSelector::~ParametersBasedSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void ParametersBasedSelector::selectInit( dbConnection &conn ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream polId; polId<<_polId;

	try {

		//Set attributes from Parameters File.
		XmlDocSptr xmlDocSptr( new XmlDoc );
		xmlDocSptr->initWithString(_polParameters);

		//Read the 'operator' section
		vector< XmlDocSptr > selOperator = xmlDocSptr->xPath("//Operator/text()");
		ACS_COND_THROW( selOperator.empty(), // PRQA S 3081
				InvPolicySelector::exInvPolicySelectorException("Error in Parameter file for policy "+polId.str()+": No Operator configured.") );
		_operatorSelections = selOperator.front()->str() ;
		boost::to_lower(_operatorSelections);
		ACS_COND_THROW( _operatorSelections != "and" && _operatorSelections != "or", // PRQA S 3081
				InvPolicySelector::exInvPolicySelectorException("Error in Parameter file for policy "+polId.str()+": Operator "+_operatorSelections+" not permitted.") );

		//Read the 'define' section
		map<string, string> parDefines;
		{
			vector< XmlDocSptr > xmlNames =  xmlDocSptr->xPath("//Define/name/text()");
			vector< XmlDocSptr > xmlValues = xmlDocSptr->xPath("//Define/value/text()");

			ACS_COND_THROW( xmlNames.size() != xmlValues.size(), // PRQA S 3081
					InvPolicySelector::exInvPolicySelectorException("Error in Parameter file for policy "+polId.str()+": Check define names and values.") );
			size_t xmln_size = xmlNames.size();
			for (size_t i=0; i<xmln_size; ++i ) {
				parDefines[ xmlNames.at(i)->str() ] = xmlValues.at(i)->str();
			}
		}

		//Read the 'Selection' section
		vector< XmlDocSptr > xmlSelections =  xmlDocSptr->xPath("//Selection/text()");
		ACS_COND_THROW( xmlSelections.empty(), // PRQA S 3081
				InvPolicySelector::exInvPolicySelectorException("Error in Parameter file for policy "+polId.str()+": No Selections configured.") );

		//For each configured selection
		_sqlSelections.clear();
		for ( vector< XmlDocSptr >::iterator it = xmlSelections.begin(); it!=xmlSelections.end(); ++it ) { // PRQA S 4238

			_sqlSelections.push_back( (*it)->str() );

			//Replace all used Definitions in current Selection. Eg. ${defName01} --> value
			for (std::map<string,string>::const_iterator it_inner=parDefines.begin(); it_inner!=parDefines.end(); ++it_inner) { // PRQA S 4238
				boost::replace_all( _sqlSelections.back(), "${"+it_inner->first+"}", it_inner->second);
			}

			//Replace System Definitions in current Selection: ${rollingPolicyId} --> value
			ostringstream ossId; ossId<<_polId;
			boost::replace_all( _sqlSelections.back(), "${rollingPolicyId}", ossId.str() );
			if ( 0 != _polStorage ) {
				ostringstream ossStId; ossStId<<_polStorage;
				boost::replace_all( _sqlSelections.back(), "${rollingStorageId}", ossStId.str() );
			}
			boost::replace_all( _sqlSelections.back(), "${rollingFileType}",  _polFileType );
			if ( not _polFileClass.empty() ) {
				boost::replace_all( _sqlSelections.back(), "${rollingFileClass}", _polFileClass );
			}
		}

	} catch(exception& ex) {
		ACS_THROW(InvPolicySelector::exInvPolicySelectorException(ex, "Error in processing policy Id ["+polId.str()+"]" ));
	}
}

_ACS_END_NAMESPACE

