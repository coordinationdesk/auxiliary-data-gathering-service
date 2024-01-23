
-- Version 10.3.0
-- POSTGRES





/*
                           !!! WARNING !!!
				           ---------------
        This script can be executed only by schema owner user
After this patch the USDFWA inventory_serial_patch_2012_01_24 is needed

*/





---------------------------------------------------------------------------------
---------------------------- TABLES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ----------------------------------------
	Remove all dependencies to t_inventory
*/ ----------------------------------------
ALTER TABLE t_distributionitems DROP CONSTRAINT fk_t_distributionitems4 ;
ALTER TABLE t_inventorycomments DROP CONSTRAINT fk_t_inventorycomments ;
ALTER TABLE t_temporaryorders DROP CONSTRAINT fk_t_temporaryorders2 ;
ALTER TABLE t_invspecialrollingpolicies DROP CONSTRAINT fk_t_invspecialrollingpolicies1 ;
ALTER TABLE t_ordersinput DROP CONSTRAINT fk_t_ordersinput1 ;
ALTER TABLE t_inventorysm DROP CONSTRAINT fk_t_inventorysm1 ;
ALTER TABLE t_inventorygaps DROP CONSTRAINT fk_t_inventorygaps ;
ALTER TABLE t_pendinginventory DROP CONSTRAINT fk_t_pendinginventory ;
ALTER TABLE t_inventorydataset DROP CONSTRAINT fk_t_inventorydataset1 ;
ALTER TABLE t_temporarydistribution DROP CONSTRAINT fk_t_temporarydistribution4 ;
ALTER TABLE t_ordtriggeringfiles DROP CONSTRAINT fk_ordtrigger_inventory ;
ALTER TABLE t_invsiterelation DROP CONSTRAINT fk_invsiterel_inventory ;
ALTER TABLE t_cartsinv DROP CONSTRAINT fk_cartsinv_fnamefver ;


/* ----------------------------------------
	Add new fields on t_inventory
*/ ----------------------------------------
ALTER TABLE t_inventory ADD unique_id VARCHAR(160) ;
UPDATE t_inventory SET unique_id = filename || fileversion ;
ALTER TABLE t_inventory ALTER COLUMN unique_id SET NOT NULL ;
ALTER TABLE t_inventory ADD id_inv BIGSERIAL NOT NULL ;
/* ----------------------------------------
	Remove old primary key on t_inventory
*/ ----------------------------------------
ALTER TABLE t_inventory DROP CONSTRAINT pk_t_inventory CASCADE ;
/* ----------------------------------------
	Create new primary key on t_inventory
*/ ----------------------------------------
ALTER TABLE t_inventory
	ADD CONSTRAINT pk_t_inventory
		PRIMARY KEY (id_inv) ;
/* ----------------------------------------
	Add new unique constraint on t_inventory
*/ ----------------------------------------
ALTER TABLE t_inventory
	ADD CONSTRAINT uq_inventory01
		UNIQUE (filename, fileversion) ;

ALTER TABLE t_inventory
	ADD CONSTRAINT uq_inventory02
		UNIQUE (unique_id) ;


/* ------------------------------------------
	Recreate all dependencies to t_inventory
*/ ------------------------------------------
ALTER TABLE t_distributionitems
  ADD CONSTRAINT fk_t_distributionitems4
  	FOREIGN KEY (filename, fileversion)
	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_inventorycomments
  ADD CONSTRAINT fk_t_inventorycomments
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_temporaryorders
  ADD CONSTRAINT fk_t_temporaryorders2
  	FOREIGN KEY (filename, fileversion)
	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_invspecialrollingpolicies
  ADD CONSTRAINT fk_t_invspecialrollingpolicies1
  	FOREIGN KEY (inventoryname, inventoryversion)
	  REFERENCES t_inventory (filename, fileversion)
	  	ON DELETE CASCADE ;

ALTER TABLE t_ordersinput
  ADD CONSTRAINT fk_t_ordersinput1
  	FOREIGN KEY (filename, fileversion)
	  REFERENCES t_inventory (filename, fileversion)
	  	ON DELETE CASCADE ;

ALTER TABLE t_inventorysm
  ADD CONSTRAINT fk_t_inventorysm1
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_inventorygaps
  ADD CONSTRAINT fk_t_inventorygaps
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_pendinginventory
  ADD CONSTRAINT fk_t_pendinginventory
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_inventorydataset
  ADD CONSTRAINT fk_t_inventorydataset1
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution
  ADD CONSTRAINT fk_t_temporarydistribution4
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_ordtriggeringfiles 
	ADD CONSTRAINT fk_ordtrigger_inventory 
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_invsiterelation 
	ADD CONSTRAINT fk_invsiterel_inventory
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_cartsinv 
	ADD CONSTRAINT fk_cartsinv_fnamefver
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_transferprotocols (id, protocol) VALUES (4, 'FS') ;








---------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTION CREATION ---------------------------
---------------------------------------------------------------------------------

/* --------------------------
	tf_inventory_uniqueid
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_inventory_uniqueid() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.unique_id IS NULL  THEN
		NEW.unique_id := NEW.filename || NEW.fileversion ;
	END IF ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;

/* --------------------------
	tr_inventoryinsert01
*/ --------------------------
CREATE TRIGGER tr_inventoryinsert01 BEFORE INSERT
  ON t_inventory
  	FOR EACH ROW EXECUTE PROCEDURE tf_inventory_uniqueid() ;








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
		'pds-10.3.0',
		'2012-01-24',
		3,
		2,
		'PDS_10-3-0',
		'Added serial field on t_inventory and changed primary key. Created trigger tr_inventoryinsert01 and trigger function tf_inventory_uniqueid.'
) ;
