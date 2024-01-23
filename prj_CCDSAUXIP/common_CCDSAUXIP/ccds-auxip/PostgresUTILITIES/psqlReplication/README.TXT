Prerequisiti:
	* expect.x86_64 installato sulla/e macchina/e dove si esegue lo script
	* openssh-askpass.x86_64 installato sulla/e macchina/e dove si esegue lo script
	* le due macchine da replicare con postgres installato, configurato e con i database creati
	* le password di root di entrambe le macchine

Installazione:
	* Login come ti pare su ana qualsiasi macchina che vede le macchine da replicare (anche sulla stessa) e che abbia installato il pacchetto PostgresUTILITIES (il tar di achillino)
	* ./psqlReplicate -S 172.30.19.87 -U root -P 12qwas -s 172.30.19.88 -u root -p 12qwas (accetta solo gli ip!!!)


