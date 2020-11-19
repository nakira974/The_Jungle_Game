#include "echiquier.h"
#include "multiplayer/server.h"
#include "multiplayer/client1.h"
#include <pthread.h>

typedef struct th_server {
    pthread_t thread_server;
} th_server;

typedef struct th_client {
    pthread_t thread_client;
} th_client;

static void *server_process(void) {
    init_server();
    app_server();
    end_server();
}

static void *client_proces(const char *srvAdd, const char *playerName) {
    init_client();
    app_client(srvAdd, playerName);
    end_client();
}


int main() {

    struct th_client v_client;
    struct th_server v_server;
    char *srvAdd = "192.168.1.1";
    char *playerName = "Maxime";
    int ret;
    app_client1("localhost");


    /*
    //GenererEchequier();
    printf("Creation du thread serveur !\n");
    ret = pthread_create(&v_server.thread_server, NULL,server_process() , NULL);
    if (!ret) {
        printf("Creation du thread client !\n");
        ret = pthread_create(&v_client.thread_client, NULL,client_proces(srvAdd, playerName), NULL);
    }
    if (ret) {
        fprintf(stderr, "%s", strerror(ret));
    } else {
        fprintf(stderr, "%s", strerror(ret));
    }

    pthread_join(v_client.thread_client, NULL);
    pthread_join(v_server.thread_server, NULL);

    return EXIT_SUCCESS;

     */



}