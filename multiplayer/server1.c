//
// Created by max on 19/11/2020.
//

#include "server1.h"


int iResult;
WSADATA wsaData;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

struct addrinfo *result = NULL;
struct addrinfo hints;
struct Animal currentAnimal;
struct Player currentPlayer;

int iSendResult;
char *recvbuf;
// #pragma comment (lib, "Mswsock.lib")
int recvbuflen = DEFAULT_BUFLEN;

const char *get_serverAddress(struct addrinfo *sa) {
    struct sockaddr_in *pV4Addr = (struct sockaddr_in *) &ListenSocket;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char *str;
    str = (char *) inet_ntoa(ipAddr);
    return (const char *) str;
}


int close_server() {
    closesocket(ListenSocket);
    WSACleanup();
    return 1;
}

int send_client(const char *sendbuf) {
    iResult = send(ClientSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
}

int create_socket() {
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
}

int __cdecl app_serv1(void) {
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *pa = &hints;

    printf("Adresse du serveur : %s\n", (const char *) get_serverAddress(pa));

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server

    create_socket();

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int) result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    printf("%s\n", "Accepting client request...");
    ClientSocket = accept(ListenSocket, NULL, NULL);
    /*if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }*/

    // No longer need server socket
    //closesocket(ListenSocket);

    // Recevoir jusqu’à ce que le pair arrête la connexion
    do {

        //RECEPTION DES INFOS DU JOUEUR DISTANT
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Player information received: %d\n", iResult);
            for (int q = 0; q < 150; q++) {
                playerTab[1].name[q] = recvbuf[q];
            }
            //playerTab[1].name = recvbuf;

            printf("Ennemy name is : %s\n", playerTab[1].name);


            //buffer back to the sender
            //ENVOI DES INFOS DE L'HOTE, LANCEMENT DE LA PARTIE

            //ON ENVOIE LE NOM DE l'HOTE
            recvbuf = playerTab[0].name;

            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult > 0) {
                printf("Host information sent: %d\n", iSendResult);

            }

            //SI L'ENVOIE DES INFOS DE L'HOTE ECHOUE ALORS ON FERME TOUT
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            int i;
            float loadingBar;

            printf("Initialisation de la partie 0/3... \n");
            printf("Nom du joueur : %s\n", (char *) &recvbuf);
            //L'INVITE EST l'ENNEMI
            afficherEchiquier();

            //On compte le tableau des animaux, pour vérifier que tout est OK
            printf("Initialisation de la partie 2/3... \n");
            for (i = 0; i < 15; ++i) {
                loadingBar + MULTIPLAYER_LOADING_BAR;
                currentAnimal = animalTab[i];
                printf("chargement... %d", loadingBar, " %");
                if (animalTab[15].type == currentAnimal.type) {
                    printf("Initialisation de la partie 3/3... \n");
                    printf("Début de la partie ! ");
                } else {
                    close_server();
                }
            }


        } else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (recvbuf != MULTIPLAYER_EXIT);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
