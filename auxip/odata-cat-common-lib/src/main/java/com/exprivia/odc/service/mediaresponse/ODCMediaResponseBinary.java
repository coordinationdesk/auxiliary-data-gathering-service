package com.exprivia.odc.service.mediaresponse;

import java.io.InputStream;

import org.apache.olingo.commons.api.http.HttpHeader;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.OData;
import org.apache.olingo.server.api.ODataResponse;
import org.apache.olingo.server.api.serializer.SerializerException;

public class ODCMediaResponseBinary implements ODCMediaResponse {
	private byte[] body;
	private String mediaContentType;
	private OData odata;
	
	public ODCMediaResponseBinary(byte[] body, String mediaContentType, OData odata) {
		this.body = body;
		this.mediaContentType = mediaContentType;
		this.odata = odata;
	}

	@Override
	public void setODataResponse(ODataResponse response) {
		InputStream responseContent;
		try {
			responseContent = odata.createFixedFormatSerializer().binary(body);
		} catch (SerializerException e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}

		response.setStatusCode(HttpStatusCode.OK.getStatusCode());
		response.setContent(responseContent);
		response.setHeader(HttpHeader.CONTENT_TYPE, mediaContentType);
	}

}
