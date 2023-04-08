#ifndef TAS_NET
#define TAS_NET

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "tas_vector.h"
#include "tas_string.h"

//TODO: well... everything

typedef struct Socket {
    int socketId;
    struct sockaddr_in address;
} Socket;

typedef struct Client {
    Socket socket;
} Client;

#define MAX_CLIENTS 32

VEC(Socket)

typedef struct Server {
    Vec_Socket clientSockets;
    Socket socket;
} Server;

Server netServerNew(uint16_t port) {
    Server s;
    if ((s.socket.socketId = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error_exit("netServerNew error: Cannot create socket\n");
    }

    int flags = fcntl(s.socket.socketId, F_GETFL, 0);
    if(flags == -1) {
        error_exit("netServerNew error: failed to make socket non-blocking\n");
    }
    flags |= O_NONBLOCK;
    if(fcntl(s.socket.socketId, F_SETFL, flags) != 0) {
        error_exit("netServerNew error: failed to make socket non-blocking\n");
    }

    int opt = 1;

    if (setsockopt(s.socket.socketId, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        error_exit("netServerNew error: Cannot set the server options\n");
    }

    memset(&s.socket.address, 0, sizeof(s.socket.address));

    s.socket.address.sin_family = AF_INET;
    s.socket.address.sin_addr.s_addr = INADDR_ANY;
    s.socket.address.sin_port = htons(port);

    if(bind(s.socket.socketId, (struct sockaddr *)&s.socket.address, sizeof(s.socket.address)) < 0) {
        error_exit("netServerNew error: cannot bind\n");
    }

    s.clientSockets = vecNew_Socket();

    if(listen(s.socket.socketId, MAX_CLIENTS) < 0) {
        error_exit("netServerNew error: cannot listen\n");
    }

    #ifdef DEBUG
        printf("Started to listen to port: %hu\n", port);
    #endif

    return s;
}

void netServerAcceptAvailableClient(Server * s) {
    Socket client;
    socklen_t size = sizeof(client.address);
    client.socketId = accept(
        s->socket.socketId, 
        (struct sockaddr *)&client.address, 
        &size);
    
    if(client.socketId == -1) return;
    
    int flags = fcntl(client.socketId, F_GETFL, 0);
    if(flags == -1) {
        error_exit("netServerAcceptAvailableClient error: failed to make socket non-blocking\n");
    }
    flags |= O_NONBLOCK;
    if(fcntl(client.socketId, F_SETFL, flags) != 0) {
        error_exit("netServerAcceptAvailableClient error: failed to make socket non-blocking\n");
    }
    
    if(client.socketId > -1) {
        vecPush_Socket(&s->clientSockets, client);

        #ifdef DEBUG
            printf("Client connected!\n");
        #endif
    }
}

typedef struct ServerMessage {
    size_t client_number;
    Str msg;
} ServerMessage;

void netServerMessageDelete(ServerMessage * m) {
    strDelete(&m->msg);
}

bool netServerSendMessage(Server s, ServerMessage m) {
    if(m.client_number >= s.clientSockets.length) return false;

    return !(send(
        s.clientSockets.data[m.client_number].socketId,
        m.msg.data,
        m.msg.length,
        0
    ) == -1);
}

VEC(ServerMessage)

#define MAX_DATA_SIZE 1024

Vec_ServerMessage netServerGetMessages(Server * s) {
    Vec_ServerMessage res = vecNew_ServerMessage();

    char buffer[MAX_DATA_SIZE];

    memset(buffer, 0, MAX_DATA_SIZE);
    
    for (size_t i = 0; i < s->clientSockets.length; i++) {
        int connect_status = recv(s->clientSockets.data[i].socketId, buffer, MAX_DATA_SIZE - 1, 0);
        if(connect_status > 0) {
            ServerMessage m;
            m.msg = strNew(buffer);
            m.client_number = i;
            vecPush_ServerMessage(&res, m);
        } else if (connect_status == 0) {
            vecEraseOne_Socket(&s->clientSockets, i);

            i--;

            #ifdef DEBUG
                printf("Client disconnected!\n");
            #endif
        }
    }
    
    return res;
}

void netServerDelete(Server * s) {
    vecDelete_Socket(&s->clientSockets);
}

#endif