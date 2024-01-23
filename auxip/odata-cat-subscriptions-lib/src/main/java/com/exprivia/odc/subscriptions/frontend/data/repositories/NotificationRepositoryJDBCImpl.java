package com.exprivia.odc.subscriptions.frontend.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.List;

import org.apache.commons.lang3.ArrayUtils;
import org.apache.commons.lang3.StringUtils;
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;
import com.exprivia.odc.service.permissions.ODCUserPermissions;
import com.exprivia.odc.subscriptions.frontend.data.dto.Notification;
import com.exprivia.odc.subscriptions.frontend.service.permissions.ODCSubscriptionsActions;
import com.exprivia.odc.util.Util;

@Repository
public class NotificationRepositoryJDBCImpl implements NotificationRepository {

	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCUserPermissions permissions;
	
	@Autowired
	private ODCActionsPermissions actionsPermissions;

	@Autowired
	private ODCDBDefaults dbConfig;

	private final static String NOTIFICATION_ORDERBY = " order by n.id";
	private final static String ROW_LIMIT = " limit ";
	private final static String ROW_OFFSET = " offset ";
	private final static String WHERE = " where ";
	private final static String AND = " and ";
	private final static String ORDERBY = " order by ";

	private final static String CONDITION_USERNAME = "u.name = ?";
	private final static String CONDITION_PARENT_SUBSCRIPTIONID = "n.subscription_uuid = ?";

	private final static String FIND_ALL = "select n.product_uuid, n.product_name, n.subscription_uuid, n.order_uuid, n.batchorder_uuid, n.subscriptionevent, n.notificationdate from t_srv_pending_notifications n inner join t_srv_subscriptions s on s.uuid = n.subscription_uuid inner join t_srv_users u on u.id = s.user_id";
	
	//private static final Logger LOG = LoggerFactory.getLogger(NotificationRepositoryJDBCImpl.class);

	/*
	 * methods to let to override NotificationRepositoryJDBCImpl
	 */
	protected String getFindAllSql() {
		return FIND_ALL;
	}
	
	protected String getConditionUsernameSql() {
		return CONDITION_USERNAME;
	}
	
	protected String getNotificationOrderBySql() {
		return NOTIFICATION_ORDERBY;
	}
	
	public static JDBCQueryParams addNotificationsFromSubscriptionIdCondition(JDBCQueryParams jdbcQueryParams, String subscriptionId) {
		return Util.addArbitrarySQLCondition(jdbcQueryParams, CONDITION_PARENT_SUBSCRIPTIONID, Arrays.asList(subscriptionId));
	}
	
	public List<Notification> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		actionsPermissions.checkCurrentUserGranted(ODCSubscriptionsActions.NOTIFICATION_FIND_WITH_PARAMS.getCode());
		
		StringBuilder query = new StringBuilder();
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		Object[] valuesPs = null;

		query.append(getFindAllSql());
		
		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}

		// TODO: check how REPORTING permission can be passed from frontend
//		if (!permissions.currentUserHasPermission(LTASystemRoles.REPORTING)) {
			if (filter != null) {
				query.append(AND).append(getConditionUsernameSql());
				valuesPs = ArrayUtils.add(valuesPs, permissions.getRequestUsername());
			} else {
				query.append(WHERE).append(getConditionUsernameSql());
				valuesPs = new Object[] { permissions.getRequestUsername() };
			}
//		}
		
		if (orderByColumns != null && orderByColumns.size() > 0) {
			query.append(ORDERBY);
			query.append(StringUtils.join(orderByColumns, ", "));
		} else {
			query.append(getNotificationOrderBySql());
		}

		query.append(ROW_LIMIT).append(
				jdbcQuery.getSafeQueryLimit(
						dbConfig.getQueryLimit()
						));

		if (jdbcQuery.getOffsetNumber() != null) {
			query.append(ROW_OFFSET).append(jdbcQuery.getOffsetNumber().intValue());
		}

		return (List<Notification>) jdbcTemplate.query(query.toString(), valuesPs, new NotificationRowMapper());
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		JDBCFilterExpression filter = jdbcQuery.toBuilder()
				.limitNumber(null).offsetNumber(null)
				.build().getJdbcFilterExpression();
		StringBuilder query = new StringBuilder();
		Object[] valuesPs = null;

		query.append(getFindAllSql());
		
		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}

		// TODO: check how REPORTING permission can be passed from frontend
//		if (!permissions.currentUserHasPermission(LTASystemRoles.REPORTING)) {
			if (filter != null) {
				query.append(AND).append(getConditionUsernameSql());
				valuesPs = ArrayUtils.add(valuesPs, permissions.getRequestUsername());
			} else {
				query.append(WHERE).append(getConditionUsernameSql());
				valuesPs = new Object[] { permissions.getRequestUsername() };
			}
//		}
		
		return jdbcTemplate.queryForObject(query.toString(), valuesPs, Integer.class);
	}
	
	public String convertColumnName(String name) {
		String result = name;
		switch (name) {
		case "SubscriptionID":
			result = "n.subscription_uuid";
			break;
		case "SubscriptionEvent":
			result = "n.subscriptionevent";
			break;
		}
		return result;
	}
	
	static class NotificationRowMapper implements RowMapper<Notification> {
		@Override
		public Notification mapRow(ResultSet rs, int rowNum) throws SQLException {
			
			String subscriptionID = rs.getString("subscription_uuid");
			if (rs.wasNull())
				subscriptionID = null;
			
			Notification.NotificationBuilder notification = Notification.builder()
					.productId(rs.getString("product_uuid"))
					.productName(rs.getString("product_name"))
					.subscriptionID(subscriptionID)
					.notificationDate(rs.getTimestamp("notificationdate"))
					;
			
			return notification.build();
		}
	}

}
