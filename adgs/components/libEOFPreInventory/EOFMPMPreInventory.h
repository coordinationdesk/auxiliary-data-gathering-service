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

	$Prod: A.C.S. libEOFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2015/12/09 15:37:28  francesco.avanzi
	now inserting Originator instead of originator_id into .MTD file
	
	Revision 1.2  2015/12/03 14:06:24  francesco.avanzi
	Minor issue involving a non void non returning function issue solved
	WorkInProgress reguarding Order_ID assignement
	
	Revision 1.1  2015/11/30 11:01:19  francesco.avanzi
	new class added
	
        
*/
#ifndef _EOFMPMPreInventory_H_
#define _EOFMPMPreInventory_H_

#include <EOFPreInventory.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;


class EOFMPMPreInventory : public EOFPreInventory //PRQA S 2109, 2153 
{
public:
    
    EOFMPMPreInventory() ; //default Public Constructor
    virtual ~EOFMPMPreInventory() throw(); 
    virtual std::string getClassId() {return "EOFMPMPreInventory"; } //PRQA S 2131
    
    virtual void addFurtherNodes(std::vector<std::string> &nodes);
    virtual void extractFurther(rsResourceSet &) ;

    virtual void setFurther(Metadata &) ;
    virtual bool overrideVersionHandling(bool& ){ return true ;} //PRQA S 2131
private:
    
    EOFMPMPreInventory(const EOFMPMPreInventory& ) ; //not implemented
    EOFMPMPreInventory& operator=(const EOFMPMPreInventory& ) ; //not implemented

private:

    std::string sensorId_ ;
    std::string chain_ ;
    std::string originator_ ;
   
private:
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFMPMPreInventory);
    

};
_ACS_END_NAMESPACE

#endif //_EOFMPMPreInventory_H_
