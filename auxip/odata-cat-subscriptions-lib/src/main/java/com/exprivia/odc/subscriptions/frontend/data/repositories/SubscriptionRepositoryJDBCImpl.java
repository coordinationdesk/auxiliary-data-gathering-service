package com.exprivia.odc.subscriptions.frontend.data.repositories;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.UUID;

import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCreator;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.support.GeneratedKeyHolder;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;
import com.exprivia.odc.service.permissions.ODCUserPermissions;
import com.exprivia.odc.subscriptions.frontend.business.NotificationEndpointHandler;
import com.exprivia.odc.subscriptions.frontend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.frontend.data.dto.SubscriptionStatus;
import com.exprivia.odc.subscriptions.frontend.data.repositories.exception.SubscriptionBadStatusChangeException;
import com.exprivia.odc.subscriptions.frontend.service.permissions.ODCSubscriptionsActions;

@Repository
public class SubscriptionRepositoryJDBCImpl implements SubscriptionRepository {

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCUserPermissions permissions;

	@Autowired
	private ODCActionsPermissions actionsPermissions;

	@Autowired
	private ODCDBDefaults dbConfig;

	private final static String SUBSCRIPTION_ORDERBY = " order by s.id";
	private final static String ROW_LIMIT = " limit ";
	private final static String ROW_OFFSET = " offset ";
	private final static String WHERE = " where ";
	private final static String AND = " and ";
	private final static String ORDERBY = " order by ";

	private final static String CONDITION_USERNAME = "u.name = ?";

	private final static String FIND_ALL = "select s.id, s.uuid, st.status, s.user_id, s.subscriptionevent, s.filterparam, s.submissiondate, s.lastquerydate, s.stageorder, s.priority, " + NotificationEndpointHandler.SELECT_FROM_COLUMNS + " from t_srv_subscriptions s inner join t_srv_statuses st on s.status_id = st.id inner join t_srv_users u on s.user_id = u.id " + NotificationEndpointHandler.LEFT_JOIN_FROM_SUBSCRIPTION;
	private final static String FIND_BY_ID = FIND_ALL + WHERE + " s.id = ?";
	private final static String FIND_BY_UUID = FIND_ALL + WHERE + " s.uuid = ?";
	
	private final static String FIND_ALREADY_DEFINED = FIND_ALL + WHERE + 
			"	u.\"name\" = ? and\r\n" + 
			"	s.filterparam = ? and\r\n" + 
			"	n.endpoint = ?";
	private final static String FIND_ALREADY_DEFINED_NO_USERNAME = FIND_ALREADY_DEFINED + 
			"	and n.username is null";
	private final static String FIND_ALREADY_DEFINED_USERNAME = FIND_ALREADY_DEFINED + 
			"	and n.username = ?";

	private final static String FIND_BY_UUID_AND_USERNAME = FIND_BY_UUID + AND + CONDITION_USERNAME;
	private final static String UPDATE_STATUS = "UPDATE t_srv_subscriptions SET status_id = (select id from t_srv_statuses where status = ?) WHERE uuid = ?";
	
	private final static String INSERT = "INSERT INTO t_srv_subscriptions (user_id, filterparam, status_id, notificationendpoint_id, subscriptionevent, stageorder, priority) VALUES ((select id from t_srv_users where name = ?), ?, (select id from t_srv_statuses where status = ?), ?, 'CREATED', 'f', 0)";
	
	private static final Logger LOG = LoggerFactory.getLogger(SubscriptionRepositoryJDBCImpl.class);
	
	/*
	 * methods to let to override SubscriptionRepositoryJDBCImpl
	 */
	protected String getFindAllSql() {
		return FIND_ALL;
	}
	
	protected String getConditionUsernameSql() {
		return CONDITION_USERNAME;
	}
	
	protected String getSubscriptionOrderBySql() {
		return SUBSCRIPTION_ORDERBY;
	}
	
	protected String getFindByUuidAndUsernameSql() {
		return FIND_BY_UUID_AND_USERNAME;
	}
	
	protected String getFindAlreadyDefinedNoUsernameSql() {
		return FIND_ALREADY_DEFINED_NO_USERNAME;
	}
	
	protected String getFindAlreadyDefinedUsernameSql() {
		return FIND_ALREADY_DEFINED_USERNAME;
	}
	
	protected String getInsertSql() {
		return INSERT;
	}
	
	protected String getFindByIdSql() {
		return FIND_BY_ID;
	}
	
	protected String getUpdateStatusSql() {
		return UPDATE_STATUS;
	}

	@Override
	public Subscription findById(String id) {
		actionsPermissions.checkCurrentUserGranted(ODCSubscriptionsActions.SUBSCRIPTION_FIND_BY_ID.getCode());
		
		Subscription ret = null;
		try {
			// TODO: check how to propagate reporting permission from frontend
//			if (!permissions.currentUserHasPermission(LTASystemRoles.REPORTING))
				ret = jdbcTemplate.queryForObject(getFindByUuidAndUsernameSql(), new SubscriptionRowMapper(), id, permissions.getRequestUsername());
//			else
//				ret = jdbcTemplate.queryForObject(FIND_BY_UUID, new SubscriptionRowMapper(), id);
			
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("subscription not found for id {}", id);
			return null;
		}
		return ret;
	}

	@Override
	public List<Subscription> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns, boolean internalService) {
		if (internalService == false) {
			actionsPermissions.checkCurrentUserGranted(ODCSubscriptionsActions.SUBSCRIPTION_FIND_WITH_PARAMS.getCode());
		}
		
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(getFindAllSql());
		
		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}
		
		// TODO: check how to propagate reporting permission from frontend
//		if (!permissions.currentUserHasPermission(LTASystemRoles.REPORTING) && !internalService) {
		if (!internalService) {
			if (filter != null) {
				query.append(AND).append(getConditionUsernameSql());
				valuesPs = ArrayUtils.add(valuesPs, permissions.getRequestUsername());
			} else {
				query.append(WHERE).append(getConditionUsernameSql());
				valuesPs = new Object[] { permissions.getRequestUsername() };
			}
		}
		
		if (orderByColumns != null && orderByColumns.size() > 0) {
			query.append(ORDERBY);
			query.append(StringUtils.join(orderByColumns, ", "));
		} else {
			query.append(getSubscriptionOrderBySql());
		}

		query.append(ROW_LIMIT).append(
				jdbcQuery.getSafeQueryLimit(
						dbConfig.getQueryLimit()));

		if (jdbcQuery.getOffsetNumber() != null) {
			query.append(ROW_OFFSET).append(jdbcQuery.getOffsetNumber().intValue());
		}

		return (List<Subscription>) jdbcTemplate.query(query.toString(), valuesPs, new SubscriptionRowMapper());
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(getFindAllSql());
		
		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}
		
		if (filter != null) {
			query.append(AND).append(getConditionUsernameSql());
			valuesPs = ArrayUtils.add(valuesPs, permissions.getRequestUsername());
		} else {
			query.append(WHERE).append(getConditionUsernameSql());
			valuesPs = new Object[] { permissions.getRequestUsername() };
		}
		
		return jdbcTemplate.queryForObject(query.toString(), valuesPs, Integer.class);
	}
	
	@Override
	public Subscription insert(Subscription subscriptionToCreate) {
		actionsPermissions.checkCurrentUserGranted(ODCSubscriptionsActions.SUBSCRIPTION_INSERT.getCode());
		
		/*
		 * Check if the same subscription already exists
		 * If a subscription is found, return it and set
		 * its status to RUNNING
		 * http://projectit.acsys.it/browse/LTA-337
		 */
		String sqlCheck = getFindAlreadyDefinedNoUsernameSql();
		ArrayList<Object> valuesPs = new ArrayList<Object>(
				Arrays.asList(
					permissions.getRequestUsername(),
					subscriptionToCreate.getFilterParam(),
					subscriptionToCreate.getNotificationEndpoint()
					)
				);
		if (subscriptionToCreate.getNotificationEpUsername() != null) {
			sqlCheck = getFindAlreadyDefinedUsernameSql();
			valuesPs.add(subscriptionToCreate.getNotificationEpUsername());
		}
		
		List<Subscription> previousSubscriptions = jdbcTemplate.query(sqlCheck, valuesPs.toArray(), new SubscriptionRowMapper());
		if (previousSubscriptions.size() > 0) {
			Subscription subscription = previousSubscriptions.get(0);
			if (subscription.getNotificationEpPassword() != 
				subscriptionToCreate.getNotificationEpPassword() ||
				(subscription.getNotificationEpPassword() != null &&
				subscriptionToCreate.getNotificationEpPassword() != null &&
				!subscription.getNotificationEpPassword().equals(
						subscriptionToCreate.getNotificationEpPassword()))) {
				/*
				 *  update endpoint password
				 */
				NotificationEndpointHandler eph = new NotificationEndpointHandler(jdbcTemplate);
				eph.upsertEndpoint(
						subscriptionToCreate.getNotificationEndpoint(), 
						subscriptionToCreate.getNotificationEpUsername(), 
						subscriptionToCreate.getNotificationEpPassword());
			}
			updateStatus(subscription, SubscriptionStatus.RUNNING);
			
			return findById(subscription.getId().toString());
		}
		
		/*
		 * No previous subscription exists, create
		 * a new one
		 */

		GeneratedKeyHolder holder = new GeneratedKeyHolder();
		
		// insert
		jdbcTemplate.update(new PreparedStatementCreator() {
			@Override
			public PreparedStatement createPreparedStatement(Connection con) throws SQLException {
				String keys[] = new String[] { "id" };
				PreparedStatement statement = con.prepareStatement(getInsertSql(), keys);
				statement.setString(1, permissions.getRequestUsername());
				statement.setString(2, subscriptionToCreate.getFilterParam());
				statement.setString(3, subscriptionToCreate.getStatus().name());
				
				NotificationEndpointHandler eph = new NotificationEndpointHandler(jdbcTemplate);
				Long endpointId = eph.upsertEndpoint(
						subscriptionToCreate.getNotificationEndpoint(), 
						subscriptionToCreate.getNotificationEpUsername(), 
						subscriptionToCreate.getNotificationEpPassword());
				if (endpointId != null)
					statement.setLong(4, endpointId);
				else
					statement.setNull(4, java.sql.Types.NUMERIC);

				return statement;
			}
		}, holder);
		long primaryKey = holder.getKey().longValue();

		return jdbcTemplate.queryForObject(getFindByIdSql(), new SubscriptionRowMapper(), primaryKey);
	}
	
	protected void updateStatus(Subscription subscription, SubscriptionStatus status) {
		actionsPermissions.checkCurrentUserGranted(ODCSubscriptionsActions.SUBSCRIPTION_UPDATE.getCode());
		jdbcTemplate.update(getUpdateStatusSql(), status.name(), subscription.getId().toString());
	}
	
	public void cancel(Subscription subscriptionToCancel) {
		if (subscriptionToCancel.getStatus() != SubscriptionStatus.PAUSED &&
			subscriptionToCancel.getStatus() != SubscriptionStatus.RUNNING) {
			throw new SubscriptionBadStatusChangeException("Cannot cancel a subscription in status " + subscriptionToCancel.getStatus().getDescription());
		}
		updateStatus(subscriptionToCancel, SubscriptionStatus.CANCELLED);
	}
	
	public void pause(Subscription subscriptionToPause) {
		if (subscriptionToPause.getStatus() != SubscriptionStatus.RUNNING) {
			throw new SubscriptionBadStatusChangeException("Cannot pause a subscription in status " + subscriptionToPause.getStatus().getDescription());
		}
		updateStatus(subscriptionToPause, SubscriptionStatus.PAUSED);
	}
	
	public void resume(Subscription subscriptionToResume) {
		if (subscriptionToResume.getStatus() != SubscriptionStatus.PAUSED) {
			throw new SubscriptionBadStatusChangeException("Cannot resume a subscription in status " + subscriptionToResume.getStatus().getDescription());
		}
		updateStatus(subscriptionToResume, SubscriptionStatus.RUNNING);
	}
	
	public String convertColumnName(String name) {
		String result = name;
		switch (name) {
		case "Status":
			result = "st.status";
			break;
		case "NotificationEndpoint":
			result = NotificationEndpointHandler.COLUMN_ENDPOINT;
			break;
		case "NotificationEpUsername":
			result = NotificationEndpointHandler.COLUMN_USERNAME;
			break;
		case "NotificationEpPassword":
			result = NotificationEndpointHandler.COLUMN_PASSWORD;
			break;
		}
		return result;
	}
	
	public String convertLiteralValue(String value) {
		if (value.equals(SubscriptionStatus.RUNNING.getDescription())) {
			return "RUNNING";
		} else if (value.equals(SubscriptionStatus.PAUSED.getDescription())) {
			return "PAUSED";
		} else if (value.equals(SubscriptionStatus.CANCELLED.getDescription())) {
			return "CANCELLED";
		}

		return value;
	}
	
	static class SubscriptionRowMapper implements RowMapper<Subscription> {
		@Override
		public Subscription mapRow(ResultSet rs, int rowNum) throws SQLException {
			String notificationEpUsername = rs.getString(NotificationEndpointHandler.ALIAS_USERNAME);
			if (rs.wasNull()) notificationEpUsername = null;

			String notificationEpPassword = rs.getString(NotificationEndpointHandler.ALIAS_PASSWORD);
			if (rs.wasNull()) notificationEpPassword = null;

			/*
			 * Field LastNotificationDate changed in LTA ICD 1.3 but still
			 * mapped on lastQueryDate on DB schema
			 */
			Timestamp lastNotificationDate = rs.getTimestamp("lastquerydate");
			if (rs.wasNull()) lastNotificationDate = null;
			
			Subscription.SubscriptionBuilder subscription = Subscription.builder()
					.id(UUID.fromString(rs.getString("uuid")))
					.status(SubscriptionStatus.fromString(rs.getString("status")))
					.filterParam(rs.getString("filterparam"))
					.submissionDate(rs.getTimestamp("submissiondate"))
					.lastNotificationDate(lastNotificationDate)
					.notificationEndpoint(rs.getString(NotificationEndpointHandler.ALIAS_ENDPOINT))
					.notificationEpUsername(notificationEpUsername)
					.notificationEpPassword(notificationEpPassword)
					.uniqueId(rs.getLong("id"))
					;
			
			return subscription.build();
		}
	}

}
