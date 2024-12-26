// Exs224DLListReverse.cpp : Defines the entry point for the console application.
//˫ѭ������͵�����

#include "stdafx.h"


#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++ͷ�ļ�

#include "..\dLinkedList.h"

bool SymmetricalDLList(node *L)
{
	node *p, *r;
	p=L->next;    //pָ����Ԫ�ؽ��
	r=L->prior;   //rָ��βԪ�ؽ��
	while(p!=r && r->next!=p)
	{    //p!=r��������Ԫ�ؽ��ѭ��
		 //r->next!=p��p->prior!=p����ż��Ԫ�ؽ��ѭ��

		if(p->data!=r->data)
			return false;  //���Գƣ�����false
		else
		{
			p=p->next;   //p��r��ǰָ������ȣ�p����һ����㣬rǰ��һ�����
			r=r->prior;
		}
	}
	return true;  //���ˣ�˫ѭ������Գƣ�����true
}

void DLListReverse(node *L)
{
	node *P, *u;  //Pָ��ǰ�����õĽ�㣬uָ��δ���õ���һ�����
	P=L->next;    //Pָ��ԭ�������Ԫ�ؽ�㣬�˽�����ú�Ϊβ��㣬��Ҫ��������	
	              
	if(P!=L)      //�ǿ����������һ�����
	{
		u=P;
		p=p->next;  //pָ��������һ�����

		u->next=L;  //����next��ѭ��
		L->prior=u; //����prior��ѭ��		
	}
	   //ѭ������ʣ����
	while(P!=L)
	{
		u=P;
		p=p->next;  //pָδ���ò�����һ�����
		            
		u->prior=L;        //uͷ���Ϊ��Ԫ�ؽ�㣬ǰ��ָ��ָ��ͷ���
		u->next=L->next;   //P->nextָ�������ò���ԭ������Ԫ�ؽ��
		L->next->prior=u;  //�����ò���ԭ����Ԫ�ؽ���priorָ��ָ��P��P�����Ϊ��ǰ����
		L->next=u;         //L->nextָ��P��p�����Ϊ��Ԫ�ؽ�� 		
	}
}


int main(int argc, char* argv[])
{
	node* L, *u;

	cout<<"β�巨����˫����>>>";

	createDllR(L);  //β�巨����˫����

	//��ӡ˫����
	cout<<"˫�������Ԫ�أ�"<<endl;
	u=L->next; //ȡ��һ��Ԫ�ؽڵ�
	while(u!=L)  //u=L����ָ��ͷ���
	{
		cout<<u->data<<", ";
		u=u->next;
	}
	cout<<endl;

    if(SymmetricalDLList(L))
		cout<<"����Ϊ�Գ�����"<<endl;
	else
		cout<<"����Ϊ�ǶԳ�����"<<endl;


           //����
	DLListReverse(L);
	       //��ӡ˫����
	cout<<"���ú�˫����"<<endl;
	u=L->next; //ȡ��һ��Ԫ�ؽڵ�
	while(u!=L)  //u=L����ָ��ͷ���
	{
		cout<<u->data<<", ";
		u=u->next;
	}
	cout<<endl;

	destroyList(L);

	return 0;
}

