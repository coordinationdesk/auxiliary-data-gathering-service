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

 $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author$

 $Log$
 Revision 5.8  2017/02/21 18:08:38  nicvac
 S3EVOL-13: Datapackage serialization.

 Revision 5.7  2017/02/21 17:02:44  nicvac
 S3EVOL-13: Inter process cache mode

 Revision 5.6  2017/02/02 09:49:20  davide.tiriticco
 S2PDGS-1655: packageId type changed from string to dbPersistent::IdType

 Revision 5.5  2015/08/20 16:18:49  nicvac
 ALSAT1B-44: cart handling

 Revision 5.4  2014/01/30 13:44:29  francesco.avanzi
 coding best practices applied & new db::Transaction used

 Revision 5.3  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.2  2013/07/18 12:28:23  marpas
 qa warnings

 Revision 5.1  2013/07/15 16:46:19  marpas
 qa warnings fixed

 Revision 5.0  2013/06/06 18:51:19  marpas
 adopting libException 5.x standards

 Revision 1.15  2013/05/17 15:33:01  nicvac
 S2PDGS-300: priority handling.

 Revision 1.14  2012/12/12 16:12:12  nicvac
 minor

 Revision 1.13  2012/11/20 18:04:18  nicvac
 private to protect to allow serialization.

 Revision 1.12  2012/10/19 15:04:06  micmaz
 updated library

 Revision 1.11  2012/10/08 13:59:19  micmaz
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.10  2012/09/19 15:57:11  micmaz
 ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.9  2012/09/14 16:31:32  micmaz
 S2GPP-611
 Development of Distribution Formatter framework for Export

 Revision 1.7  2012/09/11 16:42:31  micmaz
 removed compiling warning

 Revision 1.6  2011/03/11 11:18:02  crivig
 setduedate method added

 Revision 1.5  2011/03/01 17:29:52  crivig
 getCartInfo() made constant.

 Revision 1.4  2011/02/25 14:53:05  crivig
 bug on cartId fixed

 Revision 1.3  2010/12/17 10:22:37  crivig
 getInvFilesFileTypes overloaded

 Revision 1.2  2010/12/03 15:00:49  crivig
 cache cleaning handled with Cache class

 Revision 1.1  2010/11/29 11:06:49  crivig
 added new class

 */

#ifndef _DataPackage_H_
#define _DataPackage_H_

#include <exException.h>
#include <DateTime.h>
#include <dbConnection.h>
#include <dbPersistent.h>

#ifndef PRQA_ANALYZER
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/utility.hpp>
#endif // PRQA_ANALYZER


_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)

class DataPackage { // PRQA S 2109
public:

	exDECLARE_EXCEPTION(DataPackageException, exException); // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(DataPackageSerializationException, exException); // PRQA S 2131, 2502

	explicit DataPackage(const dbPersistent::IdType&);
	DataPackage();
	DataPackage(DataPackage const& rhl);
	DataPackage& operator=(DataPackage const& rhl);
	virtual ~DataPackage() throw ();

	static int const NullCartId;

	const dbPersistent::IdType& getPackageId() const throw() ;

	const DateTime& getDueDate() const throw() ;
	void setDueDate(DateTime& time);

	int getPriority() const throw() ;
	void setPriority(int priority) throw() ;

	const std::string& getFormat() const throw() ;

	void setFileType(const std::string&);
	const std::string& getFileType() const throw() ;

	const std::string& getUserId() const throw() ;
	int getCartId() const throw() ;
	void getCartInfo(rsResourceSet &) const;

	void addCacheFile(const std::string &);
	void getCacheFiles(std::vector<std::string> &) const ;
	bool fillPackage(dbConnection &);
	/*
	 *\brief get size of the whole files of DataPackage
	 */
	unsigned long long getPackageSize() const throw() ;
	/*
	 *\brief It returns the num of inventory file of the DataPackage
	 */
	size_t getNumInventoryFiles() const throw() ;
	/*
	 *\brief get next inventory file
	 *\warning Before looping the iterator must be moved to the begin
	 *\warning it returns exBoundsException if the user tries to go over the boundery
	 */
	void getInvFiles(std::vector<std::string>& invFiles);

	void getInvFilesFileTypes(std::vector<std::string>* fileTypes);
	void getInvFilesFileTypes(std::list<std::string>* fileTypes);
	/*
	 *\brief It compares only the DataPackageid
	 */
	bool operator==(const DataPackage&) const; // PRQA S 2070
	std::string const str() const ;

private:
	void loadCartsref(dbConnection & conn);

protected:
 // PRQA S 2101 L1
	std::map<std::string, std::string> _inventories;
	dbPersistent::IdType _packageId;
	
	std::string _userId;
	std::string _format;
	std::string _fileType;
	DateTime _dueDate;
	int _priority;
	unsigned int _maxConnRetry;
	unsigned long long _size;
	std::vector<std::string> _cacheFiles;
	int _cartId;
	rsResourceSet _rset;
 // PRQA L:L1

private:

//----- SERIALIZATION -----
#ifndef PRQA_ANALYZER
	friend class boost::serialization::access;

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {

		ar &   BOOST_SERIALIZATION_NVP(_inventories) & BOOST_SERIALIZATION_NVP(_packageId 	) &
			   BOOST_SERIALIZATION_NVP(_userId 	   ) & BOOST_SERIALIZATION_NVP(_format 		) &
               BOOST_SERIALIZATION_NVP(_fileType   ) & BOOST_SERIALIZATION_NVP(_maxConnRetry) &
               BOOST_SERIALIZATION_NVP(_size	   ) & BOOST_SERIALIZATION_NVP(_cacheFiles 	) &
               BOOST_SERIALIZATION_NVP(_cartId     );

		std::ostringstream osRset; osRset<<_rset; std::string cartInfoRset( osRset.str() );
		long double dueDateJd50( _dueDate.jd50() );

		ar & BOOST_SERIALIZATION_NVP(dueDateJd50) & BOOST_SERIALIZATION_NVP( _priority ) &
			 BOOST_SERIALIZATION_NVP(cartInfoRset);
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version)	{

		ar &   BOOST_SERIALIZATION_NVP(_inventories) & BOOST_SERIALIZATION_NVP(_packageId 	) &
			   BOOST_SERIALIZATION_NVP(_userId 	   ) & BOOST_SERIALIZATION_NVP(_format 		) &
               BOOST_SERIALIZATION_NVP(_fileType   ) & BOOST_SERIALIZATION_NVP(_maxConnRetry) &
               BOOST_SERIALIZATION_NVP(_size	   ) & BOOST_SERIALIZATION_NVP(_cacheFiles 	) &
               BOOST_SERIALIZATION_NVP(_cartId     );

		long double dueDateJd50;
		std::string cartInfoRset("");

		ar & BOOST_SERIALIZATION_NVP(dueDateJd50) & BOOST_SERIALIZATION_NVP( _priority ) &
			 BOOST_SERIALIZATION_NVP(cartInfoRset);

		_dueDate = DateTime::JD50( dueDateJd50 );
		std::istringstream isRset; isRset.str(cartInfoRset);
		isRset>>_rset;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif // PRQA_ANALYZER

public:
	/** Dump the Object */
	void dump(std::string const& filename) const;
	void dump(std::ostream& oss) const;

	/** Load the Object */
	void load(std::string const& filename);
	void load(std::istream& iss);
//----- ----- ----- -----

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DataPackage) ;
};

_ACS_END_NESTED_NAMESPACE

#endif // _DataPackage_H_
