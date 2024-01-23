package com.exprivia.odc.subscriptions.frontend.internalservice.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.subscriptions.frontend.internalservice.data.dto.PendingInventory;

@Component
public class PendingInventoryRepositoryJDBCImpl implements PendingInventoryRepository {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private PendingInventoryProductQueryHelper productQueryHelper;
	
	public PendingInventoryRepositoryJDBCImpl(
			@Autowired DBTableTranslator tableTranslator,
			@Autowired ODCDBDefaults odcConfig
			) {
		productQueryHelper = new PendingInventoryProductQueryHelper(tableTranslator, odcConfig);
	}

	@Override
	public PendingInventory findById(Long id) {
		return productQueryHelper.findById(jdbcTemplate, id);
	}
	
	@Override
	public List<PendingInventory> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {
		return productQueryHelper.findWithParams(jdbcTemplate, jdbcQuery, orderByColumns);
	}

	class PendingInventoryRowMapper implements RowMapper<PendingInventory> {

		@Override
		public PendingInventory mapRow(ResultSet rs, int rowNum) throws SQLException {
			return PendingInventory.builder()
					.id(rs.getLong("id"))
					.uuid(rs.getString("uuid"))		// uuid of the PRODUCT
					.name(rs.getString("name"))
					.operation(rs.getString("operation"))
					.build();
		}
	}

}
