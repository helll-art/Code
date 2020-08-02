
#include<mysql.h>
#include<stdio.h>
#include"my_sql.h"

void my_err(const char *string,int line)
{
    fprintf(stderr,"%d : ",line);
    perror(string);
}
void accept_mysql(MYSQL *mysql)
{
    if(NULL == mysql_init(mysql))
        my_err("mysql_init",__LINE__);

    if(mysql_library_init(0,NULL,NULL) != 0)
        my_err("mysql library init ",__LINE__);

    if(NULL == mysql_real_connect(mysql,"127.0.0.1","root","123456","chat",0,NULL,0))
        my_err("mysql real connect ",__LINE__);

    if(mysql_set_character_set(mysql,"utf8") < 0)
        my_err("mysql_set_character_set",__LINE__);

    printf("数据库链接成功！\n");
    

}