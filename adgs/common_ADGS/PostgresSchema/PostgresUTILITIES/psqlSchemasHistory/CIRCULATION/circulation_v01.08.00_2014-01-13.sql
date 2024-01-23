
-- Version 1.8.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ADD nextretrydate TIMESTAMP(6) DEFAULT now() ;


/* ------------------------------
	t_dbparameters
*/ ------------------------------
UPDATE t_dbparameters SET parameter = 'circ_def_max_ret' WHERE parameter = 'circ_max_ret' ;
--UPDATE t_dbparameters SET parameter = 'circ_dyn_max_ret' WHERE parameter = 'dyn_circ_max_ret' ;
--UPDATE t_dbparameters SET parameter = 'circ_expl_max_ret' WHERE parameter = 'expl_circ_max_ret' ;
--UPDATE t_dbparameters SET parameter = 'circ_impl_max_ret' WHERE parameter = 'impl_circ_max_ret' ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_circulationSetErrorType
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_circulationseterrortype(
	in_circ_id BIGINT) RETURNS INTEGER AS $$

DECLARE
	max_ret INT2;
	rec_err_id INT2;
	err_id INT2;
	mode VARCHAR(16);

BEGIN
	rec_err_id := (SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR');
	err_id := (SELECT id FROM t_stati WHERE status = 'ERROR');
	mode := (SELECT m.circulationmode FROM t_circulationsqueue q INNER JOIN t_circulationmodes m ON q.circulationmode_id = m.id WHERE q.id = in_circ_id);
	
	IF (mode = 'DYNAMIC') THEN
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_dyn_max_ret');
	ELSEIF (mode = 'EXPLICIT') THEN
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_expl_max_ret');
	ELSEIF (mode = 'IMPLICIT') THEN
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_impl_max_ret');
	ELSE
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_def_max_ret');
	END IF;
	
	UPDATE t_circulationsqueue SET retries = retries + 1 WHERE id = in_circ_id;
	
	IF ((SELECT retries FROM t_circulationsqueue WHERE id = in_circ_id) < max_ret) THEN
		UPDATE t_circulationsqueue SET status_id = rec_err_id WHERE id = in_circ_id;
	ELSE
		UPDATE t_circulationsqueue SET status_id = err_id WHERE id = in_circ_id;
	END IF;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.8.0',
		'2014-01-13',
		6,
		20,
		'CIRCULATION_1-8-0',
		'Added nextretrydate field on t_circulationsqueue. Changed circ_max_ret value into t_dbparameters. Changed p_circulationseterrortype.'
) ;


