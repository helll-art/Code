/*************************************************************************
	> File Name: shuixianhuashu.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月04日 星期三 21时42分02秒
 ************************************************************************/


 #include<stdio.h>
 int main()
 {
     int i,j,k,f=1;
     int m,n,temp;
     int x,y,z;
     while(scanf("%d%d",&m,&n)!=EOF)
     {
         if(m>n)
         {
             temp=m;
             m=n;
             n=temp;
         }
         for(i=m;i<=n;i++)
         {
             x=i%10;
             y=(i/10)%10;
             z=i/100;

         
             if(x*x*x+y*y*y+z*z*z==i)
             {
                 if(f==1)
                 {
                    printf("%d",i);
                     f=0;
                 }
                 else
                    printf(" %d",i);
             }
         }     

             if(f==1)
            printf("no");
            printf("\n");
     } 
     return 0;
 }
