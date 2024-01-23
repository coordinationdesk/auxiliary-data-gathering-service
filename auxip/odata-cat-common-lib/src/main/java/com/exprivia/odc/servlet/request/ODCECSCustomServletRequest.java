package com.exprivia.odc.servlet.request;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletRequestWrapper;

/**
 * This class is a decoupling layer between the input query request and the OData library
 * This is necessary due to ECS required queries not supported by the standard OData protocol
 * 
 * @author andrea
 *
 */
public class ODCECSCustomServletRequest extends HttpServletRequestWrapper {
	private HttpServletRequest request;
	private static String SYSTEM_FILTER = "$filter";
	private String filterParam;
	private String queryString;
	private StringBuffer requestUrl;

	private static Pattern QUOTED_ID = Pattern.compile("(?<=\\()'(?<id>[a-f0-9-]+)'(?=\\))");
	
	public ODCECSCustomServletRequest(HttpServletRequest request) {
		super(request);
		this.request = request;
		
		String filterParam = request.getParameter(SYSTEM_FILTER);
		if (filterParam != null && filterParam.length() > 0) {
			ODCECSCustomODataFilter ecsFilter = new ODCECSCustomODataFilter(request.getQueryString(), true);
			this.filterParam = ecsFilter.getPatchedFilter();
			this.queryString = ecsFilter.getPatchedQueryString(true);
		} else {
			this.filterParam = "";
			this.queryString = request.getQueryString();
		}

		this.requestUrl = new StringBuffer(getPatchedRequestUrl());
	}
	
	@Override
	public String getQueryString() {
		return queryString;
	}
	
	@Override
	public String getParameter(String name) {
		if (name.equals(SYSTEM_FILTER)) {
			return filterParam;
		} else
			return request.getParameter(name);
	}
	
	@Override
	public StringBuffer getRequestURL() {
		return requestUrl;
	}

	@Override
	public String getServletPath() {
		return "/odata/v1";
	}
	
	private String getPatchedRequestUrl() {
		// analyze url to check if missing quotes are present in the entities ids
		// and add missing quotes
		String url = request.getRequestURL().toString();
		
		Matcher quotedIdMatcher = QUOTED_ID.matcher(url);
		String newUrl;
		if (quotedIdMatcher.find()) {
			newUrl = quotedIdMatcher.replaceFirst("$1");
		} else {
			newUrl = url;
		}
		
		return newUrl;
	}
}
