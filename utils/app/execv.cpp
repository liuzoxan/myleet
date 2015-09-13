#include <errno.h>
#include <iostream>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

void fatalExit(const char* err)
{
    char buf[1024];
    cout << err << "" << strerror_r(errno, buf, 1024) << endl;
    exit(EXIT_FAILURE);
}

bool exists(const char* filePath)
{
    struct stat buf;
    return (stat(filePath, &buf) == 0);
}

int main(int argc, char** argv)
{
    char* cmd0 = strdup("touch");
    char* args = strdup("/tmp/myexecv.txt");
    char* cmd[3] = {
            cmd0,
            args,
            NULL,
    };

    if (exists(args))
    {
        unlink(args);
    }

    execvp(cmd[0], cmd);
    fatalExit("execvp");
    return EXIT_FAILURE;
}


