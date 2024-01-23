package com.exprivia.odc.subscriptions.frontend.data.dto;

import java.sql.Timestamp;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class Notification {
	public final static String FIELD_PRODUCTID = "ProductId";
	public final static String FIELD_PRODUCTNAME = "ProductName";
	public final static String FIELD_SUBSCRIPTIONID = "SubscriptionID";
	public final static String FIELD_NOTIFICATIONDATE = "NotificationDate";
	
	@NonNull 
	String productId;
	
	@NonNull
	String productName;
	
	String subscriptionID;
	
	@NonNull
	Timestamp notificationDate;
}
