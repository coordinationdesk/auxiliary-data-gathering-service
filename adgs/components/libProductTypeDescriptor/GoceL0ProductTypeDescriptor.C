// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

    $Log$
    Revision 2.3  2014/03/26 09:57:36  marpas
    coding best practices applied
    warnings fixed (qa & g++)

*/

#include <GoceL0ProductTypeDescriptor.h>
#include <vector>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GoceL0ProductTypeDescriptor)

//default class constructor
GoceL0ProductTypeDescriptor::GoceL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Goce")
{
}


//class constructor
GoceL0ProductTypeDescriptor::GoceL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


// //Class destructor
GoceL0ProductTypeDescriptor::~GoceL0ProductTypeDescriptor() throw()
{
}




// /////////////////////////////////////////////////////////////////////////////////////////
// // Public methods 
// /////////////////////////////////////////////////////////////////////////////////////////

vector<ProductField> GoceL0ProductTypeDescriptor::getFields(const string& prodId) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for prodId '" << prodId << "'" );

	//check if prodId is a valid Product Id
	map<string, L0ProductInfo>::const_iterator it = getProductInfo().find(prodId);
	ACS_COND_THROW( it == getProductInfo().end() , L0ProductTypeDescriptorException("Incorrect ProductId " + prodId) ); // PRQA S 3081

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg;
		msg << "GoceL0ProductTypeDescriptor::getFields returning : ";
		for(unsigned int i=0; i<((*it).second.getProductFields()).size(); i++) {
			msg << ((*it).second.getProductFields())[i] ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,msg.str()) ;
	ACS_CLASS_END_DEBUG
	
	return (*it).second.getProductFields();
}

vector< pair< string, vector<ProductField> > > GoceL0ProductTypeDescriptor::getFieldsByApidVcid(unsigned int apid, unsigned int vcid) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for APID '" << apid << "' and VCID '" << vcid << "'" );
	
	vector< pair< string, vector<ProductField> > > result;
	
	map<string, L0ProductInfo> productInfo = getProductInfo() ;
	const vector<string> &l0ProductIds = getOrderedProductIds() ;
	
	//for each Product, extract the list of APIDs and verify if there's one equal to the one in input
	size_t p_size = l0ProductIds.size() ;
    for(unsigned int i=0; i<p_size; i++)
	{
		if( (productInfo[l0ProductIds[i]]).getVCID() == vcid )
		{
			vector<unsigned int> apids = (productInfo[l0ProductIds[i]]).getAPIDList();
			if( find(apids.begin(), apids.end(),apid) != apids.end())
			{
				pair<string , vector<ProductField> >  elem;
				elem.first = l0ProductIds[i];
				elem.second = (productInfo[l0ProductIds[i]]).getProductFields();
				result.push_back(elem);
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream msg;
					msg << "GoceL0ProductTypeDescriptor::getFieldsByApid pushing back element with id='" << elem.first << "' and List\n";
					size_t es_size=elem.second.size() ; 
                    for(unsigned int j=0; j<es_size; ++j) {
						msg << (elem.second)[j];
                    }
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,msg.str()) ;
				ACS_CLASS_END_DEBUG
			}
		}
	
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg;
		msg << "GoceL0ProductTypeDescriptor::getFieldsByApidVcid returning : ";
        size_t r_size=result.size() ; 
		for(unsigned int i=0; i<r_size ; ++i) {
            size_t rs_size=result[i].second.size() ; 
			for(unsigned int j=0; j<rs_size; ++j) {
				msg << (result[i].second)[j];
            }
        }
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,msg.str()) ;
	ACS_CLASS_END_DEBUG
	
	return result;
}

string GoceL0ProductTypeDescriptor::getProdIdByApidFields(unsigned int apid, // PRQA S 4211
														  const vector<ProductField>& listOfFields)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getProdIdByApidFields called for APID '" << apid << "'" );
	string result="";
	map<string, L0ProductInfo>::const_iterator it = getProductInfo().begin();
	//for each Product, extract the list of APIDs and verify if there's one equal to the one in input
	while( it != getProductInfo().end() )
	{
		vector<unsigned int> apids = (*it).second.getAPIDList();
		//if the input apid matches retrieve the fields and verifies if they match the fields in input
		if( find(apids.begin(), apids.end(),apid) != apids.end())
		{
			vector<ProductField> fields = (*it).second.getProductFields();
			if( equal(listOfFields.begin(),listOfFields.end(),fields.begin()) )
			{
				result = (*it).second.getId();
				break;
			}
		}
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"returning '" << result << "'") ;
	return result;
}


void GoceL0ProductTypeDescriptor::getNameByApidFieldsClass(unsigned int apid, // PRQA S 4211
			 						             	       const vector<ProductField>& listOfFields,
			 						             	       const string& fClass,
												 	       string& result)
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg;
		msg << "GoceL0ProductTypeDescriptor::getNameByApidFieldsClass call for APID " << apid << ",FileClass '" << fClass << "' and List of Fields:\n";
		size_t lf_size = listOfFields.size();
        for(unsigned int i=0; i<lf_size;++i) {
			msg << listOfFields[i];
        }
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,msg.str());
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW(!getSatFlag() , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	bool exit=false;
	vector<string> fileNameRoots;
	map<string, L0ProductInfo>::const_iterator it = getProductInfo().begin();
	vector<unsigned int> apids;
	while( (!exit) && (it != getProductInfo().end()) )
	{
		//retrieve the list of APIDs associated to the current Product
		apids = (*it).second.getAPIDList();
		
		//the APID in input belongs to the list of APIDs
		if( find(apids.begin(),apids.end(),apid) != apids.end() )
		{
			//check if the list of fields in input matches the list of fields associated to the current Product
			vector<ProductField> fields = (*it).second.getProductFields();
			if( equal(listOfFields.begin(),listOfFields.end(),fields.begin()) )
			{
				//retrieve the list of filename roots associated to the current ProductId
				getFileNamesHandler()->getNamesById(it->first,fileNameRoots);
                size_t fr_size = fileNameRoots.size() ;
		    	for(unsigned int i=0; i<fr_size; i++)
				{
					if(fClass == getFileNamesHandler()->getFileClassFromRoot(fileNameRoots[i]))
					{
						result = fileNameRoots[i];
						break;
					}
				}
				exit=true;
			}
		}
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"returning '" << result << "'");
}

void GoceL0ProductTypeDescriptor::getNameByProdIdClass(const string& prodId,const string& fClass,string& result) // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getNameByProdIdClass call for ProdId '" << prodId << " and FileClass '" << fClass << "'" );
	ACS_COND_THROW(!getSatFlag() , L0ProductTypeDescriptorException("Satellite name not set") ) ; // PRQA S 3081
	//check if prodId is a valid Product Id
	ACS_COND_THROW( getProductInfo().find(prodId) == getProductInfo().end() , L0ProductTypeDescriptorException("Incorrect ProductId "+ prodId) ); // PRQA S 3081
	
	vector<string> names;
	getFileNamesHandler()->getNamesById(prodId,names);	
    size_t n_size = names.size() ;
	for(unsigned int i=0; i<n_size; ++i)
	{
		if(fClass == getFileNamesHandler()->getFileClassFromRoot(names[i]))
		{
			result = names[i];
			break;
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"returning '" << result << "'" );
}


_ACS_END_NAMESPACE
