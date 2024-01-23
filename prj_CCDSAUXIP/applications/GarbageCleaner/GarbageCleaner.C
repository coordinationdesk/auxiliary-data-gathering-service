// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, SpA

	$Prod: GarbageCleaner $
	
	$Log$
	Revision 1.16  2017/11/15 12:31:58  chicas
	some message to PRIVINFO
	
	Revision 1.15  2016/12/16 12:28:59  francesco.avanzi
	now handles a MaxSize
	
	Revision 1.14  2016/03/03 16:00:22  marpas
	regression fixed on get value
	
	Revision 1.13  2016/01/29 11:47:05  marpas
	coding best practices application in progress
	
	Revision 1.12  2015/07/08 09:59:26  damdec
	Debug Log macros updated to fix compilation warnings.
	
	Revision 1.11  2014/05/26 09:16:42  marfav
	Removing time zone when computing dates as per S3PDGS-1908
	
	Revision 1.10  2013/05/22 14:35:21  marco.mancini
	Implemented logging of removed files by the means of the optional
	boolean LogAct tag into configuration file.
	
	Revision 1.9  2012/07/04 15:15:10  giapas
	RegExp set fixed.
	
	Revision 1.8  2006/06/14 10:37:35  davcas
	Added message for debugging
	
	Revision 1.7  2005/10/03 13:23:31  clanas
	message on input time to remove
	
	Revision 1.6  2005/10/03 13:00:36  clanas
	substituted excout with excerr
	
	Revision 1.5  2005/09/28 10:12:40  marpas
	file/dir remotion now uses File::remove
	includes reorganized
	namespaces too
	
	Revision 1.4  2005/09/07 08:14:27  marpas
	Versioning fixed.
	

*/


#include "GarbageCleaner.h"

#include <File.h>
#include <RegExp.h>
#include <DateTime.h>

#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>

#include <sys/stat.h>
#include <dirent.h>
#include <Dir.h>
#include <list>
#include <algorithm>
using namespace acs;
using namespace std;

namespace {
	class check_datetime { // PRQA S 2109
	public:
		check_datetime(std::string const& d, DateTime const& dt ) : 
			dir(d) , datetime(dt.Datetime2time_t()) {} // PRQA S 2528

		check_datetime(const check_datetime &r ) : dir(r.dir), datetime(r.datetime) {} // PRQA S 2528
		~check_datetime() throw() {}
		bool operator()(dirent* d) const // PRQA S 4020
		{
			if (d && d->d_name ) {
				struct stat s; // PRQA S 4102
				string path = dir + "/" + d->d_name;
				if(0 == stat( path.c_str(), &s ) )
				{
					return s.st_mtime < datetime;
				}
				ACS_LOG_ERROR("Error in stat of \"" << path << "\"") ;
			}
			return false;
		}
	private:
		check_datetime() ; // not implemented
		check_datetime& operator=(const check_datetime &) ; // not implemented

	private:
		std::string const& dir;
		time_t const datetime;
	};
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(GarbageCleaner)

GarbageCleaner::GarbageCleaner () 
{
}

GarbageCleaner::~GarbageCleaner()
{
}

void GarbageCleaner::init ()
{
}

// exec cmd and return its stdout as string
string GarbageCleaner::sopen(string const& cmd) {
	ostringstream oss;
	FILE* fp = popen( cmd.c_str(), "r"); // PRQA S 4412
	size_t rs = 0 ;
	do {
		char buf[512]; // PRQA S 4403
		rs = fread(buf, sizeof(buf[0]), sizeof(buf)-1, fp); // PRQA S 3084
		if (rs > 0) {
			buf[rs-1] = 0; // PRQA S 3084
			oss << buf;
		}
	} while (0 != rs);
	pclose(fp);
	return oss.str();
}


namespace {
	
// aux function replacing boost functional

 bool domatch (const string& dir, RegExp const& regexp, DateTime const& datetime, dirent* entry)
 {

/**
		using boost::function;
		using namespace boost::lambda;
		function< bool(dirent*) > match;
		if (!regexp.getReg().empty()) {
			match = bind( static_cast<bool(RegExp::*)(const string&)const>(&RegExp::match) // PRQA S 3081
				, &regexp
				, _1 ->* &dirent::d_name
			); // regexp.match( for every dirent->d_name )
		}
		if ( datetime.isFilled()) {
			function< bool(dirent*) > g = check_datetime( dir, datetime);
			if (!match.empty()) {
				match = bind(match, _1) && bind(g, _1);
			}
			else {
				match = bind(g, _1);
			}
		}
 **/ 
 
	// Default is true becaus match.empty means no regexp and no datetime
	bool retval = true;
	if ( ! regexp.getReg().empty() ) 
	{
		retval = regexp.match (entry->d_name);
	}

	if ( datetime.isFilled()) 
	{
		check_datetime dateTimeChecker = check_datetime( dir, datetime);
		retval = retval && dateTimeChecker (entry);
	}

	return retval;
}

}; // End of unnamed namespace

void GarbageCleaner::remove(string const& dir, RegExp const& regexp, DateTime const& datetime, bool const& logAct,
                            const bool logDir, const bool deleteNonEmptyDir)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "\tdir: \"" << dir << "\"\n" // PRQA S 3380, 3385
			   << "\tregexp: " << (regexp.getReg().empty() ? "empty" : ('"' + regexp.getReg() + '"') ) << '\n'
			   << "\tdatetime: " << (datetime.isFilled() ? ('"' + datetime.asISO8601(0) + '"') : "empty") << '\n'
			   << "\tlogDir: " << boolalpha << logDir << '\n'
			   << "\tdeleteNonEmptyDir: " << boolalpha << deleteNonEmptyDir << '\n') ;


	DIR* ddir = opendir(dir.c_str()) ;
	if ( 0 == ddir  ) {
		ACS_LOG_ERROR("Error opening directory \"" << dir << "\": \"" << strerror(errno) << "\"") ;
	}
	else {
		dirent* d = 0;
		
		while ( (d = readdir(ddir)) ) {
			if ( (d->d_name[0] == '.') 
				&& ( (0 == d->d_name[1])
					|| ((d->d_name[1] == '.') && (0 == d->d_name[2]))) // PRQA S 4400
			   ) {
				continue; // skip `.' and `..' entries
			}
//			if (match.empty() || match( d ) ) {
			if ( domatch (dir, regexp, datetime, d) ) {
				string path = dir + "/" + d->d_name;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path: \"" << path << "\"");
				
				try {
					if (logDir && File::isDir(path)) {
						if (Dir::empty(path)) {
							File::remove(path) ;
							ACS_LOG_INFO("Removed empty directory: \"" << path << "\"");
						}
						else {
							if (deleteNonEmptyDir) {
								File::remove(path) ;
								ACS_LOG_INFO("Removed non-empty directory: \"" << path << "\"");
							}
							else {
								ACS_LOG_WARNING("Skipped non-empty directory: \"" << path << "\"");
							}
						}
					}
					else
					{
						File::remove(path) ;
						if(logAct) {
							ACS_LOG_INFO("Removed file: \"" << path << "\"");
						}
					}
				}
				catch(exception &e) {
					ACS_LOG_ERROR("Error removing file: \"" << path << "\"");
					ACS_LOG_NOTIFY_EX(e) ;
				}
			}
		}
		closedir(ddir);
	}
}

int GarbageCleaner::run ()
{
	rsResourceSet &rs = ConfigurationSingleton::instance() ->get();
	rsPushNSpace Garbage(rs, "GarbageCleaner");

	// reference time (t0) as string for date cmd input later
	string const t0 = sopen("LC_ALL=C date -u +\"%Y%m%d %H:%M:%S\" --date=\"now\"");

	size_t s = rs.getArraySize("Section"); // PRQA S 4412
	ACS_LOG_PRIVINFO("Handling " << s << " sections") ;
	
	// for each Section
	for (size_t i = 0; i < s; ++i)
	{
		try {
			string dir;
			RegExp regexp;
			DateTime datetime;

			ostringstream section ;
			section << "Section[" << i << "]" ;
			ACS_LOG_PRIVINFO(section.str()) ;

			rsPushNSpace Section(rs, section.str());
			rs.getValue("Dir", dir);
			if( ! File::exists(dir) )
			{
				ACS_LOG_WARNING("Unable to find folder '" << dir << "'")
				continue ;
			}
			string Remark;
			rs.getValue("Remark.value", Remark,false);
			ACS_LOG_PRIVINFO((Remark.empty() ? "empty" : ('"' + Remark + '"') )) ; // PRQA S 3380, 3385

			try {
				string regexp_s;
				rs.getValue("RegExp", regexp_s);
				regexp.setReg(regexp_s);
			} catch (rsResourceSet::NotFoundException& e) {
				ACS_LOG_PRIVINFO("RegExp not found. Removing all") ;
			}

			try {
				string datetime_s;
				datetime.reset();
				rs.getValue("Time", datetime_s);
				ACS_LOG_PRIVINFO("input datetime: " << datetime_s) ;
				ostringstream date_oss;
				// S3PDGS-1908 removing time zone from result
				date_oss << "LC_ALL=C date -u +\"%Y-%m-%dT%H:%M:%S\" --date=\""
		   			<< datetime_s << " " << t0
		  			<< "\"";
				datetime = sopen( date_oss.str() );
			} catch (rsResourceSet::NotFoundException& e) {
				ACS_LOG_PRIVINFO("Time not found. Removing all ages") ;
			} catch (exception& e) {
				ACS_LOG_NOTIFY_EX(e);
				continue;
			}
			
			off_t maxSize ;
			if( rs.hasKey("MaxSize") )
			{
				rs.getValue("MaxSize", maxSize) ;
			}
			else
			{
				maxSize = 0 ;
				ACS_LOG_PRIVINFO("MaxSize not found") ;
			}
			
			bool logAct = false ;
			try {			
				rs.getValue("LogAct",logAct);
				ACS_LOG_PRIVINFO("Log removed files: " << (logAct ? "yes" : "no")) ; // PRQA S 3380
			} catch (rsResourceSet::NotFoundException& e) {
				ACS_LOG_PRIVINFO("Log activity flag not found. No activity will be logged") ;
				logAct = false;
			}

			if(maxSize == 0 )
			{
				bool logDir = false ;
				bool deleteNonEmptyDir = true ;
				if( rs.hasKey("LogDir") )
				{
					rs.getValue("LogDir", logDir) ;

					if (logDir)
					{
						if( rs.hasKey("DeleteNonEmptyDir") )
						{
							rs.getValue("DeleteNonEmptyDir", deleteNonEmptyDir) ;
						}
						else
						{
							ACS_LOG_PRIVINFO("DeleteNonEmptyDir not found") ;
						}
					}
				}
				else
				{
					ACS_LOG_PRIVINFO("LogDir not found") ;
				}

				if (!deleteNonEmptyDir)
				{
					ACS_LOG_INFO("Non-empty directories will not be removed") ;
				}

				remove(dir, regexp, datetime, logAct, logDir, deleteNonEmptyDir);
			}
			else
			{
				removeIfOversize(maxSize, dir) ;
			}
		} catch (rsResourceSet::NotFoundException& e) {
			// mandatory
			ACS_LOG_NOTIFY_EX(e);
		}
	}
	
	return 0;
}
namespace 
{
	class FileEntry
	{
	public:
		FileEntry(const string &path, const string &n):name_(path + "/" + n), size_(), date_()
		{
			struct stat sS ;
			File::fileStat( name_, sS ) ;
			size_ = File::size(name_) ;
			date_ = sS.st_mtime ;
		}
		~FileEntry()throw() {}
		FileEntry(const FileEntry &f):name_(f.name_), size_(f.size_), date_(f.date_) {}
		FileEntry& operator=(const FileEntry &f)
		{
			if(this != &f)
			{
				name_ = f.name_ ;
				size_ = f.size_ ;
				date_ = f.date_ ;
			}
			return *this ;
		}
	
		string name_ ;
		off_t size_ ;
		time_t date_ ;
	};

	bool compare(const FileEntry &a , const FileEntry &b )
	{
		return ( a.date_ > b.date_ );
	}
}

void GarbageCleaner::removeIfOversize( off_t maxSize, const string &folder) 
{
	off_t fullSize =  File::size(folder) ;
	if( maxSize == 0 || fullSize < maxSize ) 
	{
		return ;
	}

	Dir dir(folder) ;
	
	list<FileEntry> fe ;
	size_t s = dir.size() ;
	
	for( size_t i = 0 ; i < s ; ++i )
	{
		fe.push_back( FileEntry( folder , dir[i] ) );
	}
	fe.sort( compare );
	
	while( fullSize > maxSize )
	{
		fullSize -= fe.back().size_ ;
		File::remove( fe.back().name_ );
		fe.pop_back() ;
	}
}
