/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_StateTransitions_Allowed table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_StateTransitions_Allowed 
	//	List of tables transitions allowed 
	//////////////////////////////////////////////
*/

-- t_dsqueue_x_cirqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
 
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;


-- t_circulationsqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_WAIT')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ; 
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;  
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ; 
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ; 
	
	  
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_WAIT'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;		
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_WAIT')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
		

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
	
	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'BLOCKED'),
	(SELECT id FROM t_stati WHERE status = 'BLOCKING')) ;




-- t_datastoragequeue IF TYPE IS RETIEVE
----------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING')) ;				
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;	
	


-- t_datastoragequeue IF TYPE IS ARCHIVE
----------------------------------------	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;		

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			
		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;			
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			


-- t_datastoragequeue IF TYPE IS LTA_ARCHIVE
--------------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;		

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			


-- t_datastoragequeue IF TYPE IS DELETE
---------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DELETING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'DELETING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'DELETING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	


-- t_datastoragequeue IF TYPE IS REPUBLISHING
--------------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 6),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 6),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 6),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 6),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 6),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;


-- t_datastoragequeue IF TYPE IS REPUBLISHING
--------------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 7),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 7),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 7),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 7),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 7),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;


