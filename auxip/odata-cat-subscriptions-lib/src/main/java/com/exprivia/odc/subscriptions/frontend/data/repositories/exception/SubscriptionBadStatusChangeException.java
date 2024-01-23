package com.exprivia.odc.subscriptions.frontend.data.repositories.exception;

import com.exprivia.odc.exception.ODCApiException;

public class SubscriptionBadStatusChangeException extends ODCApiException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 3111169437559087924L;

	public SubscriptionBadStatusChangeException(String message) {
		super(message);
	}
	
	public SubscriptionBadStatusChangeException(String message, Throwable t) {
		super(message, t);
	}
}
