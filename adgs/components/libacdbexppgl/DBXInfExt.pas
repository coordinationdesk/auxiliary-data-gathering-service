
{**********************************************************}
{                                                          }
{       dbExpress for Informix extension                   }
{                                                          }
{       Copyright (c) 2001-2004 Luxena Software Company    }
{       http://www.luxena.com                              }
{       e-mail: support@luxena.com                         }
{                                                          }
{**********************************************************}

{$ALIGN 8}

unit DBXInfExt;

interface

uses DBXpress;

const
  SQLNOTFOUND = 100;

type

  sqlcaw_s = record
    sqlwarn0: Char; // = W if any of sqlwarn[1-7] = W
    sqlwarn1: Char; // = W if any truncation occurred or database has transactions or no privileges revoked
    sqlwarn2: Char; // = W if a null value returned or ANSI database
    sqlwarn3: Char; // = W if no. in select list != no. in into list or turbo backend or no privileges granted
    sqlwarn4: Char; // = W if no where clause on prepared update, delete or incompatible float format
    sqlwarn5: Char; // = W if non-ANSI statement
    sqlwarn6: Char; // = W if server is in data replication secondary mode
    sqlwarn7: Char; // = W if database locale is different from proc_locale
  end;

  PSQLCA = ^TSQLCA;
  TSQLCA = record
    sqlcode: Integer;
    sqlerrm: array[0..71] of Char; // error message parameters
    sqlerrp: array[0..7] of Char;
    sqlerrd: array[0..5] of Integer;
    // 0 - estimated number of rows returned
    // 1 - serial value after insert or ISAM error code
    // 2 - number of rows processed
    // 3 - estimated cost
    // 4 - offset of the error into the SQL statement
    // 5 - rowid after insert
    case Integer of
      0: (sqlawarn: array[0..7] of Char);
      1: (sqlwarn: sqlcaw_s);
  end;

  TimeOutCallback = procedure (CustomData: Pointer; Status: Integer); stdcall;

  IInformixConnection = interface(ISQLConnection)
    function SQLCA: PSQLCA; stdcall;
    function ErrorMessage: PChar; stdcall; overload;
    function setTimeOutCallback(TimeOut: Integer; Callback: TimeOutCallback; CustomData: Pointer): SQLResult; stdcall;
    function interruptSQL: Boolean; stdcall;
    procedure getLastInsertedSerial8(out Value); stdcall; // returns TBcd value
  end;

implementation

end.
