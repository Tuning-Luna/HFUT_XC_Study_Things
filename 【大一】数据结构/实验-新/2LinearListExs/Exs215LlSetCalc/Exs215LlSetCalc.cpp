// Exs215LlSetCalc.cpp : Defines the entry point for the console application.
//单链表表示的集合的运算

#include "stdafx.h"

#include "stdlib.h"
#include "iostream.h"
#include "time.h"

#define _CRTDBG_MAP_ALLOC //内存泄漏检查用
#include<stdlib.h> 
#include<crtdbg.h> 

#include "..\LinkedList.h"


//求C=A∩B
void interSet(linkedList &A,linkedList &B,linkedList &C)
{
	node* pa, *pb, *Rc, *u;
	Rc=C;            //C表尾指针，空表时头尾指针相同
	pa=A->next;      //Pa指向A的第一个元素结点 	
	while(pa!=NULL)  //循环取A的元素（结点）
	{
		pb=B->next;  //Pb指向B的第一个元素结点
		while(pb)
		{
			if(pa->data==pb->data)  //交集元素，插入C中
			{
				u=new node;
				u->data=pa->data;  //或u->data=Pb->data，新结点赋值。
				Rc->next=u;        //尾插法在C中插入u，
				Rc=u;              //修改C的尾指针Rc，指向u				
				break;             //退出B表循环			
			}
			else
				pb=pb->next;       //否则，pb后移一个结点
		}
		pa=pa->next;               //Pa后移，取A的下一个元素		
	}
	Rc->next=NULL; //表C结束	
}


//求C=A∪B
void unionSet(linkedList &A,linkedList &B,linkedList &C)
{
	node* pa, *pb, *Rc, *u;
	bool succ=1;     //A的元素是否在B中的标记
	Rc=C;            //C表尾指针，空表时头尾指针相同
	pa=A->next;      //Pa指向A的第一个元素结点 	
	       //A表元素尾插入C中
	while(pa)
	{
		u=new node;
		u->data=pa->data;
		Rc->next=u;
		Rc=u;
		pa=pa->next;	
	}

	pb=B->next;     //pb指向B的第一个元素
	while(pb)        //B中不在A中的元素尾插到C
	{
		succ=1;
		pa=A->next;  //pa指向A的第一个元素结点
		while(pa)
		{
			if(pb->data==pa->data)    //B中当前元素在A中，不是并集元素
			{
				succ=0;
				break;        //退出A的循环			
			}
			else
				pa=pa->next;  //A后移一个结点		
		}

		if(succ==1)           //B当前元素是并集元素，尾插到C
		{
			u=new node;
			u->data=pb->data;
			Rc->next=u;
			Rc=u;		
		}
		pb=pb->next;         //取B的下一个结点	
	
	}
	Rc->next=NULL; //表C结束	
}

//求C=A-B，在A中，但不在B中的元素
void subtractionSet(linkedList &A,linkedList &B,linkedList &C)
{
	node* pa, *pb, *Rc, *u;
	bool succ=0;     //A的元素是否在B中的标记
	Rc=C;            //C表尾指针，空表时头尾指针相同
	pa=A->next;      //Pa指向A的第一个元素结点 	
	while(pa!=NULL)  //循环取A的元素（结点）
	{
		succ=0;
		pb=B->next;  //Pb指向B的第一个元素结点
		while(pb)
		{
			if(pa->data==pb->data)  //不是差集元素，标记，退出B表循环
			{
				succ=1;
				break;
			}
			else
				pb=pb->next;       //否则，pb后移一个结点
		}
		
		if(succ==0)                //A当前元素是差集元素，尾插C
		{
			u=new node;
			u->data=pa->data;
			Rc->next=u;
			Rc=u;
		}
		pa=pa->next;               //Pa后移，取A的下一个元素		
	}
	Rc->next=NULL; //表C结束	
}


//交互式链表表示的集合
void llSetCreate(linkedList &L)
{
	node *R,*p;
	elementType x;

	int i;

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
		if(listLocate(L,x,&i))//判断插入的元素x是否已经存在
		{
			cout<<"当前输入的值 x="<<x<<" 已经存在，违反集合规定，请重新选择输入！"<<endl;
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
	R->next=NULL;
}

int main(int argc, char* argv[])
{
	linkedList A,B,C;

	initialList(A);
	initialList(B);	

	cout<<"创建集合A："<<endl;
	llSetCreate(A); //创建链表A
	cout<<"创建集合B："<<endl;
	llSetCreate(B); //创建链表B	

	cout<<"集合A元素：";
	printList(A);
	cout<<"集合B元素：";
	printList(B);
	     //求交集	
	initialList(C);
	interSet(A,B,C);        //求A、B的交集C
	cout<<"交集C元素：";
	printList(C);
	     //求并集
	initialList(C);
	unionSet(A,B,C);        //求A、B的并集C
	cout<<"并集C元素：";
	printList(C);
	     //求差集
	initialList(C);
	subtractionSet(A,B,C);  //求A、B的差集
	cout<<"差集C元素：";
	printList(C);

	destroyList(A);            //释放链表
	destroyList(B);
	destroyList(C);
	_CrtDumpMemoryLeaks();  //debug 模式下检测是否内存泄漏	
	return 0;
}
