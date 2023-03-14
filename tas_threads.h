#ifndef TAS_THREADS
#define TAS_THREADS

#include <pthread.h>
#include "tas_commonmacro.h"
#include "tas_vector.h"

typedef struct ThreadInfo {
    size_t threadIndex;
    size_t threadCount;
} ThreadInfo;

typedef struct Thread {
    pthread_t threadId;
    ThreadInfo info;
} Thread;

VEC(Thread)

typedef struct ThreadPool {
    Vec_Thread threads;
    size_t threadCount;
} ThreadPool;

ThreadPool tpNew(size_t threadCount, void*(*func)(void *)) {
    ThreadPool tp;
    tp.threads = vecNew_Thread();
    tp.threadCount = threadCount;

    for (size_t i = 0; i < threadCount; i++) {
        Thread t = {-1, {i, threadCount}};
        vecPush_Thread(&tp.threads, t);
        
        pthread_create(&tp.threads.data[i].threadId, NULL, func, &tp.threads.data[i].info);
        
        // printf(
        //     "Thread \"%lu\" (%lu of %lu) created;\n", 
        //     tp.threads.data[i].threadId, 
        //     tp.threads.data[i].info.threadIndex,
        //     tp.threads.data[i].info.threadCount
        // );
    }

    return tp;
}

void tpJoin(ThreadPool * tp) {
    for (size_t i = 0; i < tp->threadCount; i++) {
        pthread_join(tp->threads.data[i].threadId, NULL);
        // printf(
        //     "Thread \"%lu\" (%lu of %lu) joined;\n", 
        //     tp->threads.data[i].threadId, 
        //     tp->threads.data[i].info.threadIndex,
        //     tp->threads.data[i].info.threadCount
        // );
    }
    
    tp->threadCount = 0;
    vecDelete_Thread(&tp->threads);
}

#endif