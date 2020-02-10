/*************************************************************************
	> File Name: 10.8.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月08日 星期六 14时53分28秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int r,c;
    int a[r][c];
    int k=0;
    scanf("%d%d",&r,&c);
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            a[i][j]=k;
            printf("%d\t",a[i][j]);
            k++;
        }
        printf("\n");
    }
}
