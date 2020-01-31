/*************************************************************************
	> File Name: 5.3.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 12时38分47秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int days;
    while(1){
        printf("输入天数：");
        scanf("%d",&days);
        if (days>0){    
                printf("%d days are %d weeks,%d days.\n",days,days/7,days%7);
        }
        else {
            printf("数据异常，退出程序\n");
            return 0;
        }
    }
}
