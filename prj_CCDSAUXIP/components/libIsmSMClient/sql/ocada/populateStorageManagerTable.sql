INSERT INTO t_storagemanager (type,name,description,invrelationtable,parameters)
VALUES ( 'ACSISM','ACS_ISM_01','Standard ISM','t_acsisminvrelation01',FILETOCLOB("ism_01.conf", 'client')); 
INSERT INTO t_storagemanager (type,name,description,invrelationtable,parameters)
VALUES ( 'ACSISM','ACS_ISM_02','Standard ISM','t_acsisminvrelation02',FILETOCLOB("ism_02.conf", 'client')); 
