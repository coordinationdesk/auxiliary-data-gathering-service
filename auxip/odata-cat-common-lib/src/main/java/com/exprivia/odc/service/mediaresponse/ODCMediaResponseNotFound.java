package com.exprivia.odc.service.mediaresponse;

import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.ODataResponse;

public class ODCMediaResponseNotFound implements ODCMediaResponse {
	
	@Override
	public void setODataResponse(ODataResponse response) {
		response.setStatusCode(HttpStatusCode.NOT_FOUND.getStatusCode());
	}

}
