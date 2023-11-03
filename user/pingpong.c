#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

// Write a user-level program that uses xv6 system calls to ''ping-pong'' a byte between two processes over a pair of pipes, one for each direction.
// The parent should send a byte to the child; the child should print "<pid>: received ping", where <pid> is its process ID, write the byte on the pipe to the parent, and exit;
// the parent should read the byte from the child, print "<pid>: received pong", and exit. Your solution should be in the file user/pingpong.c.

// Add the program to UPROGS in Makefile.
// Use pipe to create a pipe.
// Use fork to create a child.
// Use read to read from a pipe, and write to write to a pipe.
// Use getpid to find the process ID of the calling process.
// User programs on xv6 have a limited set of library functions available to them. You can see the list in user/user.h; the source (other than for system calls) is in user/ulib.c, user/printf.c, and user/umalloc.c.

int main(int argc, char* argv[])
{
    int child[2], parent[2];
    char buf[10];

    if (pipe(child) == -1 || pipe(parent) == -1) {
        printf("pipe");
        exit(1);
    }

    if (fork() == 0) {
        //先读再写
        // child process

        close(parent[1]);
        close(child[0]);

        read(parent[0], buf, 5);
        printf("%d: received %s\n", getpid(), buf);
        write(child[1], "pong", strlen("pong") + 1);

        close(child[1]);
        close(parent[0]);

    } else {
        // 先写再读
        close(child[1]);
        close(parent[0]);
        write(parent[1], "ping", strlen("ping") + 1); //把最后的\0也读进去
        read(child[0], buf, 5);
        printf("%d: received %s\n", getpid(), buf);

        close(child[0]);
        close(parent[1]);
    }
    exit(0);
}
