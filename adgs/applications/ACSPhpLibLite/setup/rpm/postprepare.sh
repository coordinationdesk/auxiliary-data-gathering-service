mkdir -p $RPM_BUILD_ROOT/var/www/html/logs/
mkdir -p $RPM_BUILD_ROOT/var/www/html/runpid/
mkdir -p $RPM_BUILD_ROOT/etc/php.d/

mkdir -p $RPM_BUILD_ROOT/opt/ACSPhpLibLite/pear
cp -r $RPM_BUILD_DIR/ACSPhpLibLite/pear $RPM_BUILD_ROOT/opt/ACSPhpLibLite/

cat - > $RPM_BUILD_ROOT/etc/php.d/000-acsphplib-php7.ini <<EOF
include_path=.:/opt/ACSPhpLibLite:/opt/ACSPhpLibLite/pear
EOF
