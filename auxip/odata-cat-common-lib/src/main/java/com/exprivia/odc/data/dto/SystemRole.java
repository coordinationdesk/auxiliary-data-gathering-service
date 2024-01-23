package com.exprivia.odc.data.dto;

import com.exprivia.odc.service.permissions.ODCSystemRole;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class SystemRole {
	public final static String FIELD_NAME = "Name";
	public final static String FIELD_DESCRIPTION = "Description";

	@NonNull
	ODCSystemRole value;

	@NonNull
	String description;
}
