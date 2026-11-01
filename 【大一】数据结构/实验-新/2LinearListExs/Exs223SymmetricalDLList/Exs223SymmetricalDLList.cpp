// Exs223SymmetricalDLList.cpp : Defines the entry point for the console application.
//判断双循环链表是否对称

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

	destroyList(L);

	return 0;
}

