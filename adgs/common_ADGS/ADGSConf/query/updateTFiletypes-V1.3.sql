UPDATE t_filetypes
	SET nameregularexpression = '^S1[ABC_]_.*_MPL__GTDEV_[0-9]{8}T[0-9]{6}_[0-9]{8}T[0-9]{6}_[0-9]{4}\.DBL$'
	WHERE filetype = 'S1MPL__RGTEF';
