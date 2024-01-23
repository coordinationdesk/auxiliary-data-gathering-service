package com.exprivia.odc.subscriptions.backend.data.dto;

import java.sql.Timestamp;
import java.util.UUID;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Subscription {
	@NonNull
	Long id;
	
	@NonNull
	UUID uuid;
	
	@NonNull
	SubscriptionEvent subscriptionEvent;
	
	Timestamp lastNotificationDate;
	
	@NonNull
	Boolean stageOrder;
	
	Long priority;
	
	String notificationEndpoint;

	String notificationEpUser;
	
	String notificationEpPassword;
	
	// field from join
	String username;
}
