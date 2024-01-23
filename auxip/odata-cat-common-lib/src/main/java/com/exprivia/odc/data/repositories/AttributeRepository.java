package com.exprivia.odc.data.repositories;

import java.util.List;

import com.exprivia.odc.data.dto.Attribute;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface AttributeRepository extends CountableRepository {

	List<Attribute<?>> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}
