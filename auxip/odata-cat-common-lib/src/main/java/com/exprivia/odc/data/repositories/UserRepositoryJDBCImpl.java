package com.exprivia.odc.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
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
import com.exprivia.odc.data.dto.User;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;

@Repository
public class UserRepositoryJDBCImpl implements UserRepository {

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCActionsPermissions actionsPermissions;

	@Autowired
	private ODCDBDefaults odcConfig;
	
	@Autowired
	private DBTableTranslator tableTranslator;

	private final static String SELECT = "select ";
	private final static String COUNT = " count(1) ";
	private final static String FIELDS = " name as username, '' email, TO_TIMESTAMP('1970-01-01 00:00:00', 'YYYY-MM-DD HH24:MI:SS') created, default_priority defaultpriority, highest_priority maxpriority ";
	private final static String QUERY_BODY = " from {users} ";
	private final static String USER_ORDERBY = " order by name";
	private final static String ROW_LIMIT = " limit ";
	private final static String ROW_OFFSET = " offset ";
	private final static String WHERE = " where ";
	private final static String ORDERBY = " order by ";

	private static final Logger LOG = LoggerFactory.getLogger(UserRepositoryJDBCImpl.class);

	private final static String FIND_ALL = SELECT + FIELDS + QUERY_BODY;
	private final static String FIND_COUNT = SELECT + COUNT + QUERY_BODY;
	private final static String FIND_BY_USERNAME = FIND_ALL + " where name = ?";

	@Override
	public User findByUsername(String username) {
		if (!username.equals(actionsPermissions.getRequestUsername()))
			actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.USER_FIND_BY_USERNAME.getCode());
		
		User ret = null;
		try {
			ret = jdbcTemplate.queryForObject(tableTranslator.resolveQuery(FIND_BY_USERNAME), new UserRowMapper(), username);
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("user not found for username {}", username);
			return null;
		}
		return ret;
	}

	@Override
	public List<User> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		actionsPermissions.checkCurrentUserGranted(ODCActionsPermissions.Action.USER_FIND_WITH_PARAMS.getCode());
		
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(FIND_ALL);

		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}

		if (orderByColumns != null && orderByColumns.size() > 0) {
			query.append(ORDERBY);
			query.append(StringUtils.join(orderByColumns, ", "));
		} else {
			query.append(USER_ORDERBY);
		}

		query.append(ROW_LIMIT).append(
				jdbcQuery.getSafeQueryLimit(
						odcConfig.getQueryLimit()));

		if (jdbcQuery.getOffsetNumber() != null) {
			query.append(ROW_OFFSET).append(jdbcQuery.getOffsetNumber().intValue());
		}

		return (List<User>) jdbcTemplate.query(
				tableTranslator.resolveQuery(query.toString()), 
				valuesPs, 
				new UserRowMapper());
	}

	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		// copy the jdbcQuery and remove limit and offset options
		JDBCFilterExpression filter = jdbcQuery.toBuilder()
				.limitNumber(null).offsetNumber(null)
				.build().getJdbcFilterExpression();
		StringBuilder query = new StringBuilder();
		Object[] valuesPs = null;

		query.append(FIND_COUNT);

		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}

		return jdbcTemplate.queryForObject(
				tableTranslator.resolveQuery(query.toString()), 
				valuesPs, 
				Integer.class);
	}

	class UserRowMapper implements RowMapper<User> {

		@Override
		public User mapRow(ResultSet rs, int rowNum) throws SQLException {
			User.UserBuilder userBuilder = User.builder().username(rs.getString("username")).email(rs.getString("email")).
					created(rs.getTimestamp("created")).defaultPriority(rs.getLong("defaultpriority"))
					.maxPriority(rs.getLong("maxpriority"));

			return userBuilder.build();
		}
	}
}
