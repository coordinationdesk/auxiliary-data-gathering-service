package com.exprivia.auxip.service.permissions;

import com.exprivia.odc.service.permissions.ODCSystemRole;

public class AUXIPSystemRoles {
	/*
	 * The client may perform queries on the products available at the AUXIP and perform downloads
	 */
	static final public ODCSystemRole DOWNLOAD = ODCSystemRole.builder().ordinal(1).define("DOWNLOAD").name("Download").build();
	
	/*
	 * Monitoring and Reporting functionality, including the permission to perform queries on all properties.
	 */
	static final public ODCSystemRole REPORTING = ODCSystemRole.builder().ordinal(3).define("REPORTING").name("Reporting").build();
}
