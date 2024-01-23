package com.exprivia.odc.data.repositories;

import java.util.List;

import com.exprivia.odc.data.dto.User;
import com.exprivia.odc.service.filter.JDBCQueryParams;

public interface UserRepository extends CountableRepository {

	User findByUsername(String username);

	List<User> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}
