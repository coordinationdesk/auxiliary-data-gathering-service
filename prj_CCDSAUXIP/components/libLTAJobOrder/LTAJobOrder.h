// PRQA S 1050 EOF
/*

	Copyright 2019-2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA JobOrder Library $

*/

#ifndef _LTAJobOrder_H
#define _LTAJobOrder_H

#include <JobOrder.h>
#include <exException.h>
#include <DateTime.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <fstream>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class BreakpointDescritpor;
class InputDescritpor;
class OutputDescritpor;

class LTAJobOrder : public JobOrder
{
public:

	exDECLARE_EXCEPTION (LTAJobOrderException, acs::exException);

	LTAJobOrder();
	virtual ~LTAJobOrder() ACS_NOEXCEPT ;

protected:
	virtual void write(std::ofstream& o) const;
	virtual void read (acs::XMLIstream& i);
	virtual void read (std::ifstream& i);
	virtual void readConfigSpaces (acs::rsResourceSet&);

private:
	LTAJobOrder(const LTAJobOrder& j);
	std::string  DateTime2String(acs::DateTime d) const;

#ifdef TEST_LIBLTAJOBORDER
public:
#else
private:
#endif
	std::string buildTempFilename(const std::string& Filetype, const char *ext=NULL) const;
	acs::DateTime utcs2datetime (const std::string& s) const;

// helper classes
private:

	/**
	**  XML HELPER CLASSES
	**/

	class xml_indent
	{
	protected:
		static unsigned int _indentCount;
	};

	class xml_section_tag : public xml_indent
	{
	public:
		xml_section_tag(std::ofstream& o, std::string s, std::string a="");
		~xml_section_tag();
	private:
		std::ofstream& _o;
		std::string _s;
	};

	class xml_element_tag : public xml_indent
	{
	public:
		xml_element_tag(std::ofstream& o, std::string s, std::string v);
		xml_element_tag(std::ofstream& o, std::string s, unsigned int u);
	};


	/**
	**  BREAKPOINT DUMPER CLASS
	**/

	friend class breakpoint_dumper;
	class breakpoint_dumper
	{
		const LTAJobOrder* _jo;
		std::ofstream& _o;

	public:
	    explicit breakpoint_dumper(const LTAJobOrder* jo, std::ofstream& oo);
	    void operator() (const BreakpointDescriptor& b);
	};


	/**
	**  OUTPUT DUMPER CLASS
	**/

	friend class output_dumper;
	class output_dumper
	{
		const LTAJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit output_dumper(const LTAJobOrder* jo, std::ofstream& oo);
		void operator() (const OutputDescriptor& d);
	};


	/**
	**  FILE DUMPER CLASS
	**/

	friend class file_dumper;
	class file_dumper
	{
		const LTAJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit file_dumper(const LTAJobOrder* jo, std::ofstream& oo);
		void operator() (const std::string& s);
	};


	/**
	**  ALTERNATIVE DUMPER CLASS
	**/

	friend class altern_dumper;
	class altern_dumper
	{
		const LTAJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit altern_dumper(const LTAJobOrder* jo, std::ofstream& oo);
		void operator() (const InputAlternative& i);
	};


	/**
	**  INPUT DUMPER CLASS
	**/

	friend class input_dumper;
	class input_dumper
	{
		const LTAJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit input_dumper(const LTAJobOrder* jo, std::ofstream& oo);
		void operator() (const InputDescriptor& i);
	};


	/**
	**  CONFIGURATION SPACE DUMPER CLASS
	**/

	friend class confSpaces_dumper;
	class confSpaces_dumper
	{
		std::ofstream& _o;

	public:
		explicit confSpaces_dumper (std::ofstream& oo);
		void operator() (const ConfigSpaceDescriptor& c);
	};


	/**
	**  TASK DUMPER CLASS
	**/

	friend class task_dumper;
	class task_dumper
	{
		const LTAJobOrder* _jo;
		std::ofstream& _o;
		bool _brkEnabled;

	public:
		explicit task_dumper(const LTAJobOrder* jo, std::ofstream& oo, bool b);
		void operator() (const Task& task);
	};


	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAJobOrder);
};

_ACS_END_NAMESPACE

#endif
