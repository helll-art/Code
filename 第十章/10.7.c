/*************************************************************************
	> File Name: 10.7.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月07日 星期五 21时58分32秒
 ************************************************************************/

#include<stdio.h>
void copy(double a[],double *b,int start,int end)
{
    int k=start;
    for(int i=0;i<end-start;i++)
    {
        *(b+i)=a[k];
        k++;
    }
}
int main(void)
{
    double a[7]={1.1,2.2,3.3,4.4,5.5,6.6,7.7};
    double b[7]={0};
    int start,end;
    printf("输入开始位置和结束位置：");
    scanf("%d%d",&start,&end);
    copy(a,b,start,end);
    for(int i=0;i<end-start;i++)
    {
        printf("%.2lf\t",b[i]);
    }
}
