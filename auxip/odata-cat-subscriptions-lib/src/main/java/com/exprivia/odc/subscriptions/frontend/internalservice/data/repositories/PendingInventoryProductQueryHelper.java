package com.exprivia.odc.subscriptions.frontend.internalservice.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.data.repositories.queryhelpers.ODCAbstractProductQueryHelper;
import com.exprivia.odc.subscriptions.frontend.internalservice.data.dto.PendingInventory;

public class PendingInventoryProductQueryHelper extends ODCAbstractProductQueryHelper<PendingInventory> {
	private static Logger LOG = LoggerFactory.getLogger(PendingInventoryProductQueryHelper.class);

	public PendingInventoryProductQueryHelper(DBTableTranslator tableTranslator, ODCDBDefaults odcConfig) {
		super(tableTranslator, odcConfig);
	}
	
	@Override
	protected List<String> getFieldList() {
		List<String> fields = new ArrayList<String>(super.getFieldList());
		fields.add("operation");
		return fields;
	}
	
	@Override
	protected String getInventoryTableName() {
		return "t_srv_pendinginv";
	}
	
	public PendingInventory findById(JdbcTemplate jdbcTemplate, Long id) {
		PendingInventory ret = null;
		try {
			ret = jdbcTemplate.queryForObject(
					"SELECT * FROM t_srv_pendinginv pi WHERE pi.id = ?", 
					createProductMapper(), 
					new Object[] {id});
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("product not found for id {}", id);
			return null;
		}
		return ret;
	}

	@Override
	public String convertSimpleColumnName(String name) {
		return name;
	}

	@Override
	public RowMapper<PendingInventory> createProductMapper() {
		return new RowMapper<PendingInventory>() {
			@Override
			public PendingInventory mapRow(ResultSet rs, int rowNum) throws SQLException {
				return PendingInventory.builder()
						.id(rs.getLong("id"))
						.uuid(rs.getString("uuid"))		// uuid of the PRODUCT
						.name(rs.getString("name"))
						.operation(rs.getString("operation"))
						.build();
			}
		};
	}

	
}
