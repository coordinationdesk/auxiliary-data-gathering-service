package com.exprivia.odc.subscriptions.backend.data.dto;

import java.util.UUID;

import lombok.AccessLevel;
import lombok.Builder;
import lombok.Data;
import lombok.NonNull;
import lombok.experimental.FieldDefaults;

@Builder
@Data
@FieldDefaults(level = AccessLevel.PRIVATE)
public class PendingInventory {
	@NonNull
	Long id;

	@NonNull
	UUID uuid;
	
	@NonNull
	String name;
	
	@NonNull
	PendingInventoryEvent operation;
}
