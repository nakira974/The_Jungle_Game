//
// Created by max on 19/11/2020.
//

#ifndef PROJETJUNGLE_SERVER1_H
#define PROJETJUNGLE_SERVER1_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define BUFFER_SIZE 1024
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "8888"

#include "../echiquier.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")


int __cdecl app_serv1(void);

int send_client(char *sendbuf);

#endif //PROJETJUNGLE_SERVER1_H
