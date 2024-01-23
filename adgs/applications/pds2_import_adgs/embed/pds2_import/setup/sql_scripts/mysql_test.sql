/*
SQLyog Community v8.61 
MySQL - 5.1.60 : Database - pds2
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`pds2` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `pds2`;

/*Table structure for table `acs_processes` */

DROP TABLE IF EXISTS `acs_processes`;

CREATE TABLE `acs_processes` (
  `process_name` varchar(1000) NOT NULL,
  `last_process_run_datetime` datetime NOT NULL,
  `process_enabled` tinyint(4) NOT NULL DEFAULT '1',
  PRIMARY KEY (`process_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `acs_processes` */

/*Table structure for table `pds_imp_plugins` */

DROP TABLE IF EXISTS `pds_imp_plugins`;

CREATE TABLE `pds_imp_plugins` (
  `id_plugin` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `plugin_name` varchar(1000) NOT NULL,
  `plugin_class` varchar(100) NOT NULL,
  `plugin_class_path` varchar(1000) DEFAULT NULL,
  `extra_params` text,
  PRIMARY KEY (`id_plugin`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_plugins` */

insert  into `pds_imp_plugins`(`id_plugin`,`plugin_name`,`plugin_class`,`plugin_class_path`,`extra_params`) values (1,'simple_directory_harvester','pds2_imp_pl_simple_dir_harvester',NULL,NULL);
insert  into `pds_imp_plugins`(`id_plugin`,`plugin_name`,`plugin_class`,`plugin_class_path`,`extra_params`) values (2,'archiver','pds2_imp_pl_archiver',NULL,NULL);
insert  into `pds_imp_plugins`(`id_plugin`,`plugin_name`,`plugin_class`,`plugin_class_path`,`extra_params`) values (3,'wscall','pds2_imp_pl_wscall',NULL,NULL);

/*Table structure for table `pds_imp_repos` */

DROP TABLE IF EXISTS `pds_imp_repos`;

CREATE TABLE `pds_imp_repos` (
  `id_repo` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `repo_name` varchar(1000) NOT NULL COMMENT 'Name of repository',
  `repo_uri` varchar(1000) NOT NULL COMMENT 'URI to Repository',
  `repo_operation` tinyint(4) NOT NULL DEFAULT '1' COMMENT '0=Disabled, 1=Enabled, 2=Simulation',
  `repo_extra_params` text,
  PRIMARY KEY (`id_repo`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_repos` */

insert  into `pds_imp_repos`(`id_repo`,`repo_name`,`repo_uri`,`repo_operation`,`repo_extra_params`) values (1,'Repo 1','ftp://anonymous@pc-emanuelli:/scambi/TEST/REPO1',1,NULL);
insert  into `pds_imp_repos`(`id_repo`,`repo_name`,`repo_uri`,`repo_operation`,`repo_extra_params`) values (2,'Repo 2','ssh2.sftp://dg:12qwas@pc-emanuelli:/mnt/data/scambi/TEST/REPO2',1,NULL);
insert  into `pds_imp_repos`(`id_repo`,`repo_name`,`repo_uri`,`repo_operation`,`repo_extra_params`) values (3,'Repo 3','file:/mnt/data2/REPO3',1,NULL);

/*Table structure for table `pds_imp_run_rules` */

DROP TABLE IF EXISTS `pds_imp_run_rules`;

CREATE TABLE `pds_imp_run_rules` (
  `id_run_rule` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `run_rule_name` varchar(1000) NOT NULL COMMENT 'Name of the rule',
  `run_rule_operation_type` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '0=Disabled, 1=Enabled, 2=Simulation',
  `rule_operation_code` char(1) NOT NULL DEFAULT 'H' COMMENT 'H=Harvest, D=Delete, R=Rename',
  `run_rule` varchar(2000) NOT NULL COMMENT 'rule, depending on the type',
  `run_rule_type_code` char(1) NOT NULL DEFAULT 'R' COMMENT 'R=regexp, G=GUID, S=String',
  `plugin_id` smallint(5) unsigned NOT NULL COMMENT 'plugin used to apply the rule',
  PRIMARY KEY (`id_run_rule`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_run_rules` */

insert  into `pds_imp_run_rules`(`id_run_rule`,`run_rule_name`,`run_rule_operation_type`,`rule_operation_code`,`run_rule`,`run_rule_type_code`,`plugin_id`) values (1,'Rule 1',1,'H','.*\\.txt|.*\\.doc','R',1);
insert  into `pds_imp_run_rules`(`id_run_rule`,`run_rule_name`,`run_rule_operation_type`,`rule_operation_code`,`run_rule`,`run_rule_type_code`,`plugin_id`) values (2,'Rule 2',2,'H','.*\\.jpg','R',1);
insert  into `pds_imp_run_rules`(`id_run_rule`,`run_rule_name`,`run_rule_operation_type`,`rule_operation_code`,`run_rule`,`run_rule_type_code`,`plugin_id`) values (3,'rule 3',2,'H','.*\\.tgz','R',1);
insert  into `pds_imp_run_rules`(`id_run_rule`,`run_rule_name`,`run_rule_operation_type`,`rule_operation_code`,`run_rule`,`run_rule_type_code`,`plugin_id`) values (4,'Rule 4',1,'H','\'archive_name_regex\' => \'/(.*)\\..*/\' ,  \'format\'=> \"tar\" , \'select_files_regex\' => \'/.*/\'','C',2);
insert  into `pds_imp_run_rules`(`id_run_rule`,`run_rule_name`,`run_rule_operation_type`,`rule_operation_code`,`run_rule`,`run_rule_type_code`,`plugin_id`) values (5,'Rule 5',1,'H','\'url\' => \'http://pc-emanuelli/print_param.php\', \'method\' => \'GET\', \'check_success_code\' => 200','C',3);

/*Table structure for table `pds_imp_runs` */

DROP TABLE IF EXISTS `pds_imp_runs`;

CREATE TABLE `pds_imp_runs` (
  `id_run` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `run_name` varchar(1000) NOT NULL COMMENT 'Name of the Run process',
  `run_operation_type` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '0=Disabled, 1=Enabled, 2=Simulation',
  `run_interval_seconds` smallint(5) unsigned NOT NULL DEFAULT '60' COMMENT 'interval in secodns between two consequtive run',
  `last_run_datetime` datetime DEFAULT NULL COMMENT 'last datetime the process was run',
  PRIMARY KEY (`id_run`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_runs` */

insert  into `pds_imp_runs`(`id_run`,`run_name`,`run_operation_type`,`run_interval_seconds`,`last_run_datetime`) values (1,'test1',1,34,NULL);
insert  into `pds_imp_runs`(`id_run`,`run_name`,`run_operation_type`,`run_interval_seconds`,`last_run_datetime`) values (2,'test2',0,60,NULL);
insert  into `pds_imp_runs`(`id_run`,`run_name`,`run_operation_type`,`run_interval_seconds`,`last_run_datetime`) values (3,'test3',0,60,NULL);
insert  into `pds_imp_runs`(`id_run`,`run_name`,`run_operation_type`,`run_interval_seconds`,`last_run_datetime`) values (4,'test4',0,60,NULL);

/*Table structure for table `pds_imp_x_repos_run_rules` */

DROP TABLE IF EXISTS `pds_imp_x_repos_run_rules`;

CREATE TABLE `pds_imp_x_repos_run_rules` (
  `repo_id` smallint(5) unsigned NOT NULL,
  `run_rule_id` smallint(5) unsigned NOT NULL,
  `repo_run_rule_priority` smallint(6) NOT NULL DEFAULT '0' COMMENT '0 is the highest',
  `repo_run_rule_operation_type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '0=Disabled, 1=Enabled, 2=Simulation',
  PRIMARY KEY (`repo_id`,`run_rule_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_x_repos_run_rules` */

insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (1,1,0,1);
insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (1,2,0,0);
insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (2,1,30,1);
insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (2,2,0,1);
insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (2,3,20,1);
insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (1,4,10,1);
insert  into `pds_imp_x_repos_run_rules`(`repo_id`,`run_rule_id`,`repo_run_rule_priority`,`repo_run_rule_operation_type`) values (1,5,20,1);

/*Table structure for table `pds_imp_x_runs_destinations` */

DROP TABLE IF EXISTS `pds_imp_x_runs_destinations`;

CREATE TABLE `pds_imp_x_runs_destinations` (
  `run_id` smallint(5) unsigned NOT NULL,
  `repo_id` smallint(5) unsigned NOT NULL,
  `destination_operation_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '0=Disabled, 1=Enabled, 2=Simulation',
  PRIMARY KEY (`run_id`,`repo_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_x_runs_destinations` */

/*Table structure for table `pds_imp_x_runs_repos` */

DROP TABLE IF EXISTS `pds_imp_x_runs_repos`;

CREATE TABLE `pds_imp_x_runs_repos` (
  `run_id` smallint(5) unsigned NOT NULL,
  `repo_id` smallint(5) unsigned NOT NULL,
  `repo_run_priority` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'lower is served first',
  `repo_run_operation_type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '0=Disabled, 1=Enabled, 2=Simulation',
  `with_transaction` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT 'whether run must succeded on all files or not',
  `transaction_grouping_rule` varchar(1000) DEFAULT NULL COMMENT 'regular expression to define the grouping rule for the files within a transaction',
  `transaction_start_conditions` varchar(2000) DEFAULT NULL COMMENT 'pipe separated regular expressions to distinguish files to start a transaction',
  PRIMARY KEY (`run_id`,`repo_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `pds_imp_x_runs_repos` */

insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (1,1,30,1,1,'TEST_.*','TEST_.*\\.txt|TEST_.*\\.doc');
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (1,2,20,0,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (2,2,0,1,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (2,3,0,1,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (3,1,0,1,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (4,1,0,1,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (4,2,0,1,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (4,3,0,1,1,NULL,NULL);
insert  into `pds_imp_x_runs_repos`(`run_id`,`repo_id`,`repo_run_priority`,`repo_run_operation_type`,`with_transaction`,`transaction_grouping_rule`,`transaction_start_conditions`) values (4,4,0,1,1,NULL,NULL);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
