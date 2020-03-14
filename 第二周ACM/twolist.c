/*************************************************************************
	> File Name: twolist.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月10日 星期二 20时28分28秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
typedef struct Node
{
    struct Node *prev;
    int data;
    struct Node *next;
}Node; 
Node *create_list(void);
void add_list(Node *);
void traverse_list(Node *);
void del_list(Node *);
void insert_list(Node *);
void changemessage(Node *);
void sort(Node *pHead);
int main(void)
{
    Node *pHead;
    pHead=create_list();
    add_list(pHead);
    traverse_list(pHead);
    del_list(pHead);
    traverse_list(pHead);
    insert_list(pHead);
    traverse_list(pHead);
    changemessage(pHead);
    traverse_list(pHead);
    sort(pHead);
    traverse_list(pHead);
}
Node * create_list(void)
{
    Node *pHead;
    pHead=(Node *)malloc(sizeof(Node));
    pHead->next=NULL;
    pHead->prev=NULL;
    pHead->data=0;
    return pHead;
}
void add_list(Node *pHead)
{
    Node *pTail,*pNew;
    pTail=pHead;
    int n,val;
    printf("请输入链表的长度：");
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        printf("请输入第%d个数据的值：",i+1);
        scanf("%d",&val);
        pNew=(Node *)malloc(sizeof(Node));
        pNew->data=val;
        pTail->next=pNew;
        pNew->prev=pTail;
        pNew->next=NULL;
        pTail=pNew;
    }
}
void traverse_list(Node *pHead)
{
    Node *p;
    p=pHead->next;
    while(p)
    {
        printf("%d\t",p->data);
        p=p->next;
    }
}
void del_list(Node *pHead)
{
    Node *p,*r;
    int val;
    p=pHead->next;
    printf("请输入要删除的数值：");
    scanf("%d",&val);
    while(p->data!=val)
    {
        p=p->next;
    }
    r=p->prev;
    p->prev->next=p->next;
    p->next=r;

}
void insert_list(Node *pHead)
{
    Node *p,*r;
    p=pHead;
    r=(Node *)malloc(sizeof(Node));
    int val,pos;
    printf("请输入你要插入的位置和数值：");
    scanf("%d%d",&pos,&val);
    for (int i=0;i<pos-1;i++)
    {
        p=p->next;
    }
    r->data=val;
    r->next=p->next;
    p->next=r;
    r->prev=p;
}
void changemessage(Node *pHead)
{
    int pos,val;
    printf("请输入要更改的位置：");
    scanf("%d",&pos);
    printf("请输入要更改的值：");
    scanf("%d",&val);
    Node *p;
    p=pHead;
    for(int i=0;i<pos;i++)
    {
        p=p->next;
    }
    p->data=val;
}
void sort(Node *pHead)
{
    Node *pi,*pj;
    int temp;
    printf("排序后结果为：");
    for(pi=pHead;pi!=NULL;pi=pi->next)
    {
        for(pj=pi;pj!=NULL;pj=pj->next)
        {
            if(pi->data>pj->data)
            {
            temp=pi->data;
            pi->data=pj->data;
            pj->data=temp;
           }
        }
    }
}
