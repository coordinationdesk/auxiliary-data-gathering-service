package com.exprivia.odc.data.dto;

import org.apache.olingo.commons.api.edm.FullQualifiedName;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Attribute<T> {

	public final static String FIELD_NAME = "Name";
	public final static String FIELD_VALUETYPE = "ValueType";
	public final static String FIELD_VALUE = "Value";

	@NonNull
	private String name;

	@NonNull
	private String valueType;

	private T value;
	
	private FullQualifiedName attributeType;
}
