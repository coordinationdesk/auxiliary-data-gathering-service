package com.exprivia.odc.internalservice.data.dto;

public enum AipContentStatus {
	ONLINE("online"), 		// product ready to be downloaded
	ONGOING("ongoing"),		// product not yet transferred to AIP
	EVICTED("evicted");		// product removed
	
	private String aipContentStatus;
	
	AipContentStatus(String jobStatus) {
		this.aipContentStatus = jobStatus;
	}
	
	public String status() {
		return aipContentStatus;
	}
	
    public static AipContentStatus fromString(String text) {
    	switch (text) {
    	case "ONLINE":
    		return ONLINE;
    	case "ONGOING":
    		return ONGOING;
    	case "INEVICTION":
    	case "EVICTED":
    		return EVICTED;
    	default:
    		throw new RuntimeException("Cannot decode AipContent status from string: " + text);
    	}
	}
}
