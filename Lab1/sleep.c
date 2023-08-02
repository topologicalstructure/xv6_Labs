#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2){ //处理用户忘记传递参数的情况
        fprintf(2, "please give sleep process a argument.\n");
        exit(1);
    }
    sleep(atoi(argv[1])); //将字符串转化为整数并调用系统函数
    exit(0); //退出程序
}