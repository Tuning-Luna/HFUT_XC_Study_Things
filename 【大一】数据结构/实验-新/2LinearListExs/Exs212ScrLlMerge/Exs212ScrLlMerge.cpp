// Exs212ScrLlMerge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdlib.h"
#include "iostream.h"
#include "time.h"

#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "..\scLinkedList.h"


//带头结点单循环链表合并
//合并后保留A链表的头结点，A的尾结点接B的首元素结点
//B的尾结点的next指向A
//销毁B的头结点
void scMergeAB(node *&A, node *&B)
{
	node *u,*p; 
    p=B;
	if(p->next==B)            //B表为空表，删除B表头，其它不变
	{
		delete B;
		return;
	}
	      //以下为B表不空情况
	p=A;
	while(p->next!=A)         //p指向A的尾结点
	{
		p=p->next;	
	}	
	p->next=B->next;          //A的尾结点接B的首元素结点
	
	p=B;
	while(p->next!=B)         //p指向B的尾结点
	{
		p=p->next;
	}
	p->next=A;                //形成循环	
	delete B;                 //释放B的头结点
}

int main(int argc, char* argv[])
{
	node *A,*B;
	cout<<"创建单循环链表A："<<endl;
	createSCLLR(A);
	cout<<"创建单循环链表B："<<endl;
	createSCLLR(B);
	
	cout<<"A表";
	printList(A);
	cout<<"B表";
	printList(B);
	    //合并表A、B
	scMergeAB(A,B);
	cout<<"A、B合并后表";
	printList(A);
	
     destroyList(A);  //释放合并后的表
	_CrtDumpMemoryLeaks();  //debug 模式下检测是否内存泄漏	
	return 0;
}

