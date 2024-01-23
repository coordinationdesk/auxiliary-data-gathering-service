package com.exprivia.odc.service.permissions;

import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class ODCSystemRoleList {
	
	private Map<Integer, ODCSystemRole> values = new HashMap<>();
	
	public Collection<ODCSystemRole> getValues() {
		return values.values();
	}
	
	public void addValue(int ordinal, String define, String name) {
		
		values.put(ordinal, ODCSystemRole.builder().ordinal(ordinal).define(define).name(name).build());
	}
	
	public void addValue(ODCSystemRole value) {
		values.put(value.getOrdinal(), value);
	}
	
	public String getDefine(int ordinal) {
		return getValue(ordinal).getDefine();
	}
	
	public String getName(int ordinal) {
		return getValue(ordinal).getName();
	}
	
	public ODCSystemRole getValue(int ordinal) {
		return values.get(ordinal);
	}
	
    public ODCSystemRole fromName(String text) {
    	for (Map.Entry<Integer, ODCSystemRole> role : values.entrySet()) {
    		ODCSystemRole value = role.getValue();
            if (value.getName().equalsIgnoreCase(text)) {
                return value;
            }
        }
    	throw new RuntimeException("Cannot decode SystemRoleEnum from string: " + text);
    }

    public ODCSystemRole fromDefine(String text) {
    	for (Map.Entry<Integer, ODCSystemRole> role : values.entrySet()) {
    		ODCSystemRole value = role.getValue();
            if (value.getDefine().equalsIgnoreCase(text)) {
                return value;
            }
        }
    	throw new RuntimeException("Cannot decode SystemRoleEnum from string: " + text);
    }
    
    public static String toString(Iterator<ODCSystemRole> it) {
    	StringBuilder sb = new StringBuilder("[");
    	while (it.hasNext()) {
    		sb.append(it.next().getName());
    		if (it.hasNext())
    			sb.append(",");
    	}
    	sb.append("]");
    	return sb.toString();    	
    }
    
    public static String toString(ODCSystemRole [] roles) {
    	return toString(Arrays.asList(roles).iterator());
    }
    
    public String toString() {
    	return ODCSystemRoleList.toString(values.values().iterator());
    }
}
