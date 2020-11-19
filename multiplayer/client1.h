//
// Created by max on 19/11/2020.
//

#ifndef PROJETJUNGLE_CLIENT1_H
#define PROJETJUNGLE_CLIENT1_H

#include "client1.h"
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

int send_server(char *sendbuf);

int __cdecl app_client1(char *srvAdd, char *sendbuffer);

#endif //PROJETJUNGLE_CLIENT1_H
