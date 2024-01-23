package com.exprivia.odc.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.dto.SystemRole;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;
import com.exprivia.odc.service.permissions.ODCSystemRole;
import com.exprivia.odc.service.permissions.ODCSystemRoleList;
import com.exprivia.odc.util.Util;

@Repository
public class SystemRoleRepositoryJDBCImpl implements SystemRoleRepository {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCActionsPermissions actionsPermissions;
	
	@Autowired
	private ODCSystemRoleList roleList;
	
	@Autowired
	private DBTableTranslator tableTranslator;
	
	@Autowired
	private ODCDBDefaults odcConfig;
	
	private final static String SELECT = "select ";
	private final static String COUNT = " count(1) ";
	private final static String FIELDS = "id, role, description";
	private final static String QUERY_BODY = " from {userRoles}";
	private final static String FIND_ALL = SELECT + FIELDS + QUERY_BODY;
	private final static String FIND_COUNT = SELECT + COUNT + QUERY_BODY;
	private final static String WHERE = " where ";
	private final static String AND = " and ";
	private final static String FIND_BY_ROLENAME = FIND_ALL + WHERE + " role = ?";
	private final static String ORDERBY = " order by ";
	private final static String SYSTEMROLE_ORDERBY = " order by id";
	private final static String ROW_LIMIT = " limit ";
	private final static String ROW_OFFSET = " offset ";

	private final static Logger LOG = LoggerFactory.getLogger(UserRepositoryJDBCImpl.class);

	private final static String FIND_ROLES_BY_USERNAME = " id in (select role_id from {usersXRoles} uxr inner join {users} u on u.id = uxr.user_id where u.name = ?)";

	@Override
	public SystemRole findByName(String name) {
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.ROLE_FIND_BY_ID.getCode());
		
		// remove enum type cast from name as it happens in the following query
		// /odata/v1/SystemRoles(OData.CSC.RoleType'Bulkorder')
		String newName = null;
		char quote = '\'';
		int idx = name.indexOf(quote);
		if (idx != -1) {
			int lastIdx = name.indexOf(quote, idx + 1);
			if (lastIdx == -1) lastIdx = name.length();
			newName = name.substring(idx + 1, lastIdx);
		} else {
			newName = name;
		}
		
		SystemRole ret = null;
		try {
			ret = jdbcTemplate.queryForObject(tableTranslator.resolveQuery(FIND_BY_ROLENAME), new SystemRoleRowMapper(), newName);
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("system role {} not found", newName);
			return null;
		}
		return ret;
	}

	@Override
	public List<SystemRole> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.ROLE_FIND_BY_ID.getCode());
		
		return findWithParamsInternal(jdbcQuery, orderByColumns);
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		// copy the jdbcQuery and remove limit and offset options
		JDBCFilterExpression filter = jdbcQuery.toBuilder()
				.limitNumber(null).offsetNumber(null)
				.build().getJdbcFilterExpression();
		StringBuilder query = new StringBuilder();
		List<Object> valuesPs = new ArrayList<>();

		query.append(FIND_COUNT);
		
		query.append(WHERE);
		
		if (filter != null) {
			valuesPs = filter.getParameters();
			query.append(filter.toString());
			query.append(AND);
		}
		
		appendConditionForKnownRoles(query, valuesPs);
		
		return jdbcTemplate.queryForObject(
				tableTranslator.resolveQuery(query.toString()), 
				valuesPs.toArray(), 
				Integer.class);
	}
	
	public List<SystemRole> findWithParamsInternal(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		List<Object> valuesPs = new ArrayList<>();

		query.append(FIND_ALL);
		
		query.append(WHERE);
		
		if (filter != null) {
			valuesPs = filter.getParameters();
			query.append(filter.toString());
			query.append(AND);
		}
		
		appendConditionForKnownRoles(query, valuesPs);
		
		if (orderByColumns != null && orderByColumns.size() > 0) {
			query.append(ORDERBY);
			query.append(StringUtils.join(orderByColumns, ", "));
		} else {
			query.append(SYSTEMROLE_ORDERBY);
		}
		
		query.append(ROW_LIMIT).append(
				jdbcQuery.getSafeQueryLimit(
						odcConfig.getQueryLimit()));

		if (jdbcQuery.getOffsetNumber() != null) {
			query.append(ROW_OFFSET).append(jdbcQuery.getOffsetNumber().intValue());
		}
		
		return (List<SystemRole>) jdbcTemplate.query(
				tableTranslator.resolveQuery(query.toString()), 
				valuesPs.toArray(), 
				new SystemRoleRowMapper());
	}
	
	private void appendConditionForKnownRoles(StringBuilder query, List<Object> params) {
		query.append("role in (");
		
		Iterator<ODCSystemRole> it = roleList.getValues().iterator();
		while (it.hasNext()) {
			params.add(it.next().getName());
			query.append("?");
			if (it.hasNext())
				query.append(",");	
		}
		
		query.append(")");
	}
	
	static public JDBCQueryParams addUserCondition(JDBCQueryParams jdbcQueryParams, String username) {
		// create new params, the jdbcQueryParams can be null
		return Util.addArbitrarySQLCondition(
				jdbcQueryParams,
				FIND_ROLES_BY_USERNAME,
				Arrays.asList(username));
	}
	
	class SystemRoleRowMapper implements RowMapper<SystemRole> {

		@Override
		public SystemRole mapRow(ResultSet rs, int rowNum) throws SQLException {
			return SystemRole.builder()
					.value(roleList.fromName(rs.getString("role")))
					.description(rs.getString("description"))
					.build();
		}
		
	}
}
