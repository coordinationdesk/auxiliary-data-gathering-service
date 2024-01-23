package com.exprivia.odc.data.dto;

import java.sql.Timestamp;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class User {
	public final static String FIELD_USERNAME = "Username";
	public final static String FIELD_EMAIL = "Email";
	public final static String FIELD_CREATED = "Created";
	public final static String FIELD_DEFAULTPRIORITY = "DefaultPriority";
	public final static String FIELD_MAXPRIORITY = "MaxPriority";
	
	// navigation field
	public final static String FIELD_SYSTEMROLES = "SystemRoles";
	public final static String FIELD_QUOTAS = "Quotas";

	@NonNull
	String username;

	@NonNull
	String email;

	@NonNull
	Timestamp created;

	@NonNull
	Long defaultPriority;

	@NonNull
	Long maxPriority;
}
