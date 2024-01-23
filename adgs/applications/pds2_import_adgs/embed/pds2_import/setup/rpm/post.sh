#if [ "$1" = "1" ] ; then  # first install

#TMPCRON=/tmp/emsa_csn_preinv_crontab.crontab
#crontab -u opemsa -l > $TMPCRON
# just to be sure, delete unexpected and undeleted previous versions of emsa_csn_preinv_crontab crontab
#sed -i "/ACS_EMSA_CSN_PREINV_CRONTAB.*/,/END_ACS_EMSA_CSN_PREINV_CRONTAB.*/d" $TMPCRON
# now add new ones
#cat /var/www/html/emsa_csn_preimport/setup/conf/emsa_csn_preimport_crontab >>  $TMPCRON
# install new crontab
#crontab -u opemsa  $TMPCRON
#rm -fr  $TMPCRON

#fi # First install
