#include<stdio.h>
int main()
{
    int a;
    printf("请输入总脚数：");
    scanf("%d",&a);
    if(a%2==0)
    {
        printf("%d %d\n",(a%4)/2+a/4,a/2);
    }
    else printf("0 0\n");
    return 0;
}

