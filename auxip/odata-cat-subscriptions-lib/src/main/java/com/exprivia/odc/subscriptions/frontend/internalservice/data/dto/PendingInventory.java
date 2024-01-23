package com.exprivia.odc.subscriptions.frontend.internalservice.data.dto;

import lombok.Builder;
import lombok.Data;
import lombok.NonNull;

@Data
@Builder
public class PendingInventory {
	@NonNull
	Long id;
	
	@NonNull
	String name;
	
	@NonNull
	String uuid;
	
	@NonNull
	String operation;
}
