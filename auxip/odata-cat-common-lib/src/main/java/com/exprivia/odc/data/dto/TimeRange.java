package com.exprivia.odc.data.dto;

import java.sql.Timestamp;

import lombok.AccessLevel;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class TimeRange {

	public final static String FIELD_START = "Start";
	public final static String FIELD_END = "End";

	@NonNull
	Timestamp start;

	@NonNull
	Timestamp end;
}
