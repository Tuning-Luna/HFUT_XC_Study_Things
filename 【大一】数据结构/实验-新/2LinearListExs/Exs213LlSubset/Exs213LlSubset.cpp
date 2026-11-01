// Exs213LlSubset.cpp : Defines the entry point for the console application.
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

int llSubset( node* A, node* B )
{
	node *pa, *pb;
	int suc=0;
	pa=A->next;        //pa指向A的首元素结点	
	while(pa!=NULL)
	{
		pb=B->next;    //pb指向B表的首元素结点
		suc=0;
		while(pb!=NULL)
		{
			if(pa->data==pb->data)
			{
				suc=1;
				break;
			}
			else
				pb=pb->next;		
		}
		if(suc==0)    //pa->data不在B中，A不是B的子集，返回0
			break;	

		pa=pa->next;	
	}
	return suc;
}

int main(int argc, char* argv[])
{
	node *A, *B, *p;

	cout<<"尾插法创建链表A-->>"<<endl;
	A=createListR3();

	cout<<"尾插法创建链表B-->>"<<endl;
	B=createListR3();

		//打印创建的链表
	cout<<"链表A包含："<<endl;
	printList(A);
    cout<<endl;
	cout<<"链表B包含："<<endl;
	printList(B);
    cout<<endl;

	    //调用子集判定函数
	if(llSubset(A,B))
		cout<<"A 是 B 的子集！"<<endl;
	else
		cout<<"A 不是 B 子集。"<<endl;

	destroyList(A);  //销毁链表，释放heap内存
	destroyList(B);
	return 0;
}

