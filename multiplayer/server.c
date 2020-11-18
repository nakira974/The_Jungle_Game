//
// Created by max on 09/11/2020.
//

#include "server.h"
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

static SOCKET create_server()
{
#ifdef WIN32
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        perror("socket()");
        exit(errno);
    }
    return sock;
#endif
}

static void interface_up(const SOCKET *sock, u_short port)
{
#ifdef WIN32

    SOCKADDR_IN sin = { 0 };
    sin.sin_addr.s_addr = htonl(INADDR_ANY); /*  n'importe quelle adresse */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    if(bind ((SOCKET) &sock, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR) // on lie notre socket à notre interface de connexion.
    {
        perror("bind()");
        exit(errno);
    }
#endif
}

static void listen2_Jungle(const SOCKET *sock)
{
#ifdef WIN32
    if(listen((SOCKET) &sock, 1) == SOCKET_ERROR) // LIMITE DE 1 CONNEXION MAXIMUM
    {
        perror("listen()");
        exit(errno);
    }
#endif
}

static SOCKET accept_client(const SOCKET *sock)
{
#ifdef WIN32
    SOCKADDR_IN csin = { 0 };
    SOCKET csock;

    int sinsize = sizeof csin;

    csock = accept((SOCKET) &sock, (SOCKADDR *)&csin, &sinsize);

    if(csock == INVALID_SOCKET)
    {
        perror("accept()");
        exit(errno);
    }
    return csock;
#endif
}

static void close_server(const SOCKET *sock, const SOCKET *csock)
{
#ifdef WIN32
    closesocket((SOCKET) &sock);
    closesocket((SOCKET) &csock);
#endif
}