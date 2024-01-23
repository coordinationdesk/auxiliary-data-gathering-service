package com.exprivia.odc.subscriptions.frontend.business;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;
import java.util.UUID;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCreator;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.support.GeneratedKeyHolder;

import com.exprivia.odc.subscriptions.frontend.business.exception.EndpointDefinitionException;

public class NotificationEndpointHandler {
	
	final public static String COLUMN_ENDPOINT = "n.endpoint";
	final public static String COLUMN_USERNAME = "n.username";
	final public static String COLUMN_PASSWORD = "n.password";
	
	final public static String ALIAS_ENDPOINT = "notificationendpoint";
	final public static String ALIAS_USERNAME = "notificationepuser";
	final public static String ALIAS_PASSWORD = "notificationeppassword";
	
	/*
	 * Public sql snippets can be used from other classes
	 */
	final public static String SELECT_FROM_COLUMNS = 
			COLUMN_ENDPOINT + " " + ALIAS_ENDPOINT + ", " +
			COLUMN_USERNAME + " " + ALIAS_USERNAME + ", " +
			COLUMN_PASSWORD + " " + ALIAS_PASSWORD;
	
	final public static String LEFT_JOIN_FROM_ORDER = 
			" left join t_srv_notificationsendpoints_x_ordersqueue nxo on oq.id = nxo.order_id" +
			" left join t_srv_notificationsendpoints n on nxo.endpoint_id = n.id";
	
	final public static String LEFT_JOIN_FROM_BULK = 
			" left join t_srv_notificationsendpoints n on b.notificationendpoint_id = n.id";
	
	final public static String LEFT_JOIN_FROM_SUBSCRIPTION = 
			" left join t_srv_notificationsendpoints n on s.notificationendpoint_id = n.id";
	
	final private static String SELECT_ENDPOINT = "select * from t_srv_notificationsendpoints where endpoint = ? and (username = ? or (? is null and username is null))";
	final private static String INSERT_ENDPOINT = "insert into t_srv_notificationsendpoints (endpoint, username, password) values (?, ?, ?)";
	final private static String UPDATE_ENDPOINT = "update t_srv_notificationsendpoints set password = ? where id = ?";
	
	final private static String SELECT_ORDER_LINK = "select * from t_srv_notificationsendpoints_x_ordersqueue where order_id = ? and endpoint_id = ?";
	final private static String INSERT_ORDER_LINK = "insert into t_srv_notificationsendpoints_x_ordersqueue (order_id, bulkorder_uuid, subscription_uuid, endpoint_id) values (?, ?, ?, ?)";
	final private static String UPDATE_ORDER_LINK = "update t_srv_notificationsendpoints_x_ordersqueue set bulkorder_uuid = ?, subscription_uuid = ? where order_id = ? and endpoint_id = ?";
	
	private JdbcTemplate jdbcTemplate;
	
	public NotificationEndpointHandler(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplate = jdbcTemplate;
	}
	
	public Long upsertEndpoint(final String endpoint, final String username, final String password) throws EndpointDefinitionException {
		
		if (endpoint == null || endpoint.equals(""))
			return null;
		
		Long endpointId = null;

		// find previous endpoint definition matching endpoint and username
		Object [] args = new String[] { endpoint, username, username };
		int [] argTypes = new int[] { java.sql.Types.VARCHAR, java.sql.Types.VARCHAR, java.sql.Types.VARCHAR};
		List<EndpointRow> results = jdbcTemplate.query(SELECT_ENDPOINT, args, argTypes, new EndpointRowMapper());
		
		if (results.size() == 0) {
			// no previous endpoint, insert a new record
			GeneratedKeyHolder holder = new GeneratedKeyHolder();

			jdbcTemplate.update(new PreparedStatementCreator() {
				public PreparedStatement createPreparedStatement(Connection con) throws SQLException {
					String keys[] = new String[] { "id" };
					PreparedStatement statement = con.prepareStatement(INSERT_ENDPOINT, keys);
					statement.setString(1, endpoint);
					statement.setString(2, username);
					statement.setString(3, password);
					return statement;
				}
			}, holder);
			
			endpointId = holder.getKey().longValue();
			
		} else if (results.size() == 1) {
			endpointId = results.get(0).id;
			String oldPassword = results.get(0).password;
			if ((password == null && oldPassword != null) ||
				(password != null && oldPassword == null) ||
				(password != null && !password.equals(oldPassword))) {
				// previous record, update password (if changed)
				Object [] updateArgs = new Object[] { password, endpointId };
				int [] updateArgsTypes = new int[] { java.sql.Types.VARCHAR, java.sql.Types.NUMERIC };
				jdbcTemplate.update(UPDATE_ENDPOINT, updateArgs, updateArgsTypes);
			}
		} else {
			throw new EndpointDefinitionException("More than one endpoints defined for endpoint " + endpoint + " and username " + username);
		}
		
		return endpointId;
	}
	
	public Long linkOrder(final Long endpointId, final long orderId, final UUID bulkOrderUuid, final UUID subscriptionUuid) throws EndpointDefinitionException {
		if (endpointId == null)
			// no endpoint to link with order, skip
			return null;

		GeneratedKeyHolder holder = new GeneratedKeyHolder();
		
		// check if the order already exists
		Object [] args = new Long[] { orderId, endpointId };
		final List<EndpointXOrdersQueueRow> results = jdbcTemplate.query(SELECT_ORDER_LINK, args, new EndpointXOrdersQueueRowMapper());
		if (results.size() == 0) {
			// insert new link between endpoint and order
			jdbcTemplate.update(new PreparedStatementCreator() {
				@Override
				public PreparedStatement createPreparedStatement(Connection con) throws SQLException {
					PreparedStatement statement = con.prepareStatement(INSERT_ORDER_LINK, new String [] { "id" });
					statement.setLong(1, orderId);
					if (bulkOrderUuid == null)
						statement.setNull(2, java.sql.Types.VARCHAR);
					else
						statement.setString(2, bulkOrderUuid.toString());
					if (subscriptionUuid == null)
						statement.setNull(3, java.sql.Types.VARCHAR);
					else
						statement.setString(3, subscriptionUuid.toString());
					statement.setLong(4, endpointId);
					return statement;
				}
			}, holder);
			
			return holder.getKey().longValue();
		} if (results.size() == 1) {
			// link already present
			jdbcTemplate.update(new PreparedStatementCreator() {
				@Override
				public PreparedStatement createPreparedStatement(Connection con) throws SQLException {
					PreparedStatement statement = con.prepareStatement(UPDATE_ORDER_LINK, new String [] { "id" });
					UUID updateBulkOrderUuid = results.get(0).bulkOrderUuid;
					UUID updateSubscriptionUuid = results.get(0).subscriptionUuid;
					if (updateBulkOrderUuid == null) updateBulkOrderUuid = bulkOrderUuid;
					if (updateSubscriptionUuid == null) updateSubscriptionUuid = subscriptionUuid;
					
					if (updateBulkOrderUuid == null)
						statement.setNull(1, java.sql.Types.VARCHAR);
					else
						statement.setString(1, updateBulkOrderUuid.toString());
					if (updateSubscriptionUuid == null)
						statement.setNull(2, java.sql.Types.NUMERIC);
					else
						statement.setString(2, updateSubscriptionUuid.toString());
					statement.setLong(3, orderId);
					statement.setLong(4, endpointId);
					return statement;
				}
			}, holder);
			
			return holder.getKey().longValue();
		} else {
			throw new EndpointDefinitionException(String.format("More than one endpoint-order links for endpoint %l, order %l", endpointId, orderId));
		}
	}
	
	protected static class EndpointRow {
		public long id;
		public String endpoint, username, password;
	}
	
	protected static class EndpointRowMapper implements RowMapper<EndpointRow> {
		@Override
		public EndpointRow mapRow(ResultSet rs, int rowNum) throws SQLException {
			EndpointRow row = new EndpointRow();
			row.id = rs.getLong("id");
			row.endpoint = rs.getString("endpoint");
			row.username = rs.getString("username");
			row.password = rs.getString("password");
			return row;
		}
	}
	
	protected static class EndpointXOrdersQueueRow {
		public long id, orderId, endpointId;
		public UUID bulkOrderUuid, subscriptionUuid;
	}
	
	protected static class EndpointXOrdersQueueRowMapper implements RowMapper<EndpointXOrdersQueueRow> {
		@Override
		public EndpointXOrdersQueueRow mapRow(ResultSet rs, int rowNum) throws SQLException {
			EndpointXOrdersQueueRow row = new EndpointXOrdersQueueRow();
			row.id = rs.getLong("id");
			row.orderId = rs.getLong("order_id");
			String bulkOrderUuid = rs.getString("bulkorder_uuid");
			String subscriptionUUID = rs.getString("subscription_uuid");
			row.bulkOrderUuid = bulkOrderUuid == null ? null : UUID.fromString(bulkOrderUuid);
			row.subscriptionUuid = subscriptionUUID == null ? null : UUID.fromString(subscriptionUUID);
			row.endpointId = rs.getLong("endpoint_id");
			return row;
		}
	}
}
