package com.exprivia.odc.subscriptions.frontend.internalservice.business.exception;

public class ItemNotFoundException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1136608155825184727L;

	public ItemNotFoundException(String message) {
		super(message);
	}
	
	public ItemNotFoundException(String message, Throwable t) {
		super(message, t);
	}
}
