package com.exprivia.odc.subscriptions.frontend.service.permissions;

public enum ODCSubscriptionsActions {
	NOTIFICATION_FIND_WITH_PARAMS(200),
	SUBSCRIPTION_FIND_BY_ID(201),
	SUBSCRIPTION_FIND_WITH_PARAMS(202),
	SUBSCRIPTION_INSERT(203),
	SUBSCRIPTION_UPDATE(204);

	private int action;

    private ODCSubscriptionsActions(int code) {
        this.action = code;
    }
 
    public int getCode() {
        return action;
    }
}
