#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

const char* filePath = "/tmp/dupdup2.txt";

void fatalErrno(const char* call)
{
    char buf[1024];
    cerr << call << ": " << strerror_r(errno, buf, sizeof(buf)) << endl;
    exit(EXIT_FAILURE);
}

void catFile(const char* filePath)
{
    FILE* fs = fopen(filePath, "r");
    char* lineBuf = NULL;
    size_t size;
    while(-1 != getline(&lineBuf, &size, fs))
    {
        printf("%s", lineBuf);
        free(lineBuf);
        lineBuf = NULL;
    }

    fclose(fs);
}

int main(int argc, char** argv)
{
    int save_stdout = dup(STDOUT_FILENO);

    int fd = open(filePath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (-1 == fd)
    {
        fatalErrno("open");
    }
    int res = dup2(fd, STDOUT_FILENO);
    if (-1 == res)
    {
        fatalErrno("dup2");
    }
    const char* txt = "Hello lord! In the file!\n";
    res = write(STDOUT_FILENO, txt, strlen(txt));
    if (res != strlen(txt))
    {
        fatalErrno("write");
    }

    res = dup2(save_stdout, STDOUT_FILENO);
    if (-1 == res)
    {
        fatalErrno("dup2");
    }

    catFile(filePath);
    close(fd);
    unlink(filePath);
    return EXIT_SUCCESS;
}

