/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema triggers functions script $

	$Id$

	$Author$

	$Log$
	Revision 1.13  2018/02/27 16:55:32  achnat
	Fixed bug on flow type
	
	Revision 1.12  2018/02/27 10:38:26  achnat
	Fixed bug on tf_datastoragequeuetrans
	
	Revision 1.11  2018/02/19 14:41:44  achnat
	Upgraded schema version to circulation-1.20.0
	
	Revision 1.10  2017/02/24 09:57:48  massimo.sicolo
	Added new state transitions for t_circulationsqueue and t_datastoragequeue. Changed state transition check using p_statetransitions_check. Dropped p_circulationstatetransition and p_datastoragequeue_statetrans procedure.
	
	Revision 1.9  2016/10/11 12:08:07  achnat
	Added new state transitions for t_dsqueue_x_cirqueue. Changed state transition check using p_statetransitions_check. Dropped p_dsqueuexcirqueue_statetrans procedure
	
	Revision 1.8  2015/07/24 13:00:06  achnat
	Upgraded schema version
	
	Revision 1.7  2013/10/29 14:37:58  achnat
	Upgraded schema version.
	
	Revision 1.6  2013/04/15 08:45:56  achnat
	Upgraded schema version
	
	Revision 1.5  2013/02/19 14:11:10  achnat
	Changed tf_datastoragequeueupdate trigger function to write the changes of status in the PDS_event file
	
	Revision 1.4  2012/10/05 08:54:37  achnat
	Fixed bug on tf_datastoragequeuetrans.
	
	Revision 1.3  2011/07/15 13:01:24  achnat
	Added t_datastoragequeue and new procedures.
	
	Revision 1.2  2011/06/10 13:33:42  achnat
	Added new tables.
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	   	   TRIGGERS FUNCTIONS
--
--------------------------------------------------------------------------------


/* ------------------------------
	tf_circulationsinsert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationsinsert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationcheckstartstatus(NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_circulationretries
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationretries() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationresetretries(OLD.id, OLD.status_id, NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_circulationsetprotocol
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationsetprotocol() 
	RETURNS TRIGGER AS $$

DECLARE
	transf_prot VARCHAR ;
	errMsg VARCHAR ;

BEGIN
  	IF NEW.dest_protocol_id IS NULL THEN
		
		transf_prot := UPPER(SUBSTRING(NEW.tourl FROM 0 FOR (POSITION(':' IN NEW.tourl)))) ;
		
		IF transf_prot IS NULL OR transf_prot = '' THEN
			transf_prot := 'FILE' ;
		END IF ;
		
		SELECT id INTO NEW.dest_protocol_id FROM t_transferprotocols WHERE protocol = transf_prot ;
		IF NEW.dest_protocol_id IS NULL THEN
			errMsg := 'tf_circulationsetprotocol(): dest_protocol_id "' || transf_prot || '" not found' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	RETURN NEW ;	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_circulationstatus
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationstatus() RETURNS TRIGGER AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_circulationsqueue', 1, OLD.status_id, NEW.status_id);
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_datastoragequeuetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeuetrans() RETURNS TRIGGER AS $$
BEGIN
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'ARCHIVE'THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'ARCHIVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'DELETE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'DELETE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'LTA_ARCHIVE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'LTA_ARCHIVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'RETRIEVE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'RETRIEVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_datastoragequeueupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeueupdate() RETURNS TRIGGER AS $$
DECLARE
	proc_exists INTEGER ;
  	status INTEGER ;
BEGIN
	proc_exists := (SELECT count(*) FROM pg_proc WHERE proname = 'p_events') ;
  	IF proc_exists > 0 THEN
		PERFORM p_events('t_circulationsqueue', 'status:' || NEW.status_id) ;
	ELSE 
		PERFORM p_inserteventtableitem('t_circulationsqueue', 'status:' || NEW.status_id) ;
	END IF ;

	status := p_datastoragequeue_update(NEW.id, NEW.status_id) ;

	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_datastoragequeuetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_dsqueuexcirqueuetrans() RETURNS TRIGGER AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_dsqueue_x_cirqueue', 1, OLD.globalstatus_id, NEW.globalstatus_id);
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_circ_policies_statcheck
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circ_policies_statcheck() RETURNS TRIGGER AS $$

DECLARE
  	errMsg VARCHAR(128) ;

BEGIN
	IF (TG_OP = 'INSERT') THEN
		IF NEW.status_id != (SELECT id FROM t_stati WHERE status = 'ENABLED') THEN
			errMsg := 'tf_circ_policies_statcheck: the first status must be ENABLED' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;

	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


