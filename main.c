#include "echequier.h"
#include "multijoueur.h"
#include <windowsx.h>

void main() {


    char add[32]="127.0.0.1";
    int srvPort=32666;
    SOCKET client1;
    client1=Initiate_ClientSock(add, srvPort);


    //GenererEchequier();

}