systemctl daemon-reload

if [ "$1" = "1" ]; then
	# first install
    systemctl enable %{SERVICE_FILENAME}
	systemctl start %{SERVICE_FILENAME}
else
	# after update
	systemctl restart %{SERVICE_FILENAME}
fi
