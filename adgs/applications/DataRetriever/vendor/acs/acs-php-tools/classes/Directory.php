<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2010 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.)    |
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
// $Prod:  $
// $Author:  $
// $Id:  $
// $Revision: $
//

namespace Acs\PhpTools;

use Acs\PhpTools\Exception\CannotRemoveDirectoryException;
use Acs\PhpTools\Exception\CannotRemoveFileException;
use Acs\PhpTools\Exception\CannotCreateDirectoryException;

class Directory {
	protected $dir;
	static protected $tempDir;

	public function __construct($dir) {
		$this->dir = $dir;
	}
	
	public function getPath() {
		return $this->dir;
	}
	
	public function create() {
		if (!mkdir($this->dir))
			throw new CannotCreateDirectoryException("Cannot create directory {$this->dir}");
	}

	public function recursiveRemove() {
		$iterator = new \RecursiveIteratorIterator(new \RecursiveDirectoryIterator($this->dir, \FilesystemIterator::SKIP_DOTS), \RecursiveIteratorIterator::CHILD_FIRST);
		foreach ($iterator as $filename => $fileInfo) {
			if ($fileInfo->isDir()) {
				if (!rmdir($filename))
					throw new CannotRemoveDirectoryException("Cannot remove directory {$filename}");
			} else {
				if (!unlink($filename))
					throw new CannotRemoveFileException("Cannot remove file {$filename}");
			}
		}
		if (!rmdir($this->dir))
			throw new CannotRemoveDirectoryException("Cannot remove directory {$this->dir}");
	}
	
	static public function getTempPath() {
		if (self::$tempDir == null)
			return sys_get_temp_dir();
		else
			return self::$tempDir;
	}
	
	static public function setTempPath($dir) {
		self::$tempDir = $dir;
	}
}

?>