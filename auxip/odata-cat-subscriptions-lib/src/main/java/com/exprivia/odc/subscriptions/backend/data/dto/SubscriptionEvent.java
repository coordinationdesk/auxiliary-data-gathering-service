package com.exprivia.odc.subscriptions.backend.data.dto;

public enum SubscriptionEvent {
	CREATED, DELETED, UPDATED;
	
    public static SubscriptionEvent fromString(String value) {
    	switch (value) {
    	case "CREATED": return CREATED;
    	case "DELETED": return DELETED;
    	case "UPDATED": return UPDATED;
    	}
    	throw new RuntimeException("Unrecognized SubscriptionEvent value: " + value);
    }

}
