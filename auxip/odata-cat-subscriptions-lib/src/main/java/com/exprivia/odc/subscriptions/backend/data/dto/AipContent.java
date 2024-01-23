package com.exprivia.odc.subscriptions.backend.data.dto;

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
	Integer statusId;
	
	@NonNull
	Timestamp uploadTime;
	
	@NonNull
	Timestamp evictionTime;
}
