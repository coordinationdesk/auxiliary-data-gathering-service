/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema triggers script $

	$Id$

	$Author$

	$Log$
	Revision 1.9  2018/02/27 16:55:32  achnat
	Fixed bug on flow type
	
	Revision 1.8  2015/07/24 13:00:06  achnat
	Upgraded schema version
	
	Revision 1.7  2014/02/03 09:26:50  achnat
	Added new indexes on t_circulationsqueue. Dropped trigger tr_circulationsqueue_insert1 to disable start status control.
	
	Revision 1.6  2014/01/23 15:10:07  achnat
	Dropped trigger tr_circulationsqueue_update1 to disable state transition control
	
	Revision 1.5  2013/10/29 14:37:58  achnat
	Upgraded schema version.
	
	Revision 1.4  2012/11/14 11:31:07  achnat
	Updated schema version.
	
	Revision 1.3  2011/07/15 13:01:24  achnat
	Added t_datastoragequeue and new procedures.
	
	Revision 1.2  2011/06/10 13:33:42  achnat
	Added new tables.
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TRIGGERS
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
CREATE TRIGGER tr_circulationpolicies_ins_bfr BEFORE INSERT 
	ON t_circulationpolicies 
		FOR EACH ROW EXECUTE PROCEDURE tf_circ_policies_statcheck() ;



/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
--CREATE TRIGGER tr_circulationsqueue_insert1 BEFORE INSERT 
--	ON t_circulationsqueue 
--		FOR EACH ROW EXECUTE PROCEDURE tf_circulationsinsert() ;

CREATE TRIGGER tr_circulationsqueue_ins_bfr BEFORE INSERT 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_circulationsetprotocol() ;

CREATE TRIGGER tr_circulationsqueue_insert2 AFTER INSERT 
	ON t_circulationsqueue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_circulationsqueue','INS') ;

--CREATE TRIGGER tr_circulationsqueue_update1 BEFORE UPDATE 
--	ON t_circulationsqueue 
--		FOR EACH ROW EXECUTE PROCEDURE tf_circulationstatus();

CREATE TRIGGER tr_circulationsqueue_update2 AFTER UPDATE 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_circulationretries();

CREATE TRIGGER tr_circulationsqueue_update3 AFTER UPDATE OF status_id 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_datastoragequeueupdate();

CREATE TRIGGER tr_circulationsqueue_update4 AFTER UPDATE 
	ON t_circulationsqueue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_circulationsqueue', 'UPD') ;



/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE TRIGGER tr_datastoragequeue_insert AFTER INSERT 
	ON t_datastoragequeue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_datastoragequeue','INS') ;

CREATE TRIGGER tr_datastoragequeue_update1 BEFORE UPDATE OF status_id
	ON t_datastoragequeue 
		FOR EACH ROW EXECUTE PROCEDURE tf_datastoragequeuetrans();

CREATE TRIGGER tr_datastoragequeue_update2 BEFORE UPDATE 
	ON t_datastoragequeue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_datastoragequeue', 'UPD') ;



/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
CREATE TRIGGER tr_dsqueuexcirqueue_update BEFORE UPDATE OF globalstatus_id 
	ON t_dsqueue_x_cirqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_dsqueuexcirqueuetrans();


