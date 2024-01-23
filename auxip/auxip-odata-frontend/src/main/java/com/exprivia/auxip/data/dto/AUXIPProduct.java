package com.exprivia.auxip.data.dto;

import java.sql.Timestamp;
import java.util.List;
import java.util.UUID;

import com.exprivia.odc.data.dto.Checksum;
import com.exprivia.odc.data.dto.TimeRange;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class AUXIPProduct {

	public final static String FIELD_ID = "Id";
	public final static String FIELD_NAME = "Name";
	public final static String FIELD_CONTENTTYPE = "ContentType";
	public final static String FIELD_CONTENTLENGTH = "ContentLength";
	public final static String FIELD_ORIGINDATE = "OriginDate";
	public final static String FIELD_PUBLICATIONDATE = "PublicationDate";
	public final static String FIELD_EVICTIONDATE = "EvictionDate";
	public final static String FIELD_CHECKSUMS = "Checksum";
	public final static String FIELD_CONTENTDATE = "ContentDate";
	public final static String FIELD_ATTRIBUTES = "Attributes";
	
	@NonNull
	UUID id;

	@NonNull
	String name;

	@NonNull
	String contentType;

	@NonNull
	Long contentLength;
	
	/**
	 * Date and time of the product availability at
	 * the source. Time is in UTC in the format
	 * YYYY-MM-DDThh:mm:ss.sssZ
	 */
	Timestamp originDate;

	@NonNull
	Timestamp publicationDate;

	Timestamp evictionDate;

	@NonNull
	List<Checksum> checksums;

	@NonNull
	TimeRange contentDate;
	
}
