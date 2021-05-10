#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
/* 

    同时打开一个文件，会产生两个文件表， 但是只会有一个v节点
    利用 追加方式 打开文件 同样是两个文件表，一个v节点，但是 追加方式在每一次写文件时， 都会从 v节点中 读取出文件长度
        存入 文件位移量 ， 再进行写入操作
    如果文件不以 追加方式 打开 ， 文件省去了让文件位移量 等于 文件长度 这个操作 ， 而是直接 按照原本的文件位移量进行读取
 */

void print_error(char * string)
{
    perror(string);
    exit(0);
}
void test01()
{
    int fd1 = 0;
    int ret = 0;
    fd1 = open("./file.txt",O_RDWR | O_TRUNC | O_CREAT);
    if(-1 == fd1)
        print_error("1 open failed " );
    int fd2 = open("./file.txt", O_RDWR | O_TRUNC);
    if(-1 == fd2)
        print_error("2 open failed");
    
    while(1)
    {
        write(fd1 , "hello\n",sizeof("hello") );
        sleep(1);
        write(fd2 , "world\n" , sizeof("world") );

    }
    close(fd1);
    close(fd2);
}
void test02()
{
int fd1 = 0;
    int ret = 0;
    fd1 = open("./file.txt",O_RDWR | O_TRUNC | O_CREAT | O_APPEND);
    if(-1 == fd1)
        print_error("1 open failed " );
    int fd2 = open("./file.txt", O_RDWR | O_TRUNC | O_APPEND);
    if(-1 == fd2)
        print_error("2 open failed");
    printf("fd1 = %d   fd2 = %d\n",fd1 , fd2 );
    while(1)
    {
        write(fd1 , "hello\n",sizeof("hello") );
        sleep(1);
        write(fd2 , "world\n" , sizeof("world") );

    }
    close(fd1);
    close(fd2);
}
int main(void)
{
    test02();
}