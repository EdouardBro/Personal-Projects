-- APPLICATION TFTP --

Cette application réseau TFTP a été réalisée dans le cadre d'un cours de réseau. Mon rôle était de coder la partie "Receive File", c'est pourquoi la partie "Scend File" est indisponible.

TFTP (Trivial File Transfert Protocol) est un protocole basé sur UDP qui permet le transfert de fichiers.

Cette application fonctionne avec l'application PumpKIN qui simule un serveur distant. Il faut donc bien penser à configurer l'adresse IP dans le Main avec celle du serveur PumpKIN.
Il faut aussi définir le chemin d'accès vers le fichier à transférer dans le Client.

Au lancement de l'Application vous devrez rentrer :
- 1 pour recevoir un fichier
- 2 pour envoyer un fichier (l'envoi d'un fichier n'a pas été codé).

Ensuite vous devrez rentrer le nom du fichier avec son extension. Vous pouvez transférer tout type de fichier : textes, images, vidéos ...

TFTP est un protocole généralement utilisé sur des réseaux locaux.

Téléchargement de PumpKIN :  http://kin.klever.net/pumpkin/binaries#.XxCdzC_-Q_U
