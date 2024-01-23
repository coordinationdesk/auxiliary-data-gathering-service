update t_storagemanager set parameters = FILETOCLOB("ism.conf", 'client') where name='ACS_ISM' ; 
