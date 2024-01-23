/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libFile support module $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/20 16:43:54  marpas
	adoption of libException 5.x in progress
	
	Revision 2.2  2011/06/07 14:18:56  marpas
	minor changes to compile with the new compiler and new env constraints
	
	Revision 2.1  2008/02/20 15:29:03  enrcar
	ENRCAR:: getFSUID, getMountEntries methods added
	
		

*/


/* 
   BASED ON:

   mountlist.c -- return a list of mounted file systems

   Copyright (C) 1991, 1992, 1997, 1998, 1999, 2000, 2001, 2002, 2003,
   2004, 2005, 2006, 2007 Free Software Foundation, Inc.

*/

#define _SVID_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#include <mountlist.h>


# include <mntent.h>
# if !defined MOUNTED
#  if defined _PATH_MOUNTED	/* GNU libc  */
#   define MOUNTED _PATH_MOUNTED
#  endif
# endif


#ifndef ME_DUMMY
# define ME_DUMMY(Fs_name, Fs_type)		\
    (strcmp (Fs_type, "autofs") == 0		\
     || strcmp (Fs_type, "none") == 0		\
     || strcmp (Fs_type, "proc") == 0		\
     || strcmp (Fs_type, "subfs") == 0		\
     /* for NetBSD 3.0 */			\
     || strcmp (Fs_type, "kernfs") == 0		\
     /* for Irix 6.5 */				\
     || strcmp (Fs_type, "ignore") == 0)
#endif


#ifndef ME_REMOTE
/* A file system is `remote' if its Fs_name contains a `:'
   or if (it is of type (smbfs or cifs) and its Fs_name starts with `//').  */
# define ME_REMOTE(Fs_name, Fs_type)		\
    (strchr (Fs_name, ':') != NULL		\
     || ((Fs_name)[0] == '/'			\
	 && (Fs_name)[1] == '/'			\
	 && (strcmp (Fs_type, "smbfs") == 0	\
	     || strcmp (Fs_type, "cifs") == 0)))
#endif


/* Return a list of the currently mounted file systems, or NULL on error.
   Add each entry to the tail of the list so that they stay in order.
   If NEED_FS_TYPE is true, ensure that the file system type fields in
   the returned list are valid.  Otherwise, they might not be.  */

struct mount_entry *
read_file_system_list (bool need_fs_type) // NOSONAR - pure C - can't omit arg name
{
    struct mount_entry *mount_list;
    struct mount_entry *me;
    struct mount_entry **mtail = &mount_list;


    { // NOSONAR - working block
        const struct mntent *mnt ; 
        struct mntent dummy ;
        const char *table = MOUNTED;
        FILE *fp;
	    const int buflen = 4096 ; 
	    char buffer[ buflen ] ; 	

        fp = setmntent (table, "r");
        if (fp == NULL)
          return NULL;

        while ((mnt = getmntent_r (fp, &dummy, buffer, buflen )))
        {
	        me = malloc (sizeof *me);
	        me->me_devname = strdup (mnt->mnt_fsname);
	        me->me_mountdir = strdup (mnt->mnt_dir);
	        me->me_type = strdup (mnt->mnt_type);
	        me->me_type_malloced = 1;
	        me->me_dummy = ME_DUMMY (me->me_devname, me->me_type);
	        me->me_remote = ME_REMOTE (me->me_devname, me->me_type);
	        me->me_dev = -1 ;

	        /* Add to the linked list. */
	        *mtail = me;
	        mtail = &me->me_next;
        }

        if (endmntent (fp) == 0)
            goto FREE_THEN_FAIL;
     }

    *mtail = NULL;
    return mount_list;


    FREE_THEN_FAIL: 
    { // NOSONAR - cleanup block
        int saved_errno = errno;
        *mtail = NULL;

        free_file_system_list(mount_list) ;

        errno = saved_errno;
        return NULL;
  	}
}


void
free_file_system_list (struct mount_entry * mount_list)
{
    struct mount_entry *me;

    while (mount_list)
    {
        me = mount_list->me_next;
        free (mount_list->me_devname);
        free (mount_list->me_mountdir);
        if (mount_list->me_type_malloced) {
            free (mount_list->me_type);
        }
        free (mount_list);
        mount_list = me;
    }	/* while */

	return ;
}
