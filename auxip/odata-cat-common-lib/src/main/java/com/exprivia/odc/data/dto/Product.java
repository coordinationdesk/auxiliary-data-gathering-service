package com.exprivia.odc.data.dto;

import java.sql.Timestamp;
import java.util.List;
import java.util.UUID;

import org.apache.olingo.commons.api.edm.geo.Geospatial;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Product {

	public final static String FIELD_ID = "Id";
	public final static String FIELD_NAME = "Name";
	public final static String FIELD_CONTENTTYPE = "ContentType";
	public final static String FIELD_CONTENTLENGTH = "ContentLength";
	public final static String FIELD_ORIGINDATE = "OriginDate";
	public final static String FIELD_PUBLICATIONDATE = "PublicationDate";
	public final static String FIELD_MODIFICATIONDATE = "ModificationDate";
	public final static String FIELD_ONLINE = "Online";
	public final static String FIELD_EVICTIONDATE = "EvictionDate";
	public final static String FIELD_CHECKSUMS = "Checksum";
	public final static String FIELD_CONTENTDATE = "ContentDate";
	public final static String FIELD_FOOTPRINT = "Footprint";
	public final static String FIELD_ATTRIBUTES = "Attributes";
	
	@NonNull
	UUID id;

	@NonNull
	String name;

	@NonNull
	String contentType;

	@NonNull
	Long contentLength;

	@NonNull
	Timestamp originDate;

	@NonNull
	Timestamp publicationDate;

	@NonNull
	Timestamp modificationDate;

	@NonNull
	Boolean online;

	Timestamp evictionDate;

	@NonNull
	List<Checksum> checksums;

	@NonNull
	TimeRange contentDate;

	Geospatial footprint;
}
