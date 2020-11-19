//
// Created by max on 09/11/2020.
//

#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "client.h"
#include "../echiquier.h"

#ifdef WIN32 /*  Windows */

#include <winsock2.h>

#elif defined (linux) /*  Linux */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else /* plateforme non supportée */

#error not defined for this platform

#endif

void init_server(void) {
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0) {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

void end_server(void) {
#ifdef WIN32
    WSACleanup();
#endif
}


static void close_server(const SOCKET *sock) {
#ifdef WIN32
    closesocket((SOCKET) &sock);
#endif
}

static void clear_clients(Client *clients, int actual) {
#ifdef WIN32

    int i = 0;
    for (i = 0; i < actual; i++) {
        closesocket(clients[i].sock);
    }
#endif
}

static void remove_client(Client *clients, int to_remove, int *actual) {
#ifdef WIN32
    /* On enlève le client du tableau */
    memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
    /* number client - 1 */
    (*actual)--;
#endif
}


static int init_connection(void) {
#ifdef WIN32
    SOCKET sock = socket(AF_INET, SOCK_STREAM, TCP);//tcp
    SOCKADDR_IN sin = {0};

    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }

    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORT);
    sin.sin_family = AF_INET;

    if (bind(sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR) {
        perror("bind()");
        exit(errno);
    }

    if (listen(sock, MAX_CLIENTS) == SOCKET_ERROR) {
        perror("listen()");
        exit(errno);
    }

    return sock;
#endif
}

static int read_client(SOCKET sock, char *buffer) {
#ifdef WIN32
    int n = 0;

    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        /* if recv error we disonnect the client */
        n = 0;
    }

    buffer[n] = 0;

    return n;
#endif
}

static void write_client(SOCKET sock, const char *buffer) {
#ifdef WIN32
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
#endif
}

void app_server(void) {
#ifdef WIN32
    SOCKET sock = init_connection();//Création du socket serveur
    char buffer[BUF_SIZE];//Création du buffer
    /* index */
    int actual = 0;
    int max = sock;
    /*tableau des clients */
    Client clients[MAX_CLIENTS];

    fd_set rdfs;//fixed buffer size, un sorte de const

    while (1) {
        int i = 0;
        FD_ZERO(&rdfs);
        /*
         * FD_ZERO() : source : https://www.man7.org/linux/man-pages/man3/FD_SET.3.html
         *  Cette macro efface (supprime tous les descripteurs de fichiers).
         *  Il devrait être utilisé comme première étape dans l’initialisation d’un fichier
         *  descripteur.
         */

        /* on attache notre buffer à STDIN_FILENO */
        FD_SET(STDIN_FILENO, &rdfs);
        /*
         * FD_SET() : source : https://www.man7.org/linux/man-pages/man3/FD_SET.3.html
              Cette macro ajoute le descripteur de fichier fd à définir.  Ajout d’un fichier
              descripteur qui est déjà présent dans l’ensemble est un no-op, et
              ne produit pas d’erreur.
         */

        /* ajoute la connexion au socket */
        FD_SET(sock, &rdfs);

        /* on ajoute tous les sockets clients */
        for (i = 0; i < actual; i++) {
            FD_SET(clients[i].sock, &rdfs);
        }

        if (select(max + 1, &rdfs, NULL, NULL, NULL) == -1) {
            perror("select()");
            exit(errno);
        }

        /* entrée standard : i.e keyboard */
        if (FD_ISSET(STDIN_FILENO, &rdfs)) {
            /* stop le process lorsque saisi clavier */
            break;
        } else if (FD_ISSET(sock, &rdfs)) {

            /*select () modifie le contenu des ensembles en fonction des
              règles décrites ci-dessous. Après avoir appelé select (), le FD_ISSET ()
              macro peut être utilisée pour tester si un descripteur de fichier est toujours
              présent dans un ensemble. FD_ISSET () renvoie une valeur différente de zéro si le fichier
              le descripteur fd est présent dans l'ensemble, et zéro s'il ne l'est pas.
        */
            /* new client */
            SOCKADDR_IN csin = {0};
            size_t sinsize = sizeof csin;
            int csock = accept(sock, (SOCKADDR *) &csin, &sinsize);
            if (csock == SOCKET_ERROR) {
                perror("accept()");
                continue;
            }

            /* après sa connexion le client donne son nom */
            if (read_client(csock, buffer) == -1) {
                /* disconnected */
                continue;
            }

            /* nouveau fd max ? */
            max = csock > max ? csock : max;

            FD_SET(csock, &rdfs);

            Client c = {csock};
            strncpy(c.name, buffer, BUF_SIZE - 1);
            clients[actual] = c;
            actual++;
        } else {
            int i = 0;
            for (i = 0; i < actual; i++) {
                /* a client is talking */
                if (FD_ISSET(clients[i].sock, &rdfs)) {
                    Client client = clients[i];
                    int c = read_client(clients[i].sock, buffer);
                    /* client disconnected */
                    if (c == 0) {// SI LE CLIENT EST DECONNECTE
                        closesocket(clients[i].sock);
                        remove_client(clients, i, &actual);
                        strncpy(buffer, client.name, BUF_SIZE - 1);
                        strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                        //do something
                    } else {
                        //do something
                    }
                    break;
                }
            }
        }
    }
    clear_clients(clients, actual);
    close_server((const SOCKET *) sock);
#endif
}
