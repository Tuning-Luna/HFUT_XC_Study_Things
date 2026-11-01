// Exs214Subset.cpp : Defines the entry point for the console application.
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

int SubSetDecision( node* A, node* B )
{
	node *pa, *pb;
	pa=A->next;  //pa和pb分别指向A和B表的首元素结点
	pb=B->next;
	while(pa!=NULL && pb!=NULL)
	{
		if(pa->data==pb->data)  //pa指示元素在B中，因是结合，pa、pb同时后移一个结点
		{
			pa=pa->next;
			pb=pb->next;
		}
		else if(pa->data>pb->data)  //pa指示元素可能在B中pb指示的元素后面，后移pb
			pb=pb->next;
		else
			return 0;  //此时，pa->data<pb->data，pa指示元素不可能在B中，A非B子集
	}
	  //下面根据A、B的结束情况，判定A是否B的子集
	if(pa==NULL)
		return 1;  //A到表尾，全部元素在B中，是B的子集
	else
		return 0;  //B到表尾，A未到表尾，A后面元素不在B中，A非B的子集

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
	p=A->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;
	cout<<"链表B包含："<<endl;
	p=B->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;


	    //调用子集判定函数
	if(SubSetDecision(A,B))
		cout<<"A 是 B 的子集！"<<endl;
	else
		cout<<"A 不是 B 子集。"<<endl;


	destroyList(A);  //销毁链表，释放heap内存
	destroyList(B);



	return 0;
}
