/*************************************************************************
	> File Name: 6.15.c
	> Author: 
	> Mail: 
	> Created Time: 2020年01月31日 星期五 23时49分12秒
 ************************************************************************/

#include<stdio.h>
int main(void)
{
    int cnt=0;
    double Daphne=100,Deirdre=0;
    while(1)
    {
        cnt++;
        Daphne=cnt*0.1*100;
        Deirdre=(100+Deirdre)*0.05+Deirdre;
        if(Daphne<Deirdre)
        {
            printf("当第%d年时，Deirdre的投资额会超过Daphne。\n"
                   "并且此时Deirdre的投资额为:%lf\n    Daphne的投资额为:%lf\n ",cnt,Daphne,Deirdre);
            return 0;
        }

    }
}
