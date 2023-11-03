#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "kernel/types.h"

#include "kernel/stat.h"
#include "user/user.h"

void
find(char* path, char* target)
{
    char buf[512], *p;
    struct stat st;
    struct dirent de;
    int fd;

    // 打开文件，获取fd
    if ((fd = open(path, O_RDONLY)) == -1) {
        fprintf(2, "cannot open a file");
        return;
    }

    // 获取文件相关信息，将其保存在st里
    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            if (strcmp(path + strlen(path) - strlen(target), target) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path); // 把path的内容拷贝到buf中
            p = buf + strlen(buf);

            /*
            在这段代码中，将路径 path 复制到缓冲区 buf
            后，然后在路径末尾添加斜杠 "/" 的目的是为了构建完整的目录路径。
            这是因为在遍历目录时，每个目录项的名称通常是相对于当前目录的相对路径。通过将路径与每个目录项的名称组合，可以得到目录项的完整路径。

            例如，如果目录路径 path 是
            "/home/user/documents"，而目录中有两个文件 "file1" 和
            "file2"，则在处理这两个文件时，
            需要将它们的名称与目录路径组合，得到完整的路径，如下所示：

            对于 "file1"，完整路径为 "/home/user/documents/file1"。
            对于 "file2"，完整路径为 "/home/user/documents/file2"。
            通过在 buf 的末尾添加斜杠 "/"，然后在处理每个目录项时将其名称附加到
            buf 后，可以构建每个目录项的完整路径， 以便后续使用 stat()
            函数获取文件或目录的详细信息。这样做有助于确保正确获取和处理目录项的属性信息
            */
            *p = '/';
            p++;
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    // if(de.inum == 0 || strcmp(de.name , ".") == 0 ||
                    // strcmp(de.name,"..") == 0)
                    continue;

                memmove(p, de.name, DIRSIZ); // 将下一个路径添加到p里面
                p[DIRSIZ] = 0;

                if (stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                // 创建新目录的时候会自动你创建两个文件名: . .. (.指向当前目录，
                // ..指向父目录)
                if (strcmp(p + strlen(p) - 1, ".") != 0 &&
                    strcmp(p + strlen(p) - 2, "..") != 0)
                    find(buf, target);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char* argv[])
{
    if (argc < 3) {
        exit(0);
    }
    char* path = argv[1];
    char* target = argv[2];
    find(path, target);
    exit(0);
}