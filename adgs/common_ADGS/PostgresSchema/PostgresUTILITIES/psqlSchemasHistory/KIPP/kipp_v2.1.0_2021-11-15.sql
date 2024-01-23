
-- Version 2.1.0
-- POSTGRES





--
-- Name: django_content_type; Type: TABLE; Schema: public;
--
CREATE TABLE django_content_type (
    id SERIAL NOT NULL,
    app_label VARCHAR(100) NOT NULL,
    model VARCHAR(100) NOT NULL
);

ALTER TABLE ONLY django_content_type
    ADD CONSTRAINT django_content_type_pkey PRIMARY KEY (id);

ALTER TABLE ONLY django_content_type
    ADD CONSTRAINT uq_django_content_type_app_label_model UNIQUE (app_label, model);




--
-- Name: django_migrations; Type: TABLE; Schema: public;
--
CREATE TABLE django_migrations (
    id SERIAL NOT NULL,
    app VARCHAR(255) NOT NULL,
    name VARCHAR(255) NOT NULL,
    applied TIMESTAMP WITH TIME ZONE NOT NULL
);

ALTER TABLE ONLY django_migrations
    ADD CONSTRAINT django_migrations_pkey PRIMARY KEY (id);




--
-- Name: django_session; Type: TABLE; Schema: public;
--
CREATE TABLE django_session (
    session_key VARCHAR(40) NOT NULL,
    session_data text NOT NULL,
    expire_date TIMESTAMP WITH TIME ZONE NOT NULL
);

ALTER TABLE ONLY django_session
    ADD CONSTRAINT django_session_pkey PRIMARY KEY (session_key);

CREATE INDEX ix_django_session_expire_date ON django_session USING btree (expire_date);
CREATE INDEX ix_django_session_session_key ON django_session USING btree (session_key varchar_pattern_ops);




--
-- Name: auth_group; Type: TABLE; Schema: public;
--
CREATE TABLE auth_group (
    id SERIAL NOT NULL,
    name VARCHAR(150) NOT NULL
);

ALTER TABLE ONLY auth_group
    ADD CONSTRAINT auth_group_pkey PRIMARY KEY (id);

ALTER TABLE ONLY auth_group
    ADD CONSTRAINT uq_auth_group_name UNIQUE (name);

CREATE INDEX ix_auth_group_name ON auth_group USING btree (name varchar_pattern_ops);




--
-- Name: auth_permission; Type: TABLE; Schema: public;
--
CREATE TABLE auth_permission (
    id SERIAL NOT NULL,
    name VARCHAR(255) NOT NULL,
    content_type_id INTEGER NOT NULL,
    codename VARCHAR(100) NOT NULL
);

ALTER TABLE ONLY auth_permission
    ADD CONSTRAINT auth_permission_pkey PRIMARY KEY (id);

ALTER TABLE ONLY auth_permission
    ADD CONSTRAINT uq_auth_permission_content_type_id_codename UNIQUE (content_type_id, codename);

ALTER TABLE ONLY auth_permission
    ADD CONSTRAINT auth_permission_content_type_id_2f476e4b_fk_django_co 
    FOREIGN KEY (content_type_id) REFERENCES django_content_type(id) 
    DEFERRABLE INITIALLY DEFERRED;

CREATE INDEX ix_auth_permission_content_type_id ON auth_permission USING btree (content_type_id);




--
-- Name: auth_group_permissions; Type: TABLE; Schema: public;
--
CREATE TABLE auth_group_permissions (
    id SERIAL NOT NULL,
    group_id INTEGER NOT NULL,
    permission_id INTEGER NOT NULL
);

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissions_pkey PRIMARY KEY (id);

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT uq_auth_group_permissions_group_id_permission_id UNIQUE (group_id, permission_id);

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissio_permission_id_84c5c92e_fk_auth_perm 
    FOREIGN KEY (permission_id) REFERENCES auth_permission(id) 
    DEFERRABLE INITIALLY DEFERRED;

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissions_group_id_b120cbf9_fk_auth_group_id 
    FOREIGN KEY (group_id) REFERENCES auth_group(id) 
    DEFERRABLE INITIALLY DEFERRED;

CREATE INDEX ix_auth_group_permissions_group_id ON auth_group_permissions USING btree (group_id);
CREATE INDEX ix_auth_group_permissions_permission_id ON auth_group_permissions USING btree (permission_id);




--
-- Name: auth_user; Type: TABLE; Schema: public;
--
CREATE TABLE auth_user (
    id SERIAL NOT NULL,
    password VARCHAR(128) NOT NULL,
    last_login TIMESTAMP WITH TIME ZONE,
    is_superuser BOOLEAN NOT NULL,
    username VARCHAR(150) NOT NULL,
    first_name VARCHAR(30) NOT NULL,
    last_name VARCHAR(150) NOT NULL,
    email VARCHAR(254) NOT NULL,
    is_staff BOOLEAN NOT NULL,
    is_active BOOLEAN NOT NULL,
    date_joined TIMESTAMP WITH TIME ZONE NOT NULL
);

ALTER TABLE ONLY auth_user
    ADD CONSTRAINT auth_user_pkey PRIMARY KEY (id);

ALTER TABLE ONLY auth_user
    ADD CONSTRAINT uq_auth_user_username UNIQUE (username);

CREATE INDEX ix_auth_user_username ON auth_user USING btree (username varchar_pattern_ops);




--
-- Name: auth_user_groups; Type: TABLE; Schema: public;
--
CREATE TABLE auth_user_groups (
    id SERIAL NOT NULL,
    user_id INTEGER NOT NULL,
    group_id INTEGER NOT NULL
);

ALTER TABLE ONLY auth_user_groups
    ADD CONSTRAINT auth_user_groups_pkey PRIMARY KEY (id);

ALTER TABLE ONLY auth_user_groups
    ADD CONSTRAINT uq_auth_user_groups_user_id_group_id UNIQUE (user_id, group_id);

ALTER TABLE ONLY auth_user_groups
    ADD CONSTRAINT auth_user_groups_group_id_97559544_fk_auth_group_id 
    FOREIGN KEY (group_id) REFERENCES auth_group(id) 
    DEFERRABLE INITIALLY DEFERRED;

ALTER TABLE ONLY auth_user_groups
    ADD CONSTRAINT auth_user_groups_user_id_6a12ed8b_fk_auth_user_id 
    FOREIGN KEY (user_id) REFERENCES auth_user(id) 
    DEFERRABLE INITIALLY DEFERRED;

CREATE INDEX ix_auth_user_groups_group_id ON auth_user_groups USING btree (group_id);
CREATE INDEX ix_auth_user_groups_user_id ON auth_user_groups USING btree (user_id);




--
-- Name: auth_user_user_permissions; Type: TABLE; Schema: public;
--
CREATE TABLE auth_user_user_permissions (
    id SERIAL NOT NULL,
    user_id INTEGER NOT NULL,
    permission_id INTEGER NOT NULL
);

ALTER TABLE ONLY auth_user_user_permissions
    ADD CONSTRAINT auth_user_user_permissions_pkey PRIMARY KEY (id);

ALTER TABLE ONLY auth_user_user_permissions
    ADD CONSTRAINT uq_auth_user_user_permissions_user_id_permission_id UNIQUE (user_id, permission_id);

ALTER TABLE ONLY auth_user_user_permissions
    ADD CONSTRAINT auth_user_user_permi_permission_id_1fbb5f2c_fk_auth_perm 
    FOREIGN KEY (permission_id) REFERENCES auth_permission(id) 
    DEFERRABLE INITIALLY DEFERRED;

ALTER TABLE ONLY auth_user_user_permissions
    ADD CONSTRAINT auth_user_user_permissions_user_id_a95ead1b_fk_auth_user_id 
    FOREIGN KEY (user_id) REFERENCES auth_user(id) 
    DEFERRABLE INITIALLY DEFERRED;

CREATE INDEX ix_auth_user_user_permissions_permission_id ON auth_user_user_permissions USING btree (permission_id);
CREATE INDEX ix_auth_user_user_permissions_user_id ON auth_user_user_permissions USING btree (user_id);




--
-- Name: pdgs_user_preferences_pdgsuser; Type: TABLE; Schema: public;
--
CREATE TABLE public.pdgs_user_preferences_pdgsuser (
    id SERIAL NOT NULL,
    preferences TEXT NOT NULL,
    user_id INTEGER NOT NULL
);

ALTER TABLE ONLY public.pdgs_user_preferences_pdgsuser
    ADD CONSTRAINT pdgs_user_preferences_pdgsuser_pkey PRIMARY KEY (id);

ALTER TABLE ONLY public.pdgs_user_preferences_pdgsuser
    ADD CONSTRAINT uq_pdgs_user_preferences_pdgsuser_user_id UNIQUE (user_id);

ALTER TABLE ONLY public.pdgs_user_preferences_pdgsuser
    ADD CONSTRAINT pdgs_user_preferences_pdgsuser_user_id_bb0a2048_fk_auth_user_id 
    FOREIGN KEY (user_id) REFERENCES public.auth_user(id) 
    DEFERRABLE INITIALLY DEFERRED;




