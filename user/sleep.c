#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{

    int i = 1;
    if (argc != 2) {
        printf("args invalid");
        exit(1);
    } else {
        int wait_time = atoi(argv[i]);
        sleep(wait_time);

        exit(0);
    }
}