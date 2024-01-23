package com.exprivia.odc.data.repositories;

import java.util.List;

import com.exprivia.odc.data.dto.SystemRole;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface SystemRoleRepository extends CountableRepository {

	SystemRole findByName(String name);

	public List<SystemRole> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}
