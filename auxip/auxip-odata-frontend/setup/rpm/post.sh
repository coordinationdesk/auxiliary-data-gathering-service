
systemctl daemon-reload

if [ "$1" = "1" ]; then
    systemctl enable %{SERVICE_FILENAME}
	systemctl start %{SERVICE_FILENAME}
else
	systemctl restart %{SERVICE_FILENAME}
fi
