package com.exprivia.odc.subscriptions.backend.data.repositories;

import java.util.UUID;

import com.exprivia.odc.subscriptions.backend.data.dto.AipContent;

public interface SBEAipContentRepository {
	AipContent findByProductUuid(UUID productUuid);
	
	void updateEvictionDate(AipContent aipContent);
}
