// Exs222MergeOrderedListReverse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//递增有序表A、B，合并为递减有序表
#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++头文件

#include "..\linkedList.h" 

node* MergeAndReverse(node *A, node *B)
{  
	node *pa,*pb,*p,*L;
	      //pa、pb分别指向A和B待逆置的结点
	L=A;  //以L为合并后的头结点指针，即A的头结点指针
	pa=A->next;
	pb=B->next;
	L->next=NULL;
	while(pa!=NULL && pb!=NULL)
	{
		if(pa->data<pb->data)
		{
			p=pa->next; //p指向A下一个未逆置结点
			pa->next=L->next;  
			L->next=pa;  //pa作为已经合并部分的第一个结点，至此逆置完成
			pa=p;  //pa指向A第一个未逆置结点
		}
		else
		{
			p=pb->next;  //p指向B下一个未逆置结点
			pb->next=L->next;  //pb->next指向已经合并部分的第一个数据结点
			L->next=pb;  //pb作为已经合并部分的第一个结点，至此逆置完成
			pb=p; //pb指向A第一个未逆置结点
		}
	}
	//以下处理A、B不等成情况
	while(pa!=NULL && pb==NULL)  //A未结束，B已经结束
	{  
        p=pa->next;
		pa->next=L->next;
		L->next=pa;
		pa=p;
	}
	while(pa==NULL && pb!=NULL)  //A已结束，B未结束
	{  
        p=pb->next;
		pb->next=L->next;
		L->next=pb;
		pb=p;
	}
	return(L);
}  


int main(int argc, char* argv[])
{
	node *La,*Lb,*L;
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
	free(pa);

	cout<<"链表B包含："<<endl;
	pb=Lb->next;
	while(pb)
	{
		cout<<pb->data<<",";
		pb=pb->next;
	}
    cout<<endl;
	free(pb);
	//打印完毕
	cout<<"合并后的链表为:"<<endl;
	L=MergeAndReverse(La,Lb)->next;  //反序合并两个递增有序表
	while(L)
	{
		cout<<L->data<<",";
		L=L->next;
	}
    cout<<endl;
	
	destroyList(L);  //销毁链表，释放heap内存

	return 0;
}

