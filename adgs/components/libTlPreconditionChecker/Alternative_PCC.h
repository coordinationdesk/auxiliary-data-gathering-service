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
	Revision 1.6  2015/06/16 13:09:25  damdec
	Compilation warnings solved.
	
	Revision 1.5  2013/02/21 13:43:56  marfav
	Compiler warnings removed
	
	Revision 1.4  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.3  2010/03/04 15:14:12  marfav
	Addedd support for alternative filtering based on
	- equal files detection
	- generation time sorting
	
	Revision 1.2  2008/04/30 15:57:51  marfav
	Added temporary symbol table to manage alternatives
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#ifndef _ALTERNATIVE_PCC_H_
#define _ALTERNATIVE_PCC_H_

#include <PCC_Base.h>

_ACS_BEGIN_NAMESPACE(acs)


class Alternative_PCC : public PCC_Base // PRQA S 2109, 2153
{
public:

    using PCC_Base::exitVisit;

	exDECLARE_EXCEPTION (Alternative_PCC_Exception, PCC_Base_Exception) ; // PRQA S 2131, 2153, 2502

	explicit Alternative_PCC (const PCC_ParamsPtr& theParameters);
	virtual ~Alternative_PCC () throw() ;

	// Specialized method to handle alternative filename filtering logic
	virtual bool exitVisit (InputAlternative&);

protected:
	// Virtual methods can be used to upgrade the Alternative_PCC check with customized 
	// equal and lessthan operators
	virtual bool equalInputs (const PCC_Base::InputFileName&, const PCC_Base::InputFileName&);
	virtual bool inputVersionLessThan (const PCC_Base::InputFileName&, const PCC_Base::InputFileName&);

private:
	void searchDiscardableInputs (const InputAlternative&);
	void createLUT(const InputAlternative&);
	void updateSymbolTable(const InputAlternative&);

	Alternative_PCC (); // not implemented
	Alternative_PCC (const Alternative_PCC& ); // not implemented
	Alternative_PCC & operator= (const Alternative_PCC& ); // not implemented
	
private:
	struct FileInfos // PRQA S 2173, 2175
	{
		DateTime validityStart; // PRQA S 2100 3
		DateTime validityStop;
		DateTime generationTime;

		FileInfos() :
			validityStart(),
			validityStop(),
			generationTime()
		{}
		FileInfos(const FileInfos &r) :
			validityStart(r.validityStart),
			validityStop(r.validityStop),
			generationTime(r.generationTime)
        {}
		FileInfos& operator=(const FileInfos &r) 
        {
            if (this != &r) {
			    validityStart = r.validityStart;
			    validityStop = r.validityStop ;
			    generationTime = r.generationTime ;
            }
            return *this ;
        }
        
        ~FileInfos () throw() {}
	};

	FileInfos getFileInfos (const PCC_Base::InputFileName&);


	std::map <size_t, bool> _discardMap;
	std::map <int, int> _lut;
	std::map <std::string, FileInfos> _infoMap;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Alternative_PCC)
};

_ACS_END_NAMESPACE

#endif //_ALTERNATIVE_PCC_H_
