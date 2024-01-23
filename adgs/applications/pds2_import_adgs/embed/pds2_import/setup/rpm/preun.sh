#if [ "$1" = "0" ] ; then  # Last  uninstall

#TMPCRON=/tmp/emsa_csn_scheduler.crontab
#crontab -u opemsa -l > $TMPCRON
#sed -i "/ACS_EMSA_CSN_PREINV_CRONTAB.*/,/END_ACS_EMSA_CSN_PREINV_CRONTAB.*/d" $TMPCRON
#crontab -u opemsa $TMPCRON

#fi # Last uninstall
