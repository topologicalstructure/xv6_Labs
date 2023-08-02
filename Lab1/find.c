#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//该函数取自user/ls.c，用于从路径中取文件名
char* fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void find(char *path,char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "open: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "open: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_FILE: //对于文件，判断其文件名是否与要查的相同，若是则输出
            if(strcmp(fmtname(path),name)==0){
                printf("%s\n", fmtname(path));
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("open: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            //遍历文件夹下的所有文件或文件夹
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                //避免递归进入“.” 和 ”..”
                if(strcmp(de.name,".")==0 || strcmp(de.name,"..")==0){
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(strcmp(de.name,name)==0){
                    printf("%s\n", buf);
                }
                //递归查找
                find(buf,name);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if(argc < 3){ //缺失变量的情况
        fprintf(2, "parameter missing.\n");
        exit(0);
    }
    find(argv[1],argv[2]);
    exit(0);
}