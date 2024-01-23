package com.exprivia.auxip.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.regex.Pattern;

import org.springframework.boot.json.JsonParseException;
import org.springframework.boot.json.JsonParserFactory;
import org.springframework.jdbc.core.RowMapper;

import com.exprivia.auxip.data.dto.AUXIPProduct;
import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.dto.Checksum;
import com.exprivia.odc.data.dto.TimeRange;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.data.repositories.queryhelpers.ODCAbstractProductQueryHelper;
import com.exprivia.odc.util.Util;

public class AUXIPProductQueryHelper extends ODCAbstractProductQueryHelper<AUXIPProduct> {
	private Pattern lambdaPattern = Pattern.compile("(?<type>Attributes|Checksum)\\((?<alias>\\w+)\\)\\.(?<name>\\w+)::(?<valuetype>\\w+)");

	public AUXIPProductQueryHelper(DBTableTranslator tableTranslator, ODCDBDefaults odcConfig) {
		super(tableTranslator, odcConfig);
	}
	
	@Override
	protected String getDefaultOrderBySql() {
		return getOrderByWordSql() 
				+ " i.creationdate ";
	}
	
	protected Pattern getLambdaPattern() {
		return lambdaPattern;
	}
	
	@Override
	protected List<String> getFieldList() {
		return Arrays.asList(
				"i.id", 
				"i.uuid", 
				"i.name", 
				"i.contenttype", 
				"i.contentlength",
				"i.origindate", 
				"i.creationdate", 
				"i.checksum", 
				"i.validitystart", 
				"i.validitystop", 
				"st_asbinary(i.footprint) footprint", 
				"i.evictiondate",
				"i.productiontype"
				);
	}
	
	@Override
	protected List<String> getGroupByFieldList() {
		return Arrays.asList(
				"i.id", 
				"i.uuid", 
				"i.name", 
				"i.contenttype", 
				"i.contentlength", 
				"i.origindate", 
				"i.creationdate", 
				"i.checksum", 
				"i.validitystart", 
				"i.validitystop", 
				"i.footprint", 
				"i.evictiondate",
				"i.productiontype"
				);
	}
	
	@Override
	protected String getFindSelectSql() {
		return "SELECT "
				+ getColumnsListAsString()
				+ " from {inventory} i ";
	}	

	@Override
	public String convertCommonColumnName(String name) {
		if (name.equalsIgnoreCase("evictiondate")) {
			return "i.evictiondate";
		} else {
			return super.convertCommonColumnName(name);
		}
	}
	
	@Override
	public String convertSimpleColumnName(String name) {
		return name;
	}

	@Override
	public RowMapper<AUXIPProduct> createProductMapper() {
		return new RowMapper<AUXIPProduct>() {
			@Override
			public AUXIPProduct mapRow(ResultSet rs, int rowNum) throws SQLException {

				AUXIPProduct.AUXIPProductBuilder product = AUXIPProduct.builder().id(UUID.fromString(rs.getString("uuid")))
						.name(rs.getString("name")).contentType(rs.getString("contenttype"))
						.contentLength(rs.getLong("contentlength"))
						.publicationDate(rs.getTimestamp("creationdate"));

				if (rs.getTimestamp("origindate") != null)
					product.originDate(rs.getTimestamp("origindate"));
				if (rs.getTimestamp("evictiondate") != null)
					product.evictionDate(rs.getTimestamp("evictiondate"));

				try {
					List<Checksum> checksums = new ArrayList<Checksum>();
					for (Object dbChecksum: JsonParserFactory.getJsonParser().parseList(rs.getString("checksum"))) {
						if (dbChecksum instanceof Map) {
							Map<?, ?> checksumMap = (Map<?, ?>)dbChecksum;
							checksums.add(
									Checksum.builder()
									.algorithm(checksumMap.get("Algorithm").toString())
									.value(checksumMap.get("Value").toString()) 
									.checksumDate(checksumMap.containsKey("ChecksumDate") ? Util.parseDateString(checksumMap.get("ChecksumDate").toString()) : null)
									.build()
									);
							product = product.checksums(checksums);
						}
					}
				} catch (JsonParseException|ClassCastException|NullPointerException e) {
					throw new RuntimeException("Cannot decode the following string as Json 'Checksum': " + rs.getString("checksum"), e);
				}
				if (rs.getTimestamp("validitystart") != null && rs.getTimestamp("validitystop") != null) {
					product = product
							.contentDate(new TimeRange(rs.getTimestamp("validitystart"), rs.getTimestamp("validitystop")));
				}
				
				return product.build();
			}
		};
	}

}
