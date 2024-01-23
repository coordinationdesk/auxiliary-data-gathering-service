#
#	Copyright 1995-2014, Advanced Computer Systems , Inc.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Advanced Computer Systems, Inc.
#
#
#	$Author$
#
#	$Prod: A.C.S. Project Makefile $
#
#	$Id$
#
#   $Log$
#
#   Aligned to CVS version 1.55 


SHELL=/bin/bash
MAKE_ABORT_ON_ERROR?=

LOWERCASEPROJECT=$(shell echo $${PROJECT} | sed -e 's/\(.*\)/\L\1/' )


all:: links components applications

md5: cleanlocalmd5 componentsmd5 applicationsmd5
	@echo Collecting MD5 signatures in md5result.csv file;
	@for name in `find . -name md5result.csv`; do \
	cat $$name >> md5result.csv; \
	done;

cleanmd5: cleanlocalmd5 cleancomponentsmd5 cleanapplicationsmd5

cleanlocalmd5:
	@rm -f md5result.csv;
	
cleancomponentsmd5:
	@cd components; $(MAKE) cleanmd5; result=$$?; cd ..; exit $$result ;

cleanapplicationsmd5:
	@cd applications; $(MAKE) cleanmd5; result=$$?; cd ..; exit $$result ;

componentsmd5:
	@cd components; $(MAKE) md5; result=$$?; cd ..; exit $$result ;

applicationsmd5:
	@cd applications; $(MAKE) md5; result=$$?; cd ..; exit $$result ;


abortOnError:
	@$(MAKE) MAKE_ABORT_ON_ERROR=1 all ;


verify: verifyapp verifycomp

verifyapp:
	@cd applications; $(MAKE) MAKE_ABORT_ON_ERROR=1 applicationsverify; result=$$?; cd .. ; exit $$result ;

verifycomp:
	@cd components; $(MAKE) MAKE_ABORT_ON_ERROR=1 componentsverify; result=$$?; cd .. ; exit $$result ;


refresh:
	cd components; $(MAKE) refresh; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;
	cd applications; $(MAKE) refresh; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;

nightbuild: verify
	@$(MAKE) MAKE_ABORT_ON_ERROR=1 cleandepend;
	@$(MAKE) MAKE_ABORT_ON_ERROR=1 safeupdate;
	@$(MAKE) abortOnError;

components::
	cd components; $(MAKE) ; result=$$?;  cd .. ; \
	if [ "$(MAKE_ABORT_ON_ERROR)" != "" -a "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;

applications::
	cd applications; $(MAKE) ; result=$$?; cd .. ; \
	if [ "$(MAKE_ABORT_ON_ERROR)" != "" -a "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;

install:: links all
	cd applications; $(MAKE) install; cd ..

checkout:: checkoutMake links
	cd applications; $(MAKE) checkout; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;
	cd components; $(MAKE) checkout; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;

update:: updateMake links 
	cd applications; $(MAKE) update; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;
	cd components; $(MAKE) update; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;

safeupdatecomp:: links 
	@cd components; $(MAKE) safeupdate ; result=$$?;  cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;


safeupdateapp:: links 
	@cd applications; $(MAKE) safeupdate ; result=$$?;  cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;

safeupdatecommon:
	@toolspath=$$(pwd)/tools ;\
	for ELEMENT in common_$(PROJECT) $(LOWERCASEPROJECT)_extrpm $(LOWERCASEPROJECT)_inst_scripts; do \
		if [ -d $${ELEMENT} ] ; then \
			echo -n "Updating $${ELEMENT} - " ;\
			pushd $${ELEMENT}  &> /dev/null ; \
				newversion=$$( $${toolspath}/modulestatus -u 2> /dev/null); \
				echo $${newversion} ; \
				$${toolspath}/gitupdatetobranch $${newversion} -n ; result=$$? ; \
				if [ "$$result" != "0" ] ; then exit $$result; fi; \
			popd &> /dev/null; \
		else \
			echo "Checking $${ELEMENT} - not found"; \
		fi ; \
	done ;


safeupdate:: safeupdatecommon safeupdatecomp safeupdateapp 

uptostablecommon:
	@toolspath=$$(pwd)/tools ;\
	for ELEMENT in common_$(PROJECT) $(LOWERCASEPROJECT)_extrpm $(LOWERCASEPROJECT)_inst_scripts; do \
		if [ -d $${ELEMENT} ] ; then \
			echo -n "Updating $${ELEMENT} - " ;\
			pushd $${ELEMENT}  &> /dev/null ; \
				newversion=$$( $${toolspath}/modulestatus -l 2> /dev/null); \
				echo $${newversion} ; \
				$${toolspath}/gitupdatetobranch $${newversion} -n ; result=$$? ; \
				if [ "$$result" != "0" ] ; then exit $$result; fi; \
			popd &> /dev/null; \
		else \
			echo "Checking $${ELEMENT} - not found"; \
		fi ; \
	done ;


uptostable:: uptostablecommon links 
	@cd applications; $(MAKE) uptostable; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;
	@cd components; $(MAKE) uptostable; result=$$? ; cd .. ; \
	if [ "$$result" != "0" ] ; then \
		exit $$result ;\
	fi ;


clobber: links clobbercomp clobberapp

clobbercomp::
	cd components; $(MAKE) clobber ; cd ..

clobberapp::
	cd applications; $(MAKE) clobber ; cd ..


clean: links cleancomp cleanapp cleanconftemplate

cleancomp::
	cd components; $(MAKE) clean ; cd ..

cleanapp::
	cd applications; $(MAKE) clean ; cd ..

cleancompdepend:
	cd components; $(MAKE)  cleandepend; cd ..

cleanappdepend:
	cd applications; $(MAKE)  cleandepend; cd ..

cleandepend: cleancompdepend cleanappdepend

cleanall: links cleanmd5 cleancompall cleanappall cleanconftemplate

cleancompall::
	cd components; $(MAKE) cleanall ; cd ..

cleanappall::
	cd applications; $(MAKE) cleanall ; cd ..

depend: links dependcomp dependapp

dependcomp::
	cd components; $(MAKE) depend ; cd ..

dependapp::
	cd applications; $(MAKE) depend ; cd ..

status: links statuscomp statusapp

statuscomp::
	cd components; $(MAKE) status ; cd ..

statusapp::
	cd applications; $(MAKE) status ; cd ..

statustag:
	@toolspath=$$(pwd)/tools ;\
	for ELEMENT in tools common_$(PROJECT) GITPortingTools $(LOWERCASEPROJECT)_extrpm $(LOWERCASEPROJECT)_inst_scripts; do \
		if [ -d $${ELEMENT} ] ; then \
			echo -n "Checking $${ELEMENT} - " ;\
			pushd $${ELEMENT}  &> /dev/null ; $${toolspath}/modulestatus -s ; popd &> /dev/null; \
		else \
			echo "Checking $${ELEMENT} - not found"; \
		fi ; \
	done ;
	@echo "Entering components..." ;\
	pushd components &> /dev/null; \
	$(MAKE) statustag ; \
	popd &> /dev/null;
	@echo "Entering applications..." ;\
	pushd applications &> /dev/null; \
	$(MAKE) statustag ; \
	popd &> /dev/null; 

links::
	@if [ -e applications/Makefile ] ; then \
		if [ ! -h applications/Makefile ] ; then \
			echo "WARNING applications/Makefile is not a simbolic link" ; \
		fi ; \
	else \
		echo creating applications/Makefile as simbolic link; \
		cd applications; ln -s applicationsMake/Makefile . ; cd .. ; \
	fi ;
	@if [ -e components/Makefile ] ; then \
		if [ ! -h components/Makefile ] ; then \
			echo "WARNING components/Makefile is not a simbolic link" ; \
		fi ; \
	else \
		echo creating components/Makefile as simbolic link; \
		cd components; ln -s componentsMake/Makefile . ; cd .. ; \
	fi ;


migrateTagToGit: checkDisabled createTagProjectCommon migrateCommonEnv checkout migrateSourceModules

migrateHeadToGit: checkDisabled createHeadProjectCommon migrateHeadCommonEnv checkoutMaster createAndPushReleaseFile


#DISABLE THIS TO AVOID MIGRATION OF HEAD TO GIT
#MUST BE DISABLED AFTER THE REAL MIGRATION!
ENABLE_HEAD_MIGRATION=OK

checkDisabled:
	@if [ "$$(umask)" != "0002" ] ; then echo "Set umask to 0002"; exit 1; fi;
	@if [ "$(ENABLE_HEAD_MIGRATION)" == "" ]; then echo "Operation disabled"; exit 1; fi;

createAndPushReleaseFile: 
	@RelFileList=$$(find applications components -maxdepth 1 -mindepth 1 -type d | grep -v applicationsMake | grep -v componentsMake) ; \
	for name in $${RelFileList}; \
		do category=$$(echo $${name} | cut -d'/' -f1 ) ; \
		module=$$(echo $${name} | cut -d'/' -f2 ) ; \
		echo "Category: $${category} - Module: $${module}"; \
		pushd $${category}/$${module} > /dev/null;\
		if [ ! -f ReleaseFile ] ; then \
			echo "ReleaseFile not found." ; \
			echo 1.0.0-MASTER > ./ReleaseFile; \
			echo "" >> ./ReleaseFile; \
			echo "Automatic initial release file for module" >> ./ReleaseFile; \
			git add ReleaseFile; \
			git commit -am "Importing ReleaseFile on MASTER branch" ; \
			git push origin master ; \
			CURV=$$(getmoduleversion) ; \
			gitcreatepushtag $${CURV} force ; result=$$? ; \
			if [  "$$result" != "0" ] ; then \
				exit 1; \
			fi ;\
		else \
			echo "ReleaseFile found, nothing to do" ; \
		fi; \
		popd > /dev/null;\
	done;


checkoutMaster: 
	@export MAKECOTAG="" ; $(MAKE) checkout


createHeadProjectCommon:
	@rm -rf workingArea; \
	mkdir workingArea; \
	pushd workingArea &> /dev/null; \
	git clone -q $(GITREPO)/GITPortingTools.git; result=$$?; \
	if [  "$$result" != "0" ] ; then popd; exit $$result ; fi ; \
	git clone -q $(GITREPO)/common_$(PROJECT).git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then \
		echo "Please create the repo common_$(PROJECT) on GIT server." ; \
		exit $$result ; \
	fi ; \
	pushd common_$(PROJECT) ; \
	needmigration=$$(git tag | grep -Fx MASTER_GIT) ;\
	if [ "$${needmigration}" == "MASTER_GIT" ]; then \
		echo "common_$(PROJECT) Already migrated to MASTER_GIT" ;\
		exit 0; \
	fi; \
	rm -rf componentsMake applicationsMake commonMake ProjectEnv rpms rpmsbuildroot kickstart; \
	popd ; \
	git clone -q $(GITREPO)/componentsMake.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/componentsMake; \
	cp componentsMake/$(PROJECT).* common_$(PROJECT)/componentsMake; \
	git clone -q $(GITREPO)/applicationsMake.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/applicationsMake; \
	cp applicationsMake/$(PROJECT).* common_$(PROJECT)/applicationsMake; \
	git clone -q $(GITREPO)/common.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/commonMake; \
	cp -r common/$(PROJECT)* common_$(PROJECT)/commonMake; \
	git clone -q $(GITREPO)/ProjectEnv.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/ProjectEnv; \
	cp -r ProjectEnv/$(LOWERCASEPROJECT)* common_$(PROJECT)/ProjectEnv; \
	git clone -q $(GITREPO)/rpms.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/rpms; \
	cp -r rpms/$(PROJECT)* common_$(PROJECT)/rpms; \
	git clone -q $(GITREPO)/rpmsbuildroot.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/rpmsbuildroot; \
	cp -r rpmsbuildroot/$(PROJECT)* common_$(PROJECT)/rpmsbuildroot; \
	mkdir common_$(PROJECT)/kickstart; \
	pushd common_$(PROJECT) ; \
	git add --all ;\
	git commit -m "Automatic first commit of module" ; \
	git tag -a MASTER_GIT -m "Tagging the first release of module"; \
	git push --tags ; \
	git push origin master ; \
	popd; \
	popd;


createTagProjectCommon:
	@if [ -z "$(MAKECOTAG)" ] ; then echo "No tag specified. Aborting"; exit 1; fi ;
	@rm -rf workingArea; \
	mkdir workingArea; \
	pushd workingArea &> /dev/null; \
	git clone -q $(GITREPO)/GITPortingTools.git; result=$$?; \
	if [  "$$result" != "0" ] ; then popd; exit $$result ; fi ; \
	git clone -q $(GITREPO)/common_$(PROJECT).git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then \
		echo "Please create the repo common_$(PROJECT) on GIT server." ; \
		exit $$result ; \
	fi ; \
	pushd common_$(PROJECT); rm -rf *; popd ; \
	git clone -q $(GITREPO)/componentsMake.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	pushd componentsMake; git checkout $(MAKECOTAG); result=$$?; popd; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/componentsMake; \
	cp componentsMake/$(PROJECT).* common_$(PROJECT)/componentsMake; \
	\
	git clone -q $(GITREPO)/applicationsMake.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	pushd applicationsMake; git checkout $(MAKECOTAG); result=$$?; popd; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/applicationsMake; \
	cp applicationsMake/$(PROJECT).* common_$(PROJECT)/applicationsMake; \
	\
	git clone -q $(GITREPO)/common.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	pushd common; git checkout $(MAKECOTAG); result=$$?; popd; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/commonMake; \
	cp -r common/$(PROJECT)* common_$(PROJECT)/commonMake; \
	\
	git clone -q $(GITREPO)/ProjectEnv.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	pushd ProjectEnv; git checkout $(MAKECOTAG); result=$$?; popd; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/ProjectEnv; \
	cp -r ProjectEnv/$(LOWERCASEPROJECT)* common_$(PROJECT)/ProjectEnv; \
	\
	git clone -q $(GITREPO)/rpms.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	pushd rpms; git checkout $(MAKECOTAG); result=$$?; popd; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/rpms; \
	cp -r rpms/$(PROJECT)* common_$(PROJECT)/rpms; \
	\
	git clone -q $(GITREPO)/rpmsbuildroot.git &> /dev/null; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	pushd rpmsbuildroot; git checkout $(MAKECOTAG); result=$$?; popd; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mkdir common_$(PROJECT)/rpmsbuildroot; \
	cp -r rpmsbuildroot/$(PROJECT)* common_$(PROJECT)/rpmsbuildroot; \
	\
	mkdir common_$(PROJECT)/kickstart; \
	pushd common_$(PROJECT) ; \
	\
	git tag -d $(MAKECOTAG) ; \
	git branch -D $(MAKECOTAG)_GITBRANCH ; \
	git push origin :$(MAKECOTAG)_GITBRANCH ; \
	git push origin :$(MAKECOTAG) ; \
	git fetch --all --prune ; \
	git checkout -b $(MAKECOTAG)_GITBRANCH ;\
	git add * ;\
	git commit -a -m "Automatic commit due to $(MAKECOTAG) porting" ;\
	git push origin $(MAKECOTAG)_GITBRANCH ; \
	git tag -a $(MAKECOTAG) -m "New $(MAKECOTAG) project tag inclutind GIT tools";\
	git push --tags ;\
	git checkout $(MAKECOTAG); \
	git push origin :$(MAKECOTAG)_GITBRANCH ;\
	popd; \
	popd;
	


migrateHeadCommonEnv:
	@rm -rf tools; \
	rm -rf CryosatTools; \
	rm -rf GITPortingTools; \
	git clone -q $(GITREPO)/GITPortingTools.git; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; 
	@git clone -q  $(GITREPO)/CryosatTools.git; result=$$? ; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mv CryosatTools tools ; \
	cp GITPortingTools/tools/* tools ; \
	pushd tools ; \
	needmigration=$$(git tag | grep -Fx MASTER_GIT) ;\
	if [ "$${needmigration}" == "" ]; then \
		git add * ;\
		git commit -a -m "Automatic change to include GIT tools" ;\
		git push; \
		git tag -a MASTER_GIT -m "New MASTER_GIT project tag inclutind GIT tools";\
		git push --tags ; \
	else \
		echo "Already migrated to MASTER_GIT" ;\
	fi ;\
	popd ; 
	


migrateSourceModules:
	@if [ -z "$(MAKECOTAG)" ] ; then echo "No tag specified. Aborting"; exit 1; fi ;
	@RelFileList=$$(find applications components -maxdepth 1 -mindepth 1 -type d | grep -v applicationsMake | grep -v componentsMake) ; \
	for name in $${RelFileList}; \
		do category=$$(echo $${name} | cut -d'/' -f1 ) ; \
		module=$$(echo $${name} | cut -d'/' -f2 ) ; \
		echo "Category: $${category} - Module: $${module}"; \
		pushd $${category}/$${module} ;\
		if [ ! -f ReleaseFile ] ; then \
			echo "ReleaseFile not found creating now for this release only" ; \
			echo 1.0.0-$(MAKECOTAG) > ./ReleaseFile ; \
			echo "" >> ./ReleaseFile; \
			echo "Automatic initial release file for module" >> ./ReleaseFile; \
			git add ReleaseFile ; \
			git commit -m "Automatic initial release for module" ; \
			git push ; \
			git tag -a $(MAKECOTAG) -f -m "Moved tag due to creation of ReleaseFile"; \
			git push --tags -f; \
		fi; \
		popd ; \
	done ;


migrateCommonEnv: 
	@if [ -z "$(MAKECOTAG)" ] ; then echo "No tag specified. Aborting"; exit 1; fi ;
	rm -rf tools; \
	rm -rf CryosatTools; \
	rm -rf GITPortingTools; \
	git clone -q $(GITREPO)/GITPortingTools.git; result=$$?; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; 
	@git clone -q  $(GITREPO)/CryosatTools.git; result=$$? ; \
	if [  "$$result" != "0" ] ; then exit $$result ; fi ; \
	mv CryosatTools tools ; 

checkoutMake:: makeDevEnv
	@rm -rf common_$(PROJECT) ; \
	git clone -q  $(GITREPO)/common_$(PROJECT).git; result=$$? ; \
	if [  "$$result" != "0" ] ; then echo "Unable to clone common_$(PROJECT)"; exit $$result ; fi ; \
	if [ -n "$(MAKECOTAG)" ]; then \
		pushd common_$(PROJECT) ; git checkout -q $(MAKECOTAG) ; result=$$? ; popd ; \
		if [  "$$result" != "0" ] ; then echo "Unable to move to tag $(MAKECOTAG) in common_$(PROJECT)"; exit $$result ; fi ;\
	else \
		pushd common_$(PROJECT) ;\
		needmigration=$$(git tag | grep -Fx MASTER_GIT) ;\
		if [ "$${needmigration}" == "" ]; then \
			echo "WARNING: Module common_$(PROJECT) head not migrated to MASTER_GIT" ;\
			echo "You can ignore this warning (was an error) if the project does not come from CVS" ;\
		fi ;\
		popd ;\
	fi; \
	if [ ! -e GITPortingTools ] ; then \
		git clone -q  $(GITREPO)/GITPortingTools.git; result=$$? ; \
		if [  "$$result" != "0" ] ; then echo "Unable to clone GITPortingTools"; exit $$result ; fi ; \
	fi; \
	pushd applications; \
		rm -rf applicationsMake; \
		mkdir applicationsMake; \
		cd applicationsMake; ln -fs ../../common_$(PROJECT)/applicationsMake/* .; cd ..; \
		ln -fs ../GITPortingTools/applicationsMake/* . ; \
	popd;\
	pushd components; \
		rm -rf componentsMake; \
		mkdir componentsMake; \
		cd componentsMake; ln -fs ../../common_$(PROJECT)/componentsMake/* .; cd ..; \
		ln -fs ../GITPortingTools/componentsMake/* . ; \
	popd;\
	rm -rf common; mkdir common ;\
	ln -fs common_$(PROJECT)/rpms .;\
	ln -fs common_$(PROJECT)/rpmsbuildroot .;\
	pushd common ; \
		ln -fs ../GITPortingTools/common/* . ;\
		ln -fs ../common_$(PROJECT)/commonMake/* . ;\
	popd ;\
	if [ ! -e tools ] ; then \
		git clone -q $(GITREPO)/CryosatTools.git ; result=$$?; \
		if [  "$$result" != "0" ] ; then exit $$result ; fi ;\
		mv CryosatTools tools ;\
	fi ;
	@if [ ! -e $(LOWERCASEPROJECT)_extrpm ] ; then \
		git clone -q $(GITREPO)/$(LOWERCASEPROJECT)_extrpm.git ; result=$$?; \
		if [  "$$result" != "0" ] ; then echo '*** Unable to clone $(LOWERCASEPROJECT)_extrpm - Ignored!' ; \
		else \
			if [ -n "$(MAKECOTAG)" ] ; then \
				pushd $(LOWERCASEPROJECT)_extrpm ; git checkout -q $(MAKECOTAG) ; result=$$? ; popd ; \
				if [  "$$result" != "0" ] ; then echo "Unable to move to tag $(MAKECOTAG) in $(LOWERCASEPROJECT)_extrpm"; exit $$result ; fi ;\
			fi ; \
		fi ;\
	fi ; 
	@if [ ! -e $(LOWERCASEPROJECT)_inst_scripts ] ; then \
		git clone -q $(GITREPO)/$(LOWERCASEPROJECT)_inst_scripts.git ; result=$$?; \
		if [  "$$result" != "0" ] ; then echo '*** Unable to clone $(LOWERCASEPROJECT)_inst_scripts - Ignored!' ; \
		else \
			if [ -e $(LOWERCASEPROJECT)_inst_scripts ] && [ -n "$(MAKECOTAG)" ] ; then \
				echo "MOVING TO $(MAKECOTAG) tag" ;\
				pushd $(LOWERCASEPROJECT)_inst_scripts ; git checkout -q $(MAKECOTAG) ; result=$$? ; popd ; \
				if [  "$$result" != "0" ] ; then echo "Unable to move to tag $(MAKECOTAG) in $(LOWERCASEPROJECT)_inst_scripts"; exit $$result ; fi ;\
			fi ; \
		fi ;\
	fi ; 
	


updateMake:: makeDevEnv
	@if [ -z "$(MAKECOTAG)" ] ; then echo "No tag specified. Aborting"; exit 1; fi ;
	@if [ ! -e common_$(PROJECT) ] ; then \
		echo "common_$(PROJECT) does not exists"; \
		exit 1; \
	fi; \
	pushd common_$(PROJECT) ; ../tools/gitupdatetobranch $(MAKECOTAG) ; result=$$? ; popd ; \
	if [  "$$result" != "0" ] ; then echo "Unable to update common_$(PROJECT) to tag $(MAKECOTAG)"; exit $$result ; fi ; 
	@if [ -e $(LOWERCASEPROJECT)_extrpm ] ; \
	then \
		pushd $(LOWERCASEPROJECT)_extrpm ; ../tools/gitupdatetobranch $(MAKECOTAG) ; result=$$? ; popd ; \
		if [  "$$result" != "0" ] ; then echo "Unable to update $(LOWERCASEPROJECT)_extrpm to tag $(MAKECOTAG)"; exit $$result ; fi ; \
	else \
		echo '*** Missing $(LOWERCASEPROJECT)_extrpm module. Ignored!'; \
	fi 
	@if [ -e $(LOWERCASEPROJECT)_inst_scripts ] ; \
	then \
		pushd $(LOWERCASEPROJECT)_inst_scripts ; ../tools/gitupdatetobranch $(MAKECOTAG) ; result=$$? ; popd ; \
		if [  "$$result" != "0" ] ; then echo "Unable to update $(LOWERCASEPROJECT)_inst_scripts to tag $(MAKECOTAG)"; exit $$result ; fi ; \
	else \
		echo '*** Missing $(LOWERCASEPROJECT)_inst_scripts module. Ignored!'; \
	fi 
	@if [ ! -e tools ] ; then \
		echo "tools does not exists"; \
		exit 1; \
	fi; 
	@if [ ! -e GITPortingTools ] ; then \
		echo "GITPortingTools does not exists"; \
		exit 1; \
	fi; 


checkTag:: 
	@read -p "enter TAG to check: "; echo "Checking env with the tag: $$REPLY"; export TAGTOCHECK="$$REPLY" ; $(MAKE) internalCheckTag

internalCheckTag:
	@if [ -z "$(TAGTOCHECK)" ] ; then echo "No tag specified. Aborting"; exit 1; fi ;
	@for ELEMENT in common_$(PROJECT) $(LOWERCASEPROJECT)_extrpm $(LOWERCASEPROJECT)_inst_scripts; do \
		if [ -d $${ELEMENT} ] ; then \
			echo -n "Checking $${ELEMENT} - " ;\
			pushd $${ELEMENT}  &> /dev/null ; git diff --exit-code -s $${TAGTOCHECK} 2>/dev/null ; result=$$?; popd &> /dev/null; \
			if [  "$$result" != "0" ] ; then echo "checkTag failed on module $${ELEMENT}"; exit $$result ; fi ; \
		else \
			echo "Checking $${ELEMENT} - not found"; \
		fi ; \
	done ; \
	echo "Entering components..." ;\
	pushd components &> /dev/null; \
	$(MAKE) checkTag ; result=$$?; \
	if [  "$$result" != "0" ] ; then echo "checkTag failed on components"; exit $$result ; fi ; \
	popd &> /dev/null; \
	echo "Entering applications..." ;\
	pushd applications &> /dev/null; \
	$(MAKE) checkTag ; result=$$?; \
	if [  "$$result" != "0" ] ; then echo "checkTag failed on applications"; exit $$result ; fi ; \
	popd &> /dev/null;

	
	
#check GIT good status checks
# - common
# - tools
# - GITPortingTools
# - project_rpms (optional)
# - project_inst_scripts (optional)
# - components
# - applications
	
checkGitGoodStatus::
	@toolspath=$$(pwd)/tools ;\
	for ELEMENT in tools common_$(PROJECT) GITPortingTools; do \
		if [ -d $${ELEMENT} ] ; then \
			echo "Checking $${ELEMENT} ..." ;\
			pushd $${ELEMENT}  &> /dev/null ; $${toolspath}/gitcheckgoodstatusnotag &> /dev/null; result=$$? ; popd &> /dev/null; \
			if [ "$$result" != "0" ] ; then \
				echo "$${ELEMENT} is not in a good status"; \
				exit 1 ;\
			fi ;\
		else \
			echo "no $${ELEMENT} folder found: aborted" ; exit 1 ; \
		fi ; \
	done ; \
	for ELEMENT in $(LOWERCASEPROJECT)_extrpm $(LOWERCASEPROJECT)_inst_scripts; do \
		if [ -d $${ELEMENT} ] ; then \
			echo "Checking optional element $${ELEMENT} ..." ;\
			pushd $${ELEMENT}  &> /dev/null ; $${toolspath}/gitcheckgoodstatusnotag &> /dev/null; result=$$? ; popd &> /dev/null; \
			if [ "$$result" != "0" ] ; then \
				echo "$${ELEMENT} is not in a good status"; \
				exit 1 ;\
			fi ;\
		else \
			echo "no optional $${ELEMENT} folder found: ignored!" ;  \
		fi ; \
	done ;
	@echo "Entering components..." ;\
	pushd components &> /dev/null; \
	$(MAKE) checkGitGoodStatusForAll ; result=$$? ;\
	popd &> /dev/null; \
	if [ "$$result" != "0" ] ; then \
		echo "components is not in a good status"; \
		exit 1 ;\
	fi ;
	@echo "Entering applications..." ;\
	pushd applications &> /dev/null; \
	$(MAKE) checkGitGoodStatusForAll ; result=$$? ;\
	popd &> /dev/null; \
	if [ "$$result" != "0" ] ; then \
		echo "applications is not in a good status"; \
		exit 1 ;\
	fi ;


tagall:: checkGitGoodStatus
	@if [ -n "$(MAKECITAG)" ] ; then \
		echo "Going to tag all the modules with the tag [$(MAKECITAG)]" ;\
		read -p "Are you sure (Y/N): "; \
		export result=$$REPLY; \
		if [ "$${result}" != "Y" ]; then exit 1; fi ; \
		toolspath=$$(pwd)/tools ;\
		for ELEMENT in tools common_$(PROJECT); do \
			echo Tagging element $${ELEMENT} ;\
			pushd $${ELEMENT} ;\
			if [ "$(FORCEPRJTAG)" == "1" ] ; \
			then \
				echo "Tagging module with tag [$(MAKECITAG)] (forced)" ; \
				$${toolspath}/gitcreatepushtag $(MAKECITAG) force; result=$$?; \
			else \
				echo "Tagging module with tag [$(MAKECITAG)]" ; \
				$${toolspath}/gitcreatepushtag $(MAKECITAG) ; result=$$?; \
			fi; \
			popd ;\
			if [  "$$result" != "0" ] ; then \
				exit $$result ;\
			fi ;\
		done ;\
		for ELEMENT in $(LOWERCASEPROJECT)_extrpm $(LOWERCASEPROJECT)_inst_scripts; do \
			if [ -d $${ELEMENT} ] ; then \
				echo Tagging element $${ELEMENT} ;\
				pushd $${ELEMENT} ;\
				if [ "$(FORCEPRJTAG)" == "1" ] ; \
				then \
					echo "Tagging module with tag [$(MAKECITAG)] (forced)" ; \
					$${toolspath}/gitcreatepushtag $(MAKECITAG) force; result=$$?; \
				else \
					echo "Tagging module with tag [$(MAKECITAG)]" ; \
					$${toolspath}/gitcreatepushtag $(MAKECITAG) ; result=$$?; \
				fi; \
				popd ;\
				if [  "$$result" != "0" ] ; then \
					exit $$result ;\
				fi ;\
			else \
				echo "no optional $${ELEMENT} folder found: ignored!" ;  \
			fi ; \
		done ; \
		echo Tagging components ;\
		cd components ;\
		$(MAKE) tagall ; result=$$? ;\
		cd .. ;\
		if [  "$$result" != "0" ] ; then \
			exit $$result ;\
		fi ;\
		echo Tagging applications ;\
		cd applications ;\
		$(MAKE) tagall ; result=$$? ;\
		cd .. ;\
		if [  "$$result" != "0" ] ; then \
			exit $$result ;\
		fi ;\
	else \
	        echo "no tag - aborted" ; exit 1 ; \
	fi ;
	

makeDevEnv:
	@if [ ! -e applications ] ; then \
		mkdir applications ; \
	fi ;
	@if [ ! -e components ] ; then \
		mkdir components ; \
	fi ;
	@if [ ! -e shared ] ; then \
		mkdir shared ; \
	fi ;
	@if [ ! -e include ] ; then \
		mkdir include ; \
	fi ;

checkoutRev: makeDevEnv
	@read -p "enter TAG to checkout: "; echo $$REPLY; export MAKECOTAG="$$REPLY" ; $(MAKE) checkout
	
updateRev: makeDevEnv
	@read -p "enter TAG to update: "; echo $$REPLY; export MAKECOTAG="$$REPLY" ; $(MAKE) update
	
prjTag:
	@read -p "enter TAG to set: "; echo $$REPLY; export MAKECITAG="$$REPLY" ; $(MAKE) tagall

rpmDist:
	@lsrpm.py mkdist ; result=$$? ; \
	if [  "$$result" != "0" ] ; then \
	    exit $$result ; \
	fi ; \

rpmCd:
	@lsrpm.py mkcd ; result=$$? ; \
	if [  "$$result" != "0" ] ; then \
	    exit $$result ; \
	fi ; \

rpmDvd:	
	@lsrpm.py mkdvd ; result=$$? ; \
	if [  "$$result" != "0" ] ; then \
	    exit $$result ; \
	fi ; \

rpmBase: 
	@lsrpm.py clean && lsrpm.py build ; result=$$? ; \
	if [  "$$result" != "0" ] ; then \
	    exit $$result ; \
	fi ; \



distRelease: checkTag clean abortOnError rpmBase rpmDist

cdRelease: checkTag clean abortOnError rpmBase rpmDist

dvdRelease: checkTag clean abortOnError rpmBase rpmDist


cleanconftemplate:
	cd applications; $(MAKE) cleanconftemplate ; cd ..


# PROJECT MAKE TAG FROM RELEASEFILE

check_release_file:
	@if [ "$(MAKEDEBUGACTION)" != "" ]; then echo $$(pwd) $@ ; fi 
	@if [ ! -f ReleaseFile ]; then echo "ERROR: Please add a ReleaseFile in the module"; exit 1; fi

git_check_pending_mods:
	@if [ "$(MAKEDEBUGACTION)" != "" ]; then echo $$(pwd) $@ ; fi 
	@tools/gitcheckpendingmods

specifictag: git_check_pending_mods
	@if [ "$(MAKEDEBUGACTION)" != "" ]; then echo $$(pwd) $@ ; fi 
	@if [ "$(CURV)" == "" ] ; \
	then \
		echo "Tag has not been specified" ;\
		exit 1; \
	else \
		if [ "$(FORCETAG)" == "1" ] ; \
		then \
			echo "Tagging module with tag [$$CURV] (forced)" ; \
			tools/gitcreatepushtag $$CURV force; \
		else \
			echo "Tagging module with tag [$$CURV]" ; \
			tools/gitcreatepushtag $$CURV ; \
		fi; \
	fi;

tag: git_check_pending_mods check_release_file
	@if [ "$(MAKEDEBUGACTION)" != "" ]; then echo $$(pwd) $@ ; fi 
	@CURV=$$(tools/getmoduleversion) ; \
	export CURV ; \
	$(MAKE) -s specifictag; \




# DO NOT DELETE

help:
	@echo "make                    - Default: compile all"
	@echo "make all                - Compile all"
	@echo "make abortOnError       - Compile all, but aborts on first module with errors"
	@echo "make checkGitGoodStatus - Verify if all modules are committed and tagged accordingly to its ReleaseFile"
	@echo "make clobber            - Force recompilation"
	@echo "make clean              - Clean the subproducts of the compilation"
	@echo "make cleanall           - Clean the subproducts of the compilation, docs, tests ..."
	@echo "make cleanconftemplate  - clean the configuration template directory - relies on applicationsMake Makefile"
	@echo "make install            - Do nothing. Exists for compatibility. "
	@echo "make depend             - Recompute all dependencies. "
	@echo "make status             - Shows status for each module. "
	@echo "make statustag          - Shows if the module is stable, its current version, latest and latest-on-branch versions. "
	@echo "make checkout           - Checkout a whole project"
	@echo "make checkoutRev        - Asks for a specific tag to be checked out. "
	@echo "make update             - Updates the whole project. "
	@echo "make updateRev          - Asks for a specific tag to be updated. "
	@echo "make uptostable         - Updates all the components of the project to the latest available tag on mainstream."
	@echo "make safeupdate         - Updates all the components of the project to the last tag belonging to the same branch."
	@echo "                          The branch is defined by the version but the last two figures. Examples:"
	@echo "                          * Version 1.2.3 belongs to the branch 1.X.Y"
	@echo "                          * Version 1.2.3.4.5.6 belongs to the branch 1.2.3.4.X.Y"
	@echo " "
	@echo "make rpmBase      - Creates the RPMS using lsrpm.py cleaning the env"
	@echo "make rpmDist      - Creates a distribution using lsrpm.py mkist"
	@echo "make rpmCd        - Creates a distribution using lsrpm.py mkcd"
	@echo "make rpmDvd       - Creates a distribution using lsrpm.py mkdvd"
	@echo "make checkTag     - Asks for a tag and verifies that each module is up-to-date to that tag"
	@echo "make distRelease  - checkTag clean abortOnError rpmBase rpmDist"
	@echo "make cdRelease:   - checkTag clean abortOnError rpmBase rpmCd"
	@echo "make dvdRelease:  - checkTag clean abortOnError rpmBase rpmDvd"
	@echo " "
	@echo "make prjTag       - asks for a specific tag and use it to tag listed components. "
	@echo "make refresh      - refreshes all components and applications"
	@echo "make md5          - computes md5 and CVS statistics on all the sources"
	@echo "make cleanmd5     - cleans env from md5 related garbage"
	@echo " "
	@echo "*** USE -s OPTION TO SUPPRESS USELESS MESSAGES (as in make -s abortOnError)"
