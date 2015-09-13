#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printsidgid(const char* scope)
{
    static const int SELF = 0;
    pid_t sid = getsid(SELF);
    gid_t gid = getgid();
    gid_t egid = getegid();

    fprintf(stdout, "%-10s: sid %d, gid %d, egid %d\n", scope, sid, gid, egid);
}

void demonize(void)
{
    int pid = fork();
    if (pid < 0)
    {
        perror("fork");
    }

    if (pid > 0)
    {
        printsidgid("parent");
    }
    else
    {
        printsidgid("child");
        pid_t sid = setsid();
        if (sid == -1)
        {
            perror("child setsid");
        }
        else
        {
            printsidgid("child setsid");
        }

        fprintf(stdout, "lord, demon stdout"); // demon output may not be seen when screen closed

        int nullfd = open("/dev/null", O_RDWR, 0);
        dup2(nullfd, STDIN_FILENO);
        dup2(nullfd, STDOUT_FILENO);
        dup2(nullfd, STDERR_FILENO);
        close(nullfd);

        fprintf(stdout, "lord, you should not see this");
        fprintf(stderr, "lord, you should not see this");
    }
}


int main(int argc, char** argv)
{
    demonize();
    return EXIT_SUCCESS;
}


