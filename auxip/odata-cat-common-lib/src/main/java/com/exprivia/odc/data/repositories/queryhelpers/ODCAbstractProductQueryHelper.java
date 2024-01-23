package com.exprivia.odc.data.repositories.queryhelpers;

import java.sql.Timestamp;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCFilterExpressionEditor;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.util.Util;

import lombok.Builder;
import lombok.Data;

/**
 * Utility class to handle products with attributes and checksums queries
 * 
 * @author andrea
 *
 * @param <T>
 */
abstract public class ODCAbstractProductQueryHelper<T> extends Object {
	private static final Logger LOG = LoggerFactory.getLogger(ODCAbstractProductQueryHelper.class);
	private DBTableTranslator tableTranslator;
	private ODCDBDefaults odcConfig;
	private Map<String, String> attributeNameMap = new HashMap<String, String>();
	private Pattern lambdaPattern = Pattern.compile("(?<type>Attributes|Checksum)\\((?<alias>\\w+)\\)\\.(?<name>\\w+)::(?<valuetype>\\w+)");
	
	@SuppressWarnings("serial")
	private static final Map<String, String> EDMTYPE_SQL_CAST = new HashMap<String, String>() {
		{
			put("Int64", "int");
			put("Integer", "int");
			put("String", "text");
			put("Double", "float");
			put("DateTimeOffset", "timestamp");
			put("Boolean", "boolean");
			put("bool", "boolean");
		}
	};

	public ODCAbstractProductQueryHelper(DBTableTranslator tableTranslator, ODCDBDefaults odcConfig) {
		this.tableTranslator = tableTranslator;
		this.odcConfig = odcConfig;
	}
	
	protected Pattern getLambdaPattern() {
		return lambdaPattern;
	}
	
	protected List<String> getFieldList() {
		return Arrays.asList(
				"i.id", 
				"i.uuid", 
				"i.name", 
				"i.contenttype", 
				"i.contentlength", 
				"i.origindate", 
				"i.creationdate", 
				"i.modificationdate", 
				"i.checksum", 
				"i.validitystart", 
				"i.validitystop", 
				"st_asbinary(i.footprint) footprint", 
				"i.validity", 
				"c.eviction_time"
				);
	}
	
	protected List<String> getGroupByFieldList() {
		return Arrays.asList(
				"i.id", 
				"i.uuid", 
				"i.name", 
				"i.contenttype", 
				"i.contentlength", 
				"i.origindate", 
				"i.creationdate", 
				"i.modificationdate", 
				"i.checksum", 
				"i.validitystart", 
				"i.validitystop", 
				"i.footprint", 
				"i.validity", 
				"c.eviction_time"
				);
	}
	
	protected String getColumnsListAsString() {
		return String.join(",", getFieldList());
	}
	
	protected String getGroupByColumnsListAsString() {
		return String.join(",", getGroupByFieldList());
	}
	
	protected String getInventoryTableName() {
		return "{inventory}";
	}
	
	protected String getFindByIdSql() {
		return getFindSelectSql()
				+ getWhereWordSql()
				+ " uuid = ?"
				+ getAndValidityCondition();
	}
	
	protected String getValidityCondition() {
		return "validity = 't'";
	}
	
	protected String getAndValidityCondition() {
		return " and " + getValidityCondition();
	}
	
	protected String getFindSelectSql() {
		return "SELECT "
				+ getColumnsListAsString()
				+ " FROM "
				+ getInventoryTableName()
				+ " i "
				+ " LEFT JOIN (select *, eviction_time is not null online from {aipContent}) c on c.product_uuid = i.uuid ";
	}
	
	protected String getCountSql() {
		return "SELECT "
				+ " COUNT(1) "
				+ " FROM "
				+ getInventoryTableName()
				+ " i "
				+ " LEFT JOIN (select *, eviction_time is not null online from {aipContent}) c on c.product_uuid = i.uuid ";
	}
	
	protected String getWhereWordSql() {
		return " WHERE ";
	}

	protected String getGroupByWordSql() {
		return " GROUP BY ";
	}
	
	protected String getOrderByWordSql() {
		return " ORDER BY ";
	}
	
	protected String getDefaultOrderBySql() {
		return getOrderByWordSql() 
				+ " i.id ";
	}
	
	protected String getDefaultGroupBySql() {
		/*
		return getGroupByWordSql()
				+ getGroupByColumnsListAsString();
		*/
		return "";
	}
	
	protected String getRowLimitSql(JDBCQueryParams jdbcQuery) {
		return " LIMIT " 
				+ String.valueOf(
						jdbcQuery.getSafeQueryLimit(
								odcConfig.getQueryLimit()));
	}
	
	protected String getRowOffsetSql(JDBCQueryParams jdbcQuery) {
		if (jdbcQuery.getOffsetNumber() != null) {
			return " OFFSET " + jdbcQuery.getOffsetNumber().intValue();
		} else {
			return "";
		}
	}

	public ProductQuery buildQueryById(String id) {
		return ProductQuery.builder()
				.query(tableTranslator.resolveQuery(getFindByIdSql()))
				.params(new Object[] { id })
				.build();
	}
	
	public ProductQuery buildQuery(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(getFindSelectSql());
		
		if (filter != null) {
			query.append(getWhereWordSql());
		}

		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(filter.toString());
			query.append(getAndValidityCondition());
		} else {
			query.append(getWhereWordSql());
			query.append(getValidityCondition());
		}
		
		query.append(getDefaultGroupBySql());
		
		if (orderByColumns != null && orderByColumns.size() > 0) {
			query.append(getOrderByWordSql());
			query.append(StringUtils.join(orderByColumns, ", "));
		} else {
			query.append(getDefaultOrderBySql());
		}

		query.append(getRowLimitSql(jdbcQuery));
		query.append(getRowOffsetSql(jdbcQuery));

		return ProductQuery
				.builder()
				.query(tableTranslator.resolveQuery(query.toString()))
				.params(valuesPs)
				.build();
	}
	
	public ProductQuery buildCountQuery(JDBCQueryParams jdbcQuery) {
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(getCountSql());
		
		if (filter != null) {
			query.append(getWhereWordSql());
		}

		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(filter.toString());
			query.append(getAndValidityCondition());
		} else {
			query.append(getWhereWordSql());
			query.append(getValidityCondition());
		}
		
		query.append(getDefaultGroupBySql());
		
		return ProductQuery
				.builder()
				.query(tableTranslator.resolveQuery(query.toString()))
				.params(valuesPs)
				.build();
	}
	
	public T findById(JdbcTemplate jdbcTemplate, String id) {
		T ret = null;
		try {
			ProductQuery productQuery = buildQueryById(id);
			ret = jdbcTemplate.queryForObject(
					productQuery.getQuery(), 
					createProductMapper(), 
					productQuery.getParams());
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("product not found for id {}", id);
			return null;
		}
		return ret;
	}
	
	public List<T> findWithParams(JdbcTemplate jdbcTemplate, 
			JDBCQueryParams jdbcQuery, 
			List<String> orderByColumns) {
		
		ProductQuery pq = buildQuery(jdbcQuery, orderByColumns);

		return (List<T>) jdbcTemplate.query(pq.getQuery(), pq.getParams(), createProductMapper());
	}
	
	public int getRecordsCount(JdbcTemplate jdbcTemplate, JDBCQueryParams jdbcQuery) {
		ProductQuery pq = buildCountQuery(jdbcQuery);
		return jdbcTemplate.queryForObject(pq.getQuery(), pq.getParams(), Integer.class);
	}
	
	@Data
	@Builder
	public static class ProductQuery {
		public String query;
		Object [] params;
	}
	
	public String convertColumnName(String name) {
		return convertSimpleColumnName(convertCommonColumnName(name));
	}
	
	abstract public String convertSimpleColumnName(String name);
	
	public String convertCommonColumnName(String name) {
		String result = name;

		switch (name) {
		case "Id":
			result = "i.uuid";
			break;
		case "PublicationDate":
			result = "creationdate";
			break;
		case "EvictionDate":
			result = "c.eviction_time";
			break;
		case "Online":
			result = "case when c.online is null then false else c.online end";
			break;
		case "ContentDate.Start":
			result = "validitystart";
			break;
		case "ContentDate.End":
			result = "validitystop";
			break;
		}
		
		return result;
	}
	
	public void processJsonCondition(JDBCFilterExpressionEditor expression, String fieldName, String sqlCondition, Object operand) {
		Matcher lambdaMatcher = getLambdaPattern().matcher(fieldName);
		if (lambdaMatcher.find()) {
			String jsonField = lambdaMatcher.group("type").equals("Attributes") ? "attributes" : "checksum";
			String valueType = lambdaMatcher.group("valuetype");
			String quote = valueType.equals("String") ||
					valueType.equals("DateTimeOffset") ? "\"" : "";
			String alias = lambdaMatcher.group("alias");

			String attributeName = lambdaMatcher.group("name");
			if (sqlCondition.trim().equals("=")) {
				String value = operand.toString();
				
				if (jsonField.equals("attributes")) {
					if (attributeName.equals("Name")) {
						/*
						 * save correspondence of attribute name / attribute alias
						 * for a later use
						 */
						attributeNameMap.put(alias, operand.toString());
						
						// place a dummy condition
						expression.addSql("1=1");
					} else if (attributeName.equals("Value")) {
						
						if (operand instanceof Timestamp) {
							value = Util.formatTimestamp((Timestamp)operand);
						}
						
						expression.addSql("\"" + jsonField + "\" @> '[{\"Name\":\"" + attributeNameMap.get(alias) + "\", \"" + attributeName + "\": " + quote + value + quote + "}]'");
					}
				} else if (jsonField.equals("checksum")) {
					/*
					 * use special postgresql @> operator to detect if a json array field
					 * contains the specified json object, using the JSONB index
					 */
					expression.addSql("\"" + jsonField + "\" @> '[{\"" + attributeName + "\": " + quote + value + quote + "}]'");
				}
			} else {
				/*
				 * retrieve the attribute name saved in the Attributes(d/d:Name eq '***') 
				 * portion of the query 
				 */
				expression.addParameter(attributeNameMap.get(alias));

				String placeHolder = "?";
				if (sqlCondition.equals(" IN ")) {
					if (!(operand instanceof List<?>)) {
						throw new RuntimeException("wrong type of operand for IN operator: " + operand.getClass().getCanonicalName());
					}
					List<?> operandList = (List<?>)operand;
					StringBuilder sb = new StringBuilder("(");
					for (int a = 1; a <= operandList.size(); a++) {
						if (a != operandList.size())
							sb.append("?, ");
						else
							sb.append("?");
						expression.addParameter(operandList.get(a - 1));
					}
					sb.append(")");
					placeHolder = sb.toString();
				} else {
					expression.addParameter(operand);
				}
				
				/*
				 * use the jsonb_to_recordset function for comparisons where the @> operator
				 * cannot be used 
				 */
				expression.addSql( 
							"exists (select 1 from jsonb_to_recordset(i.\"" + jsonField + "\") as (\"Name\" text, \"ValueType\" text, \"Value\" text) "
							+ " where \"Name\" = ? and \"Value\"::" + EDMTYPE_SQL_CAST.get(valueType) + " " + sqlCondition + " " + placeHolder + ")");
			}
		}
	}
	
	abstract public RowMapper<T> createProductMapper();
}
