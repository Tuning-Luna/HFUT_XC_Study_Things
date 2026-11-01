// Exs211DelLLDataX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "iostream.h"
#include "stdlib.h"
#include "time.h"

#include "..\linkedList.h" 

//【2.11】设计算法以删除链表中值为x的元素结点。
//【分析】
//与基本运算基本相同，只是这里要根据结点的元素值来搜索待删除目标结点的指针。
//与基本删除类似，我们用一个指针p指向待删除结点的前驱，则p->next指向目标结点。
//初始化时让p=L，即指向头结点。
//循环将p->next->data与x进行比较，若相等，p->next指示的即是待删除结点。
//此外，要处理x不在表中情况。

//只删除找到的一个x结点
bool listDeleteX(node* L, elementType x)
{
	node* u;
	node* p=L;   //p指向待删除结点的前驱，初始化指向头结点
	int succ=0;  //是否删除成功标记，成功删除succ=1，失败succ=0。
	while(p->next)
	{
		if(p->next->data==x)
		{     //找到目标结点，删除此结点
			u=p->next;  //p指向目标结点的前驱，p->next指向待删除目标结点，并以u保存。 
			p->next=u->next;  //ai-1的next指向ai+1节点，或为空（ai-1为最后节点）
			delete u;   //释放目标结点占据的空间
			succ=1;     //标记结点成功删除

			break; //退出循环
		}
		p=p->next;
	}

	if(succ==1)
		return true;   //成功删除，返回 true
	else
		return false;  //删除失败，返回 false，x不在链表中。	
}

//删除所有元素值为x的结点
bool listDeleteX1(node* L, elementType x)
{
	node* u;
	node* p=L;   //p指向待删除结点的前驱，初始化指向头结点
	int succ=0;  //是否删除成功标记，成功删除succ=1，失败succ=0。
	while(p->next)
	{
		if(p->next->data==x)
		{      //找到目标结点，删除此结点
			u=p->next;  //p指向目标结点的前驱，p->next指向待删除目标结点，并以u保存。 
			p->next=u->next;  //ai-1的next指向ai+1节点，或为空（ai-1为最后节点）
			delete u;   //释放目标结点占据的空间
			succ=1;     //标记结点成功删除
		}               //删除结点后p->next为一个新的结点，可能值也为x，故p不移动
		else
			p=p->next;
	}

	if(succ==1)
		return true;   //成功删除，返回 true
	else
		return false;  //删除失败，返回 false，x不在链表中。	
}

//测试删除一个结点
bool listDeleteX2(node* L, elementType x)
{
	node* u;
	node* p=L;   //p指向待删除结点的前驱，初始化指向头结点

	while(p->next && p->next->data!=x)
	{
		p=p->next;  //定位元素值为x的结点
	}
	if(p==NULL || p->next==NULL)
		return false;
	else
	{
		u=p->next;  //p指向目标结点的前驱，p->next指向待删除目标结点，并以u保存。 
		p->next=u->next;  //ai-1的next指向ai+1节点，或为空（ai-1为最后节点）
		delete u;   //释放目标结点占据的空间
		return true;
	}
}



int main(int argc, char* argv[])
{
	node *L,*p;
	elementType x;

	cout<<"尾插法--创建带头结点的单链表-->>"<<endl;
	L=createListR3();

		//打印创建的链表
	cout<<"链表包含："<<endl;
	p=L->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;


	cout<<"请输入待删除的元素（整数）：x=";
	cin>>x;

	//调用删除函数，并检查是否删除成功
	if(listDeleteX(L, x))
		cout<<"删除链表元素 "<<x<<" 节点成功！"<<endl;
	else
		cout<<"抱歉！元素"<<x<<"不在表中，删除失败！"<<endl;


	//打印删除节点后的链表
	cout<<"删除节点后的链表元素："<<endl;
	p=L->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;

	destroyList(L);  //销毁链表，释放heap内存
	return 0;
}
