<?php

require_once 'ACSPhpLib/acs_exception.php';
require_once 'ACSPhpLib/acs_app.php';

class acs_servlet extends acs_app {
	
	public $errorAsImage = false;
	
    public function __construct($iniFileName, $ident = __METHOD__) {
        parent :: __construct($iniFileName, $ident, true);
    }

    public function getRequiredParams(){
        return array();
    }

    public function checkParam($paramName){
    	if(!array_key_exists($paramName,$_REQUEST) or !isset($_REQUEST[$paramName])){
    		throw new acs_exBadParam("Parameter $paramName must be provided!");
    	}
    }

    public function parse(){
        die('parse must be implemented!');
    }

    public function run(){
        try {
            $this->authenticate();
            foreach($this->getRequiredParams() as $paramName)
                $this->checkParam($paramName);

            $this->parse();
        } catch (Exception $e) {
            $msg = $e->getMessage();
            $this->_logHandler->crit($msg, true);
            // output exception
            if($this->errorAsImage){
            	$this->imageError($msg);
            } else {
            	die($msg);
            }
        }
    }
    
    protected function imageError($msg){
		header("Content-type: image/png");
		$im     =  imagecreatetruecolor(300,200);
		$text_color = imagecolorallocate($im, 233, 14, 91);
		$px     = (imagesx($im) - 7.5 * strlen($msg)) / 2;
		imagestring($im, 3, $px, 9, $msg, $text_color);
		imagepng($im);
		imagedestroy($im);
    }

    public function authenticate(){
    	// do nothing
    }
}




?>