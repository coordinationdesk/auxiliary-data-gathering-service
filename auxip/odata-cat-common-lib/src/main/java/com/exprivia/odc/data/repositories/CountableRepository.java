package com.exprivia.odc.data.repositories;

import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface CountableRepository {
	int getRecordsCount(JDBCQueryParams jdbcQuery);
}
