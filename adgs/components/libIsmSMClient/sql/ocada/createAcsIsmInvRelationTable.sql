create table 'informix'.t_acsisminvrelation01 (
    id INT not null,
    path lvarchar not null,
    signature NCHAR(64)
)
extent size 32 next size 32
lock mode row;

alter table 'informix'.t_acsisminvrelation01 add constraint primary key 
	(id)
	constraint pk_t_acsisminvrel_01;

alter table 'informix'.t_acsisminvrelation01 add constraint foreign key 
	(id)
	references 'informix'.t_inventorysm
	(id) 
	on delete cascade 
	constraint fk_t_acsisminvrel_01;
	
	
create table 'informix'.t_acsisminvrelation02 (
    id INT not null,
    path lvarchar not null,
    signature NCHAR(64)
)
extent size 32 next size 32
lock mode row;

alter table 'informix'.t_acsisminvrelation02 add constraint primary key 
	(id)
	constraint pk_t_acsisminvrel_02;

alter table 'informix'.t_acsisminvrelation02 add constraint foreign key 
	(id)
	references 'informix'.t_inventorysm
	(id) 
	on delete cascade 
	constraint fk_t_acsisminvrel_02;

