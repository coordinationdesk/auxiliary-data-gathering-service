create TABLE t_objsminvrelation (
    id INTEGER NOT NULL,
    path VARCHAR(512) NOT NULL,
    signature VARCHAR(64)
);

ALTER TABLE t_objsminvrelation
  ADD CONSTRAINT pk_t_objsminvrel
  	PRIMARY KEY (id) ;

ALTER TABLE t_objsminvrelation
    ADD CONSTRAINT fk_t_objsminvrel
		FOREIGN KEY (id)
			REFERENCES t_srv_inventorysm (id)
				ON DELETE CASCADE;
	
ALTER TABLE t_objsminvrelation
  OWNER TO acsdba;
GRANT ALL ON TABLE t_objsminvrelation TO acsdba;
GRANT SELECT, UPDATE, INSERT, DELETE ON TABLE t_objsminvrelation TO srvuser;

