/*************************************************************************
	> File Name: caesar.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月16日 星期一 13时22分25秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
    char ct[101];
    char pt[101]="\0";
    int x;
    fets(ct,101,stdin);
    x=strlen(ct);
    for(int i=0;i<x;i++)
    {
        if(ct[i]>='A'&&ct[i]<='E')
            pt[i]=ct[i]+21;
        else if(ct[i]>'E'&&ct[i]<='Z')
            pt[i]=ct[i]-5;
        else pt[i]=ct[i];
    }
    for(int i=0;i<x;i++)
    {
        printf("%c",pt[i]);
    }
    printf("\n");

    return 0;
}
