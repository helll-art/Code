/*************************************************************************
	> File Name: 5.4.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 12时44分32秒
 ************************************************************************/

#include<stdio.h>
#define INCH 0.3937008
#define FEET 0.0328084
int main(void)
{
    double cm,feet,inch;
    while(1)
    {
        printf("Enter a height in centimeters:");
        scanf("%lf",&cm);
        if(cm>0)
        {
            printf("%.1lfcm = %.1lf feet = %.1lf inches\n",cm,cm*FEET,cm*INCH);
        }
        else {
            printf("数据异常，退出程序\n");
            return 0;
        }
    }

}
