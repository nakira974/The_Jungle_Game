//
// Created by max on 19/11/2020.
//

#ifndef PROJETJUNGLE_CLIENT1_H
#define PROJETJUNGLE_CLIENT1_H

#define WIN32_LEAN_AND_MEAN

#include "../echiquier.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


int send_server(const char *sendbuf);

int __cdecl app_client1();

#endif //PROJETJUNGLE_CLIENT1_H
