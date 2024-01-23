package com.exprivia.odc.data.dto;

import java.sql.Timestamp;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Data
@Builder
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Checksum {

	public final static String FIELD_ALGORITHM = "Algorithm";
	public final static String FIELD_VALUE = "Value";
	public final static String FIELD_CHECKSUMDATE = "ChecksumDate";

	@NonNull
	String algorithm;

	@NonNull
	String value;

	Timestamp checksumDate;
}
