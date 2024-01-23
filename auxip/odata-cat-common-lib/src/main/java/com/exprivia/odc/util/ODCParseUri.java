package com.exprivia.odc.util;

import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.core.uri.parser.Parser;
import org.apache.olingo.server.core.uri.parser.UriParserException;
import org.apache.olingo.server.core.uri.validator.UriValidationException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.servlet.ODCBaseServices;

@Component
public class ODCParseUri {
	@Autowired
	private ODCBaseServices baseServices;
	
	public UriInfo parseQuery(String path, String query, String baseUri) throws UriParserException, UriValidationException {
		UriInfo uriInfo = new Parser(
				baseServices.getServiceMetadata().getEdm(), 
				baseServices.getOdata()
				).parseUri(path, query, null, baseUri);
		return uriInfo;
	}
}
