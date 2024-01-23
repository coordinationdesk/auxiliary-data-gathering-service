INSERT INTO public.t_filetypes
(filetype, description, nameregularexpression, headerseparation, "group", invperiod, mngsngext, sngext, hdrext, dblext)
VALUES('S3_SR_2_SICNAX', 'SR_2_SICNAX auxiliary file', '^S3[ABC_]_SR_2_SICNAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*', false, 'group0', 0, false, NULL, NULL, NULL);
INSERT INTO public.t_filetypes
(filetype, description, nameregularexpression, headerseparation, "group", invperiod, mngsngext, sngext, hdrext, dblext)
VALUES('S3_SR_2_SICSAX', 'SR_2_SICSAX auxiliary file', '^S3[ABC_]_SR_2_SICSAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*', false, 'group0', 0, false, NULL, NULL, NULL);
INSERT INTO public.t_filetypes
(filetype, description, nameregularexpression, headerseparation, "group", invperiod, mngsngext, sngext, hdrext, dblext)
VALUES('S3_SR_2_SIFNAX', 'SR_2_SIFNAX auxiliary file', '^S3[ABC_]_SR_2_SIFNAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*', false, 'group0', 0, false, NULL, NULL, NULL);
INSERT INTO public.t_filetypes
(filetype, description, nameregularexpression, headerseparation, "group", invperiod, mngsngext, sngext, hdrext, dblext)
VALUES('S3_SR_2_SIFSAX', 'SR_2_SIFSAX auxiliary file', '^S3[ABC_]_SR_2_SIFSAX_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}__.*', false, 'group0', 0, false, NULL, NULL, NULL);


INSERT INTO public.t_filetypescomps
(pfiletype, cfiletype)
VALUES((SELECT id_filetype FROM t_filetypes where filetype = 'S3_VIRTUAL'), (SELECT id_filetype FROM t_filetypes where filetype = 'S3_SR_2_SICNAX'));

INSERT INTO public.t_filetypescomps
(pfiletype, cfiletype)
VALUES((SELECT id_filetype FROM t_filetypes where filetype = 'S3_VIRTUAL'), (SELECT id_filetype FROM t_filetypes where filetype = 'S3_SR_2_SICSAX'));

INSERT INTO public.t_filetypescomps
(pfiletype, cfiletype)
VALUES((SELECT id_filetype FROM t_filetypes where filetype = 'S3_VIRTUAL'), (SELECT id_filetype FROM t_filetypes where filetype = 'S3_SR_2_SIFNAX'));

INSERT INTO public.t_filetypescomps
(pfiletype, cfiletype)
VALUES((SELECT id_filetype FROM t_filetypes where filetype = 'S3_VIRTUAL'), (SELECT id_filetype FROM t_filetypes where filetype = 'S3_SR_2_SIFSAX'));
