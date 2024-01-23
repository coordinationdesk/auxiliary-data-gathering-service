UPDATE t_filetypes
	SET nameregularexpression = '^[aA]1[dD][0-9]{16}1$'
	WHERE filetype = 'AUX_WND_OR';
