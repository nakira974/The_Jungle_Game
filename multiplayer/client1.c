//
// Created by max on 19/11/2020.
//

#include "client1.h"

int iResult;
struct Animal currentAnimal;
struct Player currentPlayer;

SOCKET ConnectSocket = INVALID_SOCKET;

const char *get_clientAddress_4(struct addrinfo *sa) {
    struct sockaddr_in *pV4Addr = (struct sockaddr_in *) &ConnectSocket;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char *str;
    str = (char *) inet_ntoa(ipAddr);
    return (const char *) str;
}

const char *get_clientAddress_6(struct addrinfo *sa) {
    struct sockaddr_in6 *pV6Addr = (struct sockaddr_in6 *) &ConnectSocket;
    struct in6_addr *ipAddr = &pV6Addr->sin6_addr;
    char *str;
    str = (char *) ipAddr;
    return (const char *) str;
}


int send_server(const char *sendbuf) {
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
}

int close_client() {
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
}

int __cdecl app_client1() {
    WSADATA wsaData;

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    const char *sendbuf = playerTab[0].name;
    // Validate the parameters


    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *pa = &hints;
    printf("Adresse du client : %s\n", (const char *) get_clientAddress_4(pa));

    // Resolve the server address and port
    iResult = getaddrinfo((const char *) "127.0.0.1", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    printf("Host information has been completed... \n");
    printf("Listening on %s,: %s\n", "127.0.0.1", DEFAULT_PORT);

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }



        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int) ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    //Envoi le nom du joueur
    printf("Initialisation de la partie 0/3... \n");


    printf("Envoi des informations du joueur à l'hôte de la partie... \n");
    sendbuf = playerTab[0].name;
    //LE CLIENT EST L'ENNEMY
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);

    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }


    printf("Player information sent: %ld\n", iResult);

    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
        printf("Host information received: %d\n", iResult);
        //ON MET LE NOM DE l'HOTE DANS LE TABLEAU DES JOUEURS
        for (int q = 0; q < 150; q++) {
            playerTab[1].name[q] = recvbuf[q];
        }
        //playerTab[1].name = recvbuf;
    }

    //LANCEMENT DE LA PARTIE
    do {

        afficherEchiquier();
        int i;
        float loadingBar;

        printf("Nom du joueur : %s\n", sendbuf);
        for (int q = 0; q < 150; q++) {
            currentPlayer.name[q] = sendbuf[q];
        }
        //currentPlayer.name = sendbuf;
        //L'INVITE EST l'ENNEMI
        currentPlayer.isEnemy = true;

        //On compte le tableau des animaux, pour vérifier que tout est OK
        printf("Initialisation de la partie 2/3... %s\n");
        for (i = 0; i < 15; ++i) {
            loadingBar + MULTIPLAYER_LOADING_BAR;
            currentAnimal = animalTab[i];
            printf("chargement... %d", loadingBar, " %");
            if (animalTab[15].type == currentAnimal.type) {
                printf("Initialisation de la partie 3/3... %s\n");
                printf("Début de la partie ! ");
            } else {
                close_client();
            }
        }
        send_server(MULTIPLAYER_EXIT);

    } while (sendbuf != MULTIPLAYER_EXIT);



    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}