
-- Version 9.2.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

-------------------
--- T_INVENTORY ---
-------------------
ALTER TABLE t_inventory 
	ADD CONSTRAINT fk_inventory09 
		FOREIGN KEY (originator)
			REFERENCES t_originators (id) ;


-----------------------
--- T_WAITINGORDERS ---
-----------------------
ALTER TABLE t_waitingorders ADD generationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL;
ALTER TABLE t_waitingorders ADD expirationdate TIMESTAMP(6) ;


-------------------------------
--- T_RECEPTIONPOLICIESLOCK ---
-------------------------------
ALTER TABLE t_receptionpolicieslock ALTER COLUMN receptionpolicyid TYPE INTEGER;


--------------------------
--- T_ORDERWAITTRIGGER ---
--------------------------
INSERT INTO t_orderwaittrigger VALUES ('DELAY') ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

-----------------------------------
--- P_PURGEEXPIREDWAITINGORDERS ---
-----------------------------------
CREATE FUNCTION p_purgeexpiredwaitingorders() RETURNS INTEGER AS $$

BEGIN
	DELETE FROM t_waitingorders WHERE triggertype = 'DELAY' AND expirationdate < CURRENT;

	RETURN 0;
	
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
		'pds-9.2.0',
		'2010-03-23',
		3,
		1,
		'SchemaVersion_9-2-0',
		'Modified tables t_inventory, t_receptionpolicieslock, t_waitingorders and t_orderwaittrigger. Added procedure p_purgeexpiredwaitingorders.'
) ;
