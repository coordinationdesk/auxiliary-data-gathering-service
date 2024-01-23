mkdir -p $RPM_BUILD_ROOT/%{APPLICATION_PATH}/config
mkdir -p $RPM_BUILD_ROOT/etc/php.d
mkdir -p $RPM_BUILD_ROOT/usr/lib/systemd/system/

ln -s %{PROJECT_PATH}/pds2_import/config/pds2_import.ini $RPM_BUILD_ROOT/%{APPLICATION_PATH}/config/pds2_import.ini

ln -sf %{APPLICATION_PATH}/setup/conf/pds2_import_php.ini $RPM_BUILD_ROOT/etc/php.d/pds2_import_php.ini

cp $RPM_BUILD_DIR/%{APPLICATION_NAME}/setup/conf/pds2_import_group_none.service $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_DIR/%{APPLICATION_NAME}/setup/conf/pds2_import_group_1.service    $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_DIR/%{APPLICATION_NAME}/setup/conf/pds2_import_group_2.service    $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_DIR/%{APPLICATION_NAME}/setup/conf/pds2_import_group_none.timer   $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_DIR/%{APPLICATION_NAME}/setup/conf/pds2_import_group_1.timer      $RPM_BUILD_ROOT/usr/lib/systemd/system/
cp $RPM_BUILD_DIR/%{APPLICATION_NAME}/setup/conf/pds2_import_group_2.timer      $RPM_BUILD_ROOT/usr/lib/systemd/system/

# embed pds2_import
find $RPM_BUILD_DIR/%{APPLICATION_NAME}/embed/ \
	-type d \
	-mindepth 1 -maxdepth 1 \
	-exec cp -r {} $RPM_BUILD_ROOT/%{PROJECT_PATH} \;
