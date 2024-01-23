
-- Version usdfwa-2.2.0
-- POSTGRES

-----------------------------------------------------------------
-- ATTENZIONE!!!                                               --
-- Questa modifica è applicabile senza ulteriori interventi    --
-- sul database solo per le versioni di schema del PDS 11.4.0  --
-- o superiori.                                                --
-----------------------------------------------------------------








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* --------------------------
	p_insert_inventoryevents
*/ --------------------------
-- Nothing to do
-- The new p_insert_inventoryevents will be installed by PDS installation.








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
		'usdfwa-2.2.0',
		'2013-09-10',
		6,
		5,
		'USDFWA_2-2-0',
		'Moved (and modified) p_insert_inventoryevents procedure from USDFWA to PDS.'
) ;
