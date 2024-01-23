<?php


/**
 * Simple Join Point 
 * 
 * @author Daniel Pötzinger
 *
 */
class Threadi_Communication_SharedMemory implements Threadi_Communication_CommunicationInterface {
	
	private $shmId;
	
	private $readCache= NULL;
	
    /**
     * constructor
     */
	public function __construct() {
		if (! function_exists('shmop_open')) throw new Exception('shmop functions not available on this PHP installation');
		srand();
		$memoryKey = ftok(__FILE__, 't').getmypid().rand();
		$this->shmId = shmop_open($memoryKey, "c", 0644, 100);
	}
	
	/**
	 * @param string $key
	 * @param string $value
	 * @return boolean
	 */
	public function set($key,$value) {
		$data = $this->readFromSharedMemory();
		$data[$key] = $value;
		return (boolean) shmop_write($this->shmId, serialize($data),0);
	}
	
	/**
	 * @param string $key
	 * @return mixed 
	 */
	public function get($key) {
		$data = $this->readFromSharedMemory();
		if (!isset($data[$key])) {
			throw new Exception('Could not read key '.$key);
		}
		return $data[$key];
	}
	
	/**
	 * @return array 
	 */
	private function readFromSharedMemory() {
		$dataSer = shmop_read($this->shmId , 0, shmop_size($this->shmId));
		$data = @unserialize($dataSer);
		if (!$data) {
			return array();
		}
		return $data;
	}
	/**
	 * Close shared memory
	 */
	public function close() {
		@shmop_delete($this->shmId);
		@shmop_close($this->shmId);
	}
	
}



