package com.exprivia.odc.data.repositories;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.exprivia.odc.config.ODCDBTables;

@Component
public class DBTableTranslator {
	private Map<String, String> queries = new HashMap<>();
	private Pattern p = Pattern.compile("\\{([a-zA-Z]*?)\\}");

	@Autowired
	private ODCDBTables tables;
	
	protected boolean queryCaching = true;

	public boolean isQueryCaching() {
		return queryCaching;
	}

	public void setQueryCaching(boolean queryCaching) {
		this.queryCaching = queryCaching;
	}

	public String resolveQuery(String originalQuery) {
		if (queryCaching) {
			if (!queries.containsKey(originalQuery)) {
				queries.put(
						originalQuery, 
						translateQuery(originalQuery)
						);
			}
	
			return queries.get(originalQuery);
		} else {
			return translateQuery(originalQuery);
		}
	}

	private String translateQuery(String query) {
		Matcher m = p.matcher(query);
		StringBuffer sb = new StringBuffer();
		while (m.find()) {
			m.appendReplacement(sb, getTableFromName(m.group(1)));
		}
		m.appendTail(sb);
		return sb.toString();
	}

	private String getTableFromName(String name) {
		switch (name) {
		case "inventory":
			return tables.getInventory();
		case "aipContent":
			return tables.getAipContent();
		case "userRoles":
			return tables.getUserRoles();
		case "users":
			return tables.getUsers();
		case "usersXRoles":
			return tables.getUsersXRoles();
		case "aip":
			return tables.getAip();
		default:
			throw new RuntimeException("Unkown table name: " + name);
		}
	}
}
