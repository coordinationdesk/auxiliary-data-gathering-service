// PRQA S 1050 EOF
//////////////////////////////////////////////////////////////////////////////////////
//
//    Copyright 1995-2016, Advanced Computer Systems , Inc.
//    Via Della Bufalotta, 378 - 00139 Roma - Italy
//    http://www.acsys.it
//
//    All Rights Reserved.
//
//    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
//    the contents of this file may not be disclosed to third parties, copied or
//    duplicated in any form, in whole or in part, without the prior written
//    permission of Advanced Computer Systems, Inc.
//
//    $Prod: A.C.S. DigestTree_base $
//
//    $Id$
//
//    $Author$
//
//    $Log$
//    Revision 5.6  2016/09/29 13:24:37  enrcar
//    EC:: Warning in compilation removed.
//
//    Revision 5.5  2016/03/16 17:02:45  marpas
//    coding best practices applied
//    qa warnings removed
//
//    Revision 5.4  2016/03/08 14:59:13  marpas
//    removing mutex in File class
//    This was originally due to some system call thread safety issues (e.g. readdir)
//    The latter was explicitely changed into readdir_r call
//    Other syscall were checked and up to now seems there is none having thread safety problems
//
//    Revision 5.3  2015/10/14 13:38:20  enrcar
//    EC:: Bug fixed: an ostringstream was used to keep the binary hash, filled with operator<<. It turned out that this resulted in unspecified values if the hash contained a binary 0 (binary 0 is interpreted as end-of-string and the copy of buffer is stopped). Now a C-style buffer is used instead of ostringstream.
//
//    Revision 5.2  2015/10/02 09:44:52  enrcar
//    EC:: when computing the digest of a directory, the permissions of each file it contains are no more enforced by default. A mandatory parameter (check/don't check) must be provided.
//
//    Revision 5.1  2015/09/22 16:11:27  lucio.pulvirenti
//    EC:: LP:: Added support for computing GIT-style Digest (MD5/SHA1) for files and directories
//
//
//    
//////////////////////////////////////////////////////////////////////////////////////

#include <DigestTree_base.h>
#include <cstring>

#include <exException.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <Guard.hpp>
#include <File.h>
#include <Dir.h>

#include <sys/types.h> // PRQA S 1013
#include <fcntl.h>



//*******************************************************************************************************

// TO ACTIVATE THE TROUBLESHOOTING MODE, RUN WITH INSANE DEBUG LEVEL.

//*******************************************************************************************************



using acs::pattern::FDGuard ;

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(DigestTree_base) 

DigestTree_base::DigestTree_base(CRC_Digest_base* digestP): _digestP(digestP)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.") ;
}

DigestTree_base::DigestTree_base(): _digestP(0)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.") ;
}

DigestTree_base::~DigestTree_base() throw()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called.") ; // PRQA S 4631
}

//
//  DigestTree_base::setObject  (THIS IS THE ENTRY POINT OF THE CLASS)
//
void DigestTree_base::setObject(const string& entry_IN, bool check_perm) 	 
{
	// Entry point to set an object and compute the ID
	// Object can be a file or a directory

	ACS_CLASS_WRITE_DEBUG_TH( ACS_LOW_VERB, "Method called. Object: \"" << entry_IN << "\"  check_permissions: " << boolalpha << check_perm ) ;

	ACS_COND_THROW(!(File::exists(entry_IN)), ACS_CRC_DigestTreeError("Object not found: \"" + entry_IN + "\"")) ; // PRQA S 3081
	
	ACS_COND_THROW( (_digestP==NULL), ACS_CRC_DigestTreeError("FATAL: DIGEST IS UNDEFINED (null pointer)") ) ;  // PRQA S 3081

	const string entry (File::readlink(entry_IN)) ; // Resolve symlink(s)

	// Always reset context	
	_digestP->resetContext() ;

	if (File::isDir(entry)) 
	{
		
		getTreeID(entry, check_perm) ;

	} else {
	
		getBlobID(entry) ;
		
	}

	return ; 
}


string DigestTree_base::getObjectID(ACS_CRC_Digest_Retbuf retbuf, unsigned int* s, bool validity) // PRQA S 4211
{
	ACS_COND_THROW( (_digestP==NULL), ACS_CRC_DigestTreeError("FATAL: DIGEST IS UNDEFINED (null pointer)") ) ; // PRQA S 3081

	return _digestP->getResult(retbuf, s, validity) ;
}


string DigestTree_base::getDigestName() // PRQA S 4211
{ 
	ACS_COND_THROW( (_digestP==NULL), ACS_CRC_DigestTreeError("FATAL: DIGEST IS UNDEFINED (null pointer)") ) ; // PRQA S 3081

	return _digestP->getDigestName() + "_ObjectID" ;  // E.G. sha1_ObjectID
} 


void DigestTree_base::hashFile(const string& entry, bool resetContext) // PRQA S 4211
{
	// Just compute the hash of a given file (using the engine in CRC_Digest_base; e.g. MD5, SHA1...)

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. File: \"" << entry << "\"  reset: " << boolalpha << resetContext ) ;

	ACS_COND_THROW(!(File::exists(entry)), ACS_CRC_DigestTreeError("File not found: \"" + entry + "\"")) ; // PRQA S 3081

	ACS_COND_THROW( (0 == _digestP), ACS_CRC_DigestTreeError("FATAL: DIGEST IS UNDEFINED (null pointer)") ) ; // PRQA S 3081
	
	if (resetContext)
	{
		// Reset context	
		_digestP->resetContext() ;
	}

	// Create buffer using digest's expected Size 
	const size_t bufSize ( _digestP->expectedSize() ) ; 
	unsigned char buffer[ bufSize ] ;
	ssize_t r = 0 ; 

	int fd = open (entry.c_str(), O_RDONLY) ; 
	ACS_COND_THROW((fd<0), ACS_CRC_DigestTreeError("Could not open file: \"" + entry + "\"")) ; // PRQA S 3081
	
	{
		FDGuard g(fd) ; // Note: SELF-close(fd)

		off_t file_sz = 0 ;
        off_t iter = 0 ;
		do {

			iter ++ ;	// Count iterations
			r = read(fd, buffer, bufSize) ; 
			int err = errno;
			if (r>0)
			{
				_digestP->addBuffer(buffer, r) ; // PRQA S 3000
				file_sz += r ;

				//ENABLE ONLY IF NEEDED ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "Reading file...  Iteration #" << iter << "  size so far: " << file_sz << " bytes") ; 

			} 
            else if (r<0)
			{
				ostringstream m ;
				m << "DigestTree_base::hashFile. Could not read file: \"" << entry <<  "\"  r: " << r ;

				ACS_THROW(ACS_CRC_DigestTreeError(m.str(), err)) ; // PRQA S 3081
			}
            else { /* nothing */ }

		} while (r>0) ; 

	} // SELF-close(fd)
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished") ;

	return ; 
}


void DigestTree_base::getBlobID(const string& entry)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. File: \"" << entry << "\"" ) ;

	ACS_COND_THROW(!(File::exists(entry)), ACS_CRC_DigestTreeError("Object not found: \"" + entry + "\"")) ; // PRQA S 3081

	ACS_COND_THROW( (0 == _digestP), ACS_CRC_DigestTreeError("FATAL: DIGEST IS UNDEFINED (null pointer)") ) ; // PRQA S 3081

	// According to GIT, the hash of a single file ("blob") is computed as:
	// HASH( HEADER + FILE_STREAM) 
	// Where: HEADER="blob <STREAM_SIZE>\0"
	//
	// Examples:
	// for an empty file, with HASH=SHA1:
	// SHA1( "blob 0\0" ) = e69de29bb2d1d6434b8b29ae775ad8c2e48c5391
	// Other example:
	// SHA1( "blob 14\0Hello, World!\0A") = 8ab686eafeb1f44702738c8b0f24f2567c36da6d
	//
	// for an empty file, with HASH=MD5:
	// MD5( "blob 0\0" ) = 80ffc6eb7286b15afc63f9b86179ccb1
	// Other example:
	// MD5( "blob 14\0Hello, World!\0A") = aa0f4626d41c3b7866ce78401fdb2119
	//

	ostringstream header ;
	const bool header_not_null (true) ; // set to FALSE to omit the header (internal test, debug, etc.)
	
	if (header_not_null) // PRQA S 4090
	{
		const off_t sz (File::size(entry)) ;
		header << "blob " << sz << '\0' ; 
	}
	

	// Always reset context	
	_digestP->resetContext() ;

	if ( not header.str().empty() )
	{
		// Start the digest with the header block
		_digestP->addBuffer( reinterpret_cast<const unsigned char*>(header.str().c_str()), header.str().size() ) ; // PRQA S 3030, 3081
	}

	// Feed the engine (not reset) with the file
	const bool resetContext(false) ; // In order to take the header into account when computing the digest
	hashFile(entry, resetContext) ; 

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished." ) ;
	
	return ; 
}


void DigestTree_base::getTreeID(const string& dir_entry, bool check_perm)
{
	// According to GIT, the hash of a directory ("tree") is computed as:
	// TREE_ID=HASH( HEADER + ID_ENTRY1 + ID_ENTRY2 + ID_ENTRYn ) 
	// Where: HEADER="tree <STREAM_SIZE>\0"
	// ID_ENTRY1 = "pppppp entry1_name\0ID(entry_1)"
	// ID_ENTRY2 = "pppppp entry2_name\0ID(entry_2)"
	// etc.
	// Where pppppp are the octal permissions of the entry, on 6 octal digits (e.g. 100644)
	// ID is the binary sequence (as non-printable bytes, like \x45, not like a printable couple of nibbles, like "4""5")
	// obtained by computing the BLOB_ID (if the entry is a file) or, recursively, the TREE_ID (if the entry is a directory)
	
	//
	// Examples:
	// Given a directory with two empty files: "anotherEmpty" "empty" 
	// with HASH=SHA1:
	// ID(empty) == ID(anotherEmpty) = e69de29bb2d1d6434b8b29ae775ad8c2e48c5391 (see examples above, getBlobID)
	// Stream size: (6+1+5+1+20 + 6+1+12+1+20)=73
	// TREE_ID=SHA1( 
	// "tree 73\x00
	// 100644 anotherEmpty\x00
	// \xe6\x9d\xe2\x9b\xb2\xd1\xd6\x43\x4b\x8b
	// \x29\xae\x77\x5a\xd8\xc2\xe4\x8c\x53\x91
	// 100644 empty\x00
	// \xe6\x9d\xe2\x9b\xb2\xd1\xd6\x43\x4b\x8b
	// \x29\xae\x77\x5a\xd8\xc2\xe4\x8c\x53\x91
	// "
	// = 2b61e34a91ca9780ea2f943e72f1a4a022cdd206

	// Same example, with HASH=MD5
	// ID(empty) == ID(anotherEmpty) = 80ffc6eb7286b15afc63f9b86179ccb1 (see examples above, getBlobID)
	// Stream size: (6+1+5+1+16 + 6+1+12+1+16)=65
	// TREE_ID=MD5( 
	// "tree 65\x00
	// 100644 anotherEmpty\x00
	// \x80\xff\xc6\xeb\x72\x86\xb1\x5a
	// \xfc\x63\xf9\xb8\x61\x79\xcc\xb1
	// 100644 empty\x00
	// \x80\xff\xc6\xeb\x72\x86\xb1\x5a
	// \xfc\x63\xf9\xb8\x61\x79\xcc\xb1
	// "
	// = 2080e3148c1d2d61fd8e1a2e8b55a17d

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called. Directory: \"" << dir_entry << "\"" ) ;

	ACS_COND_THROW(!(File::exists(dir_entry)), ACS_CRC_DigestTreeError("Object not found: \"" + dir_entry + "\"")) ; // PRQA S 3081
	
	ACS_COND_THROW(!(File::isDir(dir_entry)), ACS_CRC_DigestTreeError("A directory was expected: \"" + dir_entry + "\"")) ; // PRQA S 3081

	ACS_COND_THROW( (0 == _digestP), ACS_CRC_DigestTreeError("FATAL: DIGEST IS UNDEFINED (null pointer)") ) ;  // PRQA S 3081

	vector <string> entries ;
	const size_t mode_width(6) ; // 6 octal digits in permissions (enforced with a mask)
	char tmpl[4096] ; // PRQA S 4403
	strcpy(tmpl, "/tmp/hash.XXXXXX") ;

	{
		const int fd ( mkstemp(tmpl) ) ;	// Also, set a valid name inside the templaye tmpl
		ACS_COND_THROW((fd<0), ACS_CRC_DigestTreeError("Could not create temp file.")) ; // PRQA S 3081

		FDGuard g(fd) ; // Note: SELF-close(fd)

		const bool showHidden(true) ; 
		Dir dir(dir_entry, showHidden) ;
		
		size_t entries_size(0) ;	// Compute the total occupation, in bytes, of the payload

		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Creating vector of entries ..." ) ;
		
		for (size_t i=0; i<dir.size(); i++)
		{
			const string entryFullPath = File::getAbsolutePath(  StringUtils::pathJoin(dir_entry, dir[i] ) ); 
			entries.push_back(entryFullPath) ;
			entries_size += ( mode_width +1/*space*/ + (File::getFileName(entryFullPath)).size() + getHashSize() +1/* \0 */ ) ;	// payload occupation // PRQA S 3084
		}	
				
		sort( entries.begin(), entries.end() ) ;	

		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Vector sorted successfully" ) ;
		
		ostringstream tree ; 
		tree << "tree " << entries_size ;
		const size_t tree_size ( tree.str().size() +1 ) ; // PRQA S 3084
		tree << '\0' ; 
		const ssize_t w (write(fd, tree.str().c_str(), tree_size)) ;	// Write the header
		ACS_COND_THROW((w!=static_cast<ssize_t>(tree_size)), ACS_CRC_DigestTreeError("Error writing structure")) ;  // PRQA S 3081
	
	}	// SELF-close(fd)
			
    size_t e_size = entries.size() ;
	for (size_t i=0; i<e_size; ++i)
	{
		// HERE POINT THE THE WORKING DIRECTORY

		unsigned char object_id[EVP_MAX_MD_SIZE] ; 
		memset(object_id, 0x00, EVP_MAX_MD_SIZE) ;

		const string entryFullPath( File::readlink(entries[i], true) );
		// We split the resolution in two and this is working correctly... Should be fixed (investigate readlink when recursive )

		mode_t mode_num	(0x0) ;	// Default value
		if (check_perm) {
			mode_num = ( File::getmod(entryFullPath) & 0x0003ffff ) ;	// This mask allows 18 bits, i.e. 6 octal digits // PRQA S 4400
		}
		setObject( entryFullPath,  check_perm) ;
		_digestP->getResult( object_id ) ; // Compute the object_id

		unsigned char buffer_tree[64*1024] ; // PRQA S 4403 2	
		char hdr[64*1024] ;			
		memset(buffer_tree, 0, sizeof(buffer_tree)) ;	// Do not move, do not change: it must contain all zeros.
		memset(hdr, 0, sizeof(hdr)) ;	// Do not move, do not change: it must contain all zeros.

		const string entryFilename = File::getFileName(entries[i]) ; // entries[i] is correct ! do not replace with entryFullPath
		snprintf(hdr, (sizeof(hdr)-1), "%06o %s", mode_num, entryFilename.c_str()) ;  // PRQA S 3084 2
		const size_t hdr_size ( mode_width +1/*SPACE*/ +entryFilename.size() +1/* BYTE "0" */) ; // One byte more (set to zero) is copied from the header 

		size_t 	tree_size(0) ;
		memcpy((buffer_tree+tree_size), hdr, hdr_size) ; 	// Copy the header, plus one byte (set to zero) // PRQA S 3084 2
		tree_size += hdr_size ; 

		memcpy((buffer_tree+tree_size), object_id, getHashSize()) ;	// Add the payload // PRQA S 3084 2
		tree_size += getHashSize() ;


		{	
			const int fd ( open(tmpl, O_WRONLY|O_APPEND) ) ;
			ACS_COND_THROW((fd<0), ACS_CRC_DigestTreeError("Could not update temp file.")) ; // PRQA S 3081

			FDGuard g(fd) ; // Note: SELF-close(fd)
			const ssize_t w (write(fd, buffer_tree, tree_size)) ; 
			ACS_COND_THROW((w!=static_cast<ssize_t>(tree_size)), ACS_CRC_DigestTreeError("Error writing structure")) ; // PRQA S 3081

		}	// SELF-close(fd)

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg ;
			msg << "Entry: \"" << entryFullPath << "\"  Hash: { " << bufferDump(object_id, EVP_MAX_MD_SIZE) << " }" ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str()) ;
		ACS_CLASS_END_DEBUG
	}

	// Compute the hash for the whole tree
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Computing the hash for temporary file: \"" << tmpl << "\"" ) ;
	
	const bool resetContext(true) ; // In order to take the header into account when computing the digest
	hashFile(tmpl, resetContext) ; 

	bool removeTempFile(true) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		removeTempFile = false ; 
		ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "INSANE VERBOSITY: TROUBLESHOOTING MODE. To improve the debug, the temporary file: \"" 
				<< tmpl << "\" won't be removed.") ;
	ACS_CLASS_END_DEBUG

	if (removeTempFile)
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Removing temporary file: \"" << tmpl << "\"" ) ;
		::unlink(tmpl) ; 
	} else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "TROUBLESHOOTING MODE. Keeping temporary file: \"" << tmpl << "\"" ) ;
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished." ) ;
}


string DigestTree_base::bufferDump(unsigned char* p, const size_t s) 
{
	ostringstream ret ; ret << "size: " << s << " ; dump:";
	char z[3]; memset(z, 0, sizeof(z)) ; // PRQA S 4403
	for (size_t i=0; i<s; ++i) { snprintf(z, 3, "%02x", p[i]) ; ret << " " << z ; } // PRQA S 4400
	ret << " ;"  ;
	return ret.str() ;
}

_ACS_END_NAMESPACE
 
