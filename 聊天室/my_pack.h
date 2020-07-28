#ifndef MY_PACK_H
#define MY_PACK_H

    typedef struct 
    {
        int send_fd;
        int recv_fd;
        int send_account;
        int recv_account;
        char send_user[50];
        char recv_user[50];
        char read_buf[1024];
        char write_buf[1024];
    }DATA;
    typedef struct
    {
        int type;
        DATA data;
    } PACK;

#endif 