/*************************************************************************
	> File Name: q.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月09日 星期日 18时00分55秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
        char former=' ',present;
        while((present=getchar())!=EOF)
    {
                if((former==' ' || former=='\t' || former=='\n')&& ( present >= 'a'&& present <= 'z' ))
                    printf("%c",present-26-6);
                else 
                    printf("%c",present);
                former=present;
            
    }

}

