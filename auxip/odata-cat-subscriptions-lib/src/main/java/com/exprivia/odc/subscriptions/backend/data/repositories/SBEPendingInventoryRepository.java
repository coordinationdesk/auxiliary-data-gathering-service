package com.exprivia.odc.subscriptions.backend.data.repositories;

import java.util.List;

import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;

public interface SBEPendingInventoryRepository {
	List<PendingInventory> getList();
	
	void removeItem(PendingInventory pendingInventory);
}
