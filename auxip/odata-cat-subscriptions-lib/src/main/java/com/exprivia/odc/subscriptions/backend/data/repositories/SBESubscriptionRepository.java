package com.exprivia.odc.subscriptions.backend.data.repositories;

import com.exprivia.odc.subscriptions.backend.data.dto.Subscription;

public interface SBESubscriptionRepository {
	Subscription getById(long id);
}
