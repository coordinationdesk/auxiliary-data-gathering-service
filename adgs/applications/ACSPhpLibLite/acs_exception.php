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
// $Author: andema $
// $Id: acs_exception.php,v 1.15 2008/08/08 13:27:27 andema Exp $
// $Revision: 1.15 $
//


class acs_exception extends Exception{
   // Redefine the exception so message isn't optional
   public function __construct($message, $code = 0) {
       // make sure everything is assigned properly
       parent::__construct($message, $code);
   }

   // custom string representation of object */
   public function __toString() {
       return __CLASS__ . ": [{$this->code}]: {$this->message}\n";
   }

    function getHTMLMessage(){
        $errBox = $this->getUserMessageBox($this->message);
        return  "<center><font color='red'><h1><hr>ERROR<hr></h1></font>$errBox</center><br><br><hr><center><h3>Contact the web site administrator</h3></center>";
    }
    
    function getUserMessageBox($msg){
        return "<table width=80%><tr><td class='usermessages'>".nl2br(trim($msg))."</td></tr></table>";
    }
}

class acs_exFailed extends acs_exception{}
class acs_exNotCapable extends acs_exFailed{}
class acs_exUnknownFormat extends acs_exFailed{}
class acs_exBadParam extends acs_exFailed{}
class acs_exFileError extends acs_exFailed{}

class acs_exCritical extends acs_exception{}
class acs_exINIError extends acs_exCritical{}
class acs_exLogError extends acs_exCritical{}
class acs_exDBErrorConnection extends acs_exCritical{}
class acs_exDBErrorQuery extends acs_exCritical{}
class acs_exWrongPHPVersion extends acs_exCritical{}

class acs_exSecurity extends acs_exCritical{}

?>
