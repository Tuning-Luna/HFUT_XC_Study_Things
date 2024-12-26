// Exs223SymmetricalDLList.cpp : Defines the entry point for the console application.
//�ж�˫ѭ�������Ƿ�Գ�

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

	destroyList(L);

	return 0;
}

