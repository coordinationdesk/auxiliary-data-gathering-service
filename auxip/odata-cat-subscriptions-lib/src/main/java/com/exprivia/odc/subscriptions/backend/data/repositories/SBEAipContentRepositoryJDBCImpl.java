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

import com.exprivia.odc.subscriptions.backend.config.Config;
import com.exprivia.odc.subscriptions.backend.data.dto.AipContent;

@Repository
public class SBEAipContentRepositoryJDBCImpl implements SBEAipContentRepository {
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private Config config;
	
	private static final Logger LOG = LoggerFactory.getLogger(SBEAipContentRepositoryJDBCImpl.class);

	private static final String FIND_BY_PRODUCT_UUID = "SELECT ac.*, oq.uuid order_uuid FROM t_srv_aip_content ac LEFT JOIN t_srv_ordersqueue oq ON oq.id = ac.order_id WHERE ac.product_uuid = ?";
	private static final String UPDATE_EVICTION_DATE_BY_ID = "UPDATE t_srv_aip_content SET eviction_time = now() + cast (? as interval) WHERE id = ?";

	@Override
	public AipContent findByProductUuid(UUID productUuid) {
		AipContent ret = null;
		try {
			ret = jdbcTemplate.queryForObject(FIND_BY_PRODUCT_UUID, new AipContentRowMapper(), productUuid.toString());
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("AipContent not found for productUiid {}", productUuid);
			return null;
		}
		return ret;
	}
	
	@Override
	public void updateEvictionDate(AipContent aipContent) {
		Object[] params = { config.getEvictionTimePostponeInterval(), aipContent.getId() };
		int[] paramTypes = { java.sql.Types.VARCHAR, java.sql.Types.INTEGER };
		
		jdbcTemplate.update(UPDATE_EVICTION_DATE_BY_ID, params, paramTypes);

		LOG.info("Updated evictionDate of aipContent for product {} to {} + {}", 
				aipContent.getProductUuid(), aipContent.getEvictionTime(), config.getEvictionTimePostponeInterval());
	}
	
	private class AipContentRowMapper implements RowMapper<AipContent> {
		
		@Override
		public AipContent mapRow(ResultSet rs, int rowNum) throws SQLException {
			return AipContent.builder()
					.id(rs.getLong("id"))
					.productUuid(rs.getString("product_uuid"))
					.orderId(rs.getString("order_uuid"))
					.bucketName(rs.getString("bucket_name"))
					.productSize(rs.getLong("product_size"))
					.statusId(rs.getInt("status_id"))
					.uploadTime(rs.getTimestamp("upload_time"))
					.evictionTime(rs.getTimestamp("eviction_time"))
					.build();
		}
		
	}

}
