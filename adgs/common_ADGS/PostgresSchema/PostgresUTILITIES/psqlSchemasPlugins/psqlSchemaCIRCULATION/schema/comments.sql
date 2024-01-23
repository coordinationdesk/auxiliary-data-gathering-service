/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.it A.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema comments script $

	$Id$

	$Author$
	
*/





--------------------------------------------------------------------------------
--                                                                            --
--                                  COMMENTS                                  --
--                                                                            --
--------------------------------------------------------------------------------


/* ------------------------------
   t_circulationpolicies_monitor
*/ ------------------------------
COMMENT ON TABLE t_circulationpolicies_monitor IS 'Contains the list of circulation policies to be monitored';
COMMENT ON COLUMN t_circulationpolicies_monitor.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_circulationpolicies_monitor.circulationpolicy_id IS 'Circulation policy identifier. References the t_circulationpolicies table';
COMMENT ON COLUMN t_circulationpolicies_monitor.monitoringflag IS 'Used for the monitoring of the circulation policy';
