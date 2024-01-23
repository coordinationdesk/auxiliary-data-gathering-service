package com.exprivia.odc.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.time.Duration;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.olingo.commons.api.edm.FullQualifiedName;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.data.dto.Quota;
import com.exprivia.odc.edmprovider.ODCEdmProvider;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.util.Util;

@Repository
public class QuotaRepositoryJDBCImpl implements QuotaRepository {
	
	@Autowired
	private JdbcTemplate jdbcTemplate;
	
	private final static String WHERE = " where ";

	private final static String SELECT = "select ";
	private final static String COUNT = " count(1) ";

	private final static String QUERY_BODY = " from (select {fields}, user_id from t_srv_users_quota) q";
	private final static String FIND_ALL = SELECT + "*" + QUERY_BODY;
	private final static String FIND_COUNT = SELECT + COUNT + QUERY_BODY;
	private final static String FIND_QUOTAS_BY_USERNAME = " q.user_id in (select id from t_srv_users where name = ?)";

	private final static String [] quotaFields = {
			"'TotalOrdersQuota' tp, 		orderinterval itv, 		ordersize sz",
			"'ParallelOrdersQuota' tp,		null::interval itv,		orderparallel sz",
			"'TotalBatchOrdersQuota' tp, 	batchinterval itv, 		batchsize sz",
			"'ParallelBatchOrdersQuota' tp,	null::interval itv, 	batchparallel sz",
			"'TotalDownloadsQuota' tp, 		downloadinterval itv, 	downloadsize sz",
			"'ParallelDownloadsQuota' tp, 	null::interval itv, 	downloadparallel sz"
		};
	
	static public JDBCQueryParams addUserCondition(JDBCQueryParams jdbcQueryParams, String username) {
		// create new params, the jdbcQueryParams can be null
		return Util.addArbitrarySQLCondition(
				jdbcQueryParams,
				FIND_QUOTAS_BY_USERNAME,
				Arrays.asList(username));
	}
	
	protected String[] getQuotaFields() {
		return quotaFields;
	}
	
	public List<Quota> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns) {

		List<Quota> results = new ArrayList<Quota>();

		Object[] valuesPs = null;
		JDBCFilterExpression filter = jdbcQuery.getJdbcFilterExpression();
		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
		}

		for (String fields : getQuotaFields()) {
			StringBuilder query = new StringBuilder();
	
			query.append(FIND_ALL.replace("{fields}", fields));

			if (filter != null) {
				query.append(WHERE).append(filter.toString());
			}

			if (jdbcQuery.getLimitNumber() == null ||
				results.size() < jdbcQuery.getLimitNumber())
				results.addAll((List<Quota>) jdbcTemplate.query(query.toString(), valuesPs, new QuotaRowMapper()));
		}

		return results;
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		// copy the jdbcQuery and remove limit and offset options
		JDBCFilterExpression filter = jdbcQuery.toBuilder()
				.limitNumber(null).offsetNumber(null)
				.build().getJdbcFilterExpression();
		int results = 0;
		
		Object[] valuesPs = null;
		if (filter != null) {
			valuesPs = filter.getParameters().toArray();
		}

		for (String fields : getQuotaFields()) {
			StringBuilder query = new StringBuilder();
	
			query.append(FIND_COUNT.replace("{fields}", fields));

			if (filter != null) {
				query.append(WHERE).append(filter.toString());
			}
			
			results += jdbcTemplate.queryForObject(query.toString(), valuesPs, Integer.class);
		}
		
		return results;
	}
	
	public String convertColumnName(String name) {
		switch (name) {
		case "Number":
			return "q.sz";
		case "Duration":
			return "q.itv";
		}
		return name;
	}
	
	private class QuotaRowMapper implements RowMapper<Quota> {
		
		@Override
		public Quota mapRow(ResultSet rs, int rowNum) throws SQLException {
			String textDuration = rs.getString("itv");
			Duration duration = textDuration == null ? null : Duration.parse(textDuration);
			Long number = rs.getLong("sz");
			FullQualifiedName quotaType = new FullQualifiedName(ODCEdmProvider.NAMESPACE, rs.getString("tp"));
			
			return Quota.builder()
					.duration(duration)
					.number(number)
					.quotaType(quotaType)
					.build();
		}
	}
}
