// Exs221ListReverse.cpp : Defines the entry point for the console application.
//

//单链表逆置
#include "stdafx.h"


#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++头文件

#include "..\linkedList.h" 

//单链表的就地逆置 
void ListReverse(node *L)
{      
    node *p,*u;   
    p=L->next;      //p指向首元素结点
	L->next=NULL;   //头结点独立出来，next置空
    while(p!=NULL)
	{   
        u=p;  //取出未逆置部分的第一个结点，指针为u
		p=p->next;  //p后移一个结点，保留未逆置部分的头指针
		      //逆置u指向的结点，即用头插法插入链表
        u->next=L->next;   
        L->next=u;           
    } 
}  


int main(int argc, char* argv[])
{
	node *L,*p;
	cout<<"创建带头结点的单链表L-->>"<<endl;
	L=createListR3();

	//打印创建的链表
	cout<<"链表L包含结点："<<endl;
	p=L->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;
	//打印逆置结果
	
	cout<<"链表逆置结果为："<<endl;
	ListReverse(L);
	p=L->next;   
    while(p!=NULL)
	{   
		cout<<p->data<<",";
		p=p->next;   
	}
	destroyList(L);  //销毁链表，释放heap内存
	cout<<endl;


	return 0;
}

