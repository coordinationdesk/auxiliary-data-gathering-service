package com.exprivia.odc.internalservice.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.internalservice.data.dto.AipContent;
import com.exprivia.odc.internalservice.data.dto.AipContentStatus;
import com.exprivia.odc.service.ODCRequestEnvironment;

import lombok.Builder;
import lombok.Data;

@Repository
public class AipContentRepositoryJDBCImpl implements AipContentRepository {
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	@Autowired
	private ODCRequestEnvironment reqEnv;
	
	@Autowired
	private DBTableTranslator tableTranslator;
	
	private static final Logger LOG = LoggerFactory.getLogger(AipContentRepositoryJDBCImpl.class);

	private static final String WHERE = " where ";
	private static final String FIND_ALL = "SELECT ac.*, oq.uuid order_uuid, s.status status, a.aip_name, a.base_redirect_url, i.name product_name, u.name username FROM {aipContent} ac INNER JOIN {aip} a ON a.id = ac.aip_id INNER JOIN {inventory} i ON i.uuid = ac.product_uuid INNER JOIN t_srv_statuses s ON ac.status_id = s.id LEFT JOIN t_srv_ordersqueue oq ON oq.id = ac.order_id LEFT JOIN t_srv_request_x_user rxu ON rxu.order_id = oq.id LEFT JOIN {users} u ON rxu.user_id = u.id";
	private static final String FIND_BY_PRODUCT_UUID = FIND_ALL + WHERE + " ac.product_uuid = ?";
	private static final String FIND_BY_PRODUCT_UUID_AND_AIP_ID = FIND_ALL + WHERE + " ac.product_uuid = ? AND aip_id = ?";
	private static final String FIND_BY_ORDER_UUID = FIND_ALL + WHERE + " oq.uuid = ? and u.name = ?";
	private static final String CHECK_PRODUCT_STATUS = "select\n"
			+ "	count(i.*) invcount, count(os.*) ordcount, count(sa.*) aipcount\n"
			+ "from t_srv_inventory i\n"
			+ "left join t_srv_aip_content ac     on i.uuid = ac.product_uuid\n"
			+ "left join t_srv_aip a              on a.id = ac.aip_id\n"
			+ "left join t_srv_statuses sa        on ac.status_id = sa.id and sa.status not in ('INEVICTION', 'EVICTED')\n"
			+ "left join t_srv_ordersqueue oq     on oq.product_id = i.id\n"
			+ "left join t_srv_statuses os        on oq.status_id = os.id and os.status not in ('ERROR', 'CANCELLED')\n"
			+ "left join t_srv_request_x_user rxu on rxu.order_id = oq.id\n"
			+ "where i.uuid = ?";
	
	@Override
	public AipContent findByProductUuid(String productUuid) {
		AipContent ret = null;
		try {
			ret = jdbcTemplate.queryForObject(
					tableTranslator.resolveQuery(FIND_BY_PRODUCT_UUID), 
					new AipContentRowMapper(), 
					productUuid);
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("AipContent not found for productUiid {} user {}", productUuid, reqEnv.getUsername());
			return null;
		}
		return ret;
	}
	
	@Override
	public AipContent findByProductUuidAndAipId(String productUuid, Long aipId) {
		AipContent ret = null;
		try {
			ret = jdbcTemplate.queryForObject(
					tableTranslator.resolveQuery(FIND_BY_PRODUCT_UUID_AND_AIP_ID), 
					new AipContentRowMapper(), 
					productUuid,
					aipId);
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("AipContent not found for productUiid {} user {}", productUuid, reqEnv.getUsername());
			return null;
		}
		return ret;
	}
	
	@Override
	public AipContent findByOrderUuid(String orderUuid) {
		AipContent ret = null;
		try {
			ret = jdbcTemplate.queryForObject(
					tableTranslator.resolveQuery(FIND_BY_ORDER_UUID), 
					new AipContentRowMapper(), 
					orderUuid, 
					reqEnv.getUsername());
		} catch (EmptyResultDataAccessException e) {
			// no results found
			LOG.warn("AipContent not found for orderUiid {} user {}", orderUuid, reqEnv.getUsername());
			return null;
		}
		return ret;
	}
	
	@Override
	public DownloadStatus checkDownloadStatus(String productId) {
		DownloadStatusRecord dsr = jdbcTemplate.queryForObject(
				CHECK_PRODUCT_STATUS, 
				new Object[] { productId }, 
				new DownloadStatusRowMapper());
		
		if (dsr.getInvCount() == 0)
			return DownloadStatus.PRODUCT_NOT_EXISTS;
		if (dsr.getOrdCount() == 0)
			return DownloadStatus.ORDER_NOT_SCHEDULED;
		if (dsr.getAipCount() == 0)
			return DownloadStatus.ORDER_SCHEDULED_PRODUCT_NOT_READY;
		return DownloadStatus.PRODUCT_DOWNLOADING_OR_READY;
	}
	
	@Data
	@Builder
	private static class DownloadStatusRecord {
		private int invCount, ordCount, aipCount;
	}
	
	private class DownloadStatusRowMapper implements RowMapper<DownloadStatusRecord> {

		@Override
		public DownloadStatusRecord mapRow(ResultSet rs, int rowNum) throws SQLException {
			return DownloadStatusRecord.builder()
					.invCount(rs.getInt("invcount"))
					.ordCount(rs.getInt("ordcount"))
					.aipCount(rs.getInt("aipcount"))
					.build();
		}
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
					.contentStatus(AipContentStatus.fromString(rs.getString("status")))
					.uploadTime(rs.getTimestamp("upload_time"))
					.evictionTime(rs.getTimestamp("eviction_time"))
					.aipName(rs.getString("aip_name"))
					.aipBaseRedirectUrl(rs.getString("base_redirect_url"))
					.productName(rs.getString("product_name"))
					.build();
		}
		
	}

}
