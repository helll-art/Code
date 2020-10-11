#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<linux/limits.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
#include<fcntl.h>

#define PARAM_NONE 0
#define PARAM_A 1
#define PARAM_L 2
#define PARAM_R 4
#define PARAM_r 8
#define PARAM_F 16
#define PARAM_i 32
#define PARAM_s 64

#define MAXROWLEN 120

int flag_param = PARAM_NONE;
int g_leave_len = MAXROWLEN;
int g_maxlen;
void printf_name(char * name,struct stat buf);
void display_dir(int flag,char * pathname);
void display_attribute(struct stat buf,char * name);
void my_err(const char *string,int line);
void display_single(struct stat buf,char * name);
void display(int flag,char * pathname);
//void display_R(int flag,char * pathname);
void display_dir_R(int flag,char *name);

int main(int argc,char ** argv)
{
    int i,j,k,num;
    char path[PATH_MAX + 1];
    char param[32];
    
    struct stat buf;

    j = 0;
    num = 0;
    for(i = 1;i < argc;i++)
    {
        if(argv[i][0] == '-')
        {
            for(k = 1;k < strlen(argv[i]);k++,j++)
            {
                param[j] = argv[i][k];
            }
            num ++;
        }
    }

    for(i = 0;i < j;i++)
    {
        if(param[i] == 'a')
        {
            flag_param = flag_param | PARAM_A;
            continue;
        }
        else if(param[i] == 'l')
        {
            flag_param = flag_param | PARAM_L;
            continue;
        }
        else if(param[i] == 'R')
        {
            flag_param = flag_param | PARAM_R;
            continue;
        }
        else if(param[i] == 'r')
        {
            flag_param = flag_param | PARAM_r;
            continue;
        }
        else if(param[i] == 'F')
        {
            flag_param = flag_param | PARAM_F;
            continue;
        }
        else if(param[i] == 's')
        {
            flag_param = flag_param | PARAM_s;
            continue;
        }
        else if(param[i] == 'i')
        {
            flag_param = flag_param | PARAM_i;
            continue;
        }
        else 
        {
            printf("my_ls : invalid option -%c\n",param[i]);
            exit(1);
        }

    }
    param[j] = '\0';
    
    if((num + 1) == argc)
    {
        strcpy(path,"./");
        path[2] = '\0';
        display_dir(flag_param,path);
        return 0;
    }
    
    i = 1;
    do
    {
        if(argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else 
        {
            strcpy(path,argv[i]);

            if(stat(path,&buf) == -1)
                my_err("stat",__LINE__);
            if(S_ISDIR(buf.st_mode))
            {
                if(flag_param & PARAM_R)
                    display_dir_R(flag_param,path);
                //printf("A : %s\n",path);
                else
                    display_dir(flag_param,path);
                i++;
            }
            else 
            {
                //printf("B : %s\n",path);
                g_maxlen = strlen(path);
                display(flag_param,path);
                i++;
            }

        } 
    } while (i < argc);
    printf("\n");
    
}
void my_err(const char *string,int line)
{
    fprintf(stderr,"line : %d",line);
    perror(string);
    exit(1);
}
void display_dir(int flag,char * pathname)
{
    //printf("display_dir : %s\n",pathname);
    DIR * dir;
    struct dirent * ptr;
    int count = 0;
    char filenames[256][PATH_MAX + 1];
    char temp[PATH_MAX + 1];
    dir = opendir(pathname);
    //printf("pathname = %s\n",pathname);
    if(dir == NULL)
    {
        if(errno!=13)
            perror("opendir fail");
        else
            return ;
    }

    
    while((ptr = readdir(dir)) != NULL)
    {
        //printf("%s\n",ptr -> d_name);
        if(g_maxlen < strlen(ptr -> d_name))
            g_maxlen = strlen(ptr -> d_name);
        count ++;
    }
    g_maxlen = g_maxlen + 3;
    //printf("g_maxlen = %d\n",g_maxlen);
    closedir(dir);
    if(count > 256 )
    {
        my_err("too many files under this dir",__LINE__);
        exit(-1);
    }
    dir = opendir(pathname);
    int i = 0;
    while((ptr = readdir(dir)) != NULL)
    {
        sprintf(filenames[i],"%s/%s",pathname,ptr -> d_name);
        //printf("filenames = %s\n",filenames[i]);
        filenames[i][strlen(filenames[i])] = '\0';
        i++;
    }
    
    for(int i = 0 ; i < count ;i++)
    {
        for (int j = i;j < count -1 ;j++)
        {
            if(strcmp(filenames[i],filenames[j]) > 0)
            {
                strcpy(temp,filenames[i]);
                strcpy(filenames[i],filenames[j]);
                strcpy(filenames[j],temp);
            }
        }

    }
    
    if(flag & PARAM_r)
    {
        for(int i = 0 ; i < count ;i++)
        {
            for (int j = i;j < count  ;j++)
            {
                if(strcmp(filenames[i],filenames[j]) < 0)
                {
                    strcpy(temp,filenames[i]);
                    strcpy(filenames[i],filenames[j]);
                    strcpy(filenames[j],temp);
                }
            }

        }
    }
    for(int i = 0;i < count ;i++)
    {
        //printf("ABC : %s\n",filenames[i]);
        display(flag,filenames[i]);
    }

    closedir(dir);

}
void display_attribute(struct stat buf,char * name)
{
    char buf_time[32];
    struct passwd * psd;
    struct group * grp;
    if(S_ISLNK(buf.st_mode))
        printf("l");
    else if(S_ISREG(buf.st_mode))
        printf("d");
    else if(S_ISCHR(buf.st_mode))
        printf("c");
    else if(S_ISBLK(buf.st_mode))
        printf("b");
    else if(S_ISFIFO(buf.st_mode))
        printf("f");
    else if(S_ISSOCK(buf.st_mode))
        printf("s");
    else 
        printf("-");
    if(buf.st_mode & S_IRUSR)
        printf("r");
    else 
        printf("-");
    if(buf.st_mode & S_IWUSR)
        printf("w");
    else 
        printf("-");
    if(buf.st_mode & S_IXUSR)
        printf("x");
    else 
        printf("-");

    if(buf.st_mode & S_IRGRP)
        printf("r");
    else 
        printf("-");
    if(buf.st_mode & S_IWGRP)
        printf("w");
    else 
        printf("-");
    if(buf.st_mode & S_IXGRP)
        printf("x");
    else 
        printf("-");

    if(buf.st_mode & S_IROTH)
        printf("r");
    else 
        printf("-");
    if(buf.st_mode & S_IWOTH)
        printf("w");
    else 
        printf("-");
    if(buf.st_mode & S_IXOTH)
        printf("x");
    else 
        printf("-");

    printf("    ");

    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%4d",buf.st_nlink);
    printf(" ");
    printf("%-8s",psd -> pw_name);
    printf("%-8s",grp -> gr_name);

    printf("%6d",buf.st_size);
    strcpy(buf_time,ctime(&buf.st_mtime));
    buf_time[strlen(buf_time ) - 1] = '\0';
    printf(" %s  ",buf_time);
    
    
    printf_name(name,buf);
    printf("\n");

}
void display_single(struct stat buf,char * name)
{
    
    //printf("len = %d\n",len);
   /*
    int len = g_leave_len - strlen(name);
   
    if(len > 0)
    {
        printf_name(name,buf);
        g_leave_len -= strlen(name);
    }
    else 
    {
        printf("\n");
        printf_name(name,buf);
        g_leave_len = MAXROWLEN - strlen(name);
    }*/
    int len;
    if(g_leave_len < g_maxlen)
    {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }
    len = g_maxlen - strlen(name);
    printf_name(name,buf);
    
    if(flag_param & PARAM_F)
    {
        if(S_ISLNK(buf.st_mode))
            printf("@");
            //管道文件后面加|
        else if(S_ISFIFO(buf.st_mode))
            printf("|");
            //目录后面加/
        else if(S_ISDIR(buf.st_mode))
            printf("/");
            //套接字后面加=
        else if(S_ISSOCK(buf.st_mode))
            printf("=");
        else 
            printf(" ");
            g_leave_len --;
    }
    else if(flag_param & PARAM_i)
    {
        printf(" %-8d",buf.st_ino) ;
        g_leave_len = g_leave_len - 8;

    }
    else if(flag_param & PARAM_s)
    {
        printf(" %-8d",buf.st_blocks);
        g_leave_len = g_leave_len - 8;
    }
     for(int i=0;i<len;i++)
    {
        printf(" ");
    }
    printf("  ");
    g_leave_len -= (g_maxlen+3);
    /*
    //printf("G_leavelen = %d\n",g_leave_len);
    g_leave_len = g_leave_len - strlen(name) -2;
    //printf("leave_len = %d\n",g_maxlen - strlen(name));
    for(int i = 0 ; i < (g_maxlen - strlen(name)) ;i++)
        printf(" ");
    */
}
void display(int flag,char * pathname)
{
    struct stat buf;
    //printf("display pathname = %s\n",pathname);
    char path[80] = "\0";
    int j = 0;
    for(int i = 0; i < strlen(pathname) ;i++)
    {
        path[j] = pathname[i];
        j++;
        if(pathname[i] == '/')
            j = 0;
        
    }
    path[j] = '\0';
    //printf("path = %s\n",path);
    if(lstat(pathname,&buf) == -1)
        my_err("stat",__LINE__);
    switch(flag)
    {
        case PARAM_NONE:
            if(path[0] != '.')
                display_single(buf,path);
            break;
        case PARAM_A:
            display_single(buf,path);
            break;
        case PARAM_L:
            if(path[0] != '.')
                display_attribute(buf,path);
            break;
        case PARAM_R:
            if(path[0] != '.')
                display_single(buf,path);
            break;
        case PARAM_F:
            if(path[0] != '.')
                display_single(buf,path);
            break;
        case PARAM_r:
            if(path[0] != '.')
                display_single(buf,path);
            break;
        case PARAM_i:
            if(path[0] != '.')
                display_single(buf,path);
            break;
        case PARAM_s:
            if(path[0] != '.')
                display_single(buf,path);
            break;
        case PARAM_L + PARAM_A :
            display_attribute(buf,path);
            break;
        case PARAM_R + PARAM_A:
            display_single(buf,path);
            break;
        case PARAM_R + PARAM_L:
            if(path[0] != '.')
                display_attribute(buf,path);
            break;
        
        case PARAM_L + PARAM_A + PARAM_R:
            display_attribute(buf,path);
            break;
        default:
            break;
        
    }
}
/*
void display_dir_R(int flag,char * pathname)
{
  //  printf("display_R\n");
    DIR * dir;
    struct dirent * ptr;
    char temp[PATH_MAX + 1];
    dir = opendir(pathname);
    printf("%s : \n",pathname);
    display_dir(flag,pathname);
    printf("\n");
    if((dir = opendir(pathname)) == NULL)
    {
        
        if(errno != 13)
            printf("display_dir_R failed\n");
        else 
            return ;
    }
//    printf("/........./\n");
    while((ptr = readdir(dir)) != NULL)
    {
        if(ptr -> d_type == DT_DIR && ((ptr -> d_name[0] )!= '.'))
        {
            sprintf(temp,"%s/%s",pathname,ptr -> d_name);
            temp[strlen(temp) - 1] = '\0';
            strcpy(pathname,temp);
            //printf("%s\n",pathname);
            display_dir_R(flag,pathname);

        }
    }
    
    closedir(dir);
    return ;

}
*/

void display_dir_R(int flag,char *name)
{
    DIR *dir;
    struct dirent *ptr;
    char temp_name[PATH_MAX+1];
    printf("%s:\n",name);
    
    display_dir(flag,name);
    printf("\n");
    
    if((dir = opendir(name)) == NULL)
    {
        if(errno!=13)
            perror("display_R fail:");
        
        else 
            return ;
    }
    int len = strlen(name);
    
    while((ptr = readdir(dir ))!=NULL)
    {
        //printf("display_R %s\n",ptr -> d_name);
        if(ptr-> d_type == DT_DIR && (ptr -> d_name[0] != '.'))
        {
            strncpy(temp_name,name,len);
            temp_name[len] = '\0';
            
            strcat (temp_name,ptr->d_name);
            temp_name[len+strlen(ptr->d_name)] = '/';
            
            temp_name[len+strlen(ptr->d_name)+1] = '\0';
            display_dir_R(flag,temp_name);
        }
    }
    closedir(dir);
    return ;
}


void printf_name(char * name,struct stat buf)
{
    if(S_ISLNK(buf.st_mode))
        printf("\033[1m\033[36m%-s\033[0m",name);
    else if(S_ISDIR(buf.st_mode))
        printf("\033[1m\033[34m%-s\033[0m",name);
    else if(S_ISCHR(buf.st_mode) ||S_ISBLK(buf.st_mode))
        printf("\033[1m\033[33m%-s\033[0m",name);
    else if(buf.st_mode & S_IXUSR)
        printf("\033[1m\033[32m%-s\033[0m",name);
    else 
        printf("%-s",name);

}