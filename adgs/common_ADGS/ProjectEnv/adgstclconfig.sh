# Added for DGF-FTP:


# Path for ACS components/libTclFtp :
# Example: ACS_TCL_COMMON_PATH=/raid0/goce/components/libTclFtp
ACS_TCL_COMMON_PATH=$PRJROOTTREE/components/libTclFtp


# Path for TCL Directory :
# Example: ACS_TCL_LIBRARY=/usr/lib/tcl8.3
ACS_TCL_LIBRARY=/usr/lib64/tcl8.5


# Path for TCLX Directory :
# Example: ACS_TCLX_LIBRARY=/usr/lib/tclX8.3
ACS_TCLX_LIBRARY=/usr/lib64/tcl8.5/tclx8.4


# Path for Expect Directory :
# Example: ACS_EXPECT_LIBRARY=/usr/lib/expect5.34
ACS_EXPECT_LIBRARY=/usr/lib64/tcl8.5/expect5.44.1.15


# TCL and TCLX version (as float)
# Example: ACS_TCL_VERSION=8.3
ACS_TCL_VERSION=8.5


# TCL and EXPECT shared libraries, to be used at runtime
# Example: ACS_TCL_SHARED_LIBRARIES="/usr/lib/libtcl8.4.so /usr/lib/tclx8.4/libtclx8.4.so /usr/lib/libexpect5.34.so"
ACS_TCL_SHARED_LIBRARIES="/usr/lib64/libtcl.so /usr/lib64/tcl8.5/tclx8.4/libtclx8.4.so /usr/lib64/libexpect5.44.1.15.so" 



export ACS_TCL_COMMON_PATH ACS_TCL_LIBRARY ACS_TCLX_LIBRARY ACS_TCL_VERSION ACS_EXPECT_LIBRARY ACS_TCL_SHARED_LIBRARIES


# Full path for TCL, TCLX, Expect libraries
# Example: ACS_TCL_EXPECT_LIBSPATH= "/usr/lib/libtcl.a /usr/lib/libexpect.a /usr/lib/libtclx.a"
# Default: ACS_TCL_EXPECT_LIBSPATH = "-ltcl -lexpect -ltclx"
# IF NOT DEFINED, THE VALUE IN "Default" WILL BE USED
# (*** NOT DEFINED **** DEFAULT WILL BE USED ***)  ACS_TCL_EXPECT_LIBSPATH="-ltcl8.3 -lexpect5.34 -ltclx8.3"
# (SEE ABOVE) export ACS_TCL_EXPECT_LIBSPATH

ACS_TCL_EXPECT_LIBSPATH="-ltcl -lexpect5.44.1.15 -ltclx8.4"
export ACS_TCL_EXPECT_LIBSPATH
