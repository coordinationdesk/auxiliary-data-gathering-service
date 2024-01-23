package com.exprivia.odc.metrics.data.dto;

public enum MetricType {
	COUNTER, 
	GAUGE; 

	public String getDescription() {
		String lowerName = this.name().toLowerCase();
		return lowerName.substring(0, 1).toUpperCase() + lowerName.substring(1);
	}
	
    public static MetricType fromString(String text) {
    	switch (text) {
    	case "Counter":
    		return COUNTER;
    	case "Gauge":
    		return GAUGE;
    	default:
    		throw new RuntimeException("Cannot decode metric type from string: " + text);
    	}
    }
}
