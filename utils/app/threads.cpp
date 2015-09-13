#include <ctype.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils/log.h"

using namespace std;

struct ThreadInfo
{
    pthread_t threadId;
    int threadNum;
    char* str;
};

static void* threadFunc(void* arg)
{
    ThreadInfo* info = reinterpret_cast<ThreadInfo*> (arg);
    char* newStr = strdup(info->str);
    printf("Thread %d: top of stack near %p; argv_string=%s\n", info->threadNum, &newStr, info->str);

    for(char* p = newStr; *p != '\0'; p++)
    {
        *p = toupper(*p);
    }
    return newStr;
}

int main(int argc, char** argv)
{
    int opt, stackSize;
    while ((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch (opt)
        {
        case 's':
            stackSize = strtoul(optarg, NULL, 0);
            break;

        default:
            fprintf(stderr, "Usage: %s [-s stackSize] args...", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    int numThreads = argc - optind;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stackSize);

    ThreadInfo* thrInfo = static_cast<ThreadInfo*> (calloc(numThreads, sizeof(ThreadInfo)));
    for (int ind = 0; ind < numThreads; ind++)
    {
        thrInfo[ind].threadNum = ind + 1;
        thrInfo[ind].str = argv[optind + ind];
        int res = pthread_create(&thrInfo[ind].threadId, &attr, threadFunc, &thrInfo[ind]);
        if (res != 0)
        {
            Log::errorNo("pthread_create");
            Log::fatalExit("Thread create failed!");
        }
    }

    pthread_attr_destroy(&attr);

    void* str;
    for (int ind = 0; ind < numThreads; ind++)
    {
        int res = pthread_join(thrInfo[ind].threadId, &str);
        if (res != 0)
        {
            Log::errorNo("pthread_join");
            Log::fatalExit("Thread join failed!");
        }

        printf("Joined with thread %d; returned value was %s\n", thrInfo[ind].threadNum, (char*) str);
        free(str);
    }

    free(thrInfo);
    return EXIT_SUCCESS;
}
