\c adgsdb

COPY t_filetypes FROM stdin;
1	AUX_TO_OBJSTORE	Aux Files to be distributed to DataImport	\N	f	group0	0	f	\N	\N	\N
2	CALVAL_AUX	Aux Files coming from CalVal(MPC)	\N	f	group0	0	f	\N	\N	\N
3	PRIP_AUX	Aux Files coming from PRIP	\N	f	group0	0	f	\N	\N	\N
4	EUM_AUX	Aux Files coming from EUMETSAT	\N	f	group0	0	f	\N	\N	\N
5	NPPF_AUX	NPPF Files coming from EUMETSAT	\N	f	group0	0	f	\N	\N	\N
6	NOAA_1KM_IMSDLY	NOAA IMS 1KM DAILY FILES	^ims[0-9]{7}_1km_GIS_v[0-9.]+\\.tif\\.gz$	f	group0	0	f	gz	\N	\N
7	SL_2_DIMSAX	SL_2_DIMSAX auxiliary file	^S3[ABC_]_SL_2_DIMSAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
8	AX___MA1_AX	ECMWF grib1 Analysis data	^S3[ABC_]_AX___MA1_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
9	AX___MA2_AX	ECMWF grib2 Analysis data	^S3[ABC_]_AX___MA2_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
10	AX___MF1_AX	ECMWF grib1 Forecast data	^S3[ABC_]_AX___MF1_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
11	AX___MF2_AX	ECMWF grib1 Forecast data	^S3[ABC_]_AX___MF2_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
12	AX___MFA_AX	ECMWF Forecast+Analysis data	^S3[ABC_]_AX___MFA_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
13	AX___BA__AX	Iers Bulletin auxiliary file	^S3[ABC_]_AX___BA__AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
14	AX___BB2_AX	Iers Bulletin auxiliary file	^S3[ABC_]_AX___BB2_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
15	SR___MDO_AX	SR___MDO_AX auxiliary file	^S3[ABC_]_SR___MDO_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
16	SR___POESAX	SR___POESAX auxiliary file	^S3[ABC_]_SR___POESAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
17	SR_2_PMPSAX	SR_2_PMPSAX auxiliary file	^S3[ABC_]_SR_2_PMPSAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
18	SR_2_RMO_AX	SR_2_RMO_AX auxiliary file	^S3[ABC_]_SR_2_RMO_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
19	SR_2_PMO_AX	SR_2_PMO_AX auxiliary file	^S3[ABC_]_SR_2_PMO_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
20	SR_2_POL_AX	SR_2_POL_AX auxiliary file	^S3[ABC_]_SR_2_POL_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
21	SR_2_PGI_AX	SR_2_PGI_AX auxiliary file	^S3[ABC_]_SR_2_PGI_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
22	SR_2_RGI_AX	SR_2_RGI_AX auxiliary file	^S3[ABC_]_SR_2_RGI_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
23	SR_1_USO_AX	SR_1_USO_AX auxiliary file	^S3[ABC_]_SR_1_USO_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
24	SR___MGNSAX	SR___MGNSAX auxiliary file	^S3[ABC_]_SR___MGNSAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
25	SR_2_SIC_AX	SR_2_SIC_AX auxiliary file	^S3[ABC_]_SR_2_SIC_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
26	SL_2_SSTAAX	METOffice auxiliary file	^S3[ABC_]_SL_2_SSTAAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
27	AX___FPO_AX	FOS Predicted Orbit File	^S3[ABC_]_AX___FPO_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
28	AX___FRO_AX	FOS Reconstituted Orbit File	^S3[ABC_]_AX___FRO_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
29	GN_1_MANHAX	FOS Manoeuvre Acceleration	^S3[ABC_]_GN_1_MANHAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
30	GN_1_MASSAX	FOS Sat Mass Properties Rep	^S3[ABC_]_GN_1_MASSAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
31	SR___MGNPAX	POD auxiliary file	^S3[ABC_]_SR___MGNPAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
32	SR___POEPAX	POD auxiliary file	^S3[ABC_]_SR___POEPAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
33	SR_2_PCPPAX	POD auxiliary file	^S3[ABC_]_SR_2_PCPPAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
34	SR_2_PMPPAX	POD auxiliary file	^S3[ABC_]_SR_2_PMPPAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
35	AX___OSF_AX	Ref Orbit Scenario File	^S3[ABC_]_AX___OSF_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
36	MPL_DNPPF	MPL DNPPF Aux File	^S3[ABC_]_OPER_MPL_DNPPF__[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_.*\\.[Ee][Oo][Ff]$	f	group0	0	f	\N	\N	\N
37	MPL__NPPF	MPL NPPF Aux File	^S3[ABC_]_OPER_MPL__NPPF__[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_.*\\.[Ee][Oo][Ff]$	f	group0	0	f	\N	\N	\N
38	MW_1_NIR_AX	MW_1_NIR_AX auxiliary file	^S3[ABC_]_MW_1_NIR_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
39	MW_1_DNB_AX	MW_1_DNB_AX auxiliary file	^S3[ABC_]_MW_1_DNB_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
40	MW_1_MON_AX	MW_1_MON_AX auxiliary file	^S3[ABC_]_MW_1_MON_AX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*	f	group0	0	f	\N	\N	\N
41	ECMWF_TRO	AUX_TRO files from ECMWF	\N	f	group0	0	f	\N	\N	\N
42	OSISAF_ICE	OSI SAF ICE files	\N	f	group0	0	f	\N	\N	\N
43	AUX_PP1	L1 Processor Parameters	^S1[ABC_].*AUX_PP1_.*	f	group0	0	f	\N	\N	\N
44	AUX_CAL	Calibration Parameters	^S1[ABC_].*AUX_CAL_.*	f	group0	0	f	\N	\N	\N
45	AUX_INS	Instrument Aux Data	^S1[ABC_].*AUX_INS_.*	f	group0	0	f	\N	\N	\N
46	AUX_PP2	L2 Processor Parameters	^S1[ABC_].*AUX_PP2_.*	f	group0	0	f	\N	\N	\N
47	AUX_SCS	Simulated Cross Spectra	^S1[ABC_].*AUX_SCS_.*	f	group0	0	f	\N	\N	\N
48	AUX_SCF	Aux SETAP Configuration File	^S1[ABC_].*AUX_SCF_.*	f	group0	0	f	\N	\N	\N
49	AUX_ITC	SETAP aux instr timing cal prod	^S1[ABC_].*AUX_ITC_.*	f	group0	0	f	\N	\N	\N
50	AUX_TRO_129	Tropospheric model Aux Data 129	^ECMWF_OPER_ML00_06H_129_GP_N640_[0-9]{8}T[0-9]{6}$	f	group0	0	f	\N	\N	\N
51	AUX_TRO_130	Tropospheric model Aux Data 130	^ECMWF_OPER_ML00_06H_130_GP_N640_[0-9]{8}T[0-9]{6}$	f	group0	0	f	\N	\N	\N
52	AUX_TRO_133	Tropospheric model Aux Data 133	^ECMWF_OPER_ML00_06H_133_GP_N640_[0-9]{8}T[0-9]{6}$	f	group0	0	f	\N	\N	\N
53	AUX_TRO_134	Tropospheric model Aux Data 134	^ECMWF_OPER_ML00_06H_134_GP_N640_[0-9]{8}T[0-9]{6}$	f	group0	0	f	\N	\N	\N
54	AUX_TRO_151	Tropospheric model Aux Data 151	^ECMWF_OPER_ML00_06H_151_GP_N640_[0-9]{8}T[0-9]{6}$	f	group0	0	f	\N	\N	\N
55	AUX_WND_OR	ECMWF Atmospheric Model	^[dD]1[dD][0-9]{16}1$	f	group0	0	f	\N	\N	\N
56	AUX_WAV_OR	Wavewatch III Model	^IFR_WW3-GLOBAL-30MIN.*nc$	f	group0	0	f	nc	\N	\N
57	ICE_EDGENH	Sea ice aux data North	^ice_edge_[nN][hH].*_[0-9]{12}.*nc$	f	group0	0	f	nc	\N	\N
58	ICE_EDGESH	Sea ice aux data South	^ice_edge_[sS][hH].*_[0-9]{12}.*nc$	f	group0	0	f	nc	\N	\N
59	AUX_TEC_OR	Ionospheric model Aux Data	^codg[0-9]{3}0\\.[0-9]{2}i\\.Z$	f	group0	0	f	\N	\N	\N
60	AMH_ERRMAT_OR	Excitation Coeff Error Matrix	^S1[ABC_]_(TEST|OPER|REP[1-9]|TD[0-9]{2})_AMH_ERRMAT_MPC__[0-9]{8}T[0-9]{6}_V[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}\\.EOF\\.zip$	f	group0	0	f	\N	\N	\N
61	AMV_ERRMAT_OR	Excitation Coeff Error Matrix	^S1[ABC_]_(TEST|OPER|REP[1-9]|TD[0-9]{2})_AMV_ERRMAT_MPC__[0-9]{8}T[0-9]{6}_V[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}\\.EOF\\.zip$	f	group0	0	f	\N	\N	\N
62	REP_SUP	Unavailability Reports	^S1[ABC_]_.*_REP__SUP___[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.EOF$	f	group0	0	f	EOF	\N	\N
63	MPL_ORBPRE	FOS Predicted Orbit	^S1[ABC_]_.*_MPL_ORBPRE_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.EOF$	f	group0	0	f	EOF	\N	\N
64	MPL_ORBRES	FOS Restitued Orbit	^S1[ABC_]_.*_MPL_ORBRES_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.EOF$	f	group0	0	f	EOF	\N	\N
65	MPL_TLEPRE	FOS TLE Predicted Orbit	^S1[ABC_]_.*_MPL_TLEPRE_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.EOF$	f	group0	0	f	EOF	\N	\N
66	OPER_TLM	Telemetry Parameters	^S1[ABC_]_.*_TMM__REQ_[B|C|D|E|F]_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.EOF$	f	group0	0	f	EOF	\N	\N
67	MPL_ORBSCT	Ref Orbit Scenario File	^S1[ABC_]_.*_MPL_ORBSCT_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.[tT][gG][zZ]$	f	group0	0	f	\N	\N	\N
68	MPL_ORBSCTTMP	Filetype for uncompressed data	^S1[ABC_]_.*_MPL_ORBSCT_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\\.EOF$	f	group0	0	f	\N	\N	\N
69	AUX_PREORB	POD Predicted Orbit	^S1[ABC_]_(TEST|OPER|REP[1-9]|TD[0-9]{2})_AUX_PREORB_.{4}_[0-9]{8}T[0-9]{6}_V[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}\\.EOF$	f	group0	0	f	\N	\N	\N
70	AUX_POEORB	Precise Ephemieris Orbit	^S1[ABC_]_(TEST|OPER|REP[1-9]|TD[0-9]{2})_AUX_POEORB_.{4}_[0-9]{8}T[0-9]{6}_V[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}\\.EOF$	f	group0	0	f	\N	\N	\N
71	AUX_RESORB	POD Restituted Orbit	^S1[ABC_]_(TEST|OPER|REP[1-9]|TD[0-9]{2})_AUX_RESORB_.{4}_[0-9]{8}T[0-9]{6}_V[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}\\.EOF$	f	group0	0	f	\N	\N	\N
72	AUX_RESATT	Restituted Attitude	^S1[ABC_]_(TEST|OPER|REP[1-9]|TD[0-9]{2})_AUX_RESATT_.{4}_[0-9]{8}T[0-9]{6}_V[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}\\.EOF$	f	group0	0	f	\N	\N	\N
73	AUX_TRO	Tropospheric model Aux Data	^S1[ABC_].*AUX_TRO_.*$	f	group0	0	f	\N	\N	\N
74	AUX_WND	ECMWF Atmospheric Model	^S1[ABC_].*AUX_WND_.*$	f	group0	0	f	\N	\N	\N
75	AUX_WAV	Wavewatch III Model	^S1[ABC_].*AUX_WAV_.*$	f	group0	0	f	\N	\N	\N
76	AUX_ICE	Sea ice aux data North	^S1[ABC_].*AUX_ICE_.*$	f	group0	0	f	\N	\N	\N
77	AUX_TEC	Ionospheric model Aux Data	^S1[ABC_].*AUX_TEC_.*$	f	group0	0	f	\N	\N	\N
100	CAMS_FRCST	CAM Forecast data	z_cams_c_ecmf_[0-9]{14}_prod_fc_sfc_0([0-3][0-9]|4[0-8])_(aod.*|bcaod550|duaod550|omaod550|ssaod550|suaod550).grib	f	group1	0	f	\N	\N	\N
101	CAMS_AN_Z_	CAM Forecast z data	z_cams_c_ecmf_[0-9]{14}_prod_an_sfc_000_z.grib	f	group1	0	f	\N	\N	\N
102	CAMS_MFST_	CAM Manifest file	z_cams_c_ecmf_[0-9]{14}_prod.manifest	f	group1	0	f	\N	\N	\N
103	CAM_VRTUAL	S2__VIRTUAL CAM filetype	\N	f	group1	0	f	\N	\N	\N
\.
