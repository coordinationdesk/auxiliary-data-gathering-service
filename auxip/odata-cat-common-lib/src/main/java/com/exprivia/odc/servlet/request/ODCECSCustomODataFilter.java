package com.exprivia.odc.servlet.request;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.exprivia.odc.edmprovider.ODCEdmProvider;

/**
 * This class is in charge of several OData queries fixtures to be properly
 * handled by the Apache-Olingo library
 * 
 * @author andrea
 *
 */
public class ODCECSCustomODataFilter {
	
	private static String FILTER_REGEX = "(?s)^(.*?)\\$filter=(.*?)(&(.*)|$)";	// (?s) dot match all (even newline chars)
	private static String ENCODING = StandardCharsets.UTF_8.toString();

	private Pattern filterPattern = Pattern.compile(FILTER_REGEX);
	
	private static String NAMESPACE = ODCEdmProvider.NAMESPACE.replace(".", "\\.");

	private static Pattern ATTRIBUTE_CAST = Pattern.compile("(Attributes/(?<castto>" + NAMESPACE + "\\.\\w+)/any\\(.*?(?<!" + NAMESPACE + "\\.\\w{0,30}Attribute)(?<value>/Value).*?\\))");
	private static Pattern ATTRIBUTE_ALIAS = Pattern.compile("(Attributes(/" + NAMESPACE + "\\.\\w+)?/any\\((?<alias1>\\w+?):(?<alias2>\\w+?)(/(\\w|\\.)+)?/Name(.*?)\\s(?<alias3>\\w+?)(/(\\w|\\.)+)?/Value(.*?)\\))");

	private String requestQuery;
	private String filter;
	private String queryBeforeFilter;
	private String queryAfterFilter;
	
	public ODCECSCustomODataFilter(String requestQuery, boolean isEncoded) {
		
		if (requestQuery != null && requestQuery.length() > 0 && isEncoded) {
			try {
				this.requestQuery = URLDecoder.decode(requestQuery, ENCODING);
			} catch (UnsupportedEncodingException e) {
				throw new RuntimeException(e);
			}
		} else {
			this.requestQuery = requestQuery;
		}

		Matcher m = filterPattern.matcher(this.requestQuery);
		if (m.find()) {
			this.queryBeforeFilter = m.group(1);
			this.filter = m.group(2);
			this.queryAfterFilter = m.group(4);
		} else {
			this.queryBeforeFilter = "";
			this.filter = "";
			this.queryAfterFilter = "";
		}
	}
	
	public String getPatchedQueryString(boolean encode) {
		try {
			if (filter != null && filter.length() > 0) {
				// $filter is present
				String filter = getPatchedFilter();
				if (encode) {
					filter = URLEncoder.encode(filter, ENCODING).replace("+", "%20");
				}
				
				StringBuilder sb = new StringBuilder(queryBeforeFilter);
				if (filter.length() > 0) {
					sb.append("$filter=").append(filter).append("&");
				}
				sb.append(queryAfterFilter);
					
				return sb.toString();
			} else {
				return requestQuery;
			}
		} catch (UnsupportedEncodingException e) {
			throw new RuntimeException(e);
		}
	}
	
	public String getPatchedFilter() {
		/*
		 * handle attributes cast handling
		 * use case query: Attributes/OData.CSC.StringAttribute/any(a:(a/Name eq 'productType' and a/Value eq 'MSI_L2A_DS'))
		 * the Attributes cast to OData.CSC.StringAttribute does not take place through Olingo and a/value is not found
		 * 
		 * working query:  Attributes/any(a:(a/Name eq 'productType' and a/OData.CSC.StringAttribute/Value eq 'MSI_L2A_DS'))
		 * the Attributes cast takes place inside the "any" operator
		 * 
		 * try to do this $filter patch by code
		 */
		String tempFilter = this.filter;
		Matcher attributeCastMatcher = ATTRIBUTE_CAST.matcher(this.filter);
		int attributeCastPos = 0;
		int previousCastSourcePos = 0;
		String newFilter = "";
		while (attributeCastMatcher.find(attributeCastPos)) {
			// take the cast for the Value part of the attribute condition
			String castTo = attributeCastMatcher.group("castto");
			
			// start position of Value
			int valueCastPos = attributeCastMatcher.start("value");
			// string to add to the Value condition
			String newValue =  "/" + castTo + attributeCastMatcher.group("value");
			// add to the output filter
			newFilter += tempFilter.substring(previousCastSourcePos, valueCastPos) + newValue;
			
			// position for the next match
			attributeCastPos = attributeCastMatcher.end();
			// position for the next input source
			previousCastSourcePos = attributeCastMatcher.end("value");
		}
		newFilter += tempFilter.substring(previousCastSourcePos);
		tempFilter = newFilter;
		
		/*
		 * Replace attribute aliases with different names for each couple of 
		 * attribute name/value, to avoid self join on the same table in AND
		 * condition that will retrieve no results
		 */
		Matcher attributeAliasMatcher = ATTRIBUTE_ALIAS.matcher(tempFilter);
		int attributeAliasPos = 0;
		int previousAliasSourcePos = 0;
		int aliasCounter = 1;
		newFilter = "";
		while (attributeAliasMatcher.find(attributeAliasPos)) {
			//System.out.println(attributeAliasMatcher.group());
			for (String groupAlias : Arrays.asList("alias1", "alias2", "alias3")) {
				//System.out.println(attributeAliasMatcher.group(groupAlias));
				
				newFilter += tempFilter.substring(previousAliasSourcePos, attributeAliasMatcher.start(groupAlias));
				newFilter += attributeAliasMatcher.group("alias1") + aliasCounter;
				previousAliasSourcePos = attributeAliasMatcher.end(groupAlias);
			}
			aliasCounter++;
			attributeAliasPos = attributeAliasMatcher.end();
		}
		newFilter += tempFilter.substring(previousAliasSourcePos);
		tempFilter = newFilter;
		
		/*
		 * Replace wrong casts
		 */
		tempFilter = tempFilter.replaceAll("OData\\.CSC\\.Order\\.JobStatus", "OData.CSC.JobStatus");
		
		/*
		 * Replace bad quotes
		 */
		tempFilter = tempFilter.replaceAll("‘|’", "'");
		
		return tempFilter;
	}
	
}
