package com.exprivia.odc.util;

import org.apache.commons.lang3.StringUtils;
import org.apache.olingo.server.api.ODataLibraryException;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.core.uri.parser.UriParserException;
import org.apache.olingo.server.core.uri.validator.UriValidationException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.exprivia.odc.servlet.ODCBaseServices;
import com.exprivia.odc.servlet.request.ODCECSCustomODataFilter;

public class ParseQueryFilter {
	private static final Logger LOG = LoggerFactory.getLogger(ParseQueryFilter.class);
	
	public static UriInfo parseQueryFilter(ODCBaseServices baseServices, String filter) throws UriParserException, UriValidationException {
		return parseQueryFilter(baseServices, filter, null);
	}

	public static UriInfo parseQueryFilter(ODCBaseServices baseServices, String filter, String orderBy) throws UriParserException, UriValidationException {
		return parseQueryFilter(baseServices, filter, orderBy, "Products");
	}
	
	/**
	 * Parses an entity $filter and translates it to an Olingo UriInfo object 
	 * @param ltaConfig
	 * @param filter
	 * @param orderBy
	 * @return {@link UriInfo} if parsed correctly, null otherwise
	 * @throws ODataLibraryException 
	 */
	public static UriInfo parseQueryFilter(ODCBaseServices baseServices, String filter, String orderBy, String entity) throws UriParserException, UriValidationException {
		UriInfo uriInfo;
		
		try {
			// fix filter specification
			String baseUrl = null;
			if (filter.contains("?")) { 
				baseUrl = StringUtils.substringBefore(filter, "?");
				filter = StringUtils.substringAfter(filter, "?");
			} else
				baseUrl = "/Products";
			if (!baseUrl.startsWith("/")) baseUrl = "/" + baseUrl;
			if (!filter.startsWith("$filter=")) filter = "$filter=" + filter;
			
			ODCECSCustomODataFilter ecsCustomQuery = new ODCECSCustomODataFilter(filter, false);
			
			// parse OData query
			uriInfo = baseServices.getUriParser().parseQuery(baseUrl, ecsCustomQuery.getPatchedQueryString(false), "http://localhost/odata/v1/");
			
			
		} catch (UriParserException | UriValidationException e) {
			LOG.warn("filter '{}' error: {}", filter, e.getMessage());
			throw e;
		}
		
		return uriInfo;
	}
	
	/**
	 * Fixes a product $filter prefix to be ICD compliant 
	 * @param ltaConfig
	 * @param filter
	 * @return String the new filter 
	 * @throws UriValidationException 
	 * @throws UriParserException 
	 */
	public static String sanitizeQueryFilter(ODCBaseServices baseServices, String filter) throws UriParserException, UriValidationException {
		UriInfo uriInfo = parseQueryFilter(baseServices, filter);
		
		String newFilter = uriInfo.getFilterOption().getText();
		
		return newFilter;
	}
}
