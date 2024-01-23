package com.exprivia.odc.exception;

public class ODCApiException extends RuntimeException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -8728526836439878951L;

	public ODCApiException(String message) {
		super(message);
	}
	
	public ODCApiException(String message, Throwable t) {
		super(message, t);
	}
}
