
-- KEEP

--
-- PostgreSQL database dump
--


SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET row_security = off;

SET search_path = public, pg_catalog;


--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: auth_group; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE auth_group (
    id integer NOT NULL,
    name character varying(80) NOT NULL
);


ALTER TABLE auth_group OWNER TO ecicmfuser;

--
-- Name: auth_group_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE auth_group_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE auth_group_id_seq OWNER TO ecicmfuser;

--
-- Name: auth_group_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE auth_group_id_seq OWNED BY auth_group.id;


--
-- Name: auth_group_permissions; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE auth_group_permissions (
    id integer NOT NULL,
    group_id integer NOT NULL,
    permission_id integer NOT NULL
);


ALTER TABLE auth_group_permissions OWNER TO ecicmfuser;

--
-- Name: auth_group_permissions_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE auth_group_permissions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE auth_group_permissions_id_seq OWNER TO ecicmfuser;

--
-- Name: auth_group_permissions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE auth_group_permissions_id_seq OWNED BY auth_group_permissions.id;


--
-- Name: auth_permission; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE auth_permission (
    id integer NOT NULL,
    name character varying(255) NOT NULL,
    content_type_id integer NOT NULL,
    codename character varying(100) NOT NULL
);


ALTER TABLE auth_permission OWNER TO ecicmfuser;

--
-- Name: auth_permission_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE auth_permission_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE auth_permission_id_seq OWNER TO ecicmfuser;

--
-- Name: auth_permission_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE auth_permission_id_seq OWNED BY auth_permission.id;


--
-- Name: django_content_type; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE django_content_type (
    id integer NOT NULL,
    app_label character varying(100) NOT NULL,
    model character varying(100) NOT NULL
);


ALTER TABLE django_content_type OWNER TO ecicmfuser;

--
-- Name: django_content_type_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE django_content_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE django_content_type_id_seq OWNER TO ecicmfuser;

--
-- Name: django_content_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE django_content_type_id_seq OWNED BY django_content_type.id;


--
-- Name: django_migrations; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE django_migrations (
    id integer NOT NULL,
    app character varying(255) NOT NULL,
    name character varying(255) NOT NULL,
    applied timestamp with time zone NOT NULL
);


ALTER TABLE django_migrations OWNER TO ecicmfuser;

--
-- Name: django_migrations_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE django_migrations_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE django_migrations_id_seq OWNER TO ecicmfuser;

--
-- Name: django_migrations_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE django_migrations_id_seq OWNED BY django_migrations.id;


--
-- Name: django_session; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE django_session (
    session_key character varying(40) NOT NULL,
    session_data text NOT NULL,
    expire_date timestamp with time zone NOT NULL
);


ALTER TABLE django_session OWNER TO ecicmfuser;

--
-- Name: ecicmf_wrs_fs_content; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE ecicmf_wrs_fs_content (
    id integer NOT NULL,
    name character varying(32) NOT NULL,
    area character varying(32) NOT NULL,
    instrument character varying(32) NOT NULL,
    ingestion_time timestamp with time zone NOT NULL,
    filetype_id integer
);


ALTER TABLE ecicmf_wrs_fs_content OWNER TO ecicmfuser;

--
-- Name: ecicmf_wrs_fs_content_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE ecicmf_wrs_fs_content_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE ecicmf_wrs_fs_content_id_seq OWNER TO ecicmfuser;

--
-- Name: ecicmf_wrs_fs_content_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE ecicmf_wrs_fs_content_id_seq OWNED BY ecicmf_wrs_fs_content.id;


--
-- Name: ecicmf_wrs_fs_filetype; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE ecicmf_wrs_fs_filetype (
    id integer NOT NULL,
    name character varying(32) NOT NULL
);


ALTER TABLE ecicmf_wrs_fs_filetype OWNER TO ecicmfuser;

--
-- Name: ecicmf_wrs_fs_filetype_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE ecicmf_wrs_fs_filetype_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE ecicmf_wrs_fs_filetype_id_seq OWNER TO ecicmfuser;

--
-- Name: ecicmf_wrs_fs_filetype_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE ecicmf_wrs_fs_filetype_id_seq OWNED BY ecicmf_wrs_fs_filetype.id;


--
-- Name: kipp_user; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE kipp_user (
    password character varying(128) NOT NULL,
    last_login timestamp with time zone,
    scim_id character varying(64) NOT NULL,
    username character varying(150) NOT NULL
);


ALTER TABLE kipp_user OWNER TO ecicmfuser;

--
-- Name: kipp_user_permissions; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE kipp_user_permissions (
    id integer NOT NULL,
    user_id character varying(64) NOT NULL,
    permission_id integer NOT NULL
);


ALTER TABLE kipp_user_permissions OWNER TO ecicmfuser;

--
-- Name: kipp_user_permissions_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE kipp_user_permissions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE kipp_user_permissions_id_seq OWNER TO ecicmfuser;

--
-- Name: kipp_user_permissions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE kipp_user_permissions_id_seq OWNED BY kipp_user_permissions.id;


--
-- Name: kipp_user_roles; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE kipp_user_roles (
    id integer NOT NULL,
    user_id character varying(64) NOT NULL,
    role_id integer NOT NULL
);


ALTER TABLE kipp_user_roles OWNER TO ecicmfuser;

--
-- Name: kipp_user_roles_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE kipp_user_roles_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE kipp_user_roles_id_seq OWNER TO ecicmfuser;

--
-- Name: kipp_user_roles_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE kipp_user_roles_id_seq OWNED BY kipp_user_roles.id;


--
-- Name: wasp_app; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE wasp_app (
    id integer NOT NULL,
    name character varying(64) NOT NULL
);


ALTER TABLE wasp_app OWNER TO ecicmfuser;

--
-- Name: wasp_app_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE wasp_app_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE wasp_app_id_seq OWNER TO ecicmfuser;

--
-- Name: wasp_app_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE wasp_app_id_seq OWNED BY wasp_app.id;


--
-- Name: wasp_permission; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE wasp_permission (
    id integer NOT NULL,
    codename character varying(255) NOT NULL,
    description character varying(255) NOT NULL,
    extra jsonb,
    app_id integer NOT NULL
);


ALTER TABLE wasp_permission OWNER TO ecicmfuser;

--
-- Name: wasp_permission_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE wasp_permission_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE wasp_permission_id_seq OWNER TO ecicmfuser;

--
-- Name: wasp_permission_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE wasp_permission_id_seq OWNED BY wasp_permission.id;


--
-- Name: wasp_role; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE wasp_role (
    id integer NOT NULL,
    name character varying(64) NOT NULL,
    description character varying(255) NOT NULL,
    app_id integer NOT NULL
);


ALTER TABLE wasp_role OWNER TO ecicmfuser;

--
-- Name: wasp_role_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE wasp_role_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE wasp_role_id_seq OWNER TO ecicmfuser;

--
-- Name: wasp_role_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE wasp_role_id_seq OWNED BY wasp_role.id;


--
-- Name: wasp_role_permissions; Type: TABLE; Schema: public; Owner: ecicmfuser
--

CREATE TABLE wasp_role_permissions (
    id integer NOT NULL,
    role_id integer NOT NULL,
    permission_id integer NOT NULL
);


ALTER TABLE wasp_role_permissions OWNER TO ecicmfuser;

--
-- Name: wasp_role_permissions_id_seq; Type: SEQUENCE; Schema: public; Owner: ecicmfuser
--

CREATE SEQUENCE wasp_role_permissions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE wasp_role_permissions_id_seq OWNER TO ecicmfuser;

--
-- Name: wasp_role_permissions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: ecicmfuser
--

ALTER SEQUENCE wasp_role_permissions_id_seq OWNED BY wasp_role_permissions.id;


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group ALTER COLUMN id SET DEFAULT nextval('auth_group_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group_permissions ALTER COLUMN id SET DEFAULT nextval('auth_group_permissions_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_permission ALTER COLUMN id SET DEFAULT nextval('auth_permission_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY django_content_type ALTER COLUMN id SET DEFAULT nextval('django_content_type_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY django_migrations ALTER COLUMN id SET DEFAULT nextval('django_migrations_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_content ALTER COLUMN id SET DEFAULT nextval('ecicmf_wrs_fs_content_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_filetype ALTER COLUMN id SET DEFAULT nextval('ecicmf_wrs_fs_filetype_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_permissions ALTER COLUMN id SET DEFAULT nextval('kipp_user_permissions_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_roles ALTER COLUMN id SET DEFAULT nextval('kipp_user_roles_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_app ALTER COLUMN id SET DEFAULT nextval('wasp_app_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_permission ALTER COLUMN id SET DEFAULT nextval('wasp_permission_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role ALTER COLUMN id SET DEFAULT nextval('wasp_role_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role_permissions ALTER COLUMN id SET DEFAULT nextval('wasp_role_permissions_id_seq'::regclass);


--
-- Data for Name: auth_group; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY auth_group (id, name) FROM stdin;
\.


--
-- Name: auth_group_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('auth_group_id_seq', 1, false);


--
-- Data for Name: auth_group_permissions; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY auth_group_permissions (id, group_id, permission_id) FROM stdin;
\.


--
-- Name: auth_group_permissions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('auth_group_permissions_id_seq', 1, false);


--
-- Data for Name: auth_permission; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY auth_permission (id, name, content_type_id, codename) FROM stdin;
1	Can add group	1	add_group
2	Can change group	1	change_group
3	Can delete group	1	delete_group
4	Can add permission	2	add_permission
5	Can change permission	2	change_permission
6	Can delete permission	2	delete_permission
7	Can add content type	3	add_contenttype
8	Can change content type	3	change_contenttype
9	Can delete content type	3	delete_contenttype
10	Can add session	4	add_session
11	Can change session	4	change_session
12	Can delete session	4	delete_session
13	Can add role	5	add_role
14	Can change role	5	change_role
15	Can delete role	5	delete_role
16	Can add app	6	add_app
17	Can change app	6	change_app
18	Can delete app	6	delete_app
19	Can add permission	7	add_permission
20	Can change permission	7	change_permission
21	Can delete permission	7	delete_permission
22	Can add user	8	add_user
23	Can change user	8	change_user
24	Can delete user	8	delete_user
25	Can add file type	9	add_filetype
26	Can change file type	9	change_filetype
27	Can delete file type	9	delete_filetype
28	Can download content of this type	9	download_filetype
29	Can add content	10	add_content
30	Can change content	10	change_content
31	Can delete content	10	delete_content
\.


--
-- Name: auth_permission_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('auth_permission_id_seq', 31, true);


--
-- Data for Name: django_content_type; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY django_content_type (id, app_label, model) FROM stdin;
1	auth	group
2	auth	permission
3	contenttypes	contenttype
4	sessions	session
5	wasp	role
6	wasp	app
7	wasp	permission
8	kipp	user
9	ecicmf_wrs_fs	filetype
10	ecicmf_wrs_fs	content
\.


--
-- Name: django_content_type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('django_content_type_id_seq', 10, true);


--
-- Data for Name: django_migrations; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY django_migrations (id, app, name, applied) FROM stdin;
1	contenttypes	0001_initial	2017-10-09 17:07:39.455233+02
2	contenttypes	0002_remove_content_type_name	2017-10-09 17:07:39.5337+02
3	auth	0001_initial	2017-10-09 17:07:40.047624+02
4	auth	0002_alter_permission_name_max_length	2017-10-09 17:07:40.080468+02
5	auth	0003_alter_user_email_max_length	2017-10-09 17:07:40.10496+02
6	auth	0004_alter_user_username_opts	2017-10-09 17:07:40.122494+02
7	auth	0005_alter_user_last_login_null	2017-10-09 17:07:40.14709+02
8	auth	0006_require_contenttypes_0002	2017-10-09 17:07:40.158559+02
9	auth	0007_alter_validators_add_error_messages	2017-10-09 17:07:40.18168+02
10	auth	0008_alter_user_username_max_length	2017-10-09 17:07:40.203039+02
11	ecicmf_wrs_fs	0001_initial	2017-10-09 17:07:40.560158+02
12	wasp	0001_initial	2017-10-09 17:07:41.318215+02
13	kipp	0001_initial	2017-10-09 17:07:41.997123+02
14	sessions	0001_initial	2017-10-09 17:07:42.195949+02
\.


--
-- Name: django_migrations_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('django_migrations_id_seq', 14, true);


--
-- Data for Name: django_session; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY django_session (session_key, session_data, expire_date) FROM stdin;
\.


--
-- Data for Name: ecicmf_wrs_fs_content; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY ecicmf_wrs_fs_content (id, name, area, instrument, ingestion_time, filetype_id) FROM stdin;
\.


--
-- Name: ecicmf_wrs_fs_content_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('ecicmf_wrs_fs_content_id_seq', 1, false);


--
-- Data for Name: ecicmf_wrs_fs_filetype; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY ecicmf_wrs_fs_filetype (id, name) FROM stdin;
\.


--
-- Name: ecicmf_wrs_fs_filetype_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('ecicmf_wrs_fs_filetype_id_seq', 1, false);


--
-- Data for Name: kipp_user; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY kipp_user (password, last_login, scim_id, username) FROM stdin;
\.


--
-- Data for Name: kipp_user_permissions; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY kipp_user_permissions (id, user_id, permission_id) FROM stdin;
\.


--
-- Name: kipp_user_permissions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('kipp_user_permissions_id_seq', 1, false);


--
-- Data for Name: kipp_user_roles; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY kipp_user_roles (id, user_id, role_id) FROM stdin;
\.


--
-- Name: kipp_user_roles_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('kipp_user_roles_id_seq', 1, false);


--
-- Data for Name: wasp_app; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY wasp_app (id, name) FROM stdin;
\.


--
-- Name: wasp_app_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('wasp_app_id_seq', 1, false);


--
-- Data for Name: wasp_permission; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY wasp_permission (id, codename, description, extra, app_id) FROM stdin;
\.


--
-- Name: wasp_permission_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('wasp_permission_id_seq', 1, false);


--
-- Data for Name: wasp_role; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY wasp_role (id, name, description, app_id) FROM stdin;
\.


--
-- Name: wasp_role_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('wasp_role_id_seq', 1, false);


--
-- Data for Name: wasp_role_permissions; Type: TABLE DATA; Schema: public; Owner: ecicmfuser
--

COPY wasp_role_permissions (id, role_id, permission_id) FROM stdin;
\.


--
-- Name: wasp_role_permissions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: ecicmfuser
--

SELECT pg_catalog.setval('wasp_role_permissions_id_seq', 1, false);


--
-- Name: auth_group_name_key; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group
    ADD CONSTRAINT auth_group_name_key UNIQUE (name);


--
-- Name: auth_group_permissions_group_id_permission_id_0cd325b0_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissions_group_id_permission_id_0cd325b0_uniq UNIQUE (group_id, permission_id);


--
-- Name: auth_group_permissions_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissions_pkey PRIMARY KEY (id);


--
-- Name: auth_group_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group
    ADD CONSTRAINT auth_group_pkey PRIMARY KEY (id);


--
-- Name: auth_permission_content_type_id_codename_01ab375a_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_permission
    ADD CONSTRAINT auth_permission_content_type_id_codename_01ab375a_uniq UNIQUE (content_type_id, codename);


--
-- Name: auth_permission_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_permission
    ADD CONSTRAINT auth_permission_pkey PRIMARY KEY (id);


--
-- Name: django_content_type_app_label_model_76bd3d3b_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY django_content_type
    ADD CONSTRAINT django_content_type_app_label_model_76bd3d3b_uniq UNIQUE (app_label, model);


--
-- Name: django_content_type_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY django_content_type
    ADD CONSTRAINT django_content_type_pkey PRIMARY KEY (id);


--
-- Name: django_migrations_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY django_migrations
    ADD CONSTRAINT django_migrations_pkey PRIMARY KEY (id);


--
-- Name: django_session_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY django_session
    ADD CONSTRAINT django_session_pkey PRIMARY KEY (session_key);


--
-- Name: ecicmf_wrs_fs_content_name_key; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_content
    ADD CONSTRAINT ecicmf_wrs_fs_content_name_key UNIQUE (name);


--
-- Name: ecicmf_wrs_fs_content_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_content
    ADD CONSTRAINT ecicmf_wrs_fs_content_pkey PRIMARY KEY (id);


--
-- Name: ecicmf_wrs_fs_filetype_name_key; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_filetype
    ADD CONSTRAINT ecicmf_wrs_fs_filetype_name_key UNIQUE (name);


--
-- Name: ecicmf_wrs_fs_filetype_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_filetype
    ADD CONSTRAINT ecicmf_wrs_fs_filetype_pkey PRIMARY KEY (id);


--
-- Name: kipp_user_permissions_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_permissions
    ADD CONSTRAINT kipp_user_permissions_pkey PRIMARY KEY (id);


--
-- Name: kipp_user_permissions_user_id_permission_id_aff51202_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_permissions
    ADD CONSTRAINT kipp_user_permissions_user_id_permission_id_aff51202_uniq UNIQUE (user_id, permission_id);


--
-- Name: kipp_user_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user
    ADD CONSTRAINT kipp_user_pkey PRIMARY KEY (scim_id);


--
-- Name: kipp_user_roles_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_roles
    ADD CONSTRAINT kipp_user_roles_pkey PRIMARY KEY (id);


--
-- Name: kipp_user_roles_user_id_role_id_5c731925_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_roles
    ADD CONSTRAINT kipp_user_roles_user_id_role_id_5c731925_uniq UNIQUE (user_id, role_id);


--
-- Name: kipp_user_username_key; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user
    ADD CONSTRAINT kipp_user_username_key UNIQUE (username);


--
-- Name: wasp_app_name_key; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_app
    ADD CONSTRAINT wasp_app_name_key UNIQUE (name);


--
-- Name: wasp_app_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_app
    ADD CONSTRAINT wasp_app_pkey PRIMARY KEY (id);


--
-- Name: wasp_permission_codename_app_id_b20292ee_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_permission
    ADD CONSTRAINT wasp_permission_codename_app_id_b20292ee_uniq UNIQUE (codename, app_id);


--
-- Name: wasp_permission_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_permission
    ADD CONSTRAINT wasp_permission_pkey PRIMARY KEY (id);


--
-- Name: wasp_role_name_app_id_021fa03c_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role
    ADD CONSTRAINT wasp_role_name_app_id_021fa03c_uniq UNIQUE (name, app_id);


--
-- Name: wasp_role_permissions_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role_permissions
    ADD CONSTRAINT wasp_role_permissions_pkey PRIMARY KEY (id);


--
-- Name: wasp_role_permissions_role_id_permission_id_c9d9d9a5_uniq; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role_permissions
    ADD CONSTRAINT wasp_role_permissions_role_id_permission_id_c9d9d9a5_uniq UNIQUE (role_id, permission_id);


--
-- Name: wasp_role_pkey; Type: CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role
    ADD CONSTRAINT wasp_role_pkey PRIMARY KEY (id);


--
-- Name: auth_group_name_a6ea08ec_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX auth_group_name_a6ea08ec_like ON auth_group USING btree (name varchar_pattern_ops);


--
-- Name: auth_group_permissions_group_id_b120cbf9; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX auth_group_permissions_group_id_b120cbf9 ON auth_group_permissions USING btree (group_id);


--
-- Name: auth_group_permissions_permission_id_84c5c92e; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX auth_group_permissions_permission_id_84c5c92e ON auth_group_permissions USING btree (permission_id);


--
-- Name: auth_permission_content_type_id_2f476e4b; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX auth_permission_content_type_id_2f476e4b ON auth_permission USING btree (content_type_id);


--
-- Name: django_session_expire_date_a5c62663; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX django_session_expire_date_a5c62663 ON django_session USING btree (expire_date);


--
-- Name: django_session_session_key_c0390e0f_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX django_session_session_key_c0390e0f_like ON django_session USING btree (session_key varchar_pattern_ops);


--
-- Name: ecicmf_wrs_fs_content_filetype_id_bc8798b3; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX ecicmf_wrs_fs_content_filetype_id_bc8798b3 ON ecicmf_wrs_fs_content USING btree (filetype_id);


--
-- Name: ecicmf_wrs_fs_content_ingestion_time_8b86098b; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX ecicmf_wrs_fs_content_ingestion_time_8b86098b ON ecicmf_wrs_fs_content USING btree (ingestion_time);


--
-- Name: ecicmf_wrs_fs_content_name_2e496a86_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX ecicmf_wrs_fs_content_name_2e496a86_like ON ecicmf_wrs_fs_content USING btree (name varchar_pattern_ops);


--
-- Name: ecicmf_wrs_fs_filetype_name_a54963b5_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX ecicmf_wrs_fs_filetype_name_a54963b5_like ON ecicmf_wrs_fs_filetype USING btree (name varchar_pattern_ops);


--
-- Name: kipp_user_permissions_permission_id_cd2a2d42; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_permissions_permission_id_cd2a2d42 ON kipp_user_permissions USING btree (permission_id);


--
-- Name: kipp_user_permissions_user_id_494fc605; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_permissions_user_id_494fc605 ON kipp_user_permissions USING btree (user_id);


--
-- Name: kipp_user_permissions_user_id_494fc605_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_permissions_user_id_494fc605_like ON kipp_user_permissions USING btree (user_id varchar_pattern_ops);


--
-- Name: kipp_user_roles_role_id_c868f84d; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_roles_role_id_c868f84d ON kipp_user_roles USING btree (role_id);


--
-- Name: kipp_user_roles_user_id_242016e7; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_roles_user_id_242016e7 ON kipp_user_roles USING btree (user_id);


--
-- Name: kipp_user_roles_user_id_242016e7_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_roles_user_id_242016e7_like ON kipp_user_roles USING btree (user_id varchar_pattern_ops);


--
-- Name: kipp_user_scim_id_0dd3df81_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_scim_id_0dd3df81_like ON kipp_user USING btree (scim_id varchar_pattern_ops);


--
-- Name: kipp_user_username_02d617c0_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX kipp_user_username_02d617c0_like ON kipp_user USING btree (username varchar_pattern_ops);


--
-- Name: wasp_app_name_a3e28b52_like; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX wasp_app_name_a3e28b52_like ON wasp_app USING btree (name varchar_pattern_ops);


--
-- Name: wasp_permission_app_id_ca106e8b; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX wasp_permission_app_id_ca106e8b ON wasp_permission USING btree (app_id);


--
-- Name: wasp_role_app_id_72353b84; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX wasp_role_app_id_72353b84 ON wasp_role USING btree (app_id);


--
-- Name: wasp_role_permissions_permission_id_c5d1a958; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX wasp_role_permissions_permission_id_c5d1a958 ON wasp_role_permissions USING btree (permission_id);


--
-- Name: wasp_role_permissions_role_id_e928e785; Type: INDEX; Schema: public; Owner: ecicmfuser
--

CREATE INDEX wasp_role_permissions_role_id_e928e785 ON wasp_role_permissions USING btree (role_id);


--
-- Name: auth_group_permissio_permission_id_84c5c92e_fk_auth_perm; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissio_permission_id_84c5c92e_fk_auth_perm FOREIGN KEY (permission_id) REFERENCES auth_permission(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: auth_group_permissions_group_id_b120cbf9_fk_auth_group_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_group_permissions
    ADD CONSTRAINT auth_group_permissions_group_id_b120cbf9_fk_auth_group_id FOREIGN KEY (group_id) REFERENCES auth_group(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: auth_permission_content_type_id_2f476e4b_fk_django_co; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY auth_permission
    ADD CONSTRAINT auth_permission_content_type_id_2f476e4b_fk_django_co FOREIGN KEY (content_type_id) REFERENCES django_content_type(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: ecicmf_wrs_fs_conten_filetype_id_bc8798b3_fk_ecicmf_wr; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY ecicmf_wrs_fs_content
    ADD CONSTRAINT ecicmf_wrs_fs_conten_filetype_id_bc8798b3_fk_ecicmf_wr FOREIGN KEY (filetype_id) REFERENCES ecicmf_wrs_fs_filetype(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: kipp_user_permission_permission_id_cd2a2d42_fk_wasp_perm; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_permissions
    ADD CONSTRAINT kipp_user_permission_permission_id_cd2a2d42_fk_wasp_perm FOREIGN KEY (permission_id) REFERENCES wasp_permission(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: kipp_user_permissions_user_id_494fc605_fk_kipp_user_scim_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_permissions
    ADD CONSTRAINT kipp_user_permissions_user_id_494fc605_fk_kipp_user_scim_id FOREIGN KEY (user_id) REFERENCES kipp_user(scim_id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: kipp_user_roles_role_id_c868f84d_fk_wasp_role_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_roles
    ADD CONSTRAINT kipp_user_roles_role_id_c868f84d_fk_wasp_role_id FOREIGN KEY (role_id) REFERENCES wasp_role(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: kipp_user_roles_user_id_242016e7_fk_kipp_user_scim_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_user_roles
    ADD CONSTRAINT kipp_user_roles_user_id_242016e7_fk_kipp_user_scim_id FOREIGN KEY (user_id) REFERENCES kipp_user(scim_id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: wasp_permission_app_id_ca106e8b_fk_wasp_app_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_permission
    ADD CONSTRAINT wasp_permission_app_id_ca106e8b_fk_wasp_app_id FOREIGN KEY (app_id) REFERENCES wasp_app(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: wasp_role_app_id_72353b84_fk_wasp_app_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role
    ADD CONSTRAINT wasp_role_app_id_72353b84_fk_wasp_app_id FOREIGN KEY (app_id) REFERENCES wasp_app(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: wasp_role_permission_permission_id_c5d1a958_fk_wasp_perm; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role_permissions
    ADD CONSTRAINT wasp_role_permission_permission_id_c5d1a958_fk_wasp_perm FOREIGN KEY (permission_id) REFERENCES wasp_permission(id) DEFERRABLE INITIALLY DEFERRED;


--
-- Name: wasp_role_permissions_role_id_e928e785_fk_wasp_role_id; Type: FK CONSTRAINT; Schema: public; Owner: ecicmfuser
--

ALTER TABLE ONLY wasp_role_permissions
    ADD CONSTRAINT wasp_role_permissions_role_id_e928e785_fk_wasp_role_id FOREIGN KEY (role_id) REFERENCES wasp_role(id) DEFERRABLE INITIALLY DEFERRED;




--
-- Name: pdgs_preferences_preference; Type: TABLE; Owner: ecicmfuser
--

CREATE TABLE pdgs_preferences_preference (
    id integer NOT NULL,
    interface_class character varying(300) NOT NULL,
    target_id character varying(300) NOT NULL,
    preference text NOT NULL,
    user_id character varying(64) NOT NULL
);


ALTER TABLE pdgs_preferences_preference OWNER TO ecicmfuser;

--
-- Name: pdgs_preferences_preference_id_seq; Type: SEQUENCE; Owner: ecicmfuser
--

CREATE SEQUENCE pdgs_preferences_preference_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE pdgs_preferences_preference_id_seq OWNER TO ecicmfuser;

--
-- Name: pdgs_preferences_preference_id_seq; Type: SEQUENCE OWNED BY; Owner: ecicmfuser
--

ALTER SEQUENCE pdgs_preferences_preference_id_seq OWNED BY pdgs_preferences_preference.id;


--
-- Name: pdgs_preferences_preference id; Type: DEFAULT; Owner: ecicmfuser
--

ALTER TABLE ONLY pdgs_preferences_preference ALTER COLUMN id SET DEFAULT nextval('pdgs_preferences_preference_id_seq'::regclass);


--
-- Name: pdgs_preferences_preference pdgs_preferences_prefere_user_id_interface_class__13f2528b_uniq; Type: CONSTRAINT; Owner: ecicmfuser
--

ALTER TABLE ONLY pdgs_preferences_preference
    ADD CONSTRAINT pdgs_preferences_prefere_user_id_interface_class__13f2528b_uniq UNIQUE (user_id, interface_class, target_id);


--
-- Name: pdgs_preferences_preference pdgs_preferences_preference_pkey; Type: CONSTRAINT; Owner: ecicmfuser
--

ALTER TABLE ONLY pdgs_preferences_preference
    ADD CONSTRAINT pdgs_preferences_preference_pkey PRIMARY KEY (id);


--
-- Name: pdgs_preferences_preference_user_id_643938cc; Type: INDEX; Owner: ecicmfuser
--

CREATE INDEX pdgs_preferences_preference_user_id_643938cc ON pdgs_preferences_preference USING btree (user_id);


--
-- Name: pdgs_preferences_preference_user_id_643938cc_like; Type: INDEX; Owner: ecicmfuser
--

CREATE INDEX pdgs_preferences_preference_user_id_643938cc_like ON pdgs_preferences_preference USING btree (user_id varchar_pattern_ops);


--
-- Name: pdgs_preferences_preference pdgs_preferences_pre_user_id_643938cc_fk_kipp_user; Type: FK CONSTRAINT; Owner: ecicmfuser
--

ALTER TABLE ONLY pdgs_preferences_preference
    ADD CONSTRAINT pdgs_preferences_pre_user_id_643938cc_fk_kipp_user FOREIGN KEY (user_id) REFERENCES kipp_user(scim_id) DEFERRABLE INITIALLY DEFERRED;



--
-- Name: kipp_wso2role; Type: TABLE; Schema: ecicmf_auth; Owner: ecicmfuser
--

CREATE TABLE kipp_wso2role (
    role_ptr_id integer NOT NULL,
    scim_id character varying(64) NOT NULL
);


ALTER TABLE kipp_wso2role OWNER TO ecicmfuser;

--
-- Name: kipp_wso2role kipp_wso2role_pkey; Type: CONSTRAINT; Schema: ecicmf_auth; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_wso2role
    ADD CONSTRAINT kipp_wso2role_pkey PRIMARY KEY (role_ptr_id);


--
-- Name: kipp_wso2role kipp_wso2role_scim_id_key; Type: CONSTRAINT; Schema: ecicmf_auth; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_wso2role
    ADD CONSTRAINT kipp_wso2role_scim_id_key UNIQUE (scim_id);


--
-- Name: kipp_wso2role_scim_id_78d20ba5_like; Type: INDEX; Schema: ecicmf_auth; Owner: ecicmfuser
--

CREATE INDEX kipp_wso2role_scim_id_78d20ba5_like ON kipp_wso2role USING btree (scim_id varchar_pattern_ops);


--
-- Name: kipp_wso2role kipp_wso2role_role_ptr_id_a9486622_fk_wasp_role_id; Type: FK CONSTRAINT; Schema: ecicmf_auth; Owner: ecicmfuser
--

ALTER TABLE ONLY kipp_wso2role
    ADD CONSTRAINT kipp_wso2role_role_ptr_id_a9486622_fk_wasp_role_id FOREIGN KEY (role_ptr_id) REFERENCES wasp_role(id) DEFERRABLE INITIALLY DEFERRED;



--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--



