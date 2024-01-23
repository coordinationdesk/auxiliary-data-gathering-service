package com.exprivia.odc.internalservice.data.dto;

import java.sql.Timestamp;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class AipContent {
	@NonNull
	Long id;
	
	@NonNull
	String productUuid;
	
	@NonNull
	String orderId;
	
	@NonNull
	String bucketName;
	
	@NonNull
	Long productSize;
	
	@NonNull
	AipContentStatus contentStatus;
	
	@NonNull
	Timestamp uploadTime;
	
	@NonNull
	Timestamp evictionTime;
	
	/*
	 * Virtual fields (not in t_srv_aip_content table
	 */
	@NonNull
	String aipName;
	
	@NonNull
	String aipBaseRedirectUrl;
	
	@NonNull
	String productName;	
}
