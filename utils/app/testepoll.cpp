#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils/sleep.h"
#include "utils/log.h"

using namespace utils;

int sWrite(int fd, char* buf, int count)
{
    int nwrittern, totlen = 0;

    while (totlen != count)
    {
        nwrittern = write(fd, buf, count - totlen);
        if (nwrittern == 0)
            return totlen;
        if (nwrittern == -1)
            return -1;
        totlen += nwrittern;
        buf += nwrittern;
    }
    return nwrittern;
}

static void* threadFunc(void* arg)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5601);
    inet_aton("127.0.0.1", &sa.sin_addr);
    int res = connect(s, (sockaddr*)&sa, sizeof(sa));
    if (-1 == res)
    {
        Log::errorNo("connect");
        return strdup("Lord, connect failed!");
    }

    char msg[] = "Lord, in the socket";
    int nwrite = sWrite(s, msg, sizeof(msg));
    if (nwrite == -1)
    {
        return strdup("Lord, write failed!");
    }
    else if (nwrite < sizeof(msg))
    {
        Log::debug("write:%d", nwrite);
        return strdup("Lord, write less");
    }

    return strdup("Lord, success");
}

int main(int argc, char** argv)
{
    int on = 1;
    int s = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(5601);
    inet_aton("127.0.0.1", &sa.sin_addr);
    bind(s, (sockaddr*)&sa, sizeof(sa));
    listen(s, 511);

    pthread_t pid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&pid, &attr, threadFunc, NULL);
    pthread_attr_destroy(&attr);

    ns_sleep(SECOND_IN_NS * 2);

    sockaddr_in sacli;
    socklen_t salen = sizeof(sacli);
    accept(s, (sockaddr*)&sa, &salen);

    void* status = NULL;
    pthread_join(pid, &status);
    if (NULL != status)
    {
        Log::debug("Main: from client:%s", status);
        free(status);
    }
    return EXIT_SUCCESS;
}

