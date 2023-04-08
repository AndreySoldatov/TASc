#define DEBUG

#include "tas_net.h"

int main() {
    Server s = netServerNew(8080);

    while(1) {
        netServerAcceptAvailableClient(&s);

        Vec_ServerMessage messages = netServerGetMessages(&s);

        for (size_t i = 0; i < messages.length; i++) {
            strPrintln(messages.data[i].msg);
            netServerMessageDelete(&messages.data[i]);
        }

        vecDelete_ServerMessage(&messages);

        sleep(1);
    }
}