<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2006 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.)    |
// |                                                                           |
// | Via Della Bufalotta, 378 - 00139 Roma - Italy                             |
// | http://www.acsys.it                                                       |
// | All rights reserved.                                                      |
// +---------------------------------------------------------------------------+
// | This is UNPUBLISHED PROPRIETARY SOURCE CODE of A.C.S. S.p.A.              |
// | The contents of this file may not be disclosed to third parties, copied or|
// | duplicated in any form, in whole or in part, without the prior written    |
// | permission of A.C.S. S.p.A.                                               | 
// +---------------------------------------------------------------------------+
//
// $Prod: ACSPhpLib $  
// $Author: roberto.riccardi $
// $Id: acs_idb.php,v 1.6 2006/10/30 16:38:17 roberto.riccardi Exp $
// $Revision: 1.6 $
//

interface acs_idb {

  const _SQL_GETROW= 0;
  const _SQL_GETALL= 1;
  const _SQL_EXEC= 2;
  const _SQL_GETASSOC= 3;
  const _SQL_GETCOL= 4;


    public function & getDBConnection();
    public function execQuery($sql, $type, $mode= MDB2_FETCHMODE_ASSOC, $prepareParam= array (), $paramtypes= null, $types= null, $column= 0);
    public function lastId($table= null, $field= null, $id= null);
    public function getDBType();
}
?>
