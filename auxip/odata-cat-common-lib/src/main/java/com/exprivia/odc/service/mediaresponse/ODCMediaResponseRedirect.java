package com.exprivia.odc.service.mediaresponse;

import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.ODataResponse;
import org.springframework.http.HttpHeaders;

public class ODCMediaResponseRedirect implements ODCMediaResponse {
	private String location;
	
	public ODCMediaResponseRedirect(String location) {
		this.location = location;
	}
	
	public String getLocation() {
		return location;
	}

	@Override
	public void setODataResponse(ODataResponse response) {
		response.setStatusCode(HttpStatusCode.TEMPORARY_REDIRECT.getStatusCode());
		response.setHeader(HttpHeaders.LOCATION, location);
	}

}
