#include <errno.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <utils/log.h>

using namespace std;

static bool pSignaled = false;

void sigHandler(int sig, siginfo_t*, void*)
{
    pSignaled = true;
}

int main(int argc, char**argv)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_sigaction = sigHandler;
    sigaction(SIGINT, &act, NULL);

    pid_t pid = fork();

    if (pid < 0)
    {
        cout << "Error forking" << endl;
        exit(EXIT_FAILURE);
    }

    if (0 == pid)
    {
        cout << "Child lord!" << endl;
        sleep(15);
        exit(EXIT_SUCCESS);
    }

    int status;
    int res = waitpid(pid, &status, 0);
    if (-1 == res)
    {
        char buf[1024];
        cout << "waitpid:" << strerror_r(errno, buf, sizeof(buf)) << endl;
    }

    if (pSignaled)
    {
        kill(pid, SIGINT);
        cout << "killing child ..." << endl;

//        res = waitpid(pid, &status, 0);
//        if (-1 == res)
//        {
//            char buf[1024];
//            cout << "waitpid:" << strerror_r(errno, buf, sizeof(buf)) << endl;
//        }
    }

    if (WIFEXITED(status) or WIFSIGNALED(status))
    {
        if (0 != WTERMSIG(status))
        {
            cout << "Child terminated by signal " << WTERMSIG(status) << endl;
        }
        if (0 != WEXITSTATUS(status))
        {
            cout << "Child exited with status " << WEXITSTATUS(status) << endl;
        }
    }
    return EXIT_SUCCESS;
}


