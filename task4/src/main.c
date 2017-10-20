#include "err_check.h"

#include <stdio.h>          // printf
#include <pthread.h>        // pthread_*
#include <stdlib.h>         // exit
#include <unistd.h>         // sleep

#define SECONDS_TO_WAIT_BEFORE_CANCELLING 3
#define SECONDS_BETWEEN_MESSAGES 1
#define IGNORE_STATUS NULL

void *Run(void *);

int main() {
    pthread_t tid;
    int code = pthread_create(&tid, NULL, Run, NULL);
    ExitIfNonZeroWithMessage(code, "pthread_create");

    sleep(SECONDS_TO_WAIT_BEFORE_CANCELLING);

    code = pthread_cancel(tid);
    ExitIfNonZeroWithMessage(code, "pthread_cancel");

    code = pthread_join(tid, IGNORE_STATUS);
    ExitIfNonZeroWithMessage(code, "pthread_join");

    exit(EXIT_SUCCESS);
}

void *Run(void *ignored) {
    int count;
    for (count = 1; ; ++count) {
        pthread_testcancel();
        printf("Child counts \"%d\"\n", count);
        sleep(SECONDS_BETWEEN_MESSAGES);
    }
    pthread_exit(NULL);
}
