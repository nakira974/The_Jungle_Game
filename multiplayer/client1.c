//
// Created by max on 19/11/2020.
//

#include "client1.h"

int iResult;
struct Animal currentAnimal;
struct Player currentPlayer;

SOCKET ConnectSocket = INVALID_SOCKET;


int send_server(char *sendbuf) {
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
}

int close_client(){
    closesocket(ConnectSocket);
    WSACleanup();
    return 1;
}

int __cdecl app_client1(char *srvAdd, char *sendbuffer) {
    WSADATA wsaData;

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    const char *sendbuf = sendbuffer;
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

    // Resolve the server address and port
    iResult = getaddrinfo(srvAdd, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

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
        printf("%s\n","Unable to connect to server!");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    //Envoi le nom du joueur
    printf("%s\n","Initialisation de la partie 0//3...");


    printf("%s\n","Envoi des informations du joueur à l'hôte de la partie...");
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);

    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Player information sent: %ld\n", iResult);

    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    if (iResult > 0)
        printf("Bytes received: %d\n", iResult);

    //LANCEMENT DE LA PARTIE
    do {

        int i;
        float loadingBar;


        GenererEchequier();
        printf("Nom du joueur : %s\n", (char *) &sendbuf);
        currentPlayer.name = (char*) sendbuf;
        //L'INVITE EST l'ENNEMI
        currentPlayer.isEnemy = true;
        playerTab[0]=currentPlayer;

        //On compte le tableau des animaux, pour vérifier que tout est OK
        printf("%s\n","Initialisation de la partie 2//3...");
        for (i = 0; i <15 ; ++i) {
            loadingBar+MULTIPLAYER_LOADING_BAR;
            currentAnimal = animalTab[i];
            printf("chargement... %d", loadingBar," %");
            if (animalTab[15].type == currentAnimal.type){
                printf("%s\n","Initialisation de la partie 3//3...");
                printf("%s\n","Début de la partie ! ");
            } else{
                close_client();
            }
        }
        send_server("Ce qu'on va envoyer avec une boucle While la partie pas finie");

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