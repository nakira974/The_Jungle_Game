//
// Created by max on 08/10/2020.
//

/*
	TCP socket
*/

#include "multijoueur.h"
#include<stdio.h>
#include<winsock2.h>
#include <stdbool.h>

#pragma comment(lib, "ws2_32.lib") //Winsock Library

deftype struct ClientSock{
    char clientAdd[32];
    int clientPort;
    SOCKET socket;
    bool isLocalSrv;
    char connectedAdd[32];
    int connectedPort;
    char loopback[32];
}ClientSock;

deftype struct ServerSock{
    char serverAdd[32];
    int serverPort;
    time_t utc;
    SOCKET socket;
    char dataFrame[2048];
    int nbClient;
    struct ClientSock clientList[2];


}ServerSock;

ClientSock set_to_clientList(struct ClientSock sock){
    struct ServerSock serverSock;
    for(i=0 ; i<2 ; i++){
        if (serverSock.clientList[i]== NULL){
            serverSock.clientList[i]=sock;
        }
    }
}

int CloseClient(SOCKET s){
    closesocket(s);
    WSACleanup();
    return 0;
}

SOCKET Initiate_ClientSock(char srvAdd[32], int srvPort) {

    {
        WSADATA wsa;
        ClientSock *p_s;// pointeur sur la struct client
        p_s s;//Instanciation de notre petite socket
        SOCKET clientSocket;
        sockaddr_in server;

        printf("\nInitilisation du Winsock...");
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            printf("La tentative de connexion au serveur a échouée.\n Code d'erreur : %d", WSAGetLastError());
            return 1;
        }

        printf("Initialised.\n");

        //Create a socket
        //0 == tcp
        if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
            printf("Impossible de créer le socket :  : %d", WSAGetLastError());
        }

        printf("Socket créé.\n");
        &s.socket= clientSocket;
        &s.connectedAdd[32]=srvAdd[32];
        &s.connectedPort=srvPort;


        server.sin_addr.s_addr = inet_addr(srvAdd);
        server.sin_family = AF_INET;
        server.sin_port = htons(srvPort);

        //Connect to remote server
        if (connect(clientSocket, (struct sockaddr *) &server, sizeof(server)) < 0) {
            puts("Erreur de connexion");
            return 1;
        }

        puts("Connecté au serveur");

        return 0;
        return s.socket;
    }
}

int Send(SOCKET s, char message[]){
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");
    return 0;
}


int Initiate_LocalServer() {

}