//
// Created by max on 08/10/2020.
//
#include <winsock.h>
#ifndef PROJETJUNGLE_MULTIJOUEUR_H
#define PROJETJUNGLE_MULTIJOUEUR_H


SOCKET Initiate_ClientSock(char srvAdd[32], int srvPort);
int Send(SOCKET s, char message[]);
int Close(SOCKET s);
#endif //PROJETJUNGLE_MULTIJOUEUR_H
