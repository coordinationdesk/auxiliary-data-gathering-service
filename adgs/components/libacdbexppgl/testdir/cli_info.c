/****************************************************************************
** Licensed Materials - Property of IBM
**
** Governed under the terms of the International
** License Agreement for Non-Warranted Sample Code.
**
** (C) COPYRIGHT International Business Machines Corp. 1995 - 2002
** All Rights Reserved.
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*****************************************************************************
**
** SOURCE FILE NAME: cli_info.c
**
** SAMPLE: How to get and set environment attributes at the client level
**
** CLI FUNCTIONS USED:
**         SQLAllocHandle -- Allocate Handle
**         SQLFreeHandle -- Free Handle Resources
**         SQLGetEnvAttr -- Retrieve Current Environment Attribute Value
**         SQLSetEnvAttr -- Set Environment Attribute
**
** OUTPUT FILE: cli_info.out (available in the online documentation)
*****************************************************************************
**
** For more information on the sample programs, see the README file.
**
** For information on developing CLI applications, see the CLI Guide
** and Reference.
**
** For information on using SQL statements, see the SQL Reference.
**
** For the latest information on programming, building, and running DB2
** applications, visit the DB2 application development website:
**     http://www.software.ibm.com/data/db2/udb/ad
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlcli1.h>
#include "utilcli.h" /* Header file for CLI sample code */

/* function called by main() that sets and gets an
   an environment attribute */
int EnvAttrSetGet(SQLHANDLE);

int main(int argc, char *argv[])
{
  SQLRETURN cliRC = SQL_SUCCESS;
  int rc = 0;
  SQLHANDLE henv; /* environment handle */

  char dbAlias[SQL_MAX_DSN_LENGTH + 1];
  char user[MAX_UID_LENGTH + 1];
  char pswd[MAX_PWD_LENGTH + 1];

  /* check the command line arguments */
  rc = CmdLineArgsCheck1(argc, argv, dbAlias, user, pswd);
  if (rc != 0)
  {
    return rc;
  }

  printf("\nTHIS SAMPLE SHOWS HOW TO GET AND SET ENVIRONMENT ATTRIBUTES.\n");

  /* allocate an environment handle */
  cliRC = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
  if (cliRC != SQL_SUCCESS)
  {
    printf("\n--ERROR while allocating the environment handle.\n");
    printf("  cliRC = %d\n", cliRC);
    printf("  line  = %d\n", __LINE__);
    printf("  file  = %s\n", __FILE__);
    return 1;
  }

  /* set and get an environment attribute */
  rc = EnvAttrSetGet(henv);

  /* free the environment handle */
  cliRC = SQLFreeHandle(SQL_HANDLE_ENV, henv);
  ENV_HANDLE_CHECK(henv, cliRC);

  return 0;
} /* main */

int EnvAttrSetGet(SQLHANDLE henv)
{
  SQLRETURN cliRC = SQL_SUCCESS;
  int rc = 0;
  SQLINTEGER output_nts;

  printf("\n-----------------------------------------------------------");
  printf("\nUSE THE CLI FUNCTIONS\n");
  printf("  SQLSetEnvAttr\n");
  printf("  SQLGetEnvAttr\n");
  printf("TO SET AND GET ENVIRONMENT ATTRIBUTES:\n");

  printf("\n    Set the environment attribute SQL_ATTR_OUTPUT_NTS\n");
  printf("     (null termination of output strings) to TRUE.\n");

  /* set environment attribute */
  cliRC = SQLSetEnvAttr(henv, SQL_ATTR_OUTPUT_NTS, (SQLPOINTER) SQL_TRUE, 0);
  ENV_HANDLE_CHECK(henv, cliRC);

  printf("  Get the environment attribute SQL_ATTR_OUTPUT_NTS.\n");

  /* retrieve the current environment attribute value */
  cliRC = SQLGetEnvAttr(henv, SQL_ATTR_OUTPUT_NTS, &output_nts, 0, NULL);
  ENV_HANDLE_CHECK(henv, cliRC);

  /* output the attribute value */
  printf("  The null termination of output strings is: ");
  printf(output_nts == SQL_TRUE ? "True\n" : "False\n");

  return 0;
} /* EnvAttrSetGet */


