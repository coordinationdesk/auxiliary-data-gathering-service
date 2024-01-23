/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema tables script $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/09/10 09:27:55  achnat
	Moved (and modified) p_insert_inventoryevents procedure from USDFWA to PDS.
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_usdfwa
*/ ------------------------------
CREATE TABLE t_usdfwa (
    id_usdfwa SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    mmmcupdatefile TEXT,
    modificationdate TIMESTAMP(6) DEFAULT NOW()
) ;
ALTER TABLE t_usdfwa
	ADD CONSTRAINT pk_t_usdfwa
		PRIMARY KEY (id_usdfwa) ;



/* ------------------------------
	t_usdfwa_delivery_details
*/ ------------------------------
CREATE TABLE t_usdfwa_delivery_details (
    id_delivery_details SERIAL NOT NULL,
    usdfwa_id INT NOT NULL,
    url VARCHAR(100),
    deliverydate TIMESTAMP(6) DEFAULT NOW(),
    itemstatus VARCHAR(16),
    statusdetails VARCHAR(100)
) ;
ALTER TABLE t_usdfwa_delivery_details
	ADD CONSTRAINT pk_t_usdfwa_dd
		PRIMARY KEY (id_delivery_details) ;



/* ------------------------------
	t_inventorylinks
*/ ------------------------------
CREATE TABLE t_inventorylinks (
	id SERIAL NOT NULL,
    orig_filename VARCHAR(128) NOT NULL,
    orig_fileversion VARCHAR(32) NOT NULL,
    dest_filename VARCHAR(128) NOT NULL,
    dest_fileversion VARCHAR(32) NOT NULL,
    linkdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    description VARCHAR(64)
);
ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT pk_t_inventorylinks 
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inventorylinksconf
*/ ------------------------------
CREATE TABLE t_inventorylinksconf (
    id SERIAL NOT NULL,
    orig_filetype VARCHAR(16) NOT NULL,
    dest_filetype VARCHAR(16) NOT NULL,
    algotype VARCHAR(24) NOT NULL,
    description VARCHAR(64)
);
ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT pk_t_inventorylinksconf
		PRIMARY KEY (id) ;


