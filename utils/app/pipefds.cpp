#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int pipefds[2];

    int res = pipe(pipefds);
    if (-1 == res)
    {
        exit(EXIT_FAILURE);
    }

    printf("STDIN %d, STDOUT %d, STDERR %d\n", STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
    printf("PIPES %d, %d\n", pipefds[0], pipefds[1]);
    res = dup2(pipefds[0], STDERR_FILENO);
    return EXIT_SUCCESS;
}

