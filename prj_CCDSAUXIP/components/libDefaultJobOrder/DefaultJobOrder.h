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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:34:07  marpas
	adopting libException 5.x standards and coding best practices
	qa and compilation warnings removed
	
	Revision 1.1.1.1  2012/10/17 13:11:09  chicas
	imported sources
	
	
*/


#ifndef _DefaultJobOrder_H
#define _DefaultJobOrder_H

#include <acs_c++config.hpp>
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

class DefaultJobOrder : public JobOrder // PRQA S 2109
{
public:

	exDECLARE_EXCEPTION (DefaultJobOrderException, exException); // PRQA S 2131, 2502

	DefaultJobOrder();
	virtual ~DefaultJobOrder() throw() ;

private:
	/** Added Fields to Base JobOrder */
	std::string _satelliteId;
	std::string _sensorId;

protected:
	virtual void write(std::ofstream& o) const;  // PRQA S 2502 3
	virtual void read (XMLIstream&);
	virtual void read (std::ifstream&);
	virtual void readConfigSpaces (rsResourceSet&);

private:
	DefaultJobOrder(const DefaultJobOrder& j);
	DefaultJobOrder& operator= (const DefaultJobOrder& j); // not implemented
	std::string  DateTime2String(const DateTime &d) const;

private:
	std::string buildTempFilename(const std::string& Filetype, const char *ext=0) const;
	DateTime utcs2datetime (const std::string& s) const;

public:

	/** Getter and Setter */
	const std::string& getSatelliteId() const 			{ return _satelliteId;	}
	void setSatelliteId(const std::string& satelliteId) { _satelliteId = satelliteId; }

	const std::string& getSensorId() const 				{ return _sensorId;	}
	void setSensorId(const std::string& sensorId) 		{ _sensorId = sensorId;	}


// helper classes
private:

	/**
	**  XML HELPER CLASSES
	**/

	class xml_indent
	{
    public:
        xml_indent() throw() {}
        xml_indent(const xml_indent &) throw() {}
        xml_indent &operator= (const xml_indent &) throw() { return *this ;} // PRQA S 4072
        virtual ~xml_indent() throw() {} // PRQA S 2131
	protected:
		static unsigned int _indentCount; // PRQA S 2101
	};

	class xml_section_tag : public xml_indent // PRQA S 2109, 2153
	{
	public:
		xml_section_tag(std::ofstream& o, const std::string &s, const std::string &a="");
		virtual ~xml_section_tag() throw() ;
	private:
        xml_section_tag() ; // not implemented
        xml_section_tag(const xml_section_tag &) ; // not implemented
        xml_section_tag& operator=(const xml_section_tag &) ; // not implemented
	private:
		std::ofstream& _o;
		std::string _s;
	};

	class xml_element_tag : public xml_indent // PRQA S 2109, 2153
	{
    public:
        virtual ~xml_element_tag() throw() {} // PRQA S 2131
	public:
		xml_element_tag(std::ofstream& o, const std::string &s, const std::string &v);
		xml_element_tag(std::ofstream& o, const std::string &s, unsigned int u);
	private:
        xml_element_tag() ; // not implemented
        xml_element_tag(const xml_element_tag &) ; // not implemented
        xml_element_tag& operator=(const xml_element_tag &) ; // not implemented
	};


	/**
	**  BREAKPOINT DUMPER CLASS
	**/

	friend class breakpoint_dumper; // PRQA S 2107
    
	class breakpoint_dumper // PRQA S 2109, 2502
	{
	public:
	    explicit breakpoint_dumper(const DefaultJobOrder* jo, std::ofstream& oo);
        breakpoint_dumper(const breakpoint_dumper &r) : _jo(r._jo), _o(r._o) {} // PRQA S 2528
        ~breakpoint_dumper() throw() {} // PRQA S 2131
	    void operator() (const BreakpointDescriptor& b);
	private:
        breakpoint_dumper() ; // not implemented
        breakpoint_dumper& operator=(const breakpoint_dumper &) ; // not implemented
    private:
		const DefaultJobOrder* _jo;
		std::ofstream& _o;

	};


	/**
	**  OUTPUT DUMPER CLASS
	**/

	friend class output_dumper; // PRQA S 2107
	class output_dumper // PRQA S 2109, 2502
	{
	public:
		explicit output_dumper(const DefaultJobOrder* jo, std::ofstream& oo);
        output_dumper(const output_dumper &r) : _jo(r._jo), _o(r._o) {} // PRQA S 2528
        ~output_dumper() throw() {} // PRQA S 2131
		void operator() (const OutputDescriptor& d);
	private:
        output_dumper() ; // not implemented
        output_dumper& operator=(const output_dumper &) ; // not implemented
    private:
		const DefaultJobOrder* _jo;
		std::ofstream& _o;

	};


	/**
	**  FILE DUMPER CLASS
	**/

	friend class file_dumper; // PRQA S 2107
	class file_dumper // PRQA S 2109, 2502
	{
	public:
		explicit file_dumper(const DefaultJobOrder* jo, std::ofstream& oo);
        file_dumper(const file_dumper &r) : _jo(r._jo), _o(r._o) {} // PRQA S 2528
        ~file_dumper() throw() {} // PRQA S 2131
		void operator() (const std::string& s);
	private:
        file_dumper() ; // not implemented
        file_dumper& operator=(const file_dumper &) ; // not implemented
    private:
		const DefaultJobOrder* _jo;
		std::ofstream& _o;
	};


	/**
	**  ALTERNATIVE DUMPER CLASS
	**/

	friend class altern_dumper; // PRQA S 2107
	class altern_dumper // PRQA S 2109, 2502
	{
	public:
		explicit altern_dumper(const DefaultJobOrder* jo, std::ofstream& oo);
        altern_dumper(const altern_dumper &r) : _jo(r._jo), _o(r._o) {} // PRQA S 2528
        ~altern_dumper() throw() {} // PRQA S 2131
		void operator() (const InputAlternative& i);
	private:
        altern_dumper() ; // not implemented
        altern_dumper& operator=(const altern_dumper &) ; // not implemented
    private:
		const DefaultJobOrder* _jo;
		std::ofstream& _o;

	};


	/**
	**  INPUT DUMPER CLASS
	**/

	friend class input_dumper; // PRQA S 2107
	class input_dumper // PRQA S 2109, 2502
	{
	public:
		explicit input_dumper(const DefaultJobOrder* jo, std::ofstream& oo);
        input_dumper(const input_dumper &r) : _jo(r._jo), _o(r._o) {} // PRQA S 2528
        ~input_dumper() throw() {}
		void operator() (const InputDescriptor& i);
	private:
        input_dumper() ; // not implemented
        input_dumper& operator=(const input_dumper &) ; // not implemented
    private:
		const DefaultJobOrder* _jo;
		std::ofstream& _o;

	};


	/**
	**  CONFIGURATION SPACE DUMPER CLASS
	**/

	friend class confSpaces_dumper; // PRQA S 2107
	class confSpaces_dumper // PRQA S 2109, 2502
	{

	public:
		explicit confSpaces_dumper (std::ofstream& oo);
        confSpaces_dumper(const confSpaces_dumper &r) : _o(r._o) {} // PRQA S 2528
        ~confSpaces_dumper() throw() {}
		void operator() (const ConfigSpaceDescriptor& c);
	private:
        confSpaces_dumper() ; // not implemented
        confSpaces_dumper& operator=(const confSpaces_dumper &) ; // not implemented
    private:
		std::ofstream& _o;
	};


	/**
	**  TASK DUMPER CLASS
	**/

	friend class task_dumper; // PRQA S 2107
	class task_dumper // PRQA S 2109, 2502
	{

	public:
		explicit task_dumper(const DefaultJobOrder* jo, std::ofstream& oo, bool b);
        task_dumper(const task_dumper &r) : _jo(r._jo), _o(r._o),  _brkEnabled(r._brkEnabled) {} // PRQA S 2528
        ~task_dumper() throw() {}
		void operator() (const Task& task);
	private:
        task_dumper() ; // not implemented
        task_dumper& operator=(const task_dumper &) ; // not implemented
    private:
		const DefaultJobOrder* _jo;
		std::ofstream& _o;
		bool _brkEnabled;
	};


	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultJobOrder) ;
};

_ACS_END_NAMESPACE


#endif // _DefaultJobOrder_H
