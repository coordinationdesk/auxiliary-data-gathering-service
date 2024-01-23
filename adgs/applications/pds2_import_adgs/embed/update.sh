#!/bin/bash

PDS2_IMPORT_TAG=RPMLAB_REL_5_8-S3PD-ADC-pds2_import
GIT_URL=ssh://git@gitserver.acs/ACS

git archive --format=tar --remote=${GIT_URL}/pds2_import.git --prefix=pds2_import/ ${PDS2_IMPORT_TAG} | tar x
