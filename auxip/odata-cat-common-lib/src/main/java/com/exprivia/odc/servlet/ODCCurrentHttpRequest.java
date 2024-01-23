package com.exprivia.odc.servlet;

import java.io.IOException;
import java.util.Arrays;
import java.util.Base64;
import java.util.Enumeration;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.context.annotation.ScopedProxyMode;
import org.springframework.stereotype.Component;

import com.exprivia.odc.service.ODCRequestEnvironment;
import com.exprivia.odc.service.permissions.ODCUserPermissions;
import com.exprivia.odc.servlet.request.ODCECSCustomServletRequest;

import lombok.Data;

@Component
@Data
@Scope(value="request", proxyMode=ScopedProxyMode.TARGET_CLASS)
public class ODCCurrentHttpRequest {
	private static final Logger LOG = LoggerFactory.getLogger(ODCCurrentHttpRequest.class);
	
	@Autowired
	protected ODCRequestEnvironment reqEnv;
	
	@Autowired
	protected ODCUserPermissions permissions;
	
	protected ODCECSCustomServletRequest currentServletRequest = null;
	
	final static protected List<String> allowedArguments = Arrays.asList(
			"$search",
		    "$filter",
		    "$count",
		    "$orderby",
		    "$skip",
		    "$top",
		    "$expand",
		    "$select",
		    "$format");

	public ODCECSCustomServletRequest getCurrentServletRequest() {
		return currentServletRequest;
	}
	
	public void initializeRequest(HttpServletRequest req) throws IOException {
		/*
		 *  Setup main application data
		 */
		String currentUser = getUser(req.getHeader("Authorization"));
		
		// set request user
		reqEnv.setUsername(currentUser);
		// set request user permissions
		permissions.initializeUserPermissions(currentUser);

		currentServletRequest = new ODCECSCustomServletRequest(req);
	}
	
	protected List<String> getAllowedArguments() {
		return allowedArguments;
	}
	
	public boolean checkRequestParams() {
		/*
		 * Check the wicked case when the parameter name is not set
		 * for instance:
		 * {{service-root-uri}}/odata/v1/Products?=Attributes/OData.CSC.StringAttribute/any(att:att/Name eq 'name' and att/Value eq 'S2A_OPER_MSI_L0_DS_SGS_20200427T140245_S20200427T121652_N02.09')
		 * 
		 */
		String qs = currentServletRequest.getQueryString();
		
		if (qs == null)
			return true;

		if (qs.startsWith("=") || 
			qs.contains("&="))
			return false;
		
		/*
		 * Allow only default OData parameters like $filter, $count, etc.
		 */
		Enumeration<String> parameters = currentServletRequest.getParameterNames();
		while (parameters.hasMoreElements())
			if (!getAllowedArguments().contains(parameters.nextElement()))
				return false;
		return true;
	}
	
	private static String getRequestUrl(HttpServletRequest req) {
		return req.getRequestURI() + (req.getQueryString() == null ? "" : "?" + req.getQueryString());
	}
	
	public void logRequest() {
		LOG.info(String.format("REQUEST  %-10s %-5s: %s", reqEnv.getUsername(), currentServletRequest.getMethod(), getRequestUrl(currentServletRequest)));
	}

	public void logResponse(HttpServletResponse resp) {
		LOG.info(String.format("RESPONSE %-10s %d  : %s", reqEnv.getUsername(), resp.getStatus(), getRequestUrl(currentServletRequest)));
	}
	
	protected String getUser(String authorization) throws IOException {
		if (authorization == null) {
			LOG.warn("No authentication specified");
			return null;
		}

		if (!authorization.toUpperCase().startsWith("BASIC ")) {
			LOG.warn("Only Accept Basic Auth");
			return null;
		}

		// Get encoded user and password, comes after "BASIC "  
		String userpassEncoded = authorization.substring(6);
		// Decode it, using any base 64 decoder
		String userpassDecoded = new String(Base64.getDecoder().decode(userpassEncoded));

		String account[] = userpassDecoded.split(":");

		return account[0];
	}
}
