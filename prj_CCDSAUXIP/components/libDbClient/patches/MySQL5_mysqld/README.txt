#
# (C) 2005, Advanced Computer Systems , Inc.
# $Id$
# $Author$
#



In order to upload a file/blob into a table, following SQL command should be issued:

load data local infile "/path/binaryfile" into table TABLENAME fields terminated by "" enclosed by "" escaped by "" lines starting by "" terminated by "" (COLUMNAME) set OTHERCOLUMN=value;

This allows to upload a BINARY file, and store into TABLENAME.COLUMNAME.


So far, MySQL Server is not able to fit only a single field/column. So, a field terminator MUST be set even if only a column is passed (above command will fail since no terminator is set to a NOT-NULL value).

For example:
fields terminated by "\0"
fields terminated by "PATTERN"
...

Since binary file could content ANY char/pattern, this patch prevent the server to really match the PATTERN (and truncate the binary file).
In order to let the matching method still working (for private use), PATTERN must be set by the user with the word "UNTERMINATED" :

fields terminated by "UNTERMINATED"


Even if binary file contents this pattern, file won't be truncated.


