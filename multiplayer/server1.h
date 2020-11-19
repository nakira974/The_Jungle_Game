//
// Created by max on 19/11/2020.
//

#ifndef PROJETJUNGLE_SERVER1_H
#define PROJETJUNGLE_SERVER1_H
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

int __cdecl app_serv1(void);

int send_client(char *sendbuf);

#endif //PROJETJUNGLE_SERVER1_H
