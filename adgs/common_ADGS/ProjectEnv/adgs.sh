project=adgs

# the PROJECT env var is used inside Makefile for specific project purposes
PROJECT=ADGS
export PROJECT

# The directory PRJENVDIR will host environments script for all projects
PRJENVDIR=/raid0/$project/common_ADGS/ProjectEnv


# The main entry level for the whole project
PRJROOTTREE=/raid0/$project
export PRJROOTTREE

# the setting needed from the makefiles
LIB_INSTALL_PATH=$PRJROOTTREE
LIB_INTEGRATION_PATH=$LIB_INSTALL_PATH
EXTCOMP_PATH=$LIB_INSTALL_PATH
EXE_INTEGRATION_PATH=$LIB_INSTALL_PATH/bin


export LIB_INSTALL_PATH LIB_INTEGRATION_PATH EXE_INTEGRATION_PATH EXTCOMP_PATH


#Added for TCL-FTP
if [ -f ${PRJENVDIR}/${project}tclconfig.sh ] ; then
	. ${PRJENVDIR}/${project}tclconfig.sh
fi


# the following code is to set the right X window display
# automatically as much as possible
if [ -z $DISPLAY ]
then
        TTYPORT=`tty`
        if [ "$TTYPORT" = "/dev/console" ]
        then
                DISPLAY="localhost:0"
        elif [ -z $REMOTEHOST ]
        then
                TTYNAME=`echo $TTYPORT | cut -c 6-`
		REMOTEHOST=`w | egrep "$TTYNAME[ \t]+" | head -1 | awk '{print $3}' `
		DISPLAY="${REMOTEHOST}:0"
		export DISPLAY
		unset TTYNAME
	else
		DISPLAY="${REMOTEHOST}:0"
		export DISPLAY
        fi     
	unset TTYPORT	
fi

#PATH setting
source /opt/rh/devtoolset-8/enable
PATH=$PATH:$HOME/bin:$PRJROOTTREE/tools:$PRJROOTTREE/GITPortingTools/tools
export PATH

#CVSROOT settings
if [ -f ${PRJENVDIR}/${project}cvsusers ]
then
	CVSROOT=`grep -w ^$USER ${PRJENVDIR}/${project}cvsusers | awk '{print $2}' `
	export CVSROOT
fi

#this PATH definition is to prepend /usr/bin to /usr/local/bin in order to use the g++ 2.96
#PATH=$PATH:/usr/java/j2re1.4.1_01/bin/:$QTDIR/bin:/usr/acs/bin:/usr/local/bin:/bin:/usr/bin:/usr/X11R6/bin
PATH=$PATH:$QTDIR/bin:/usr/acs/bin:/usr/local/bin:/bin:/usr/bin:/usr/X11R6/bin
export PATH
. ${PRJENVDIR}/${project}shrc.sh

#qacpp environment
if [ -f /opt/qacpp-2.5.2R/.profile  ];  then
	. /opt/qacpp-2.5.2R/.profile
fi

if [ -f $PRJROOTTREE/GITPortingTools/common/make_completion.sh ]
then
	. $PRJROOTTREE/GITPortingTools/common/make_completion.sh
fi
