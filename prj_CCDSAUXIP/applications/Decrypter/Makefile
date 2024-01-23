# DO NOT COMMIT

include ../../common/commondefs.mk


  USERFLAG = $(PROJECTFLAG)

 LIBRARYNAME = Decrypter


  VERSIONFILE = DecrypterV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
   		Decrypter.C  DecrypterV.C


   C++INCLUDE = \

   SPECIFICLIBS =	
#prevent remotion of objects, target and versioning
#      TARGETFILES = Decrypter
      VERSIONRULE =

EXES =  \
	Decrypter


INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)


$(TARGETFILES): $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS) $(DEPLIBS) $(PRJLIBS) 
        	

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk

