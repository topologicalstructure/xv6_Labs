#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc<2){
        fprintf(2, "parameter missing.\n");
        exit(0);
    }
    char *args[MAXARG];
    int args_i = 0;
    for (int i = 1; i < argc; ++i){
        args[args_i++] = argv[i];
    }
    char* line =(char *)malloc(512*sizeof(char));
    int index=0;
    char c;
    while(read(0,&c,1)>0){
        if(c==' '||c=='\n'){ //遇到空格或回车，则将line数组加尾零结束，添加到args并重置
            line[index++]='\0';
            args[args_i++]=line;
            line =(char *)malloc(512*sizeof(char));
            index=0;
            if(fork()==0){
                args[args_i] = 0;
                exec(args[0], args);
            }
            else{
                wait(0);
                args_i=argc-1;
            }
        }
        else{
            line[index++]=c;
        }
    }
    exit(0);
}