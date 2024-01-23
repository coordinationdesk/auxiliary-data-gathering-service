package com.exprivia.auxip.internalservice.processor;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.fasterxml.jackson.core.JsonGenerationException;
import com.fasterxml.jackson.databind.JsonMappingException;

public interface InternalServiceProcessor {
	void processRequest(HttpServletRequest req, HttpServletResponse resp) throws JsonGenerationException, JsonMappingException, IOException;
}
