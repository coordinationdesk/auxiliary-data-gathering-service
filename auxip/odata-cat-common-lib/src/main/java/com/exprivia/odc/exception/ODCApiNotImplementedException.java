package com.exprivia.odc.exception;

public class ODCApiNotImplementedException extends ODCApiException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -2169073554913999737L;

	public ODCApiNotImplementedException(String message) {
		super(message);
	}
	
	public ODCApiNotImplementedException(String message, Throwable t) {
		super(message, t);
	}
}
