/*************************************************************************
	> File Name: list2.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月21日 星期五 20时09分26秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct Node 
{
    int data;
    struct Node * Next;
}Node;
void insert(Node *p);
void sort(Node *pHead);
int  delete(Node *p);
Node * create_list(void);
void traverse_list(Node *p);
int main(void)
{
    Node *pHead;
    pHead=create_list();//此处pHead为头指针  函数处为头结点                        
    printf("该链表的数据是：");
    traverse_list(pHead->Next);             //若传入的地址是pHead 则会输出头节点的数据段
    insert(pHead);
    printf("\n插入后的结果是：");
    traverse_list(pHead->Next);
    delete(pHead);
    printf("\n删除后的结果是：");
    traverse_list(pHead->Next);
    sort(pHead);
    printf("\n降序排序后的结果是：");
    traverse_list(pHead->Next);
}
void insert(Node *p)     //insert(pHead);
{
    int pos,val;
    printf("\n请输入要插入的位置和数据：");
    scanf("%d%d",&pos,&val);
    Node *r;
    r=(Node *)malloc(sizeof(Node));       // 不给r分配内存会产生段错误
    for(int i=0;i<pos-1;i++)
        p=p->Next;
    r->data=val;
    r->Next=p->Next;
    p->Next=r;
}
int  delete (Node *p)             //delete(pHead,pos)
{
    int pos;
    printf("\n请输入要删除的元素位置:");
    scanf("%d",&pos);
    Node *r;
    r=(Node *)malloc(sizeof(Node));
    for(int i=0;i<pos-1;i++)
    {
        p=p->Next;
    }
    r=p->Next;
    p->Next=r->Next;
    return r->data;                         //返回删除的数据
}
Node * create_list(void)
{
    Node *pHead,*pNew,*pTail;
    pHead=(Node *)malloc(sizeof(Node));      
    pTail=pHead;
    int length;
    printf("请输入要创建的链表长度：");
    scanf("%d",&length);
    for(int i=0;i<length;i++)
    {
        int val;
        printf("请输入第%d个数据：",i+1);
        scanf("%d",&val);
        pNew=(Node *)malloc(sizeof(Node));
        pTail->Next=pNew;
        pNew->data=val;
        pNew->Next=NULL;
        pTail=pNew;
    }
    return pHead;
}
void traverse_list(Node *p)            //traverse_list(pHead)
{
    while(p!=NULL)
    {
        printf("%d\t",p->data);
        p=p->Next;
    }
}
void sort(Node *pHead)     //降序       
{
    int temp;
    Node *pi,*pj;
    for(pi=pHead->Next;pi!=NULL;pi=pi->Next)
    {
        for(pj=pi;pj!=NULL;pj=pj->Next)
        {
            if(pi->data<=pj->data)
            {
                temp=pi->data;
                pi->data=pj->data;
                pj->data=temp;
            }
        }
    }
}
