package com.exprivia.odc.subscriptions.backend.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;
import java.util.UUID;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventoryEvent;

@Repository
public class SBEPendingInventoryRepositoryJDBCImpl implements SBEPendingInventoryRepository {
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private Config config;
	
	private static final Logger LOG = LoggerFactory.getLogger(SBEPendingInventoryRepositoryJDBCImpl.class);
	
	private static final String FIND_ALL = "SELECT * FROM t_srv_pendinginv ORDER BY id LIMIT ?";
	private static final String DELETE_BY_ID = "DELETE FROM t_srv_pendinginv WHERE id = ?";

	@Override
	public List<PendingInventory> getList() {
		Object [] args = { config.getPendingInvLimit() };
		
		return (List<PendingInventory>) jdbcTemplate.query(FIND_ALL, args, new PendingInventoryRowMapper());
	}

	@Override
	public void removeItem(PendingInventory pendingInventory) {
		removeItemInternal(pendingInventory.getId());
		LOG.info("removed pending inventory {} ({}/{})", pendingInventory.getId(), pendingInventory.getName(), pendingInventory.getOperation());
	}
	
	private void removeItemInternal(Long id) {
		Object [] args = { id };
		int [] types = { java.sql.Types.INTEGER };
		jdbcTemplate.update(DELETE_BY_ID, args, types);
	}

	private class PendingInventoryRowMapper implements RowMapper<PendingInventory> {
		
		@Override
		public PendingInventory mapRow(ResultSet rs, int rowNum) throws SQLException {
			Long id = rs.getLong("id");
			String operation = rs.getString("operation");
			
			try {
				return PendingInventory.builder()
						.id(id)
						.uuid(UUID.fromString(rs.getString("uuid")))
						.name(rs.getString("name"))
						.operation(PendingInventoryEvent.fromString(operation))
						.build();
			} catch (RuntimeException e) {
				removeItemInternal(id);
				LOG.warn("Removed pending inventory item {} due to an error: {}", id, e.getMessage());
			}
			return null;
		}
		
	}
}
