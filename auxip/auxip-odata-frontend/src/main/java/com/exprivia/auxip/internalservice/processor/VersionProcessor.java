package com.exprivia.auxip.internalservice.processor;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.olingo.commons.api.format.ContentType;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.auxip.util.Version;
import com.fasterxml.jackson.core.JsonGenerationException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;

@Component
public class VersionProcessor implements InternalServiceProcessor {
	@Autowired
	private Version version;

	@Override
	public void processRequest(HttpServletRequest req, HttpServletResponse resp) throws JsonGenerationException, JsonMappingException, IOException {
		resp.setContentType(ContentType.APPLICATION_JSON.toContentTypeString());

		new ObjectMapper().writeValue(resp.getOutputStream(), version.getAsMap());
	}
	
	public String getVersion() throws IOException {
		return new ObjectMapper().writeValueAsString(version.getAsMap());
	}
}
