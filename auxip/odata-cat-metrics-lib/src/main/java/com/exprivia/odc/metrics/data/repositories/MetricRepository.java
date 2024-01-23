package com.exprivia.odc.metrics.data.repositories;

import java.util.List;

import com.exprivia.odc.data.repositories.CountableRepository;
import com.exprivia.odc.metrics.data.dto.Metric;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface MetricRepository extends CountableRepository
{
	Metric findById(String id);

	List<Metric> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}