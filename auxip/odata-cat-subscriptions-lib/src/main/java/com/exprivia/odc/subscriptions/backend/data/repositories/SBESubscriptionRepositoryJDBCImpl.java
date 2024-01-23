package com.exprivia.odc.subscriptions.backend.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.subscriptions.backend.data.dto.Subscription;
import com.exprivia.odc.subscriptions.backend.data.dto.SubscriptionEvent;
import com.exprivia.odc.subscriptions.frontend.business.NotificationEndpointHandler;

@Repository
public class SBESubscriptionRepositoryJDBCImpl implements SBESubscriptionRepository {
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private static final Logger LOG = LoggerFactory.getLogger(SBESubscriptionRepositoryJDBCImpl.class);
	
	private static final String FIND_BY_ID = "SELECT s.*, u.name username, " + NotificationEndpointHandler.SELECT_FROM_COLUMNS + " FROM t_srv_subscriptions s INNER JOIN t_srv_users u ON u.id = s.user_id " + NotificationEndpointHandler.LEFT_JOIN_FROM_SUBSCRIPTION + " WHERE s.id = ?";

	@Override
	public Subscription getById(long id) {
		Subscription ret = null;
		try {
			ret = jdbcTemplate.queryForObject(FIND_BY_ID, new SubscriptionRowMapper(), id);
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("Subscription not found for id {}", id);
			return null;
		}
		return ret;
	}
	
	private class SubscriptionRowMapper implements RowMapper<Subscription> {

		@Override
		public Subscription mapRow(ResultSet rs, int rowNum) throws SQLException {
			/*
			 * Field LastNotificationDate changed in LTA ICD 1.3 but still
			 * mapped on lastQueryDate on DB schema
			 */
			return Subscription.builder()
					.id(rs.getLong("id"))
					.uuid(UUID.fromString(rs.getString("uuid")))
					.subscriptionEvent(SubscriptionEvent.fromString(rs.getString("subscriptionevent")))
					.lastNotificationDate(rs.getTimestamp("lastquerydate"))
					.stageOrder(rs.getBoolean("stageorder"))
					.priority(rs.getLong("priority"))
					.notificationEndpoint(rs.getString(NotificationEndpointHandler.ALIAS_ENDPOINT))
					.notificationEpUser(rs.getString(NotificationEndpointHandler.ALIAS_USERNAME))
					.notificationEpPassword(rs.getString(NotificationEndpointHandler.ALIAS_PASSWORD))
					.username(rs.getString("username"))
					.build();
		}
		
	}
}
