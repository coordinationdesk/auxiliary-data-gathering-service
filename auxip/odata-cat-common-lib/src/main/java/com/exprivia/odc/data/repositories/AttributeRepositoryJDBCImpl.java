package com.exprivia.odc.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.dto.Attribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTBooleanAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTDateTimeOffsetAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTDoubleAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTIntegerAttribute;
import com.exprivia.odc.edmprovider.primitivetypes.ODCEdmPTStringAttribute;
import com.exprivia.odc.service.ODCJsonSerializer.ODCJsonPristineValue;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.util.Util;

@Repository
public class AttributeRepositoryJDBCImpl implements AttributeRepository {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private DBTableTranslator tableTranslator;
	
	@Autowired
	private ODCDBDefaults odcConfig;
	
	private final static String SELECT = "select ";
	private final static String COUNT = "count(1)";
	private final static String ROW_LIMIT = " limit ";
	private final static String ROW_OFFSET = " offset ";
	private final static String WHERE = " where ";
	private final static String ORDERBY = " order by ";

	private final static String FIND_ALL_FIELDS = "x.*";
	private final static String QUERY_BODY = " from {inventory} i inner join jsonb_to_recordset(i.\"attributes\") as x(\"Name\" text, \"ValueType\" text, \"Value\" text) on true";
	private final static String FIND_ALL = SELECT + FIND_ALL_FIELDS + QUERY_BODY;
	private final static String FIND_COUNT = SELECT + COUNT + QUERY_BODY;
		
	
	public List<Attribute<?>> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(tableTranslator.resolveQuery(FIND_ALL));

		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}
		
		if (orderByColumns != null && orderByColumns.size() > 0) {
			query.append(ORDERBY);
			query.append(StringUtils.join(orderByColumns, ", "));
		}
		
		query.append(ROW_LIMIT).append(
				jdbcQuery.getSafeQueryLimit(
						odcConfig.getQueryLimit()));

		if (jdbcQuery.getOffsetNumber() != null) {
			query.append(ROW_OFFSET).append(jdbcQuery.getOffsetNumber().intValue());
		}

		return (List<Attribute<?>>) jdbcTemplate.query(query.toString(), valuesPs, new AttributeRowMapper());
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		// copy the jdbcQuery and remove limit and offset options
		JDBCFilterExpression filter = jdbcQuery.toBuilder()
				.limitNumber(null).offsetNumber(null)
				.build().getJdbcFilterExpression();
		StringBuilder query = new StringBuilder();
		Object[] valuesPs = null;

		query.append(tableTranslator.resolveQuery(FIND_COUNT));

		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}
		
		return jdbcTemplate.queryForObject(query.toString(), valuesPs, Integer.class);
	}
	
	public String convertColumnName(String name) {
		return "x.\"" + name + "\"";
	}
	
	class AttributeRowMapper implements RowMapper<Attribute<?>> {
		
		@Override
		public Attribute<?> mapRow(ResultSet rs, int rowNum) throws SQLException {
			Object value = null;
			FullQualifiedName edmType = null;
			String valueType = rs.getString("ValueType");
			String stringValue = rs.getString("Value");
			String name = rs.getString("Name");

			/*
			 * initialize OData type
			 */
			switch (valueType) {
			case "String":
				edmType = ODCEdmPTStringAttribute.ET_FQN;
				break;
			case "Int64":
			case "Integer":
				edmType = ODCEdmPTIntegerAttribute.ET_FQN;
				valueType = "Integer"; // override db contents with ICD-1.4 strict value
				break;
			case "Double":
				edmType = ODCEdmPTDoubleAttribute.ET_FQN;
				break;
			case "DateTimeOffset":
				edmType = ODCEdmPTDateTimeOffsetAttribute.ET_FQN;
				break;
			case "bool":
			case "Boolean":
				edmType = ODCEdmPTBooleanAttribute.ET_FQN;
				valueType = "Boolean"; // override db contents with ICD-1.4 strict value
				break;
			default:
				throw new RuntimeException("Unexpected attribute value type " + valueType + " for attribute " + name);
			}
			
			/*
			 * in some cases the attribute value can be a list of values
			 */
			if (stringValue != null && stringValue.startsWith("[") && stringValue.endsWith("]")) {
				/**
				 *  in this case, the value is initialized as an array
				 *  of strings and handled later in
				 *  @see LTAODataJsonSerializer.writePrimitiveValue
				 *  the payload is handled as a Json fragment and
				 *  rendered as-is
				 */
				value = new ODCJsonPristineValue(stringValue);
			} else {
				/*
				 * get the value
				 */
				switch (valueType) {
				case "String":
					value = rs.getString("Value");
					break;
				case "Int64":
				case "Integer":
					value = rs.getInt("Value");
					break;
				case "Double":
					value = rs.getDouble("Value");
					break;
				case "DateTimeOffset":
					// rs.getTimestamp("Value") gives error in case the time-stamp is not 
					// formatted as "YYYY-MM-DD HH24:MI.SS.FFFFFF"
					value = Util.parseDateString(rs.getString("Value"));
					break;
				case "bool":
				case "Boolean":
					value = rs.getBoolean("Value");
					break;
				default:
					throw new RuntimeException("Unexpected attribute value type " + valueType + " for attribute " + name);
				}
			}
			
			return Attribute.builder()
					.attributeType(edmType)
					.name(name)
					.value(value)
					.valueType(valueType)
					.build();
		}
	}

}
