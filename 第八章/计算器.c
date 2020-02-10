/*************************************************************************
	> File Name: 8.8.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月04日 星期二 15时53分12秒
 ************************************************************************/
 #include<stdio.h>
 #include<ctype.h>
 char get_first(void) // 得到字符串的第一个字符，虑掉其他字符
 {                    //  为什么？？？？？？？？？？？？？？？？？？
     char ch;
     while(isspace(ch=getchar()));
     while(getchar()!='\n');
     return ch;
 }
 void action(void)
 {
     printf("Enter the operation of your choice:\n");
     printf("a.add                s.subtract\n");
     printf("m.multiply           d.divide\n");
     printf("q.quit\n");
 }
 int main(void)
 {
     double a,b;
     char choice;
     int cnt=0;
     while(1)
     {
         action();
         choice=get_first();    //  不要参数为什么会调出来参数
         if(choice=='a'||choice=='s'||choice=='m'||choice=='d')
         {
            printf("Enter first number:");
            scanf("%lf",&a);
            printf("Enter second number:");
            scanf("%lf",&b);
            switch(choice)
             {
                 case 'a':
                        printf("%.2lf + %.2lf = %.2lf\n",a,b,a+b);
                        break;
                 case 's':
                        printf("%.2lf - %.2lf = %.2lf\n",a,b,a-b);
                        break;
                 case 'm':
                        printf("%.2lf * %.2lf = %.2lf\n",a,b,a*b);
                        break;
                 case 'd':
                        while(b==0)
                        {
                            printf("Enter a number other than 0:");
                            scanf("%lf",&b);
                        }
                        printf("%.2lf / %.2lf = %.2lf",a,b,a/b);
                        break;
                default:break;
             }
         }
         else {
             printf("Bye\n");
             return 0;
         }
         printf("\n\n\n");
       //  cnt++;
       //  action();
        // printf("%d\n",cnt);
     }
     
 }
