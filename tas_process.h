#ifndef TAS_PROCESS
#define TAS_PROCESS

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "tas_string.h"
#include "tas_commonmacro.h"

typedef struct Args {
    int argc;
    char **argv;
} Args;

#define PROCESS_CALLBACK(NAME) void NAME (Args * args)

void processFork(void(*parentCallback)(Args *), void(*childCallback)(Args *), Args * args) {
    pid_t processId = fork();

    if(processId < 0) { // Error creating process
        error_exit("Cannot create child process");
    } else if(processId == 0) { // Child process
        if (childCallback) (*childCallback)(args);
    } else { // Parent process
        if(parentCallback) (*parentCallback)(args);
    }
}

#define BUFFER_SIZE_FOR_PIPE_READING 1024

Str processOutToStr(char * command) {
    char buf[BUFFER_SIZE_FOR_PIPE_READING];

    FILE *fp = popen(command, "r");
    
    if(fp == NULL) {
        error_exit("processOutToStr error: cannot open process\n")
    }

    Str res = strNew("");

    while(fgets(buf, BUFFER_SIZE_FOR_PIPE_READING, fp) != NULL) {
        strAppendCStr(&res, buf);
        strPush(&res, '\n');
    }

    res.length--;
    res.data[res.length] = 0;

    pclose(fp);

    return res;
}

#endif