package com.exprivia.auxip.util;

import java.net.URI;
import java.net.URISyntaxException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Component;

import com.exprivia.auxip.servlet.AUXIPConfig;
import com.exprivia.odc.service.permissions.ODCUserPermissions;

import lombok.Builder;
import lombok.Data;

@Component
public class ProductUrlResolver {
	@Autowired
	private JdbcTemplate jdbcTemplate;
	@Autowired
	private AUXIPConfig auxipConfig;
	
	@Autowired
	private ODCUserPermissions userPermissions;
	
	private static final String SMTYPE_ISM = "ISM";
	private static final String SMTYPE_OBJ = "OBJSM";
	
	private final static String STORAGE_MANAGER_FROM_PRODUCT_UUID = "SELECT sm.type, sm.invrelationtable, invsm.id \n" + 
			"  FROM t_srv_inventory inv\n" +
			"  INNER JOIN t_srv_inventorysm invsm ON invsm.inv_id = inv.id\n" +
			"  INNER JOIN t_storagemanager sm ON invsm.storagemanid = sm.id\n" +
			"  WHERE inv.uuid = ?";
	
	public URI getProductObjectStoragePath(String productUuid) throws DataAccessException, URISyntaxException {
		ExternalStorageReference productRef = jdbcTemplate.queryForObject(
				STORAGE_MANAGER_FROM_PRODUCT_UUID, 
				new RowMapper<ExternalStorageReference>() {
					@Override
					public ExternalStorageReference mapRow(ResultSet rs, int rowNum) throws SQLException {
						return ExternalStorageReference.builder()
								.type(rs.getString("type"))
								.tableName(rs.getString("invrelationtable"))
								.id(rs.getLong("id"))
								.build();
					}
				}, 
				productUuid
				);
		
		String pathFieldName = getPathFieldName(productRef);
		String path = jdbcTemplate.queryForObject(
				"SELECT " + pathFieldName + 
				" FROM " + productRef.getTableName() + " WHERE id = ?",
				new RowMapper<String>() {
					@Override
					public String mapRow(ResultSet rs, int rowNum) throws SQLException {
						return rs.getString(pathFieldName);
					}
				}, 
				productRef.getId());
				
		return new URI(path);
	}
	
	protected String getPathFieldName(ExternalStorageReference productRef) {
		switch (productRef.getType()) {
		case SMTYPE_ISM:
			return "path";
		case SMTYPE_OBJ:
			return "http_path";
		default:
			throw new RuntimeException("unforeseen storage manager type: " + productRef.getType());
		}
	}
	
	public URI getDownloadUrl(String productUuid) throws DataAccessException, URISyntaxException {
		Long userId = getUserID(userPermissions.getRequestUsername());
		
		URI origUrl = getProductObjectStoragePath(productUuid);
		
		String args = origUrl.getQuery() == null ? "?userid=" + userId : "?" + origUrl.getRawQuery() + "&userid=" + userId;
		
		return new URI(auxipConfig.getDownloadLocation() + origUrl.getPath() + args);
	}
	
	private Long getUserID(String username) {
		List<Long> results = jdbcTemplate.queryForList(
				"SELECT id FROM t_srv_users WHERE name = ?", 
				new String[] {username}, 
				Long.class);
		
		if (results.size() == 0)
			throw new RuntimeException("User " + username + " not found");
		
		return results.get(0);
	}
	
	@Data
	@Builder
	static protected class ExternalStorageReference {
		public String type;
		public String tableName;
		public Long id;
	}
}
