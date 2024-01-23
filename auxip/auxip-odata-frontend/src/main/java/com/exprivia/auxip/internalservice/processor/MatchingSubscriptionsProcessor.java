package com.exprivia.auxip.internalservice.processor;

import java.io.IOException;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.olingo.commons.api.format.ContentType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;

import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.subscriptions.frontend.internalservice.business.MatchingSubscriptionsBusiness;
import com.exprivia.odc.subscriptions.frontend.internalservice.business.exception.ItemNotFoundException;
import com.fasterxml.jackson.core.JsonGenerationException;
import com.fasterxml.jackson.databind.JsonMappingException;
import com.fasterxml.jackson.databind.ObjectMapper;

@Component
public class MatchingSubscriptionsProcessor implements InternalServiceProcessor {
	@Autowired
	private MatchingSubscriptionsBusiness matcher;

	private static final Logger LOG = LoggerFactory.getLogger(MatchingSubscriptionsProcessor.class);

	@Override
	public void processRequest(HttpServletRequest req, HttpServletResponse resp) throws JsonGenerationException, JsonMappingException, IOException {
		Long pendingInventoryId = null;
		
		String pendingInventoryIdString = req.getParameter("pendingInventoryId");
		if (pendingInventoryIdString == null) {
			errorResponse(resp, HttpStatus.BAD_REQUEST, "missing required argument pendingInventoryId");
			return;
		}
		try {
			pendingInventoryId = Long.valueOf(pendingInventoryIdString);
		} catch (NumberFormatException e) {
			errorResponse(resp, HttpStatus.BAD_REQUEST, e.getMessage());
			return;
		}

		List<Long> ret = null;
		try {
			// call business logic
			ret = matcher.getMatchingSubscriptions(pendingInventoryId);
		} catch (ItemNotFoundException e) {
			errorResponse(resp, HttpStatus.NOT_FOUND, e.getMessage());
			return;
		} catch (ODCApiUnauthorizedException e) {
			errorResponse(resp, HttpStatus.BAD_REQUEST, e.getMessage());
			return;
		}
		
		resp.setContentType(ContentType.APPLICATION_JSON.toContentTypeString());
		
		ObjectMapper mapper = new ObjectMapper();
		mapper.writeValue(resp.getOutputStream(), ret);
	}
	
	private void errorResponse(HttpServletResponse resp, HttpStatus code, String message) {
		resp.setStatus(code.value());
		LOG.error(message);
	}
}
