package com.exprivia.odc.service;

import org.apache.olingo.commons.api.data.Entity;

import lombok.Builder;
import lombok.Data;

@Data
@Builder
public class ODCReturnedEntity {
	private Entity entity;
	private Boolean created;
	
	public Boolean isCreated() {
		return created;
	}
}
