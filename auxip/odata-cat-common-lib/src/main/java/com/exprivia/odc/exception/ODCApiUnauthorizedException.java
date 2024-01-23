package com.exprivia.odc.exception;

public class ODCApiUnauthorizedException extends ODCApiException {

	/**
	 * 
	 */
	private static final long serialVersionUID = 7257664820933615429L;

	public ODCApiUnauthorizedException(String message) {
		super(message);
	}
	
	public ODCApiUnauthorizedException(String message, Throwable t) {
		super(message, t);
	}
}
