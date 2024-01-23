INSERT INTO t_storagemanager (type,name,description,invrelationtable,parameters)
VALUES ( 'ACSISM','ACS_ISM','Standard ISM','t_acsisminvrelation',FILETOCLOB("ism.conf", 'client')); 
