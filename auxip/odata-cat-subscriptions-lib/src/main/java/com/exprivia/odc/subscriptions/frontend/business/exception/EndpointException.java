package com.exprivia.odc.subscriptions.frontend.business.exception;

public class EndpointException extends RuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -5302671063320333483L;

	public EndpointException(String message) {
		super(message);
	}
	
	public EndpointException(String message, Throwable t) {
		super(message, t);
	}
}
