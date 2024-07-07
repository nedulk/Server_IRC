
## Définitions

### Port
Un port est comme une porte numérotée sur un ordinateur par laquelle les données peuvent entrer ou sortir. Imaginez que votre ordinateur est une grande maison avec de nombreuses portes, et chaque porte a un numéro unique. Pour se connecter à un serveur IRC, vous utilisez un numéro de port spécifique (comme la porte 6667 pour le protocole IRC) pour garantir que les messages arrivent au bon endroit.

### I/O (Input/Output)
L'Input/Output (Entrée/Sortie) représente les communications entre un système informatique et le monde extérieur. C'est comme parler et écouter dans une conversation. Quand vous tapez un message dans un client IRC (input), ce message est envoyé au serveur. Le serveur répond ensuite avec des messages (output) que vous voyez à l'écran.

### poll()
La fonction `poll()` est utilisée pour surveiller plusieurs descripteurs de fichiers (comme des sockets) pour voir s'ils sont prêts à effectuer des opérations d'entrée/sortie (I/O). Imaginez un chef d'orchestre qui garde un œil sur tous les musiciens (sockets) pour voir qui est prêt à jouer (envoyer ou recevoir des données). `poll()` permet au programme de gérer plusieurs connexions sans bloquer, en vérifiant l'état des sockets et en répondant rapidement aux événements.

### TCP/IP
Le TCP/IP est une suite de protocoles de communication utilisés pour interconnecter des dispositifs réseau sur Internet. Pensez à TCP/IP comme à l'ensemble des règles de la route pour les données qui voyagent sur Internet. TCP (Transmission Control Protocol) garantit que les messages arrivent complets et dans le bon ordre, un peu comme un service de courrier fiable. IP (Internet Protocol) est responsable de l'acheminement des paquets de données vers la bonne adresse, comme une carte routière pour les données.
