
-- Version 7.3.0
-- POSTGRES

-------------------------------------------------------------
-- ATTENZIONE!!!                                           --
-- Legata a questa modifica c'è anche quella per lo USDWFA --
-- da aggiornare nei progetti in cui è richiesta           --
-------------------------------------------------------------








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

------------------------------
--- T_STANDINGDISTRIBUTION ---
------------------------------
CREATE TABLE "t_standingdistribution" (
    "id" SERIAL NOT NULL,
    "distpoliciesid" INT NOT NULL,
    "validitystart" TIMESTAMP(6) NOT NULL,
    "validitystop" TIMESTAMP(6) NOT NULL,
    "geographiclocalization" VARCHAR(32000) NOT NULL,
    "cartsid" INT,
    "data" TEXT
);

ALTER TABLE "t_standingdistribution" 
	ADD CONSTRAINT "pk_standingdistribution" 
		PRIMARY KEY ("id") ;

ALTER TABLE "t_standingdistribution" 
	ADD CONSTRAINT "ck_stendingdistribution01" 
	CHECK ("validitystart" <= "validitystop" ) ;

ALTER TABLE "t_standingdistribution" 
	ADD CONSTRAINT "fk_stendingdistribution01" 
		FOREIGN KEY ("distpoliciesid")
			REFERENCES "t_distributionpolicies" ("id") 
				ON DELETE CASCADE ;

ALTER TABLE "t_standingdistribution" 
	ADD CONSTRAINT "fk_stendingdistribution02" 
		FOREIGN KEY ("cartsid")
			REFERENCES "t_cartsref" ("id") 
				ON DELETE CASCADE ;

ALTER TABLE "t_standingdistribution" 
	ADD CONSTRAINT "uq_stendingdistribution01" 
		UNIQUE ("distpoliciesid", "validitystart", "validitystop", "cartsid") ;








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

---------------------
--- T_SCHEMATYPES ---
---------------------
INSERT INTO t_schematypes ("id", "schematype") VALUES (10, 'BASELINE') ;
INSERT INTO t_schematypes ("id", "schematype") VALUES (11, 'USDFWA') ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
--- P_INVENTORYLINKS_CHECK ---
------------------------------
CREATE FUNCTION p_inventorylinks_check (
	fname VARCHAR(128), 
	fversion VARCHAR(8),
	ftype VARCHAR(16),
	ordid INTEGER
) RETURNS INTEGER AS $$

BEGIN
	
	--  NOT NEEDED IN THIS SCHEMA
	--  To implement this procedure install USDFWA 2.0 schema.
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

--------------------------
--- P_PROCESSINVENTORY ---
--------------------------
CREATE OR REPLACE FUNCTION p_processinventory(
	fname VARCHAR(128), 
	fversion VARCHAR(8),
	valflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)
) RETURNS INTEGER AS $$

DECLARE
  	rec        RECORD ;
  	pendingcnt SMALLINT ;
	err   	   INTEGER ;
	
BEGIN

	---------------------------------------------------------------------------------
	-- The procedure assumes the triplet satellite/sensor/mission is already verified
	---------------------------------------------------------------------------------

	SELECT INTO rec COUNT(*) FROM t_pendinginventory p WHERE p.filename=fname AND p.fileversion=fversion ;
  	pendingcnt := rec.count;

  	-- return without processing if the inventory was not pending
  	IF pendingcnt = 0 THEN
  	  	RETURN 0 ;
  	END IF ;

  	-- remove pending so nobody will try the same
  	DELETE FROM t_pendinginventory WHERE "filename"=fname AND "fileversion"=fversion ;

	-----------------------------------------------------------------------------
	-- TEMPORARY ORDER GENERATION SECTION
	-----------------------------------------------------------------------------
	err := p_generateTmpOrder('t', fname, fversion, ftype, fclass, in_satellite) ;

	-----------------------------------------------------------------------------
	-- DISTRIBUTION SECTION
	-----------------------------------------------------------------------------
	err :=  p_distribute_standard_inventory(fname, fversion, orderid, ftype) ;

	-----------------------------------------------------------------------------
	-- ORDER'S BLACK LIST SECTION
	-----------------------------------------------------------------------------
	err :=  p_purgewaitingorders(ftype, valstart, valstop) ;

	-----------------------------------------------------------------------------
	-- INVENTORY LINKS SECTION
	-----------------------------------------------------------------------------
	err := p_inventorylinks_check(fname, fversion, ftype, orderid) ;

RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		"version", 
		"modificationdate",
		"modificationtype",
		"schematype",
		"cvstag",
		"description" )
VALUES	(
		'7.3.0',
		'2009-05-08',
		3,
		1,
		'SchemaVersion_7-3-0',
		'Added table t_standingdistribution. Added a fake version of procedure p_inventorylinks_check. Modified procedure p_processinventory.'
) ;
