create table t_acsisminvrelation_01 (id INT not null,path lvarchar not null,signature NCHAR(64)) extent size 32 next size 32 lock mode row;
alter table t_acsisminvrelation_01 add constraint primary key (id) constraint pk_t_acsisminvrel_01;
alter table t_acsisminvrelation_01 add constraint foreign key (id) references t_inventorysm (id) on delete cascade constraint fk_t_acsisminvrel_01;

DELETE from t_storagemanager where type='ACSISM' AND name='ACS_ISM';
DELETE from t_smtype where type='ACSISM';
INSERT INTO t_smtype  (type) VALUES ('ACSISM');
INSERT INTO t_storagemanager (type,name,description,invrelationtable,parameters) VALUES ( 'ACSISM','ACS_ISM','Standard ISM','t_acsisminvrelation_01',FILETOCLOB("/usr/informix/ism.conf", 'client')); 
