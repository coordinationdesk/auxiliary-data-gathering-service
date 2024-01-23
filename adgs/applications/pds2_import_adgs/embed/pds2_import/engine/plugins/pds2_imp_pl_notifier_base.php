<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2011 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.) |
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
// $Prod: pds2_import $  
// $Author$
// $Id$
// $Revision$
//

require_once 'pds2_imp_pl_base.php';
require_once 'ACSPhpLib/acs_tools.php';

define('_NOTIFICATION_STATUS_PENDING', 0);
define('_NOTIFICATION_STATUS_PROCESSING', 1);
define('_NOTIFICATION_STATUS_NOTIFIED', 2);
define('_NOTIFICATION_STATUS_ERROR', 3);

class pds2_imp_pl_notifier_base extends pds2_imp_pl_base {
	public	$notification,
			$max_notification_retries = 3,
			$email_from = 'notification.daemon@acsys.it',
			$email_from_name = 'Notification Daemon',
			$email_server = 'localhost',
			$email_subject = 'File ready',
			$email_body = 'The file {filename} is available at the following link: {link}',
			$email_attachment = null;
	
	public	$email_compl_from = 'notification.daemon@acsys.it',
			$email_compl_from_name = 'Notification Daemon',
			$email_compl_server = 'localhost',
			$email_compl_subject_ok = 'File ready',
			$email_compl_subject_err = 'File transfer failed',
			$email_compl_body_ok = 'The file {filename} is available',
			$email_compl_body_err = 'The transfer for file {filename} has failed';

	public function initialize($userid) {
		parent::initialize($userid);
		
		// reconfig based on the application configuration file and database configuration
		foreach (array('email_from', 'email_from_name', 'email_server', 'email_subject', 'email_body',
						'email_compl_from', 'email_compl_from_name', 'email_compl_server',
						'email_compl_subject_ok', 'email_compl_subject_err', 'email_compl_body_ok', 'email_compl_body_err') as $item) {
			if (array_key_exists($item, $this->_iniVars['notification_plugin']))
				$this->$item = $this->_iniVars['notification_plugin'][$item];
			if (array_key_exists($item, $this->_opVars['run_rule_config']))
				$this->$item = $this->_opVars['run_rule_config'][$item];
		}
	}
	
	// Get the first notification to process
	public function getNotificationDetails($id) {
		$sql = sprintf('SELECT * FROM pds_imp_notifications n 
						INNER JOIN pds_imp_exported_files e ON n.exported_file_id = e.id_exported_file 
						WHERE notification_status = %d AND id_notification = %d', _NOTIFICATION_STATUS_PENDING, $id);
		$notification_row = $this->pool_ref->db_handler->execQuery($sql, acs_idb::_SQL_GETROW);

		return $notification_row;
	}
	
	// Change the status for the specified notification
	public function changeNotificationStatus($status, $status_msg, $increment_retries = false) {
		$retries_inc = '';
		if ($increment_retries)
			$retries_inc = ', notification_retry = notification_retry + 1';
		$sql = sprintf('UPDATE pds_imp_notifications SET	notification_status = %d,
															notification_message = %s
															%s
						WHERE id_notification = %d', 
						$status,
						$this->pool_ref->db_handler->getSqlObj()->sql_quote_str($status_msg),
						$retries_inc,
						$this->notification['id_notification']);
		$this->pool_ref->db_handler->execQuery($sql, acs_idb::_SQL_EXEC);
		
		$this->sendCompletionNotification($status, $status_msg);
	}
	
	public function sendCompletionNotification($status, $status_msg) {
		if (!array_key_exists('notification_email', $this->_opVars['run_rule_config']))
			return;
		
		$url = parse_url($this->notification['notification_uri']);
		if ($url['scheme'] != 'mailto') {
			switch ($status) {
				case _NOTIFICATION_STATUS_ERROR:
					$subject = $this->email_compl_subject_err;
					$body = $this->email_compl_body_err;
					break;
				case _NOTIFICATION_STATUS_NOTIFIED:
					$subject = $this->email_compl_subject_ok;
					$body = $this->email_compl_body_ok;
					break;
				default:
					return;
			}
			
			$body = str_replace('{filename}', $this->notification['exported_filename'], $body) . '<br>' . $status_msg;

			require_once 'ACSPhpLib/acs_communication_tools.php';
			$comm = new acs_communication_tools($this->_iniVars, $this->_logHandler, $this->pool_ref->db_handler);
			$comm->messageAsHtml = true;
			$comm->sendMessage(	$this->email_compl_from,
								$this->email_compl_from_name,
								$this->_opVars['run_rule_config']['notification_email'],
								_ACS_COMM_EMAIL,
								$body,
								$this->email_compl_server,
								$subject,
								array());
		}
	}
	
	public function handleNotificationError($msg) {
		if ($this->notification['notification_retry'] >= $this->max_notification_retries) {
			$this->changeNotificationStatus(_NOTIFICATION_STATUS_ERROR, "maximum number of retries reached: $msg");
		} else {
			$this->changeNotificationStatus(_NOTIFICATION_STATUS_PENDING, 
											"notification retry (" . ($this->notification['notification_retry'] + 1) . "): $msg", 
											true);
		}
	}
	
	public function sendEmail(& $comm, $address) {
		$comm->sendMessage(	$this->email_from,
							$this->email_from_name,
							$address,
							_ACS_COMM_EMAIL,
							$this->email_body,
							$this->email_server,
							$this->email_subject,
							$this->email_attachment
							);
	}
	
	public function sendNotification($uri) {
		// refer to the $this->notification array to get some details about the notification
//		var_dump($this->notification['id_notification']);
//		var_dump($uri);
//		var_dump($this->notification);

		$parsedURI = parse_url($uri);
		$local_filename = $this->notification['exported_filepath'] . '/' . $this->notification['exported_filename'];

		if ($parsedURI['scheme'] == 'mailto') {
			require_once 'ACSPhpLib/acs_communication_tools.php';
			$comm = new acs_communication_tools($this->_iniVars, $this->_logHandler, $this->pool_ref->db_handler);
			$comm->messageAsHtml = true;
			
			// if specified in pds_imp_notifications.notification_params, add the file as the email attachment
			if ($this->_opVars['run_rule_config']['email_attach_file'])
				$this->email_attachment = array($this->notification['exported_filename'] => file_get_contents($local_filename));

//			$this->_logHandler->debug("Email attachments: " . print_r($this->email_attachment, true));

			$this->sendEmail($comm,
							"{$parsedURI['user']}@{$parsedURI['host']}"
							);
		} else {
			require_once dirname(__FILE__) . '/../pds2_file_transfer_tools.php';
			$tr = new preimp_wrapper($parsedURI['scheme'],
									$parsedURI['host'],
									$parsedURI['user'],
									$parsedURI['pass'],
									$parsedURI['port']);
			$tr->putBinaryFile($local_filename,
								$parsedURI['path'] . '/' . $this->notification['exported_filename']);
		}
	}
	
	// Overrides
	public function precondition($userid) {
		$this->notification = $this->getNotificationDetails($this->_opVars['id_run']);		// the ID_RUN MUST correspond to the primary key of the pds_imp_notifications table
		if (isset($this->notification)) {
			$this->changeNotificationStatus(_NOTIFICATION_STATUS_PROCESSING, 'Processing');
			$this->pool_ref->found_files[] = $this->notification['notification_uri'];
			return true;
		} else
			return false;
	}

    public function execute($userid) {
    	try {
    		// do the job
	    	$this->_logHandler->notice("Notification to {$this->notification['notification_uri']} for {$this->notification['exported_filename']}");

			$this->sendNotification($this->notification['notification_uri']);
	    	
	    	$this->changeNotificationStatus(_NOTIFICATION_STATUS_NOTIFIED, "Notification completed");
    	} catch (Exception $e) {
    		$this->handleNotificationError($e->getMessage());
    		throw $e;
    	}
    }
    
    public function cleanup($userid) {
    }
    
    public function cleanupTemp($userid) {
    }
	
}

?>