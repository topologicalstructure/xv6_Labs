#include "kernel/types.h"
#include "user/user.h"

int main()
{
    char buffer[512];
    //创建两个管道，子进程在fp1中写，父进程在fp2中写
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int pid = fork(); //创建子进程
    if (pid < 0) { //正常情况下不会出现小于0的进程id
        fprintf(2, "fork error.\n");
        exit(1);
    }
    if (pid == 0) { //pid=0，即当前进程为子进程
        read(fd2[1],buffer,1);
        fprintf(1, "%d: received ping\n",getpid());
        write(fd1[0],buffer,1);
    }
    else{ //反之，当前进程为父进程
        write(fd2[0],buffer,1);
        wait(0); //等待子进程结束
        read(fd1[1],buffer,1);
        fprintf(1, "%d: received pong\n",getpid());
    }
    exit(0);
}