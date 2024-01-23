package com.exprivia.odc.metrics.service.permissions;

public enum ODCMetricsActions {
	METRIC_FIND_BY_ID(100),
	METRIC_FIND_WITH_PARAMS(101);

	private int action;

    private ODCMetricsActions(int code) {
        this.action = code;
    }
 
    public int getCode() {
        return action;
    }
}
