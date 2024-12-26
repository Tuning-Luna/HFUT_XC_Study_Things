// Exs220MergeOrderedLists.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++头文件

#include "..\linkedList.h" 


void MergeLinkedList(node* La,node *Lb)
{
	//合并后，以La为头结点指针，Lb指示的头结点删除
	node*pa,*pb,*R;  //pa和pb为La和Lb当前结点指针
	                 //R为已经部分的尾指针 
	pa=La->next;
	pb=Lb->next;
	R=La;
	while(pa!=NULL && pb!=NULL)
	{
		if(pa->data<=pb->data)
		{
			R->next=pa;  //pa指示结点，接到合并表尾
			R=pa;  //移动尾指针
			pa=pa->next;  //pa指向下一个结点
		}
		else
		{
			R->next=pb; //pb结点接到合并表尾
			R=pb;  //移动尾指针
			pb=pb->next;  //pb移到下一个结点

		}	
	}
	   //以下处理2表不等长情况，
	   //其中一个链表已经结束，另一个表尚未结束，
	   //元素值都不小于已经接入部分，直接接入即可
	if(pa!=NULL)  //Lb已结束，La未结束
		R->next=pa ;
	else   //La已结束，Lb未结束
		R->next=pb ;  

	delete(Lb);  //删除 Lb 头结点
}

void main(int argc, char* argv[])
{
	
	node *La,*Lb;
	cout<<"两个递增链表合并-->>"<<endl;
	La=createListR3();
	Lb=createListR3();
	//打印创建的链表
	node *pa,*pb;
	cout<<"链表A包含："<<endl;
	pa=La->next;
	while(pa)
	{
		cout<<pa->data<<",";
		pa=pa->next;
	}
    cout<<endl;


	cout<<"链表B包含："<<endl;
	pb=Lb->next;
	while(pb)
	{
		cout<<pb->data<<",";
		pb=pb->next;
	}
    cout<<endl;
	
	MergeLinkedList(La,Lb);  //调用合并函数

	cout<<"连接结果为："<<endl;
	pa=La->next;
	while(pa!=NULL)
	{
		cout<<pa->data<<",";
		pa=pa->next;
	}
    cout<<endl;

	//打印完毕
	destroyList(La);  //销毁链表，释放heap内存
	

}

