//
// Created by max on 09/11/2020.
//

#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "server.h"

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


static void init(void) {
#ifdef WIN32
    //initialise une DLL permettant d'utiliser les sockets et pour libérer cette même DLL.
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);

    if (err < 0) {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void) {
#ifdef WIN32
    WSACleanup();
#endif
}

static SOCKET create_client() {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("socket()");
        exit(errno);
    }
    return sock;
}

static bool connect2_JungleServer(const char srvAdd[ADD_SIZE], const SOCKET *sock) {
#ifdef WIN32
    struct hostent *hostinfo = NULL;
    SOCKADDR_IN sin = {0}; /* initialise la structure avec des 0 */
    const char *hostname = srvAdd;

    hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
    if (hostinfo == NULL) /* l'hôte n'existe pas */
    {
        fprintf(stderr, "Unknown host %s.\n", hostname);
        exit(EXIT_FAILURE);
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'add se trouve dans le champ h_addr de la struct hostinfo */
    sin.sin_port = htons(PORT); /* on utilise la méthode htons pour le port */
    sin.sin_family = AF_INET;

    if (connect(*sock, (SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        perror("connect()");
        exit(errno);
    }
#endif
}


static void close_client(SOCKET *sock) {
#ifdef WIN32
    closesocket((SOCKET) &sock);
#endif
}

static int read_server(SOCKET sock, char *buffer) {
#ifdef WIN32
    int n = 0;

    if ((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0) {
        perror("recv()");
        exit(errno);
    }

    buffer[n] = 0;

    return n;
#endif
}

static void write_server(SOCKET sock, const char *buffer) {
#ifdef WIN32

    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        perror("send()");
        exit(errno);
    }
#endif
}

static void app_client(const char *srvAdd, const char *playerName) {
#ifdef WIN32
    SOCKET *sock = (SOCKET *) create_client();
    char buffer[BUF_SIZE];
    fd_set rdfs;// fixed size buffer
    connect2_JungleServer(srvAdd, sock);
    write_server((SOCKET) sock, playerName);

    while (1) {
        FD_ZERO(&rdfs);
        /*
         * FD_ZERO() : source : https://www.man7.org/linux/man-pages/man3/FD_SET.3.html
         *  Cette macro efface (supprime tous les descripteurs de fichiers).
         *  Il devrait être utilisé comme première étape dans l’initialisation d’un fichier
         *  descripteur.
         */


        /* add STDIN_FILENO */
        FD_SET(STDIN_FILENO, &rdfs);
        /*
         * FD_SET() : source : https://www.man7.org/linux/man-pages/man3/FD_SET.3.html
              Cette macro ajoute le descripteur de fichier fd à définir.  Ajout d’un fichier
              descripteur qui est déjà présent dans l’ensemble est un no-op, et
              ne produit pas d’erreur.
         */

        /* add the socket */
        FD_SET(sock, &rdfs);

        if (select((int) (sock + 1), &rdfs, NULL, NULL, NULL) == -1) {
            perror("select()");
            exit(errno);
        }

        /* entrée standard : i.e keyboard */
        if (FD_ISSET(STDIN_FILENO, &rdfs))
            /*
            * select () modifie le contenu des ensembles en fonction des
                  règles décrites ci-dessous. Après avoir appelé select (), le FD_ISSET ()
                  macro peut être utilisée pour tester si un descripteur de fichier est toujours
                  présent dans un ensemble. FD_ISSET () renvoie une valeur différente de zéro si le fichier
                  le descripteur fd est présent dans l'ensemble, et zéro s'il ne l'est pas.
            */
        {
            fgets(buffer, BUF_SIZE - 1, stdin);
            {
                char *p = NULL;
                p = strstr(buffer, "\n");
                if (p != NULL) {
                    *p = 0;
                } else {
                    /* fclean */
                    buffer[BUF_SIZE - 1] = 0;
                }
            }
            write_server((SOCKET) sock, buffer);
        } else if (FD_ISSET(sock, &rdfs)) {
            int n = read_server((SOCKET) sock, buffer);
            /* server down */
            if (n == 0) {
                printf("Server disconnected !\n");
                break;
            }
            puts(buffer);
        }
    }

    close_client(sock);
#endif
}

