package com.exprivia.odc.subscriptions.backend.engine;

import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.subscriptions.backend.data.dto.PendingInventory;
import com.exprivia.odc.subscriptions.backend.data.repositories.SBEPendingInventoryRepository;

@Component
public class QueueManager {
	@Autowired
	private SBEPendingInventoryRepository pendingInventoryRepository;
	
	private static final Logger LOG = LoggerFactory.getLogger(QueueManager.class);
	
	public List<PendingInventory> getQueuedItems() {
		List<PendingInventory> items = pendingInventoryRepository.getList();
		LOG.info("found {} pendinginv items", items.size());
		return items;
	}
	
	public void wipeQueuedItems(List<PendingInventory> items) {
		for (PendingInventory item : items) {
			pendingInventoryRepository.removeItem(item);
		}

		LOG.debug("wiped {} items from the pendingInventory queue", items.size());
	}
}
