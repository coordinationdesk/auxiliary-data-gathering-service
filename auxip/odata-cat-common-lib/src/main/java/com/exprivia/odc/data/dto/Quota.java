package com.exprivia.odc.data.dto;

import java.time.Duration;

import org.apache.olingo.commons.api.edm.FullQualifiedName;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Quota {
	public final static String FIELD_NUMBER = "Number";
	public final static String FIELD_DURATION = "Duration";
	
	@NonNull
	Long number;

	// this field exists only when type = TOTAL
	Duration duration;
	
	FullQualifiedName quotaType;
}
