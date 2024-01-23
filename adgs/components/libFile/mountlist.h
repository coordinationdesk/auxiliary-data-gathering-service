// PRQA S 1050 EOF
/*

	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libFile support module - HEADER FILE $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2012/11/29 19:31:30  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.1  2008/02/20 15:29:03  enrcar
	ENRCAR:: getFSUID, getMountEntries methods added
	
*/


/* 
   BASED ON:

   mountlist.h -- return a list of mounted file systems

   Copyright (C) 1991, 1992, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
   2004, 2005, 2006, 2007 Free Software Foundation, Inc.

*/


#ifndef MOUNTLIST_H_
# define MOUNTLIST_H_

# include <stdbool.h>
# include <sys/types.h> // PRQA S 1013


#ifdef __cplusplus
	  extern "C" {
#endif

/* A mount table entry. */
struct mount_entry
{
  char *me_devname;		/* Device node name, including "/dev/". */
  char *me_mountdir;		/* Mount point directory name. */
  char *me_type;		/* "nfs", "4.2", etc. */
  dev_t me_dev;			/* Device number of me_mountdir. */
  unsigned int me_dummy : 1;	/* Nonzero for dummy file systems. */
  unsigned int me_remote : 1;	/* Nonzero for remote fileystems. */
  unsigned int me_type_malloced : 1; /* Nonzero if me_type was malloced. */
  struct mount_entry *me_next;
};

struct mount_entry *read_file_system_list (bool need_fs_type);

void free_file_system_list (struct mount_entry *);

#ifdef __cplusplus
	}
#endif

#endif /* MOUNTLIST_H_ */
