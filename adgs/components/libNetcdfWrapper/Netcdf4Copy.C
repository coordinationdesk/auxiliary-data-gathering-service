// PRQA S 1050 EOF
/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libNetcdfWrapper$

	$Id$

	$Author$

	$Log$
	Revision 1.21  2017/05/23 13:31:54  enrcar
	EC:: copyVars: filler handled
	
	Revision 1.20  2017/05/18 12:05:51  enrcar
	EC:: copyVars i/f improved in order to optionally handle a correction factor (in progress)
	
	Revision 1.19  2016/07/15 15:06:36  enrcar
	EC:: S3PDGS-2901 copyVars: I/F added
	
	Revision 1.18  2016/03/10 10:36:25  enrcar
	EC:: Warning removed since too verbose
	
	Revision 1.17  2016/03/08 15:46:08  enrcar
	EC:: missing "const" attribute
	
	Revision 1.16  2016/03/01 13:42:07  enrcar
	S3PDGS-2797  EC:: Added method copyAtts to handle a vector of pre-filled attributes;  A specific method copyFile was added to handle this.
	
	Revision 1.15  2015/06/18 08:50:37  enrcar
	EC:: Debug slightly improved
	
	Revision 1.14  2014/10/21 13:23:24  enrcar
	EC:: Added method: copyFile (to copy/compress a NetCDF file)
	
	Revision 1.13  2014/08/01 12:37:45  enrcar
	EC:: concatVars interface changed: row-index should be provided from outside
	
	Revision 1.12  2014/07/18 08:05:06  enrcar
	EC:: scope reduced
	
	Revision 1.11  2014/07/15 15:41:48  enrcar
	EC:: Compression cannot be applied to dimension-less (empty) variables. Fixed.
	
	Revision 1.10  2014/06/19 14:32:31  enrcar
	EC:: improvment of messages debug and exceptions
	
	Revision 1.9  2014/05/14 13:50:50  ricfer
	Removing compiling warning due to new ACS logging handling.
	
	Revision 1.8  2014/01/16 17:07:25  enrcar
	EC:: Added method concatenateVars(first, second, output)
	
	Revision 1.7  2014/01/10 11:16:16  enrcar
	EC:: All methods copyVars were modified in order to handle the compression
	
	Revision 1.6  2013/11/20 16:36:03  enrcar
	EC:: Interface of the method copyVars updated in order to copy an array accordingly to a pair of index and to the dimension the index belongs to.
	
	Revision 1.5  2013/05/08 10:29:16  enrcar
	EC:: Methods added: copyDimsWithExclusions, copyVarsWithExclusions
	
	Revision 1.4  2013/04/19 15:30:40  enrcar
	EC:: copyVars overload interface added
	
	Revision 1.3  2012/05/23 14:23:36  damdec
	Compilation warnings fixed.
	
	Revision 1.2  2011/04/18 12:41:01  marant
	Overloaded method copyVars() added to allow variables' compression
	
	Revision 1.1  2011/03/02 11:05:16  marant
	Usage of netcdf-4.1.1 library with netCDF-4 support enabled with C++ interface provided by cxx4 APIs
	
        
*/ 

#include <Netcdf4Copy.h>
#include <map>
#include <algorithm>	 
#include <Filterables.h>
#include <File.h>
#include <RegExp.h>
#include <Guard.hpp>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(Netcdf4Copy) ;



Netcdf4Copy::Netcdf4Copy() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy::"<< __LINE__ <<" Id: "<< Id) ;
}

Netcdf4Copy::~Netcdf4Copy() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy: Destructor called.") ;
}

void Netcdf4Copy::copyDims(const Netcdf4DataFile& src, Netcdf4DataFile& dest)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyDims() called.") ;

	 try
	 {
    	multimap<string,NcDim> dims = src.getDims();

    	for (multimap<string,NcDim>::iterator it = dims.begin(); it!=dims.end(); it++) {
        	dest.addDim(it->second.getName(), it->second.getSize());

        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyDims(): dimension <" << it->second.getName() << "> added to destination file.") ;
    	}
	  }
	  catch(exceptions::NcException& ex)
	  {
    	  ACS_CLASS_BGN_DEBUG(10)
			ex.what();
    	  ACS_CLASS_END_DEBUG
		  throwException(exNetcdf4CopyException("Error occurred while copying netcdf dimensions from file " + src.getFilename()));
	  }

}


void Netcdf4Copy::copyDimsWithExclusions(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const vector<string>& exclude_dims)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyDims() called. Size of exclude_dims: " << exclude_dims.size()) ;
    ACS_CLASS_BGN_DEBUG(20)
		ostringstream os ;
        os << "Netcdf4Copy:copyDims()  Exclusions:" ;
		for (size_t k=0; k<exclude_dims.size(); k++) os << " \"" << exclude_dims[k] << "\"" ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, os.str()) ;
    ACS_CLASS_END_DEBUG

	 try
	 {
    	multimap<string,NcDim> dims = src.getDims();

    	for (multimap<string,NcDim>::iterator it = dims.begin(); it!=dims.end(); it++) {
		
			if ( find(exclude_dims.begin(), exclude_dims.end(), it->second.getName()) == exclude_dims.end() )
			{		
        		dest.addDim(it->second.getName(), it->second.getSize());

        		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyDims(): dimension <" << it->second.getName() << "> added to destination file.") ;
			}
    	}
	  }
	  catch(exceptions::NcException& ex)
	  {
    	  ACS_CLASS_BGN_DEBUG(10)
			ex.what();
    	  ACS_CLASS_END_DEBUG
    	  throwException(exNetcdf4CopyException("Error occurred while copying netcdf dimensions from file " + src.getFilename()));
	  }

}


void Netcdf4Copy::copyAtts(const Netcdf4DataFile& src, Netcdf4DataFile& dest)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts() called.") ;
    
  try
  {
        multimap<string,NcGroupAtt> atts = src.getAtts();
    
        for (multimap<string,NcGroupAtt>::iterator it = atts.begin(); it!=atts.end(); it++) {
            
            dest.addAttr(it->second);
            
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts(): attribute <" << it->second.getName() << "> added to destination file.") ;
    
        }
  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf attributes from file " + src.getFilename()));
  }


}


void Netcdf4Copy::copyAtts(	const Netcdf4DataFile& src, 	// Source file
							Netcdf4DataFile& dest, 			// Destination file
							const vector <NetcdfAttribute> &atts_to_process) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts() called.  Size of atts_to_process: " << atts_to_process.size()) ;
    
  try
  {
        map<string, NetcdfAttribute> atts_to_merge_force ;	// Attributes to merge, even if not in input file
	    map<string, NetcdfAttribute> atts_to_merge_skip ;	// Attributes to merge, but only if already in input file (o/w: skip)
        map<string, NetcdfAttribute> atts_to_replace_force ;// Attributes to replace, even if not in input file
	    map<string, NetcdfAttribute> atts_to_replace_skip ;	// Attributes to replace, but only if already in input file (o/w: skip)

	
		// Turn the input from vector (easier to create) to a four maps (easier to handle and to remove items from)
		for (size_t i=0; i<atts_to_process.size(); i++)
		{
			const NetcdfAttribute att ( atts_to_process[i] ) ;
			const bool merge(att.getMerge()) ; 
			const bool force(att.getForce()) ;

			if		( (merge == true)	&& (force == true) )	{ atts_to_merge_force.insert(make_pair(att.getName(), att)) ;	}
			else if	( (merge == true)	&& (force == false) )	{ atts_to_merge_skip.insert(make_pair(att.getName(), att)) ;	}
			else if	( (merge == false)	&& (force == true) )	{ atts_to_replace_force.insert(make_pair(att.getName(), att)) ;	}
			else if	( (merge == false)	&& (force == false) )	{ atts_to_replace_skip.insert(make_pair(att.getName(), att)) ;	}
		}

			
		// Process all the attributes in the input file (they must be ALL copied to the output).
		// If the attribute is found in one of the map, it will be replaced or merged
		// If not found: it will be just added with the value read from input file	
        const multimap<string, NcGroupAtt> atts ( src.getAtts() ) ;
        for (multimap<string,NcGroupAtt>::const_iterator it = atts.begin(); it!=atts.end(); it++) {
            
			map<string, NetcdfAttribute>::iterator f ;
			NetcdfAttribute attr( (it->second).getName() ) ; // Initialize the attribute with the name and no payload
			bool merged(false), replaced(false) ;
				
			if ( (f = atts_to_replace_force.find(it->first)) != atts_to_replace_force.end() )
			{
				attr = f->second ; // Attribute replaced since found in: atts_to_replace_force
				replaced = true ; atts_to_replace_force.erase(f) ;
			} 
			
			if ( (f = atts_to_replace_skip.find(it->first)) != atts_to_replace_skip.end() )
			{
				attr = f->second ; // Attribute replaced since found in: atts_to_replace_skip
				replaced = true ; atts_to_replace_skip.erase(f) ;
			}  
		
			if ( (f = atts_to_merge_force.find(it->first)) != atts_to_merge_force.end() )
			{
				attr.set(it->second, f->second.getUnique()) ; attr.add(f->second) ;	// Merge
				merged = true ; atts_to_merge_force.erase(f) ;
			} 
			
			if ( (f = atts_to_merge_skip.find(it->first)) != atts_to_merge_skip.end() )
			{
				attr.set(it->second, f->second.getUnique()) ; 
				attr.add(f->second) ;	// Merge
				merged = true ; atts_to_merge_skip.erase(f) ;
			}  
			
						
			if (merged || replaced)
			{
            	dest.addAttr(attr);	// Store the attribute. It can be either replaced or merged. 

           		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts(): attribute <" << attr.getName() 
					<< "> replaced/merged to destination file.  Replaced? " << (replaced?"T":"F") << "  Merged? " << (merged?"T":"F")) ;

            } else {
            	dest.addAttr(it->second);	// Copy the original attribute.
			
            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts(): attribute <" << it->second.getName() 
					<< "> just copied to destination file.") ;
			}
    	}


		// WHEN HERE: All the attributes present the input file has been added to the output file (possibly: replaced or merged, in provided in some vectors).
		
		// Now, the REMAING, NOT EXISTING attributes (provided in the input vector with attribute: FORCE IF NOT EXISTING) will be added to the output.
		
		// Attributes still in atts_to_replace_force:
		for (map<string,NetcdfAttribute>::iterator it = atts_to_replace_force.begin(); it!=atts_to_replace_force.end(); it++) {

            dest.addAttr(it->second);	

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts(): new attribute <" << it->second.getName() 
				<< "> added to destination file since provided in atts_to_replace_force") ;
		}
		
		// Attributes still in atts_to_merge_force:
		for (map<string,NetcdfAttribute>::iterator it = atts_to_merge_force.begin(); it!=atts_to_merge_force.end(); it++) {

            dest.addAttr(it->second);	

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts(): new attribute <" << it->second.getName() 
				<< "> added to destination file since provided in atts_to_merge_force") ;
		}
		
		
		// LAST STEP: A warning for the attributes still remaining in the SKIP IF NOT EXISTING vectors will be issued (if it occurs)
		for (map<string,NetcdfAttribute>::iterator it = atts_to_replace_skip.begin(); it!=atts_to_replace_skip.end(); it++) {
			ostringstream msg ; msg << "Netcdf4Copy:copyAtts(). " << __LINE__ << "  Attribute: <" << it->second.getName() 
									<< ">  could not be replaced on destination since not present in the source file" ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str()) ;
			// ACS_LOG_WARNING(msg.str()) ;
		}

		for (map<string,NetcdfAttribute>::iterator it = atts_to_merge_skip.begin(); it!=atts_to_merge_skip.end(); it++) {
			ostringstream msg ; msg << "Netcdf4Copy:copyAtts(). " << __LINE__ << "  Attribute: <" << it->second.getName() 
									<< ">  could not be merged on destination since not present in the source file" ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str()) ;
			// ACS_LOG_WARNING(msg.str()) ;
		}

  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf attributes from file " + src.getFilename()));
  }


}


void Netcdf4Copy::concatVars(const NcVar& first_var, const NcVar& second_var, NcVar& out_var, const size_t& main_index)
{	
	/** Concatenate the first variable at the end of the second one */
		
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream os ; os << "Netcdf4Copy:concatVars() called with main_index: " << main_index ;
		ACS_LOG_DEBUG( os.str() ) ;
	ACS_CLASS_END_DEBUG

	string first_var_name ("UNDEF"), second_var_name ("UNDEF"), out_var_name ("UNDEF") ;

	try {

    	throwConditionedException( (first_var.getDimCount() != second_var.getDimCount()), \
			exNetcdf4CopyCriticalException("Cannot concatenate two variables with different number of dimensions !")) ;

  		throwConditionedException( (first_var.getDimCount() != out_var.getDimCount()), \
			exNetcdf4CopyCriticalException("Destination (output) variable should have the same number of dimensions !")) ;

		const size_t array_dim_size = first_var.getDimCount() ;    // The same for all the variables
		
    	throwConditionedException( (array_dim_size < 1), exNetcdf4CopyCriticalException("Invalid number of dimensions: <1")) ;
    	throwConditionedException( (main_index >= array_dim_size), exNetcdf4CopyCriticalException("Main index exceedes the number of dimensions")) ;

		first_var_name = first_var.getName() ; second_var_name = second_var.getName() ; out_var_name = out_var.getName() ;
	
		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			{
			 ostringstream os ;
			 os << "First Input variable: \"" << first_var_name << "\" has sizes: (" ;
			 for (size_t i=0; i<array_dim_size; i++) {
				 os << (i?" x ":"") << first_var.getDim(i).getSize() << " " ; } os << ")" ;					
			 ACS_LOG_DEBUG( os.str() ) ;
			}
			{ 
			 ostringstream os ;
			 os << "Second Input variable: \"" << second_var_name << "\" has sizes: (" ;
			 for (size_t i=0; i<array_dim_size; i++) {
				 os << (i?" x ":"") << second_var.getDim(i).getSize() << " " ; } os << ")" ;
			 ACS_LOG_DEBUG( os.str() ) ;
			}
			{ 
			 ostringstream os ;
			 os << "Output variable: \"" << out_var_name << "\" has sizes: (" ;
			 for (size_t i=0; i<array_dim_size; i++) {
				 os << (i?" x ":"") << out_var.getDim(i).getSize() << " " ; } os << ")" ;
			 ACS_LOG_DEBUG( os.str() ) ;
			} 
		ACS_CLASS_END_DEBUG



		/* Allocate the vectors for handling all dimensions: */
		vector<size_t> read_pos(array_dim_size), write_pos(array_dim_size), counts(array_dim_size) ; 

		/* Maximum memory size (for the malloc) */
		const size_t maximum_buffer_size = 16*1024*1024 ; // 16 MB

		/* We have to compute the elements to process in each chunk. The memory for each chunk should not exceed maximum_buffer_size: */

		// Compute the size for all the dimensions of the vector, except the selected one (which is the unknown in the equation)
		size_t first_buffer_size(1) ;
		{	
			size_t second_buffer_size(1), out_buffer_size(1) ;	
			for (size_t i=0; (i<array_dim_size); i++) {
				if (i!=main_index) {
					first_buffer_size *= first_var.getDim(i).getSize() ;	// Number of elements for all the dimensions but the selected one
					second_buffer_size *= second_var.getDim(i).getSize() ;	
					out_buffer_size *= out_var.getDim(i).getSize() ;	
				} // END: if (i!=main_index)
			}	// END: for i


			// The size of the variables (first, second, output) should match, except for the first dimension that will be used for the joining
			throwConditionedException( (first_buffer_size != second_buffer_size)||(first_buffer_size != out_buffer_size), \
				exNetcdf4CopyCriticalException("Size mismatching!")) ;
		}

		size_t buffer_size ( first_buffer_size * first_var.getType().getSize() ) ;	// From number of elements to number of bytes (except the first dimension, unknown)

		/* We solve the equation: maximum number of rows to be processed in each chunk of iteration: */
		const size_t max_elems = ( ((maximum_buffer_size / buffer_size) < 1) ? 1 : (maximum_buffer_size / buffer_size) ) ; 
		buffer_size *= max_elems ; // Memory to be allocated to process each chunk 

		{
			unsigned char* buffer = static_cast<unsigned char*>(malloc(buffer_size)) ;
			throwConditionedException( (buffer == NULL), 	exNetcdf4CopyCriticalException("Out of memory")) ;
			pattern::CMemoryScopeGuard memscg(buffer) ; 

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream os ; os << "Netcdf4Copy:concatVars() memory allocated: " << buffer_size 
		    	  << " bytes. Number of elements along the main index, per iteration: " << max_elems ;
				ACS_LOG_DEBUG( os.str() ) ;
			ACS_CLASS_END_DEBUG

			// Reading the first variable and copying to the output (merged) file
			{
				const size_t write_offset ( 0 ) ;	// The first variable is written from the beginning of the output file
				size_t leftovers ( first_var.getDim(main_index).getSize() ) ;
				size_t loops ( 0 ) ;

				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream os ; os << "Netcdf4Copy:concatVars() merging first variable: " << first_var_name 
						<< "  with size: " << leftovers << "  writing with offset: " << write_offset ; 
					ACS_LOG_DEBUG( os.str() ) ;
				ACS_CLASS_END_DEBUG

				// Set read indexes 
				for (size_t i=0; i<array_dim_size; i++) { read_pos[i] = 0 ; }

				// Set write indexes 
				for (size_t i=0; (i<array_dim_size); i++) {
					write_pos[i] = ( (i==main_index)? write_offset : 0) ;
				}

				/* Concatenate the array */
				while ( leftovers>0 )
				{
					/* Compute the # of elements to process in this iteration */
					const size_t to_process ( min(leftovers, max_elems) ) ;

					// Set counters
					for (size_t i=0; (i<array_dim_size); i++) { 
						counts[i] = ( (i==main_index)? to_process : first_var.getDim(i).getSize() ) ;
					}

					first_var.getVar(read_pos, counts, static_cast<void*>(buffer)) ;
					out_var.putVar(write_pos, counts, static_cast<void*>(buffer)) ;

					/* Prepare the next iteration : */
					read_pos[main_index] += to_process ;
					write_pos[main_index] += to_process ;
					leftovers -= to_process ;
					loops ++ ; 
				}
			}	// END: Reading the first variable and copying to the output (merged) one

			// Reading the second variable and copying to the output (merged) file
			{
				const size_t write_offset ( first_var.getDim(main_index).getSize() ) ;	// The first variable is written from the end of the first variable
				size_t leftovers ( second_var.getDim(main_index).getSize() ) ;
				size_t loops ( 0 ) ;
				
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream os ; os << "Netcdf4Copy:concatVars() merging second variable: " << second_var_name << "  with size: " 
							<< leftovers << "  writing with offset: " << write_offset ;
					ACS_LOG_DEBUG( os.str() ) ;
				ACS_CLASS_END_DEBUG

				// Set read indexes 
				for (size_t i=0; i<array_dim_size; i++) { read_pos[i] = 0 ; }// was start_pos

				// Set write indexes 
				for (size_t i=0; (i<array_dim_size); i++) {
					write_pos[i] = ( (i==main_index)? write_offset : 0) ;
				}

				/* Concatenate the array */
				while ( leftovers>0 )
				{
					/* Compute the # of elements to process in this iteration */
					const size_t to_process ( min(leftovers, max_elems) ) ;

					// Set counters
					for (size_t i=0; (i<array_dim_size); i++) { 
						counts[i] = ( (i==main_index)? to_process : second_var.getDim(i).getSize() ) ;
					}

					second_var.getVar(read_pos, counts, static_cast<void*>(buffer)) ;
					out_var.putVar(write_pos, counts, static_cast<void*>(buffer)) ;

					/* Prepare the next iteration : */
					read_pos[main_index] += to_process ;
					write_pos[main_index] += to_process ;
					leftovers -= to_process ;
					loops ++ ; 
				}
			}	// END: Reading the second variable and copying to the output (merged) one

		}	/* END OF SCOPE: CMemoryScopeGuard */

	} catch(exceptions::NcException& ex)
	{
		ACS_CLASS_BGN_DEBUG(10)
			ex.what();
		ACS_CLASS_END_DEBUG
		throwException(exNetcdf4CopyException("Netcdf4Copy::concatVars: Error occurred while concatenating netcdf first variable: \"" + first_var_name + "\"" + \
			" and second variable: \"" + second_var_name + "\" and writing result to variable: \"" + out_var_name + "\"" ));
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream os ; os << "Netcdf4Copy:concatVars()  Method finished" ;
		ACS_LOG_DEBUG( os.str() ) ;
	ACS_CLASS_END_DEBUG
}


void Netcdf4Copy::copyVars(	const NcVar& in_var, NcVar& out_var, const pair<size_t, size_t>& p, 
							const size_t& dim_to_resize)	// 0-relative
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called.") ;

	const size_t start_index = p.first ; 
	const size_t stop_index = p.second ;
	
	const size_t array_dim_size = in_var.getDimCount() ;

    throwConditionedException( (array_dim_size < 1), exNetcdf4CopyCriticalException("Invalid number of dimensions: <1")) ;
    
	ACS_CLASS_BGN_DEBUG(20)
		ostringstream os ;
		os << "Variable: \"" << in_var.getName() << "\" has sizes: (" ;
		for (size_t i=0; i<array_dim_size; i++)
			os << (i?" x ":"") << in_var.getDim(i).getSize() << " " ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, os.str() << ")") ; 
	ACS_CLASS_END_DEBUG
   
    throwConditionedException( (in_var.getDim(dim_to_resize).getSize() < stop_index), exNetcdf4CopyCriticalException("Dimension to resize lower than stop_value")) ;

	
	/* Allocate the vectors for handling all dimensions: */
	vector<size_t> start_pos(array_dim_size), copy_pos(array_dim_size), counts(array_dim_size) ; 

	/* Maximum memory size (for the malloc) */
	const size_t maximum_buffer_size = 16*1024*1024 ; // 16 MB
	
	/* We have to compute the elements to process in each chunk. The memory for each chunk should not exceed maximum_buffer_size: */
	
	// Compute the size for all the dimensions of the vector, except the selected one (which is the unknown in the equation)
	size_t buffer_size = 1 ;	
	for (size_t i=0; (i<array_dim_size); i++) {
		if (i!=dim_to_resize) {
			buffer_size *= in_var.getDim(i).getSize() ;	// Number of elements for all the dimensions but the selected one
		} // END: if (i!=dim_to_resize)
	}	// END: for i
	
	buffer_size *= in_var.getType().getSize() ;	// From number of elements to number of bytes (except the first dimension, unknown)
	
	/* We solve the equation: maximum number of rows to be processed in each chunk of iteration: */
	const size_t max_elems = ( ((maximum_buffer_size / buffer_size) < 1) ? 1 : (maximum_buffer_size / buffer_size) ) ; 
	
	buffer_size *= max_elems ; // Memory to be allocated to process each chunk 

	{
		unsigned char* buffer = static_cast<unsigned char*>(malloc(buffer_size)) ;
		throwConditionedException( (buffer == NULL), 	exNetcdf4CopyCriticalException("Out of memory")) ;
		pattern::CMemoryScopeGuard memscg(buffer) ; 

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() memory allocated: " << buffer_size 
		      << " bytes. Number of elements along the main index, per iteration: " << max_elems) ;


		// Check if (stop>=start), as expected
		throwConditionedException( ((stop_index>=start_index)==false),
			exNetcdf4CopyCriticalException("FATAL: Invalid Start, Stop order")
		);

		size_t leftovers = (stop_index - start_index + 1) ;
		size_t loops = 0 ;

		// Set start indexes (read)
		for (size_t i=0; (i<array_dim_size); i++) {
			start_pos[i] = ( (i==dim_to_resize)? start_index : 0) ;
		}

		// Set start indexes (write)
		for (size_t i=0; i<array_dim_size; i++) { copy_pos[i] = 0 ; }

		/* Copy the arra */
		while ( leftovers>0 )
		{
			/* Compute the # of elements to process in this iteration */
			const size_t to_process ( min(leftovers, max_elems) ) ;

			// Set counters
			for (size_t i=0; (i<array_dim_size); i++) { 
				counts[i] = ( (i==dim_to_resize)? to_process : in_var.getDim(i).getSize() ) ;
			}

			/* Fill a vector by reading the NetCDF */
			in_var.getVar(start_pos, counts, static_cast<void*>(buffer)) ;

			out_var.putVar(copy_pos, counts, static_cast<void*>(buffer)) ;


			/* Prepare the next iteration : */
			start_pos[dim_to_resize] += to_process ;
			copy_pos[dim_to_resize] += to_process ;
			leftovers -= to_process ;
			loops ++ ; 
		}

	}	/* END OF SCOPE: CMemoryScopeGuard */
	 
}


void Netcdf4Copy::copyVars(	const NcVar& in_var, NcVar& out_var, const vector < pair<size_t, size_t> >& ranges_v, 
							const size_t& dim_to_resize,	// 0-relative
							const pair <long double, long double>& correction_factor) 
{
	const long double correction_mult(correction_factor.first) ;
	const long double correction_add(correction_factor.second) ;

	if ((correction_mult != 1.0) || (correction_add != 0.0))
    { ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called.  correction_mult: " << correction_mult << "  correction_add: " << correction_add) ; }
	else
    { ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called."); }
	
    throwConditionedException( (ranges_v.size() < 1), exNetcdf4CopyCriticalException("Empty vector")) ;

	size_t max_stop_index(0) ; for (size_t i=0; i<ranges_v.size(); i++)
	{
		size_t cur_val (ranges_v.at(i).second) ;
		if ( (i == 0) || (cur_val>max_stop_index)) { max_stop_index = cur_val ; } 
	}
		
	const size_t array_dim_size = in_var.getDimCount() ;

    throwConditionedException( (array_dim_size < 1), exNetcdf4CopyCriticalException("Invalid number of dimensions: <1")) ;
    
	ACS_CLASS_BGN_DEBUG(20)
		ostringstream os ;
		os << "Variable: \"" << in_var.getName() << "\" has sizes: (" ;
		for (size_t i=0; i<array_dim_size; i++)
			os << (i?" x ":"") << in_var.getDim(i).getSize() << " " ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, os.str() << ")") ; 
	ACS_CLASS_END_DEBUG
   
    throwConditionedException( (in_var.getDim(dim_to_resize).getSize() < max_stop_index), exNetcdf4CopyCriticalException("Dimension to resize lower than stop_value")) ;

	// Set start indexes (write)
	vector<size_t> copy_pos(array_dim_size) ; 
	for (size_t i=0; i<array_dim_size; i++) { copy_pos[i] = 0 ; }

	for (size_t c=0; c<ranges_v.size(); c++)
	{
		const size_t start_index = ranges_v.at(c).first ; 
		const size_t stop_index = ranges_v.at(c).second ;

		/* Allocate the vectors for handling all dimensions: */
		vector<size_t> start_pos(array_dim_size), counts(array_dim_size) ; 

		/* Maximum memory size (for the malloc) */
		const size_t maximum_buffer_size = 16*1024*1024 ; // 16 MB

		/* We have to compute the elements to process in each chunk. The memory for each chunk should not exceed maximum_buffer_size: */

		// Compute the size for all the dimensions of the vector, except the selected one (which is the unknown in the equation)
		size_t elements_per_row = 1 ;	
		for (size_t i=0; (i<array_dim_size); i++) {
			if (i!=dim_to_resize) {
				elements_per_row *= in_var.getDim(i).getSize() ;	// Number of elements for all the dimensions but the selected one
			} // END: if (i!=dim_to_resize)
		}	// END: for i

		size_t buffer_size ( elements_per_row * in_var.getType().getSize() ) ;	// From number of elements to number of bytes (except the first dimension, unknown)

		/* We solve the equation: maximum number of rows to be processed in each chunk of iteration: */
		const size_t max_elems = ( ((maximum_buffer_size / buffer_size) < 1) ? 1 : (maximum_buffer_size / buffer_size) ) ; 

		buffer_size *= max_elems ; // Memory to be allocated to process each chunk 

		{
			unsigned char* buffer = static_cast<unsigned char*>(malloc(buffer_size)) ;
			throwConditionedException( (buffer == NULL), 	exNetcdf4CopyCriticalException("Out of memory")) ;
			pattern::CMemoryScopeGuard memscg(buffer) ; 

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() memory allocated: " << buffer_size 
		    	  << " bytes. Number of elements along the main index, per iteration: " << max_elems) ;


			// Check if (stop>=start), as expected
			throwConditionedException( ((stop_index>=start_index)==false),
				exNetcdf4CopyCriticalException("FATAL: Invalid Start, Stop order")
			);

			size_t leftovers = (stop_index - start_index + 1) ;
			size_t loops = 0 ;

			// Set start indexes (read)
			for (size_t i=0; (i<array_dim_size); i++) {
				start_pos[i] = ( (i==dim_to_resize)? start_index : 0) ;
			}

			/* Copy the array */
			while ( leftovers>0 )
			{
				/* Compute the # of elements to process in this iteration */
				const size_t to_process ( min(leftovers, max_elems) ) ;

				// Set counters
				for (size_t i=0; (i<array_dim_size); i++) { 
					counts[i] = ( (i==dim_to_resize)? to_process : in_var.getDim(i).getSize() ) ;
				}

				/* Fill a vector by reading the NetCDF */
				if ((correction_mult != 1.0) || (correction_add != 0.0))
				{
					const netCDF::NcType::ncType t ( in_var.getType().getTypeClass() ) ;

					switch(t)
					{
						case netCDF::NcType::nc_CHAR: 
						{ 
							// No correction possibile since a "CHAR" is actually a printable character. For math use "BYTE"
							in_var.getVar(start_pos, counts, static_cast<void*>(buffer)) ;
							out_var.putVar(copy_pos, counts, static_cast<void*>(buffer)) ;
							break ; 
						}
						case netCDF::NcType::nc_BYTE: 
						{ 
							signed char* p = reinterpret_cast<signed char*>(buffer) ;
							signed char filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_UBYTE: 
						{ 
							unsigned char* p = reinterpret_cast<unsigned char*>(buffer) ;
							unsigned char filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_SHORT: 
						{ 
							int16_t* p = reinterpret_cast<int16_t*>(buffer) ;
							int16_t filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_USHORT: 
						{ 
							uint16_t* p = reinterpret_cast<uint16_t*>(buffer) ;
							uint16_t filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_INT: 
						{ 
							int32_t* p = reinterpret_cast<int32_t*>(buffer) ;
							int32_t filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_UINT: 
						{ 
							uint32_t* p = reinterpret_cast<uint32_t*>(buffer) ;
							uint32_t filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_INT64: 
						{ 
							int64_t* p = reinterpret_cast<int64_t*>(buffer) ;
							int64_t filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_UINT64: 
						{ 
							uint64_t* p = reinterpret_cast<uint64_t*>(buffer) ;
							uint64_t filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_FLOAT: 
						{ 
							float* p = reinterpret_cast<float*>(buffer) ;
							float filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						case netCDF::NcType::nc_DOUBLE: 
						{ 
							double* p = reinterpret_cast<double*>(buffer) ;
							double filler ; bool filler_set ;
							in_var.getVar(start_pos, counts, p) ; in_var.getFillModeParameters(filler_set, filler) ;
							for (size_t i=0; i<(to_process*elements_per_row); i++) 
							{ if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
							out_var.putVar(copy_pos, counts, p) ;
							break ; 
						}
						default: 
						{ throwException( exNetcdf4CopyException( "Netcdf4Copy::copyVars : Unsupported type: " + (in_var.getType().getName())) ) ; }  

					}	// END: switch

				} else {
				
					in_var.getVar(start_pos, counts, static_cast<void*>(buffer)) ;
					out_var.putVar(copy_pos, counts, static_cast<void*>(buffer)) ;

				}	// END: if ((correction_mult != 1.0) || (correction_add != 0.0))


				/* Prepare the next iteration : */
				start_pos[dim_to_resize] += to_process ;
				copy_pos[dim_to_resize] += to_process ;
				leftovers -= to_process ;
				loops ++ ; 
			}

		}	/* END OF SCOPE: CMemoryScopeGuard */
	}
}


void Netcdf4Copy::copyVars(	const NcVar& in_var, NcVar& out_var, const vector < pair<size_t, size_t> >& ranges_v, 
							const size_t& dim_to_resize)	// 0-relative
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called.") ;
	
	const pair <long double, long double> correction_factor (pair<long double, long double>(1.0, 0.0) ) ;	// No correction

	copyVars(in_var, out_var, ranges_v, dim_to_resize, correction_factor) ;
}


void Netcdf4Copy::copyVars(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression& compression, const bool& copyData)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called.") ;

 	string src_filename("UNDEF"), dest_filename("UNDEF") ;
  try
  {
		src_filename = src.getFilename() ; dest_filename = dest.getFilename() ;
        multimap<string,NcVar> vars = src.getVars() ;
    
        for (multimap<string,NcVar>::iterator it = vars.begin(); it!=vars.end(); it++) {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "preparing to add variable <" << it->second.getName() << "> metadata from source file: \""
				<< src_filename << "\"" ) ;

			NcVar newVar = dest.addVariable( it->second ) ;
			if ( (it->second).getDimCount() >= 1) {
				// If the variable has at least ONE dimension its content will be compressed
				// (It is InvalidArg to compress variables with no dimension)
	      		Netcdf4DataFile::setCompressionValue(newVar, compression) ;
    		}

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "variable <" << it->second.getName() << "> metadata added to destination file.\"" \
				<< dest_filename << "\"" ) ;

        }

        if (copyData)
            for (multimap<string,NcVar>::iterator it = vars.begin(); it!=vars.end(); it++) {

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " preparing to copy variable data <" << it->second.getName()<< ">  from the source file \"" \
					<< src_filename << "\" to the destination file \"" << dest_filename << "\"" ) ;

                NcVar newVar = dest.getVar(it->second.getName());
    
                Netcdf4DataFile::copyVarData(it->second, newVar);
            }

  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf variables from file \"" + src_filename + "\" to file \"" + dest_filename \
	  	+ "\" copy data: " + (copyData? "T":"F") ));  
  }
}

void Netcdf4Copy::copyNestedGroups(const multimap<string,NcGroup>::iterator& itPrev, const NcGroup& inGroup, NcGroup& outGroup)
{

  try
  {

       	multimap<string,NcGroup> groups = itPrev->second.getGroups() ;
        for (multimap<string,NcGroup>::iterator it = groups.begin(); it!=groups.end(); it++) {
			NcGroup newGroup;
			Netcdf4DataFile::copyGroup(inGroup, it->first, it->second, newGroup);
			copyNestedGroups(it, newGroup, outGroup);
		}
  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
  }
}



void Netcdf4Copy::copyGroups(const Netcdf4DataFile& src, Netcdf4DataFile& dest)
{
// aleber - August 2017 - utility function that copies the whole content of each netcdf Group (read from the source netdcf file) into a corresponding new group 
// (to be written into the destination netcdf file)

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyGroups() called.") ;

 	string src_filename("UNDEF"), dest_filename("UNDEF") ;
	NcGroup fatherGroup;
	NcGroup childGroup0;
	NcGroup childGroup1;
	multimap<string,NcGroup> groups0, groups1;

  try
  {
  
        groups0 = src.getGroups() ;		
        for (multimap<string,NcGroup>::iterator it0 = groups0.begin(); it0!=groups0.end(); it0++) {
			fatherGroup = dest.addGroup(it0->first) ;
 
        	groups1 = it0->second.getGroups() ;			
      		for (multimap<string,NcGroup>::iterator it1 = groups1.begin(); it1!=groups1.end(); it1++) {			 
				Netcdf4DataFile::copyGroup(fatherGroup, it1->first, it1->second, childGroup0);
 				Netcdf4Copy::copyNestedGroups(it1, childGroup0, childGroup1);
			}			

		}			

  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
  }
}

void Netcdf4Copy::copyVarsWithExclusions(	const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression& compression, \
											const vector<string>& exclude_vars, const bool& copyData )
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream os ;
		os << exDebugShortSignatureFile << "method called.  CopyData: " << (copyData? "T":"F") << endl << "exclude_vars: " ;
		for (size_t i=0; i<exclude_vars.size(); i++) { os << (i?", ":"") << exclude_vars[i] << endl ; }
		ACS_LOG_DEBUG( os.str() ) ;
	ACS_CLASS_END_DEBUG
	  
	string src_filename("UNDEF"), dest_filename("UNDEF") ;

	try
	{
		src_filename = src.getFilename() ; dest_filename = dest.getFilename() ;
        multimap<string,NcVar> vars = src.getVars() ;
    
        for (multimap<string,NcVar>::iterator it = vars.begin(); it!=vars.end(); it++) {
			
			if ( exclude_vars.empty() || find(exclude_vars.begin(), exclude_vars.end(), it->second.getName()) == exclude_vars.end() )
			{
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream os ;
					os << exDebugShortSignatureFile <<  "preparing to add variable <" << it->second.getName() << "> from source file: \"" << src_filename << "\"" ;
					ACS_LOG_DEBUG( os.str() ) ;
				ACS_CLASS_END_DEBUG

				NcVar newVar = dest.addVariable( it->second ) ;
				if ( (it->second).getDimCount() >= 1) {
					// If the variable has at least ONE dimension its content will be compressed
					// (It is InvalidArg to compress variables with no dimension)
	      			Netcdf4DataFile::setCompressionValue(newVar, compression) ;
    			}

				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream os ;
					os << exDebugShortSignatureFile << "variable <" << it->second.getName() << "> added to destination file.\"" << dest_filename << "\"" ;
					ACS_LOG_DEBUG( os.str() ) ;
				ACS_CLASS_END_DEBUG
    		}
        }

        if (copyData)
		{
            for (multimap<string,NcVar>::iterator it = vars.begin(); it!=vars.end(); it++) {
				if ( exclude_vars.empty() || find(exclude_vars.begin(), exclude_vars.end(), it->second.getName()) == exclude_vars.end() )
				{
					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ostringstream os ;
						os << exDebugShortSignatureFile << "preparing to copy variable data <" << it->second.getName() << "> from source file." ;
						ACS_LOG_DEBUG( os.str() ) ;
					ACS_CLASS_END_DEBUG

                	NcVar newVar = dest.getVar(it->second.getName());

                	Netcdf4DataFile::copyVarData(it->second, newVar);
				}
            }
		}
  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
		throwException(exNetcdf4CopyException("Error occurred while copying netcdf variables from file \"" + src_filename + "\" to file \"" + dest_filename \
	  		+ "\" copy data: " + (copyData? "T":"F") ));  
 }
 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "  Method finished") ;
}

void Netcdf4Copy::copyDims(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const std::vector<std::string>& dimnames)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyDims() called.") ;
	
	string src_filename("UNDEF"), dest_filename("UNDEF") ;

  try 
  {
		src_filename = src.getFilename() ; dest_filename = dest.getFilename() ;
        vector<string>::const_iterator it;
        for (it=dimnames.begin(); it!=dimnames.end(); it++) 
        {
            NcDim dim = src.getDim(*it);
            dest.addDim(dim.getName(), dim.getSize());
    
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " dimension <" << dim.getName()<< ">  read from the source file \"" \
				<< src_filename << "\" and added to the destination file \"" << dest_filename << "\"" ) ;
         }
  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf dimensions from file " + src_filename + " to file " + dest_filename ));
  }
    

}


void Netcdf4Copy::copyAtts(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const vector<string>& attnames, const vector<string>& exclude_att)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyAtts() called.  Size of matching_attnames: " << attnames.size() << "  Size of exclude_dims: " << exclude_att.size()) ;
	
	string src_filename("UNDEF"), dest_filename("UNDEF") ;

  try
  {
		src_filename = src.getFilename() ; dest_filename = dest.getFilename() ;
        vector<string>::const_iterator it;
        for (it=attnames.begin(); it!=attnames.end(); it++) 
        {
			if ( find(exclude_att.begin(), exclude_att.end(), *it) == exclude_att.end() )
			{
            	 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " reading attribute <" << *it << ">  from the source file " \
					<< src_filename) ;
           		
				NcGroupAtt att = src.getAtt(*it);

	 			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " writing variable <" << *it << "> attributes to the destination file " \
					<< dest_filename) ;		
						
            	dest.addAttr(att);
			}
			else
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " skipping attribute: \"" << *it << "\"") ;
			}
        }
  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf attributes from file \"" + src_filename + "\" to file \"" + dest_filename ));  
  }
    
}


void Netcdf4Copy::copyVars(	const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression& compression, \
							std::map<std::string, NcVar>& newVars, const bool& copyData )
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called.") ;
	
	string src_filename("UNDEF"), dest_filename("UNDEF") ;

  try
  {
		src_filename = src.getFilename() ; dest_filename = dest.getFilename() ;
        multimap<string,NcVar> vars = src.getVars() ;
    
        for (multimap<string,NcVar>::iterator it = vars.begin(); it!=vars.end(); it++) {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " variable <" << it->second.getName() << "> metadata read from the source file " \
				<< src_filename) ;

            //copy schema
			pair<string,NcVar> p( make_pair<string,NcVar>(it->second.getName(), dest.addVariable(it->second)) );
			if ( (it->second).getDimCount() >= 1) {
				// If the variable has at least ONE dimension its content will be compressed
				// (It is InvalidArg to compress variables with no dimension)
				Netcdf4DataFile::setCompressionValue(p.second, compression) ;
			}

            
	 		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " variable <" << it->second.getName() << "> metadata written to the destination file " \
				<< dest_filename) ;
            newVars.insert(p);
   
        }
        //copy data
        if (copyData)
            for (multimap<string,NcVar>::iterator it = vars.begin(); it!=vars.end(); it++) {

              ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " preparing to copy data for variable <" << it->second.getName() << "> from source file \""
					<< src_filename << "\" to destination file: \"" << dest_filename << "\"" ) ;

                Netcdf4DataFile::copyVarData(it->second, newVars[it->second.getName()]);
            }

  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf variables from file \"" + src_filename + "\" to file \"" + dest_filename \
	  	+ "\" copy data: " + (copyData? "T":"F") ));  }
}


void Netcdf4Copy::copyVars(	const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression& compression, \
							const std::vector<std::string>& varnames, const bool& copyData)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Copy:copyVars() called.") ;

	string src_filename("UNDEF"), dest_filename("UNDEF") ;

	try
	{   
		src_filename = src.getFilename() ; dest_filename = dest.getFilename() ;
        vector<string>::const_iterator it;
        for (it=varnames.begin(); it!=varnames.end(); it++) 
        {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " reading variable <" << *it << "> metadata from the source file " \
				<< src_filename) ;
				
            NcVar var = src.getVar(*it);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " writing variable <" << *it << "> metadata to the destination file " \
				<< dest_filename) ;
          
		    //copy schema
            NcVar newVar = dest.addVariable(var);
            if ( var.getDimCount() >= 1) {
				// If the variable has at least ONE dimension its content will be compressed
				// (It is InvalidArg to compress variables with no dimension)
				Netcdf4DataFile::setCompressionValue(newVar, compression) ;
    		}
	    }

    
        //copy data
        if (copyData)
            for (it=varnames.begin(); it!=varnames.end(); it++) 
            {
                NcVar var = src.getVar(*it);
                NcVar newVar = dest.getVar(*it);

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << " preparing to copy data for variable <" << *it << "> from source file \""
					<< src_filename << "\" to destination file: \"" << dest_filename << "\"" ) ;

                Netcdf4DataFile::copyVarData(var, newVar);
            }
  }
  catch(exceptions::NcException& ex)
  {
      ACS_CLASS_BGN_DEBUG(10)
		ex.what();
      ACS_CLASS_END_DEBUG
      throwException(exNetcdf4CopyException("Error occurred while copying netcdf variables from file \"" + src_filename + "\" to file \"" + dest_filename \
	  	+ "\" copy data: " + (copyData? "T":"F") ));
  }
    
}


void Netcdf4Copy::copyVars(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const bool& shuffle, const bool& deflate, const int& deflateLevel, const bool& copyData)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "  called with shuffle =" << shuffle << ", deflate_level=" << deflateLevel) ;

	
	/* CompressionLevel is a positive number only if the deflation is requested, with positive index. Otherwise is set to zero */
	const unsigned short int compressionLevel ( (deflate && (deflateLevel>0))? deflateLevel : 0 ) ;
	
	const NetcdfCompression compression(compressionLevel, shuffle) ;

	copyVars(src, dest, compression, copyData) ;
}


void Netcdf4Copy::copyFile(const string& srcFile, const string& dstFile, const NetcdfCompression& compression, bool forceOverwrite) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "  Method called. srcFile: \"" << srcFile 
		<< "\"  dstFile: \"" << dstFile << "\"  forceOverwrite: " << (forceOverwrite? "T":"F")) ;

	throwConditionedException( (! File::exists(srcFile)), exNetcdf4CopyException( "Input file not found: \"" + srcFile + "\"" ) ) ; 

	throwConditionedException( (!forceOverwrite && File::exists(dstFile)), exNetcdf4CopyException("Destination file already exists: \"" + dstFile + "\"" ) ) ; 

	::unlink ( dstFile.c_str() ) ;	// Errors ignored
	Netcdf4DataFile input_file(srcFile, NcFile::read);		
	Netcdf4DataFile output_file(dstFile, NcFile::replace);	/* replace means that the file is created empty, even if existing */

	
	/* Copy all the file attributes */
	Netcdf4Copy::copyAtts(input_file, output_file) ;	

	/* Copy all the dimensions */
	Netcdf4Copy::copyDims(input_file, output_file) ;

	/* Copy all the variables (metadata and data) */
	const bool copyData (true) ; 
	copyVars(input_file, output_file, compression, copyData) ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "  Method finished") ;
}


void Netcdf4Copy::copyFile(	const string& srcFile, const string& dstFile, \
							const vector <NetcdfAttribute>& atts_to_process, \
							//const vector < pair <NetcdfAttribute, bool> >&atts_to_replace, const vector < pair <NetcdfAttribute, bool> >&atts_to_merge, 
							const NetcdfCompression& compression, bool forceOverwrite) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "  Method called. srcFile: \"" << srcFile 
		<< "\"  dstFile: \"" << dstFile << "\"  Size of atts_to_process: " << atts_to_process.size() 
		<< "  forceOverwrite: " << (forceOverwrite? "T":"F")) ;

	throwConditionedException( (! File::exists(srcFile)), exNetcdf4CopyException( "Input file not found: \"" + srcFile + "\"" ) ) ; 

	throwConditionedException( (!forceOverwrite && File::exists(dstFile)), exNetcdf4CopyException("Destination file already exists: \"" + dstFile + "\"" ) ) ; 

	::unlink ( dstFile.c_str() ) ;	// Errors ignored
	Netcdf4DataFile input_file(srcFile, NcFile::read);		
	Netcdf4DataFile output_file(dstFile, NcFile::replace);	/* replace means that the file is created empty, even if existing */

	
	/* Copy all the file attributes */
	Netcdf4Copy::copyAtts(input_file, output_file, atts_to_process) ;	

	/* Copy all the dimensions */
	Netcdf4Copy::copyDims(input_file, output_file) ;

	/* Copy all the variables (metadata and data) */
	const bool copyData (true) ; 
	copyVars(input_file, output_file, compression, copyData) ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, exDebugShortSignatureFile << "  Method finished") ;
}

_ACS_END_NAMESPACE
