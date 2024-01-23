systemctl stop %{SERVICE_FILENAME}
if [ "$1" = "0" ]; then
	systemctl disable %{SERVICE_FILENAME}
	systemctl daemon-reload
fi
