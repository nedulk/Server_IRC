
## Fonctions utiles pour le projet :

### `socket`
Crée un point de communication et renvoie un descripteur de socket.
```cpp
int socket(int domain, int type, int protocol);
```

### `close`
Ferme une socket ou un fichier ouvert.
```cpp
int close(int sockfd);
```

### `setsockopt`
Configure les options pour la socket.
```cpp
int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
```

### `getsockname`
Récupère l'adresse locale de la socket.
```cpp
int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

### `getprotobyname`
Récupère l'entrée du fichier des protocoles correspondant à un nom.
```cpp
struct protoent *getprotobyname(const char *name);
```

### `gethostbyname`
Récupère les informations sur un hôte donné son nom.
```cpp
struct hostent *gethostbyname(const char *name);
```

### `getaddrinfo`
Traduit le nom de l'hôte ou le service en structures d'adresses.
```cpp
int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
```

### `freeaddrinfo`
Libère la mémoire allouée par `getaddrinfo`.
```cpp
void freeaddrinfo(struct addrinfo *res);
```

### `bind`
Assigne une adresse locale à une socket.
```cpp
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

### `connect`
Établit une connexion avec une socket.
```cpp
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

### `listen`
Met la socket en mode passif, prête à accepter les connexions entrantes.
```cpp
int listen(int sockfd, int backlog);
```

### `accept`
Accepte une connexion entrante sur une socket.
```cpp
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

### `htons`
Convertit l'ordre des octets d'un entier 16 bits de l'hôte au réseau.
```cpp
uint16_t htons(uint16_t hostshort);
```

### `htonl`
Convertit l'ordre des octets d'un entier 32 bits de l'hôte au réseau.
```cpp
uint32_t htonl(uint32_t hostlong);
```

### `ntohs`
Convertit l'ordre des octets d'un entier 16 bits du réseau à l'hôte.
```cpp
uint16_t ntohs(uint16_t netshort);
```

### `ntohl`
Convertit l'ordre des octets d'un entier 32 bits du réseau à l'hôte.
```cpp
uint32_t ntohl(uint32_t netlong);
```

### `inet_addr`
Convertit une adresse IPv4 de la notation pointée en binaire.
```cpp
in_addr_t inet_addr(const char *cp);
```

### `inet_ntoa`
Convertit une adresse IPv4 binaire en notation pointée.
```cpp
char *inet_ntoa(struct in_addr in);
```

### `send`
Envoie des données via une socket connectée.
```cpp
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

### `recv`
Reçoit des données via une socket connectée.
```cpp
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

## Fonctions Système

### `signal`
Configure les gestionnaires de signaux.
```cpp
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

### `sigaction`
Examine et modifie l'action associée à un signal.
```cpp
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```

### `lseek`
Déplace le pointeur de lecture/écriture dans un fichier ouvert.
```cpp
off_t lseek(int fd, off_t offset, int whence);
```

### `fstat`
Récupère les informations sur un fichier ouvert.
```cpp
int fstat(int fd, struct stat *statbuf);
```

### `fcntl`
Manipule les descripteurs de fichiers.
```cpp
int fcntl(int fd, int cmd, ... /* arg */ );
```

### `poll`
Surveille plusieurs descripteurs de fichiers pour voir s'ils sont prêts pour I/O.
```cpp
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

Vous pouvez adapter ce modèle selon vos besoins, ajouter des sections supplémentaires si nécessaire, et personnaliser les informations de l'auteur et de la licence.
