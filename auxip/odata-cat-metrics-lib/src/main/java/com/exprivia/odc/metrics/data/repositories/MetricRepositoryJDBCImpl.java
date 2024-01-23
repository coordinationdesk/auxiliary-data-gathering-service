package com.exprivia.odc.metrics.data.repositories;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import com.exprivia.odc.config.ODCDBDefaults;
import com.exprivia.odc.metrics.data.dto.Metric;
import com.exprivia.odc.metrics.data.dto.MetricType;
import com.exprivia.odc.metrics.service.permissions.ODCMetricsActions;
import com.exprivia.odc.service.filter.JDBCFilterExpression;
import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.service.permissions.ODCActionsPermissions;

@Repository
public class MetricRepositoryJDBCImpl implements MetricRepository
{
	@Autowired
	private JdbcTemplate jdbcTemplate;

	@Autowired
	private ODCActionsPermissions actionsPermissions;

	@Autowired
	private ODCDBDefaults dbConfig;

	private final static String SELECT         = "SELECT ";
	private final static String COUNT          = " COUNT(1) ";
	private final static String FIELDS         = " m.id, m.name, m.tstamp, m.metrictype, m.counter, m.gauge ";
	private final static String ORDERBY        = " ORDER BY ";
	private final static String WHERE          = " WHERE ";
	private final static String QUERY_BODY     = "FROM public.t_srv_lastmetrics m";
	private final static String FIND_ALL       = SELECT + FIELDS + QUERY_BODY;
	private final static String FIND_COUNT     = SELECT + COUNT + QUERY_BODY;
	private final static String FIND_BY_ID     = FIND_ALL + WHERE + " m.name = ?";
	private final static String ROW_LIMIT      = " LIMIT ";
	private final static String ROW_OFFSET     = " OFFSET ";

	private static final Logger logger = LoggerFactory.getLogger(MetricRepositoryJDBCImpl.class);

	@Override
	public Metric findById(String id)
	{
		actionsPermissions.checkCurrentUserGranted(ODCMetricsActions.METRIC_FIND_BY_ID.getCode());

		Metric ret = null;

		try
		{
			ret = jdbcTemplate.queryForObject(FIND_BY_ID, new MetricRowMapper(), id);
		}
		catch(EmptyResultDataAccessException emptyResultDataAccessException)
		{
			logger.warn("order not found for id {}", id);
		}

		return ret;
	}

	@Override
	public List<Metric> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns)
	{
		actionsPermissions.checkCurrentUserGranted(ODCMetricsActions.METRIC_FIND_WITH_PARAMS.getCode());

		StringBuilder        query    = new StringBuilder();
		JDBCFilterExpression filter   = jdbcQuery.getJdbcFilterExpression();
		Object[]             valuesPs = null;

		query.append(FIND_ALL);

		if (filter != null)
		{
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}

		if (orderByColumns != null && orderByColumns.size() > 0)
		{
			query.append(ORDERBY);
			query.append(StringUtils.join(orderByColumns, ", "));
		}

		query.append(ROW_LIMIT).append(jdbcQuery.getSafeQueryLimit(dbConfig.getQueryLimit()));

		if (jdbcQuery.getOffsetNumber() != null)
		{
			query.append(ROW_OFFSET).append(jdbcQuery.getOffsetNumber().intValue());
		}

		return (List<Metric>) jdbcTemplate.query(query.toString(), valuesPs, new MetricRowMapper());
	}
	
	@Override
	public int getRecordsCount(JDBCQueryParams jdbcQuery) {
		actionsPermissions.checkCurrentUserGranted(ODCMetricsActions.METRIC_FIND_WITH_PARAMS.getCode());

		StringBuilder        query    = new StringBuilder();
		JDBCFilterExpression filter   = jdbcQuery.getJdbcFilterExpression();
		Object[]             valuesPs = null;

		query.append(FIND_COUNT);

		if (filter != null)
		{
			valuesPs = filter.getParameters().toArray();
			query.append(WHERE).append(filter.toString());
		}

		return jdbcTemplate.queryForObject(query.toString(), valuesPs, Integer.class);
	}

	public String convertColumnName(String name)
	{
		String result = name;

		switch(name)
		{
			case "Name":
				result = "m.name";
				break;

			case "Timestamp":
				result = "m.tstamp";
				break;

			case "MetricType":
				result = "m.metrictype";
				break;

			case "Gauge":
				result = "m.gauge";
				break;

			case "Counter":
				result = "m.counter";
				break;
		}

		return result;
	}

	class MetricRowMapper implements RowMapper<Metric>
	{
		@Override
		public Metric mapRow(ResultSet rs, int rowNum) throws SQLException
		{
			Metric.MetricBuilder metricBuilder = Metric.builder();

			metricBuilder.id        (rs.getLong     ("id"))
				         .name      (rs.getString   ("name"))
				         .timestamp (rs.getTimestamp("tstamp"))
				         .metricType(MetricType.fromString(rs.getString("metrictype")));

			Object gauge   = rs.getObject("gauge");
			Object counter = rs.getObject("counter");

			if (counter == null)
			{
				metricBuilder.counter(null);
			}
			else
			{
				metricBuilder.counter((Long) counter);
			}

			if (gauge == null)
			{
				metricBuilder.gauge(null);
			}
			else
			{
				metricBuilder.gauge((Double) gauge);
			}

			return metricBuilder.build();
		}
	}
}