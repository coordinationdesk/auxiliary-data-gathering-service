package com.exprivia.odc.subscriptions.frontend.data.dto;

public enum SubscriptionStatus {
	RUNNING, 
	PAUSED, 
	CANCELLED;

	public String getDescription() {
		return this.name().toLowerCase();
	}
	
    public static SubscriptionStatus fromString(String text) {
    	switch (text) {
    	case "RUNNING":
    	case "running":
    		return RUNNING;
    	case "PAUSED":
    	case "paused":
    		return PAUSED;
    	case "CANCELLED":
    	case "cancelled":
    		return CANCELLED;
    	default:
    		throw new RuntimeException("Cannot decode subscription status from string: " + text);
    	}
    }
}
