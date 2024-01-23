package com.exprivia.odc.util;

import java.net.URI;
import java.net.URISyntaxException;
import java.sql.Timestamp;
import java.time.Instant;
import java.time.ZoneId;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Locale;
import java.util.TimeZone;

import org.apache.commons.lang3.RegExUtils;
import org.apache.commons.lang3.StringUtils;
import org.apache.olingo.commons.api.data.ContextURL;
import org.apache.olingo.commons.api.edm.EdmBindingTarget;
import org.apache.olingo.commons.api.edm.EdmEntitySet;
import org.apache.olingo.commons.api.http.HttpStatusCode;
import org.apache.olingo.server.api.ODataApplicationException;
import org.apache.olingo.server.api.uri.UriInfo;
import org.apache.olingo.server.api.uri.UriInfoResource;
import org.apache.olingo.server.api.uri.UriResource;
import org.apache.olingo.server.api.uri.UriResourceEntitySet;
import org.apache.olingo.server.api.uri.UriResourceNavigation;
import org.apache.olingo.server.api.uri.queryoption.ExpandItem;
import org.apache.olingo.server.api.uri.queryoption.SelectItem;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.dao.DataAccessResourceFailureException;

import com.exprivia.odc.exception.ODCApiUnauthorizedException;
import com.exprivia.odc.internalservice.data.dto.AipContent;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public class Util {
	private static final Logger LOG = LoggerFactory.getLogger(Util.class);
	private static final DateTimeFormatter ISO8601DateTimeFormatter = DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ss.SSSSSS'Z'").withZone(ZoneId.of("UTC"));

	static {
		TimeZone.setDefault(TimeZone.getTimeZone("UTC"));
	}
	
	public static String trimQuotes(String str) {
		return RegExUtils.removePattern(str, "^['\"]|['\"]$");
	}

	public static Timestamp parseDateString(String date) {
		String newDate = date.replace(' ', 'T');
		if (!date.endsWith("Z"))
			newDate += "Z";
		Timestamp tm = Timestamp.from(Instant.parse(newDate));
		return tm;
	}
	
	public static String formatTimestamp(Timestamp ts) {
		// format to ISO 8601 UTC timestamp
		return ISO8601DateTimeFormatter.format(ts.toInstant());
	}

	public static EdmEntitySet getEdmEntitySet(UriInfoResource uriInfo) throws ODataApplicationException {
		List<UriResource> resourcePaths = uriInfo.getUriResourceParts();
		// To get the entity set we have to interpret all URI segments
		if (!(resourcePaths.get(0) instanceof UriResourceEntitySet)) {
			// Here we should interpret the whole URI but in this example we do not support navigation so we throw an exception
			throw new ODataApplicationException("Invalid resource type for first segment.",
					HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ENGLISH);
		}

		UriResourceEntitySet uriResource = (UriResourceEntitySet) resourcePaths.get(0);

		return uriResource.getEntitySet();
	}

	public static EdmEntitySet getNavigationTargetEntitySet(final UriInfoResource uriInfo) throws ODataApplicationException {

		EdmEntitySet entitySet;
	    final List<UriResource> resourcePaths = uriInfo.getUriResourceParts();

	    // First must be entity set (hence function imports are not supported here).
	    if (resourcePaths.get(0) instanceof UriResourceEntitySet) {
	        entitySet = ((UriResourceEntitySet) resourcePaths.get(0)).getEntitySet();
	    } else {
	        throw new ODataApplicationException("Invalid resource type.",
	                HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(), Locale.ROOT);
	    }

	    int navigationCount = 0;
	    while (entitySet != null
	        && ++navigationCount < resourcePaths.size()
	        && resourcePaths.get(navigationCount) instanceof UriResourceNavigation) {
	        final UriResourceNavigation uriResourceNavigation = (UriResourceNavigation) resourcePaths.get(navigationCount);
	        final EdmBindingTarget target = entitySet.getRelatedBindingTarget(uriResourceNavigation.getProperty().getName());
	        if (target instanceof EdmEntitySet) {
	            entitySet = (EdmEntitySet) target;
	        } else {
	            throw new ODataApplicationException("Singletons not supported", HttpStatusCode.NOT_IMPLEMENTED.getStatusCode(),
	                                                 Locale.ROOT);
	        }
	    }

	    return entitySet;
	}
	
	public static JDBCQueryParams addArbitrarySQLCondition(JDBCQueryParams jdbcQueryParams, String sql, List<Object> params) {
		JDBCFilterExpression filter = null;
		if (jdbcQueryParams != null && jdbcQueryParams.getJdbcFilterExpression() != null) {
			// copy the filter instance from the source jdbcQueryParams
			filter = JDBCFilterExpression.clone(jdbcQueryParams.getJdbcFilterExpression());
			filter.setPrepeparedWhere("(" + jdbcQueryParams.getJdbcFilterExpression() + ") and " + sql);
		} else {
			filter = new JDBCFilterExpression(null);
			filter.setPrepeparedWhere(sql);
		}
		for (Object param : params) {
			filter.addParameter(param);
		}
		
		// create new params, the jdbcQueryParams can be null
		JDBCQueryParams newParams = JDBCQueryParams.builder().jdbcFilterExpression(filter).build();

		return newParams;
	}
	
	public static String getDownloadUrl(AipContent aipContent) {
		return aipContent.getAipBaseRedirectUrl() + "/" + aipContent.getBucketName() + "/" + aipContent.getProductName();
	}
	
	public static String hidePassword(String password) {
		if (password == null)
			return null;
		return StringUtils.repeat('*', password.length());
	}
	
	static public ODataApplicationException createUnauthorizedODataApplicationException(ODCApiUnauthorizedException e) {
		LOG.warn(e.getMessage());

		return new ODataApplicationException(
				e.getMessage(), 
				HttpStatusCode.UNAUTHORIZED.getStatusCode(), 
				Locale.US, 
				e);
	}
	
	static public ODataApplicationException createSqlTimeoutODataApplicationException(DataAccessResourceFailureException e) {
		e.printStackTrace();
		LOG.error(e.getClass().getName() + ": " + e.getMessage());

		return new ODataApplicationException(
				"Query execution timeout exceeded", 
				HttpStatusCode.REQUEST_TIMEOUT.getStatusCode(), 
				Locale.US, 
				e);
	}
	
	static public ContextURL createContextUrl(UriInfo uriInfo) {
		/* 	
		 * http://docs.oasis-open.org/odata/odata/v4.0/odata-v4.0-part1-protocol.html
		 * 
		 * 10.2 Collection of Entities
		 * 
		 * Context URL template:
		 * {context-url}#{entity-set}
		 * {context-url}#Collection({type-name})
		 * 
		 * If all entities in the collection are members of one entity set, its name is the context URL fragment.
		 * Example 11: resource URL and corresponding context URL
		 * http://host/service/Customers
		 * http://host/service/$metadata#Customers
		 * 
		 * If the entities are contained, then entity-set is the top-level entity set followed by the path to the 
		 * containment navigation property of the containing entity.
		 * Example 12: resource URL and corresponding context URL for contained entities
		 * http://host/service/Orders(4711)/Items
		 * http://host/service/$metadata#Orders(4711)/Items
		 * 
		 * If the entities in the response are not bound to a single entity set, such as from a function or action 
		 * with no entity set path, a function import or action import with no specified entity set, or a navigation 
		 * property with no navigation property binding, the context URL specifies the type of the returned entity 
		 * collection. 
		 */
		/*
		 * set service root
		 */
		ContextURL.Builder builder = ContextURL.with();
		try {
			builder.serviceRoot(new URI(""));
		} catch (URISyntaxException e) {
		}
		
		/*
		 * set path
		 */
		for (UriResource uriResource : uriInfo.getUriResourceParts()) {
			if (uriResource instanceof UriResourceEntitySet) {
				UriResourceEntitySet uriResourceEntitySet = (UriResourceEntitySet)uriResource;
				builder.entitySet(uriResourceEntitySet.getEntitySet());
				/*
				if (uriResourceEntitySet.getKeyPredicates() != null &&
					uriResourceEntitySet.getKeyPredicates().size() > 0) {
					builder.keyPath(uriResourceEntitySet.getKeyPredicates().get(0).getText());
				}
				*/
				if (uriResourceEntitySet.getKeyPredicates() != null &&
					uriResourceEntitySet.getKeyPredicates().size() > 0 &&
					uriResourceEntitySet.getKeyPredicates().get(0).getName().equals("Id")) {
					builder.suffix(ContextURL.Suffix.ENTITY);
				}
			}
			if (uriResource instanceof UriResourceNavigation) {
//				builder.navOrPropertyPath(((UriResourceNavigation)uriResource).getSegmentValue());
//				builder.type(((UriResourceNavigation)uriResource).getType());
//				builder.navOrPropertyPath(((UriResourceNavigation)uriResource).getProperty().getName());
				builder.entitySetOrSingletonOrType(((UriResourceNavigation)uriResource).getType().getName());
			}
		}
		
		/*
		 * process $select options
		 */
		String selectList = "";
		if (uriInfo.getSelectOption() != null) {
			for (SelectItem item : uriInfo.getSelectOption().getSelectItems()) {
				String itemPath = "";
				for (UriResource part : item.getResourcePath().getUriResourceParts()) {
					itemPath += part.getSegmentValue();
				}
				selectList += selectList.length() > 0 ? "," + itemPath : itemPath;
			}
		}
		
		/*
		 * process $expand options
		 */
		if (uriInfo.getExpandOption() != null) {
			for (ExpandItem item : uriInfo.getExpandOption().getExpandItems()) {
				String itemPath = "";
				for (UriResource part : item.getResourcePath().getUriResourceParts()) {
					itemPath += part.getSegmentValue();
				}
				itemPath += "()";
				selectList += selectList.length() > 0 ? "," + itemPath : itemPath;
			}
		}

		if (!selectList.equals(""))
			builder.selectList(selectList);
		
		return builder.build();
	}
}
