//
// Created by max on 09/11/2020.
//

#ifndef PROJETJUNGLE_SERVER_H
#define PROJETJUNGLE_SERVER_H

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

static SOCKET create_server();
static void interface_up(const SOCKET *sock, u_short port);
static void listen2_Jungle(const SOCKET *sock);
static SOCKET accept_client(const SOCKET *sock);
static void close_server(const SOCKET *sock, const SOCKET *csock);



#endif //PROJETJUNGLE_SERVER_H
