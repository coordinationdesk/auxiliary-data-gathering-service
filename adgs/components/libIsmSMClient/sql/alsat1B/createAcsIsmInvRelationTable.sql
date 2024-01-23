create table t_acsisminvrelation (
    id INT not null,
    path lvarchar not null,
    signature NCHAR(64)
)
extent size 32 next size 32
lock mode row;

alter table t_acsisminvrelation add constraint primary key 
	(id)
	constraint pk_t_acsisminvrel;

alter table t_acsisminvrelation add constraint foreign key 
	(id)
	references t_inventorysm
	(id) 
	on delete cascade 
	constraint fk_t_acsisminvrel;
	
	
