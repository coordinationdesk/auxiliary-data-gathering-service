package com.exprivia.odc.data.repositories;

import java.util.List;

import com.exprivia.odc.data.dto.Quota;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface QuotaRepository extends CountableRepository {

	public List<Quota> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}
