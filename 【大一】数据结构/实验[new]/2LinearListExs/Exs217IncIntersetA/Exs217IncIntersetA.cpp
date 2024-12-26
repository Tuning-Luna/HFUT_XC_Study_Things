// Exs217IncIntersetA.cpp : Defines the entry point for the console application.
//求递增链表表示的集合A和B的交集，结果保存到A

#include "stdafx.h"

#include "iostream.h"
#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"


#include "..\LinkedList.h" 

//求A=A∩B
void interSet(linkedList &A,linkedList &B)
{
	node* pa, *pb, *u, *p;	
	pa=A;         //Pa指向A
	pb=B->next;   //pb指向B的首元素结点
	while(pa->next && pb)  //A、B都有结点，循环处理
	{
		if(pa->next->data<pb->data)   //pa->next不是交集结点，删除pa->next结点，pa和pb不变
		{
			u=pa->next;
			pa->next=u->next;
			delete u;		
		}
		else if(pa->next->data>pb->data)  //pa->next可能在pb后面，pa不变，pb后移
			pb=pb->next;
		else       //pa->next->data==pb->data情况，pa->next是交集结点，同时后移pa、pb
		{
			pa=pa->next;
			pb=pb->next;
		}	
	}

	      //如果A表未结束，pa->next及后面所有结点都是非交集结点，删除
	if(pa->next)   //置A表结束
	{
		u=pa->next;
		pa->next=NULL;
		p=u;
		  //删除A表剩下的非交集结点
		while(p)
		{
			u=p;
			p=p->next;
			delete u;	
		}		
	}
}


//本题有不同的理解，
//第一个不同处理：结果链表可能是递增的，也可不作要求。
//第二种情况的不同处理：B表元素可以利用B结点重接，释放B的多余结点，也可以用B的元素值申请新结点，B表保持不变
//目标表不要求递增，B表元素申请新结点，B表保持不变。
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
	cout<<"创建递增单链表A-->>"<<endl;
	A=createListR3();  //创建递增单链表A
	cout<<"创建递增单链表B-->>"<<endl;
	B=createListR3();  //创建递增单链表B

	         //打印原始链表
    cout<<"链表A包含："<<endl;
	printList(A);
    cout<<endl;

	cout<<"链表B包含："<<endl;
	printList(B);
    cout<<endl;


	interSet(A,B);  //求A=A∩B

	cout<<"A=A∩B结果为："<<endl;
	printList(A);
    cout<<endl;

	destroyList(A);
	destroyList(B);

	return 0;
}
