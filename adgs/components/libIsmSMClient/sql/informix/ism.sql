set explain ON ;
SELECT 
	ism.id, 
	ism.filename, 
	ism.fileversion,
	ism.StorageManId,
	sm.type,
	sm.name,
	sm.InvRelationTable
FROM 
	t_inventorySM ism, 
	t_storagemanager sm
WHERE 
		ism.filename="CS_OFFL_SIR1LRM_FR_20040704T104925_20040704T105441_0001" 
	AND ism.fileversion="0001" 
	AND sm.id = ism.StorageManId;
	

SELECT 
	sp.retrievecost, sp.id 
FROM 
	t_storagepriority sp
WHERE 
	sp.filetype='SIR1LRM_FR' AND sp.id in (1) ;

SELECT externalreference from t_AcsHsmInvRelation where id = 2 ;
	
set EXPLAIN OFF ;
