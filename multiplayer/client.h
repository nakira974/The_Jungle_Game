//
// Created by max on 09/11/2020.
//

#ifndef PROJETJUNGLE_CLIENT_H
#define PROJETJUNGLE_CLIENT_H

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

#else /* plateforme non supportée */

#error not defined for this platform

#endif

#define CRLF     "\r\n"
#define PORT     1998
#define BUF_SIZE 1024
#define ADD_SIZE 256

static void init(void);

static void end(void);

static SOCKET create_client();

static bool connect2_JungleServer(const char srvAdd[256], const SOCKET *sock);

static int send_str(const SOCKET *sock, const char *buffer[BUF_SIZE]);

static char reception_str(const SOCKET *sock);

static void close_client(SOCKET *sock);


typedef struct {
    SOCKET sock;
    char name[BUF_SIZE];
} Client;


#endif //PROJETJUNGLE_CLIENT_H
