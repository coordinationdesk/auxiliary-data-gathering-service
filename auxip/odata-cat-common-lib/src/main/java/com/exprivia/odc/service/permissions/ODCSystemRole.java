package com.exprivia.odc.service.permissions;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class ODCSystemRole {
	private int ordinal;
	private String define;
	private String name;
}
