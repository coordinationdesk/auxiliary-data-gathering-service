package com.exprivia.odc.subscriptions.backend.data.dto;

public enum PendingInventoryEvent {
	INSERT, DELETE;
	
    public static PendingInventoryEvent fromString(String value) {
    	switch (value) {
    	case "INSERT": return INSERT;
    	case "DELETE": return DELETE;
    	}
    	throw new RuntimeException("Unrecognized PendingInventoryEvent value: " + value);
    }

}
