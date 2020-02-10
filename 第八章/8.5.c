/*************************************************************************
	> File Name: tiaoshi.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月04日 星期二 14时09分31秒
 ************************************************************************/

#include<stdio.h>
#define LOW 0
#define HIGH 100
int main(void)
{
    int guess=50;
    char ch;
    printf("Pick an integer from 1 to 100.i will try to guess it.\n");
    printf("Respond with a s if my guess is small and with an b if it is Big and with an y if it is true.\n");
    printf("Uh...is your number %d?\n",guess);
    scanf("%c",&ch);
    while(1)
    {
        if(ch=='s')
        {
            printf("Well,then, is it %d\n",(HIGH+guess)/2);
            guess=(HIGH+guess)/2;
        }
        if(ch=='b')
        {
            printf("Well,then, is it %d\n",(LOW+guess)/2);
            guess=(LOW+guess)/2;
        }
        if(ch=='y')
        {
            printf("I knew i could do it!\n");
            return 0;
        }
        scanf("%c",&ch);
    }
}
