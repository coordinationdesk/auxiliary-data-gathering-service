package com.exprivia.odc.subscriptions.frontend.data.dto;

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
	public final static String FIELD_ID = "Id";
	public final static String FIELD_STATUS = "Status";
	public final static String FIELD_SUBSCRIPTIONEVENT = "SubscriptionEvent";
	public final static String FIELD_FILTERPARAM = "FilterParam";
	public final static String FIELD_SUBMISSIONDATE = "SubmissionDate";
	public final static String FIELD_LASTNOTIFICATIONDATE = "LastNotificationDate";
	public final static String FIELD_NOTIFICATIONENDPOINT = "NotificationEndpoint";
	public final static String FIELD_NOTIFICATIONEPUSERNAME = "NotificationEpUsername";
	public final static String FIELD_NOTIFICATIONEPPASSWORD = "NotificationEpPassword";
	
	public final static String FIELD_NOTIFICATIONS = "Notifications";
	public final static String FIELD_ORDERS = "Orders";
	
	@NonNull 
	UUID id;
	
	@NonNull
	SubscriptionStatus status;
	
	@NonNull
	String filterParam;
	
	@NonNull
	Timestamp submissionDate;
	
	Timestamp lastNotificationDate;
	
	@NonNull
	String notificationEndpoint;
	
	String notificationEpUsername;
	
	String notificationEpPassword;
	
	// internal postgres id
	Long uniqueId;
}
