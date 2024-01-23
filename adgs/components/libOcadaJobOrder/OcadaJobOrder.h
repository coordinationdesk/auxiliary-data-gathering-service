/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Ocada JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2012/05/18 15:19:25  marpas
	getting rid of some compiler warnings
	
	Revision 1.2  2012/03/28 16:14:04  damdec
	Destructor updated.
	
	Revision 1.1.1.1  2009/03/02 14:40:14  chicas
	Just imported
	
*/


#ifndef _OCADA_JOB_ORDER_H
#define _OCADA_JOB_ORDER_H

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

class OcadaJobOrder : public JobOrder
{
  public:

	/*! class OcadaJobOrderException declaration */
	exDECLARE_EXCEPTION(OcadaJobOrderException,acs::exException) ;


    OcadaJobOrder();
    virtual ~OcadaJobOrder() throw();

    OcadaJobOrder& operator = (const OcadaJobOrder& j);

  protected:
    virtual void write(std::ofstream& o) const;
    virtual void read (acs::XMLIstream& i);
    virtual void read (std::ifstream& i);
    virtual void readConfigSpaces (acs::rsResourceSet&);

  private:
    OcadaJobOrder(const OcadaJobOrder& j);
    std::string  DateTime2String(acs::DateTime d) const;
    std::string  DateTime2FileNamePart(acs::DateTime d) const;

  private:
    std::string buildTempFilename(const std::string& Filetype, const char *ext=NULL) const;
    acs::DateTime utcs2datetime (const std::string& s) const;

// helper classes
private:

	/**
	**  XML HELPER CLASSES
	**/

	class xml_indent
	{
    public:
        virtual ~xml_indent() throw() {} ;
	protected:
		static unsigned int _indentCount;
	};

	class xml_section_tag : public xml_indent
	{
	public:
		xml_section_tag(std::ofstream& o, std::string s, std::string a="");
		~xml_section_tag() throw() ;
	private:
		std::ofstream& _o;
		std::string _s;
	};

	class xml_element_tag : public xml_indent
	{
	public:
		xml_element_tag(std::ofstream& o, std::string s, std::string v);
		xml_element_tag(std::ofstream& o, std::string s, unsigned int u);
		~xml_element_tag() throw() {} ;
	};


	/**
	**  BREAKPOINT DUMPER CLASS
	**/

	friend class breakpoint_dumper;
	class breakpoint_dumper
	{
		const OcadaJobOrder* _jo;
		std::ofstream& _o;

	public:
	explicit breakpoint_dumper(const OcadaJobOrder* jo, std::ofstream& oo);
	void operator() (const BreakpointDescriptor& b);
	};


	/**
	**  OUTPUT DUMPER CLASS
	**/

	friend class output_dumper;
	class output_dumper
	{
		const OcadaJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit output_dumper(const OcadaJobOrder* jo, std::ofstream& oo);
		void operator() (const OutputDescriptor& d);
	};


	/**
	**  FILE DUMPER CLASS
	**/

	friend class file_dumper;
	class file_dumper
	{
		const OcadaJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit file_dumper(const OcadaJobOrder* jo, std::ofstream& oo);
		void operator() (const std::string& s);
	};


	/**
	**  ALTERNATIVE DUMPER CLASS
	**/

	friend class altern_dumper;
	class altern_dumper
	{
		const OcadaJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit altern_dumper(const OcadaJobOrder* jo, std::ofstream& oo);
		void operator() (const InputAlternative& i);
	};


	/**
	**  INPUT DUMPER CLASS
	**/

	friend class input_dumper;
	class input_dumper
	{
		const OcadaJobOrder* _jo;
		std::ofstream& _o;

	public:
		explicit input_dumper(const OcadaJobOrder* jo, std::ofstream& oo);
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
		const OcadaJobOrder* _jo;
		std::ofstream& _o;
		bool _brkEnabled;

	public:
		explicit task_dumper(const OcadaJobOrder* jo, std::ofstream& oo, bool b);
		void operator() (const Task& task);
	};

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OcadaJobOrder)
};

_ACS_END_NAMESPACE

#endif // _OCADA_JOB_ORDER_H
