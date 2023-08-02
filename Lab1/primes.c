#include "kernel/types.h"
#include "user/user.h"

void Eratosthenes(int* fdparent)
{
    int first; //每个子进程通过管道读取的第一个数据即为质数
    read(fdparent[0], &first, sizeof(int));
    if(first==0){
        exit(0);
    }   
    printf("prime %d\n", first);
    int* fd=malloc(2*sizeof(int));
    pipe(fd);
    int pid = fork();
    if(pid==0){
        close(fd[1]);
        Eratosthenes(fd);
    }
    else{
        close(fd[0]);
        int x;
        while(read(fdparent[0], &x, sizeof(int))!=0){
            if(x%first){
                write(fd[1],&x,sizeof(int));
            }
        }
        close(fd[1]); //写入完成后，关闭写入端，子进程从管道读取到0就说明父进程已经写入完成
    }
    wait(0); //等待子进程结束
    free(fd);
}

int main()
{
    //创建管道，使得fd[0]作为读取端，fd[1]作为输入端
    int* fd=malloc(2*sizeof(int));
    pipe(fd);
    int pid = fork();
    if(pid==0){
        close(fd[1]);
        Eratosthenes(fd);
    }
    else{
        close(fd[0]);
        for(int i=2;i<=35;i++){
            write(fd[1],&i,sizeof(int));
        }
        close(fd[1]); //写入完成后，关闭写入端，子进程从管道读取到0就说明父进程已经写入完成
    }
    wait(0); //等待子进程结束
    free(fd);
    exit(0);
}