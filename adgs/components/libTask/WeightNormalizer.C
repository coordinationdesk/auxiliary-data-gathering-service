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
	Revision 2.3  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.2  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.1  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#include <WeightNormalizer.h>
#include <TaskTableElement.h>

#include <Filterables.h>

#include <cmath>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
						 

ACS_CLASS_DEFINE_DEBUG_LEVEL(WeightNormalizer)


WeightNormalizer::WeightNormalizer()
{
}

WeightNormalizer::~WeightNormalizer() throw() {}


void 
WeightNormalizer::normalize (TaskTableElement& theElement)
{
    ACS_CLASS_WRITE_DEBUG ( ACS_VLO_VERB, "Normalize (ELEMENTS) : BEGIN")

	float elementWeight = theElement.getAssignedPercentage();

    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "original overall element weight is [" << elementWeight << "]")

	if (elementWeight <= 0) 
	{
		// protect against negative weights and divisions by zero
		elementWeight = 100; // PRQA S 4400
	}

    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "overall element weight is [" << elementWeight << "]")

	bool allWeitgtSpecified = true;
	float overallWeight = 0;
		
	// First pass .....
    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "First pass")	

	vector<TaskTableVisitable*> elements = theElement.getElements();	

	
	for (vector<TaskTableVisitable*>::iterator it = elements.begin(); it != elements.end(); ++it)  // PRQA S 4238
	{
		TaskTableElement *p = dynamic_cast<TaskTableElement *>(*it); // PRQA S 3081
		if (!p) {
		    continue;
        }

		float weight = p->getAssignedPercentage();

        ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "normalizing element [" << it - elements.begin() << "]. Weight is [" << weight << "]")
		
		allWeitgtSpecified = allWeitgtSpecified && (weight != 0);
		overallWeight+= fabs (weight);

        ACS_CLASS_WRITE_DEBUG ( 
            ACS_MID_VERB, 
            "Normalize element - allSpecified [" << boolalpha << allWeitgtSpecified << noboolalpha << 
            "] overall weight = " << overallWeight
        )
	}
	
	// Second pass ....
    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Second pass")	
	
	size_t numOfElements = elements.size();
	float rescaledOverall = 0;		
	
	for (vector<TaskTableVisitable*>::iterator it = elements.begin(); it != elements.end(); ++it) // PRQA S 4238 
	{
		TaskTableElement *p = dynamic_cast<TaskTableElement *>(*it); // PRQA S 3081
		if (!p) {
		    continue;
        }

		float newWeight = 0;
		if (allWeitgtSpecified) 
		{
			newWeight = fabs ( p->getAssignedPercentage() / overallWeight * 100 ); // PRQA S 4400
		}
		else
		{
			newWeight = 100.0 / numOfElements; // PRQA S 3011, 3012, 3084, 4400
		}

		// Apply rescaling factor of the whole element weight
		newWeight = newWeight * elementWeight / 100; // PRQA S 4400
		
		p->setAssignedPercentage ( newWeight );
		rescaledOverall += newWeight;

        ACS_CLASS_WRITE_DEBUG ( 
            ACS_LOW_VERB, 
            "Normalizing element [" << it - elements.begin() << "]." << 
            " new weight is [" << newWeight << "]. Rescaled overall = " << rescaledOverall << 
            " / " << elementWeight
        )	
	}
	
	// Third pass
	// Normalize each sub-element of the array
	for (vector<TaskTableVisitable*>::iterator it = elements.begin(); it != elements.end(); ++it)  // PRQA S 4238
	{
		TaskTableElement *p = dynamic_cast<TaskTableElement *>(*it); // PRQA S 3081
		if (!p) {
		    continue;
        }

        ACS_CLASS_WRITE_DEBUG ( ACS_MID_VERB, "Normalizing element [" << it - elements.begin() << "] nested elements...")	

		p->normalize(this);
	}

    ACS_CLASS_WRITE_DEBUG ( ACS_VLO_VERB, "Normalize (ELEMENTS) : END")
}


_ACS_END_NAMESPACE



