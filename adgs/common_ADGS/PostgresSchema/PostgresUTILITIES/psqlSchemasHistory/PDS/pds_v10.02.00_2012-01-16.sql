
-- Version 10.2.0
-- POSTGRES








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_DISTRIBUTIONSTATE
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distributionstate(
	in_packageid INT,
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	ret INT;

BEGIN
	
	ret := 0 ;
	
	-- state transition :
	----------------------------------
	-- TRANSIENT -> ERROR
	--         / ELIGIBLE <- ERROR
	-- ELIGIBLE
	--         \ SUSPENDED <- ELIGIBLE
	-- SUSPENDED -> ERROR
	-- DOWNLOADING -> ERROR
	-- DISTRIBUTION -> ERROR
	-- COMPLETED
	-----------------------
	-- ERROR
	-- ABORTED
	----------------------------------


  	IF post_status IS NULL THEN
  	  	RETURN 0 ;
  	END IF ;


  	IF pre_status = post_status THEN
  	  	IF post_status = 'TRANSIENT' OR pre_status = 'ELIGIBLE' OR pre_status = 'DOWNLOADING' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
  	END IF ;


-- state control
  	IF post_status = 'ELIGIBLE' THEN
  	  	IF pre_status = 'TRANSIENT' OR pre_status = 'SUSPENDED' OR pre_status = 'ERROR'  THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	
	ELSIF post_status = 'SUSPENDED' THEN
  	  	IF pre_status = 'ELIGIBLE' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	
  	ELSIF post_status = 'DOWNLOADING' THEN
  	  	IF pre_status = 'ELIGIBLE' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'DISTRIBUTION' THEN
  	  	IF pre_status = 'DOWNLOADING' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'COMPLETED' THEN
  	  	IF pre_status = 'DISTRIBUTION' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'ERROR' THEN
  	  	IF pre_status = 'DOWNLOADING' OR pre_status = 'DISTRIBUTION' OR pre_status = 'SUSPENDED' OR pre_status = 'TRANSIENT' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	
	ELSIF post_status = 'ABORTED' THEN
  	  	ret := p_cartsinv_update(in_packageid, post_status);
		RETURN ret ;
	
	ELSE
  	  	 RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
	END IF ;
	
  	RETURN ret ;
	
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
		'pds-10.2.0',
		'2012-01-16',
		3,
		2,
		'PDS_10-2-0',
		'Changed procedure p_distributionstate.'
) ;
