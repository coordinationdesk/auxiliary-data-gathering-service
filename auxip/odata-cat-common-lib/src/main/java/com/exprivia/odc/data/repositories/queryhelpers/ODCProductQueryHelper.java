package com.exprivia.odc.data.repositories.queryhelpers;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import org.springframework.boot.json.JsonParseException;
import org.springframework.boot.json.JsonParserFactory;
import org.springframework.jdbc.core.RowMapper;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.data.dto.Checksum;
import com.exprivia.odc.data.dto.Product;
import com.exprivia.odc.data.dto.TimeRange;
import com.exprivia.odc.data.repositories.DBTableTranslator;
import com.exprivia.odc.util.EWKBToGeospatial;
import com.exprivia.odc.util.Util;

public class ODCProductQueryHelper extends ODCAbstractProductQueryHelper<Product> {

	public ODCProductQueryHelper(DBTableTranslator tableTranslator, ODCDBDefaults odcConfig) {
		super(tableTranslator, odcConfig);
	}

	@Override
	public String convertSimpleColumnName(String name) {
		return name;
	}

	@Override
	public RowMapper<Product> createProductMapper() {
		return new RowMapper<Product>() {
			@Override
			public Product mapRow(ResultSet rs, int rowNum) throws SQLException {
				Boolean online = rs.getTimestamp("eviction_time") != null;

				Product.ProductBuilder product = Product.builder().id(UUID.fromString(rs.getString("uuid")))
						.name(rs.getString("name")).contentType(rs.getString("contenttype"))
						.contentLength(rs.getLong("contentlength")).originDate(rs.getTimestamp("origindate"))
						.publicationDate(rs.getTimestamp("creationdate"))
						.modificationDate(rs.getTimestamp("modificationdate")).online(online);
				
				if (online)
					product.evictionDate(rs.getTimestamp("eviction_time"));

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

				byte bytes[] = rs.getBytes("footprint");
				if (bytes != null && bytes.length > 0) {
					product.footprint(new EWKBToGeospatial().read(bytes));
				}

				return product.build();
			}
		};
	}

}
