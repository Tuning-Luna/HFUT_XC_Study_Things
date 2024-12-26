// Exs216IncInterset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdlib.h"
#include "iostream.h"
#include "time.h"

#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "..\LinkedList.h"

//��������򼯺ϵĽ���
void incInterset(linkedList &A,linkedList &B,linkedList &C)
{
	node* Pa, *Pb, *Rc, *u;
	Rc=C; //C��βָ�룬�ձ�ʱͷβָ����ͬ
	Pa=A->next;
	Pb=B->next;  //Pa��Pb�ֱ�ָ��A��B��ĵ�һ��Ԫ�ؽڵ�
	while(Pa!=NULL && Pb!=NULL)  //A��BֻҪһ��û��Ԫ�أ���û�н���Ԫ�أ��˳�
	{
		if(Pa->data<Pb->data)  //B��û��A�ĵ�ǰԪ�أ���Pa->data
			Pa=Pa->next;      //ȡA����һ��Ԫ�أ���ȥѭ��
		else if (Pa->data>Pb->data)  //A��ǰԪ��ֵ����B��ǰԪ��ֵ���ƶ�Pb��������B
			Pb=Pb->next;
		else  //Pa->data=Pb->data����Ϊ����Ԫ�أ���C�в����½ڵ㣬Pa,Pbͬʱ���ƣ���ȥѭ��
		{
			u=new node;
			u->data=Pa->data;  //��u->data=Pb->data���½�㸳ֵ��
			Rc->next=u;        //β�巨��C�в���u��
			Rc=u;              //�޸�C��βָ��Rc��ָ��u
			Pa=Pa->next;       //Pa��Pbͬʱ���ƣ��ֱ�ȡA��B����һ��Ԫ��
			Pb=Pb->next; 
		}
		Rc->next=NULL; //��C����
	}
}

//����ʽ����������
void incListCreate(linkedList &L)
{
	node *R,*p;
	elementType x;

	if(L!=NULL)
		destroyList(L);
	
	L=new node;
	L->next=NULL;
	R=L;

	cout<<"����������Ԫ��(������-9999�˳�):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999)
	{
		if(L->next!=NULL && x<R->data)//�ж��Ƿ����
		{
			cout<<"��ǰ�����ֵ x="<<x<<" Υ����������Ҫ��������ѡ�����룡"<<endl;
			cout<<"x=";
			cin>>x;  //���¶�������
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

	cout<<"�����������򼯺�A��"<<endl;
	incListCreate(A); //������������A
	cout<<"�����������򼯺�B��"<<endl;
	incListCreate(B); //������������B	
	
	cout<<"�󼯺�A��B�Ľ���C��"<<endl;
	incInterset(A,B,C);  //��A��B�Ľ���C
	
	cout<<"����A";
	printList(A);	
	cout<<"����B";
	printList(B);	
	cout<<"����C";
	printList(C);

	destroyList(A);            //�ͷ�����
	destroyList(B);
	destroyList(C);
	_CrtDumpMemoryLeaks();  //debug ģʽ�¼���Ƿ��ڴ�й©	
	return 0;
}
