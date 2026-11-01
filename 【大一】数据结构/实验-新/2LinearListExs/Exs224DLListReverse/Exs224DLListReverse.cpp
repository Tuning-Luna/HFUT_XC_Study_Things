// Exs224DLListReverse.cpp : Defines the entry point for the console application.
//双循环链表就地逆置

#include "stdafx.h"


#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++头文件

#include "..\dLinkedList.h"

bool SymmetricalDLList(node *L)
{
	node *p, *r;
	p=L->next;    //p指向首元素结点
	r=L->prior;   //r指向尾元素结点
	while(p!=r && r->next!=p)
	{    //p!=r控制奇数元素结点循环
		 //r->next!=p或p->prior!=p控制偶数元素结点循环

		if(p->data!=r->data)
			return false;  //不对称，返回false
		else
		{
			p=p->next;   //p、r当前指向结点相等，p后移一个结点，r前移一个结点
			r=r->prior;
		}
	}
	return true;  //到此，双循环链表对称，返回true
}

void DLListReverse(node *L)
{
	node *P, *u;  //P指向当前待逆置的结点，u指向未逆置的下一个结点
	P=L->next;    //P指向原链表的首元素结点，此结点逆置后为尾结点，需要单独处理	
	              
	if(P!=L)      //非空链表，处理第一个结点
	{
		u=P;
		p=p->next;  //p指向链表下一个结点

		u->next=L;  //建立next向循环
		L->prior=u; //建立prior向循环		
	}
	   //循环逆置剩余结点
	while(P!=L)
	{
		u=P;
		p=p->next;  //p指未逆置部分下一个结点
		            
		u->prior=L;        //u头插成为首元素结点，前向指针指向头结点
		u->next=L->next;   //P->next指向已逆置部分原来的首元素结点
		L->next->prior=u;  //已逆置部分原来首元素结点的prior指针指向P（P插入成为其前驱）
		L->next=u;         //L->next指向P，p插入成为首元素结点 		
	}
}


int main(int argc, char* argv[])
{
	node* L, *u;

	cout<<"尾插法创建双链表>>>";

	createDllR(L);  //尾插法创建双链表

	//打印双链表
	cout<<"双链表包含元素："<<endl;
	u=L->next; //取第一个元素节点
	while(u!=L)  //u=L，即指向头结点
	{
		cout<<u->data<<", ";
		u=u->next;
	}
	cout<<endl;

    if(SymmetricalDLList(L))
		cout<<"链表为对称链表。"<<endl;
	else
		cout<<"链表为非对称链表。"<<endl;


           //逆置
	DLListReverse(L);
	       //打印双链表
	cout<<"逆置后双链表："<<endl;
	u=L->next; //取第一个元素节点
	while(u!=L)  //u=L，即指向头结点
	{
		cout<<u->data<<", ";
		u=u->next;
	}
	cout<<endl;

	destroyList(L);

	return 0;
}

