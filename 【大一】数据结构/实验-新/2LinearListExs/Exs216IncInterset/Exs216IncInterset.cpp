// Exs216IncInterset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdlib.h"
#include "iostream.h"
#include "time.h"

#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "..\LinkedList.h"

//求递增有序集合的交集
void incInterset(linkedList &A,linkedList &B,linkedList &C)
{
	node* Pa, *Pb, *Rc, *u;
	Rc=C; //C表尾指针，空表时头尾指针相同
	Pa=A->next;
	Pb=B->next;  //Pa和Pb分别指向A和B表的第一个元素节点
	while(Pa!=NULL && Pb!=NULL)  //A和B只要一个没有元素，即没有交集元素，退出
	{
		if(Pa->data<Pb->data)  //B中没有A的当前元素，即Pa->data
			Pa=Pa->next;      //取A的下一个元素，回去循环
		else if (Pa->data>Pb->data)  //A当前元素值大于B当前元素值，移动Pb继续搜索B
			Pb=Pb->next;
		else  //Pa->data=Pb->data，即为交集元素，在C中产生新节点，Pa,Pb同时后移，回去循环
		{
			u=new node;
			u->data=Pa->data;  //或u->data=Pb->data，新结点赋值。
			Rc->next=u;        //尾插法在C中插入u，
			Rc=u;              //修改C的尾指针Rc，指向u
			Pa=Pa->next;       //Pa和Pb同时后移，分别取A和B的下一个元素
			Pb=Pb->next; 
		}
		Rc->next=NULL; //表C结束
	}
}

//交互式创建递增表
void incListCreate(linkedList &L)
{
	node *R,*p;
	elementType x;

	if(L!=NULL)
		destroyList(L);
	
	L=new node;
	L->next=NULL;
	R=L;

	cout<<"请输入数据元素(整数，-9999退出):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999)
	{
		if(L->next!=NULL && x<R->data)//判断是否递增
		{
			cout<<"当前输入的值 x="<<x<<" 违反递增有序要求，请重新选择输入！"<<endl;
			cout<<"x=";
			cin>>x;  //重新读入数字
			continue;		
		}
		p=new node;
		p->data=x;

		p->next=NULL;
		R->next=p;
		R=p;
		
		cout<<"x=";
		cin>>x;
	}
}

int main(int argc, char* argv[])
{
	linkedList A,B,C;

	initialList(A);
	initialList(B);
	initialList(C);

	cout<<"创建递增有序集合A："<<endl;
	incListCreate(A); //创建递增链表A
	cout<<"创建递增有序集合B："<<endl;
	incListCreate(B); //创建递增链表B	
	
	cout<<"求集合A、B的交集C："<<endl;
	incInterset(A,B,C);  //求A、B的交集C
	
	cout<<"集合A";
	printList(A);	
	cout<<"集合B";
	printList(B);	
	cout<<"集合C";
	printList(C);

	destroyList(A);            //释放链表
	destroyList(B);
	destroyList(C);
	_CrtDumpMemoryLeaks();  //debug 模式下检测是否内存泄漏	
	return 0;
}
