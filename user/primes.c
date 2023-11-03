#include "kernel/types.h"

#include "kernel/stat.h"

#include "user/user.h"

// 要用管道实现的素数筛
// void
// seive(int n)
// {
//     int prime[1000];
//     memset(is_prime, 1, sizeof(is_label));
//     for (int i = 2; i <= n; i++) {
//         if (prime[i]) {
//             for (int j = i; i * j <= n; j++) {
//                 prime[i * j] = 0;
//             }
//         }
//     }
// }

// 核心思想的伪代码
// p = get a number from left neighbor
// print p
// loop:
//     n = get a number from left neighbor
//     if (p does not divide n)
//         send n to right neighbor

// int
// main(int argc, char* argv[])
// {
//     int pleft[2];
//     int pright;
//     int num;

//     pipe(pleft);
//     pright = pleft[0];
//     for (int i = 2; i <= 35; i++) {
//         write(pleft[1], &i, sizeof(i));
//     }
//     close(pleft[1]);

//     while ((read(pright, &num, sizeof(num)) > 0)) {
//         int cur_prime = num;
//         printf("prime %d\n", num);
//         pipe(pleft);

//         if (fork() > 0) {
//             close(pleft[0]);
//             while (read(pright, &num, sizeof(num)) > 0) {
//                 if (num % cur_prime) {
//                     write(pleft[1], &num, sizeof(num));
//                 }
//             }
//             close(pleft[0]);
//             close(pright);
//             wait(0);
//         } else {
//             close(pleft[1]);
//             pright = pleft[0];
//         }
//     }

//     exit(0);
// }

int
main(int argc, char* argv[])
{
    int p[2];
    pipe(p);

    for (int i = 2; i <= 35; ++i) {
        write(p[1], &i, 1);
    }
    close(p[1]);

    int num;
    int fd = p[0];
    // close(fd);

    while ((read(fd, &num, 1)) > 0) {
        int cur_prime = num;
        printf("prime %d\n", cur_prime);
        pipe(p);
        if (fork() == 0) {
            close(p[1]);
            fd = p[0];
        } else {
            close(p[0]);
            while ((read(fd, &num, 1)) > 0) {
                if (num % cur_prime) {
                    write(p[1], &num, 1);
                }
            }
            close(p[1]);
            close(fd);
            wait(0);
        }
    }
    exit(0);
}
