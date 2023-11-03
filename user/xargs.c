#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

// 编写 xargs 工具，从标准输入读入数据，将每一行当作参数，加入到传给 xargs
// 的程序名和参数后面作为额外参数，然后执行。
int
main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("enter more args");
        exit(1);
    }

    char buf[2048]; // 缓冲区

    char c;
    int i = 0;
    while (read(0, &c, sizeof(c))) {
        if (c != '\n') {
            buf[i++] = c;
        } else {
            buf[i] = 0;
            int index = 0;
            if (fork() == 0) {          // 为什么要用fork
                char* commands[MAXARG]; // 保存参数的数组

                for (int k = 1; k < argc;
                     k++) // 把命令行参数保存到参数数组中 比如 (echo 1 ; echo 2)
                          // | xargs -n 1 echo
                {
                    commands[index++] = argv[k];
                }
                commands[index] = buf;

                // 注意的是，如果exec函数执行成功则不会返回
                // 这里两个参数分别指的是 可执行文件名称和所需传入的参数
                // execl("/bin/sh", "sh", "-c", "echo byy | xargs echo",NULL);
                exec(
                  argv[1],
                  commands); // 通常用于在一个进程中加载并执行另一个程序。它会用新的程序替代当前进程的镜像，即在执行新程序时，当前进程的代码和数据都会被新程序替代。
            } else {
                wait(0);
                i = 0;
                memset(buf, 0, sizeof(buf));
            }
        }
    }
    exit(0);
}

// void
// main(int argc, char* argv[])
// {
//     char c;
//     int i = 0;
//     char buf[14];
//     while (read(0, &c, sizeof(c))) {
//         /**
//          * 收集字符直到遇到换行符,并将之前的字符作为一个字符串
//          */
//         if (c != '\n') {
//             buf[i++] = c;
//         } else {
//             int index = 0;
//             if (fork() == 0) {
//                 // child process
//                 char* commands[MAXARG];
//                 // 将xargs命令行参数传入到commands字符串数组中
//                 for (int k = 1; k < argc; k++) {
//                     commands[index++] = argv[k];
//                 }
//                 commands[2] = buf;
//                 exec(argv[1], commands);
//             } else {
//                 wait(0);
//                 i = 0;
//                 memset(buf, 0, sizeof(buf));
//             }
//         }
//     }
//     exit(0);
// }