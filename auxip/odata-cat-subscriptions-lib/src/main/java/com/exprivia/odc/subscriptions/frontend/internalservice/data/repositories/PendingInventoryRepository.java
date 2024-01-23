package com.exprivia.odc.subscriptions.frontend.internalservice.data.repositories;

import java.util.List;

import com.exprivia.odc.service.filter.JDBCQueryParams;
import com.exprivia.odc.subscriptions.frontend.internalservice.data.dto.PendingInventory;

public interface PendingInventoryRepository {
	public PendingInventory findById(Long id);
	public List<PendingInventory> findWithParams(JDBCQueryParams jdbcQuery, List<String> orderByColumns);
}
