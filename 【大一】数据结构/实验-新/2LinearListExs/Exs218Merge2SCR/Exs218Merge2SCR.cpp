// Exs218Merge2SCR.cpp : Defines the entry point for the console application.
//
//合并2个递增有序单循环链表A和B，使成为1个递增有序单循环链表A=A∪B

#include "stdafx.h"

#include "iostream.h"
#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"


#include "..\scLinkedList.h" 



//求C=A∪B
//本题有不同的理解，
//第一个不同处理：结果链表可能是递增的，也可不作要求。
//第二种情况的不同处理：B表元素可以利用B结点重接，释放B的多余结点，也可以用B的元素值申请新结点，B表保持不变
//目标表不要求递增，B表元素申请新结点，B表保持不变。
void unionSet(node *A, node *B)
{
	node*pa,*pb,*u,*R;  //pa和pb为A和B当前结点指针
	pa=A;
	pb=B->next;
	while(pa->next!=A && pb!=B)
	{
		if(pa->next->data<pb->data)        //pa为并集结点
			pa=pa->next;
		else if(pa->next->data>pb->data)   //pa有可能在pb的后面，pb是交集结点，申请新结点插入pa之后
		{                            //pa不动
			u=new node;
			u->data=pb->data;
			u->next=pa->next;
			pa->next=u;	
			pb=pb->next;
		}
		else    //pa->data==pb->data，pa、pb同时后移
		{
			pa=pa->next;
			pb=pb->next;		
		}	
	}
	    //如果B没有结束，pb及其后结点皆为并集结点，插入A表
	while(pb!=B)
	{
		u=new node;
		u->data=pb->data;
		u->next=pa->next;
		pa->next=u;
		pa=u;           //pa指尾结点
		pb=pb->next;	
	}
}

void MergeLLCircular(node* A,node *B)
{
	//合并后，以A为头结点指针，B指示的头结点删除
	node*pa,*pb,*R;  //pa和pb为A和B当前结点指针
	                 //R为新表的尾指针 
	pa=A->next;
	pb=B->next;
	R=A;
	while(pa!=A && pb!=B)
	{
		if(pa->data<=pb->data)
		{
			R->next=pa;   //pa指示结点链接到新表尾
			R=pa;         //移动新表尾指针
			pa=pa->next;  //pa指向A未合并部分的下一个结点
		}
		else
		{
			R->next=pb;   //pb指示结点链接到新表尾
			R=pb;         //移动新表尾指针
			pb=pb->next;  //pb移到B表未合并部分的下一个结点

		}	
	}
	   //以下处理2表不等长情况，
	   //其中一个链表已经结束，另一个表尚未结束，
	   //元素值都不小于已经接入部分，直接接入即可
	if(pa!=A)  //B已结束，A未结束，直接链接A剩下部分，且已构成单循环
		R->next=pa ;
	else   //A已结束，B未结束，需要将R移到B最后一个结点，让其next为A，形成循环
	{
		R->next=pb ; 
		while(R->next!=B)
		{
			R=R->next;
		}
		R->next=A;
	}
	
	//形成新的循环链表
	//R->next=A;

	delete(B);  //删除 B 头结点
}

int main(int argc, char* argv[])
{
	node *A, *B, *p, *Pa, *Pb;
	cout<<"创建单循环链表-->>"<<endl;
	createSCLLR(A);  //创建单循环链表A
	createSCLLR(B);  //创建单循环链表B

	printList(A);
	printList(B);

	unionSet(A,B);

	printList(A);
	

	destroyList(A);
	destroyList(B);

	return 0;
}

