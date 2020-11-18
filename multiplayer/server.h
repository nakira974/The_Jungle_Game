//
// Created by max on 09/11/2020.
//

#ifndef PROJETJUNGLE_SERVER_H
#define PROJETJUNGLE_SERVER_H

#include "client.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
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

#define CRLF     "\r\n"
#define PORT     1998
#define BUF_SIZE 1024
#define ADD_SIZE 256
#define MAX_CLIENTS 1
#define TCP 0


static void init(void);

static void end(void);

static void close_server(const SOCKET *sock);

static void write_client(SOCKET sock, const char *buffer);

static void clear_clients(Client *clients, int actual);

static void remove_client(Client *clients, int to_remove, int *actual);

static int init_connection(void);

static int read_client(SOCKET sock, char *buffer);

static void app_server(void);


#endif //PROJETJUNGLE_SERVER_H
