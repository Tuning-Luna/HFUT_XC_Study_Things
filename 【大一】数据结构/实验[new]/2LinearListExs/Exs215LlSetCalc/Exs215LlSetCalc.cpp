// Exs215LlSetCalc.cpp : Defines the entry point for the console application.
//�������ʾ�ļ��ϵ�����

#include "stdafx.h"

#include "stdlib.h"
#include "iostream.h"
#include "time.h"

#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "..\LinkedList.h"


//��C=A��B
void interSet(linkedList &A,linkedList &B,linkedList &C)
{
	node* pa, *pb, *Rc, *u;
	Rc=C;            //C��βָ�룬�ձ�ʱͷβָ����ͬ
	pa=A->next;      //Paָ��A�ĵ�һ��Ԫ�ؽ�� 	
	while(pa!=NULL)  //ѭ��ȡA��Ԫ�أ���㣩
	{
		pb=B->next;  //Pbָ��B�ĵ�һ��Ԫ�ؽ��
		while(pb)
		{
			if(pa->data==pb->data)  //����Ԫ�أ�����C��
			{
				u=new node;
				u->data=pa->data;  //��u->data=Pb->data���½�㸳ֵ��
				Rc->next=u;        //β�巨��C�в���u��
				Rc=u;              //�޸�C��βָ��Rc��ָ��u				
				break;             //�˳�B��ѭ��			
			}
			else
				pb=pb->next;       //����pb����һ�����
		}
		pa=pa->next;               //Pa���ƣ�ȡA����һ��Ԫ��		
	}
	Rc->next=NULL; //��C����	
}


//��C=A��B
void unionSet(linkedList &A,linkedList &B,linkedList &C)
{
	node* pa, *pb, *Rc, *u;
	bool succ=1;     //A��Ԫ���Ƿ���B�еı��
	Rc=C;            //C��βָ�룬�ձ�ʱͷβָ����ͬ
	pa=A->next;      //Paָ��A�ĵ�һ��Ԫ�ؽ�� 	
	       //A��Ԫ��β����C��
	while(pa)
	{
		u=new node;
		u->data=pa->data;
		Rc->next=u;
		Rc=u;
		pa=pa->next;	
	}

	pb=B->next;     //pbָ��B�ĵ�һ��Ԫ��
	while(pb)        //B�в���A�е�Ԫ��β�嵽C
	{
		succ=1;
		pa=A->next;  //paָ��A�ĵ�һ��Ԫ�ؽ��
		while(pa)
		{
			if(pb->data==pa->data)    //B�е�ǰԪ����A�У����ǲ���Ԫ��
			{
				succ=0;
				break;        //�˳�A��ѭ��			
			}
			else
				pa=pa->next;  //A����һ�����		
		}

		if(succ==1)           //B��ǰԪ���ǲ���Ԫ�أ�β�嵽C
		{
			u=new node;
			u->data=pb->data;
			Rc->next=u;
			Rc=u;		
		}
		pb=pb->next;         //ȡB����һ�����	
	
	}
	Rc->next=NULL; //��C����	
}

//��C=A-B����A�У�������B�е�Ԫ��
void subtractionSet(linkedList &A,linkedList &B,linkedList &C)
{
	node* pa, *pb, *Rc, *u;
	bool succ=0;     //A��Ԫ���Ƿ���B�еı��
	Rc=C;            //C��βָ�룬�ձ�ʱͷβָ����ͬ
	pa=A->next;      //Paָ��A�ĵ�һ��Ԫ�ؽ�� 	
	while(pa!=NULL)  //ѭ��ȡA��Ԫ�أ���㣩
	{
		succ=0;
		pb=B->next;  //Pbָ��B�ĵ�һ��Ԫ�ؽ��
		while(pb)
		{
			if(pa->data==pb->data)  //���ǲԪ�أ���ǣ��˳�B��ѭ��
			{
				succ=1;
				break;
			}
			else
				pb=pb->next;       //����pb����һ�����
		}
		
		if(succ==0)                //A��ǰԪ���ǲԪ�أ�β��C
		{
			u=new node;
			u->data=pa->data;
			Rc->next=u;
			Rc=u;
		}
		pa=pa->next;               //Pa���ƣ�ȡA����һ��Ԫ��		
	}
	Rc->next=NULL; //��C����	
}


//����ʽ�����ʾ�ļ���
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

	cout<<"����������Ԫ��(������-9999�˳�):"<<endl;
	cout<<"x=";
	cin>>x;

	while(x!=-9999)
	{
		if(listLocate(L,x,&i))//�жϲ����Ԫ��x�Ƿ��Ѿ�����
		{
			cout<<"��ǰ�����ֵ x="<<x<<" �Ѿ����ڣ�Υ�����Ϲ涨��������ѡ�����룡"<<endl;
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
	R->next=NULL;
}

int main(int argc, char* argv[])
{
	linkedList A,B,C;

	initialList(A);
	initialList(B);	

	cout<<"��������A��"<<endl;
	llSetCreate(A); //��������A
	cout<<"��������B��"<<endl;
	llSetCreate(B); //��������B	

	cout<<"����AԪ�أ�";
	printList(A);
	cout<<"����BԪ�أ�";
	printList(B);
	     //�󽻼�	
	initialList(C);
	interSet(A,B,C);        //��A��B�Ľ���C
	cout<<"����CԪ�أ�";
	printList(C);
	     //�󲢼�
	initialList(C);
	unionSet(A,B,C);        //��A��B�Ĳ���C
	cout<<"����CԪ�أ�";
	printList(C);
	     //��
	initialList(C);
	subtractionSet(A,B,C);  //��A��B�Ĳ
	cout<<"�CԪ�أ�";
	printList(C);

	destroyList(A);            //�ͷ�����
	destroyList(B);
	destroyList(C);
	_CrtDumpMemoryLeaks();  //debug ģʽ�¼���Ƿ��ڴ�й©	
	return 0;
}
