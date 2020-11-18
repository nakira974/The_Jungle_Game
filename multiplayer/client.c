//
// Created by max on 09/11/2020.
//

#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
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

#else /* sinon vous êtes sur une plateforme non supportée */

#error not defined for this platform

#endif

static void init(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

static SOCKET create_client()
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }
    return sock;
}

static bool connect2_JungleServer(const char srvAdd[256], const SOCKET *sock, const u_short *port)
{
#ifdef WIN32
    struct hostent *hostinfo = NULL;
    SOCKADDR_IN sin = { 0 }; /* initialise la structure avec des 0 */
    const char *hostname =  srvAdd;

    hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
    if (hostinfo == NULL) /* l'hôte n'existe pas */
    {
        fprintf (stderr, "Unknown host %s.\n", hostname);
        exit(EXIT_FAILURE);
    }

    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr; /* l'add se trouve dans le champ h_addr de la struct hostinfo */
    sin.sin_port = htons(*port); /* on utilise la méthode htons pour le port */
    sin.sin_family = AF_INET;

    if(connect(*sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        perror("connect()");
        exit(errno);
    }
#endif
}

static void send_str(const SOCKET *sock,const char *buffer[1024])
{
#ifdef WIN32
    if(send(*sock, *buffer, strlen(*buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
#endif
}

static char reception_str(const SOCKET *sock)
{
    char buffer[1024];
    int n = 0;

#ifdef WIN32
    if((n = recv(*sock, buffer, sizeof buffer - 1, 0)) < 0)
    {
        perror("recv()");
        exit(errno);
    }

    buffer[n] = '\0';
    return *buffer;
#endif
}

static void close_client(SOCKET *sock)
{
#ifdef WIN32
    closesocket((SOCKET) &sock);
#endif
}