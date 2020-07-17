/*************************************************************************
	> File Name: TTMS.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月17日 星期五 15时39分55秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include "./View/Main_Menu.h"
#include "./View/Account_UI.h"

account_t gl_CurUser = {0,USR_ADMIN,"Anonymous",""};
int main(void)
{
    system("cls");
    system("color 70");
    system("mode con cols=103 lines=35");
    setvbuf(stdout, NULL, _IONBF, 0);
    if (!SysLogin()) {
        printf("\n\t\t\t\t\t错误次数超限,即将退出系统\n");
        sleep(3);
        return EXIT_SUCCESS;
    }
    Main_Menu(gl_CurUser);
    system("pause"); 
    return EXIT_SUCCESS;
}
    
