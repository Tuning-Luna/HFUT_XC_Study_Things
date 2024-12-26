// Exs217IncIntersetA.cpp : Defines the entry point for the console application.
//����������ʾ�ļ���A��B�Ľ�����������浽A

#include "stdafx.h"

#include "iostream.h"
#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"


#include "..\LinkedList.h" 

//��A=A��B
void interSet(linkedList &A,linkedList &B)
{
	node* pa, *pb, *u, *p;	
	pa=A;         //Paָ��A
	pb=B->next;   //pbָ��B����Ԫ�ؽ��
	while(pa->next && pb)  //A��B���н�㣬ѭ������
	{
		if(pa->next->data<pb->data)   //pa->next���ǽ�����㣬ɾ��pa->next��㣬pa��pb����
		{
			u=pa->next;
			pa->next=u->next;
			delete u;		
		}
		else if(pa->next->data>pb->data)  //pa->next������pb���棬pa���䣬pb����
			pb=pb->next;
		else       //pa->next->data==pb->data�����pa->next�ǽ�����㣬ͬʱ����pa��pb
		{
			pa=pa->next;
			pb=pb->next;
		}	
	}

	      //���A��δ������pa->next���������н�㶼�Ƿǽ�����㣬ɾ��
	if(pa->next)   //��A�����
	{
		u=pa->next;
		pa->next=NULL;
		p=u;
		  //ɾ��A��ʣ�µķǽ������
		while(p)
		{
			u=p;
			p=p->next;
			delete u;	
		}		
	}
}


//�����в�ͬ����⣬
//��һ����ͬ���������������ǵ����ģ�Ҳ�ɲ���Ҫ��
//�ڶ�������Ĳ�ͬ����B��Ԫ�ؿ�������B����ؽӣ��ͷ�B�Ķ����㣬Ҳ������B��Ԫ��ֵ�����½�㣬B���ֲ���
//Ŀ���Ҫ�������B��Ԫ�������½�㣬B���ֲ��䡣
void MergeLLCircular(node* A,node *B)
{
	//�ϲ�����AΪͷ���ָ�룬Bָʾ��ͷ���ɾ��
	node*pa,*pb,*R;  //pa��pbΪA��B��ǰ���ָ��
	                 //RΪ�±��βָ�� 
	pa=A->next;
	pb=B->next;
	R=A;
	while(pa!=A && pb!=B)
	{
		if(pa->data<=pb->data)
		{
			R->next=pa;   //paָʾ������ӵ��±�β
			R=pa;         //�ƶ��±�βָ��
			pa=pa->next;  //paָ��Aδ�ϲ����ֵ���һ�����
		}
		else
		{
			R->next=pb;   //pbָʾ������ӵ��±�β
			R=pb;         //�ƶ��±�βָ��
			pb=pb->next;  //pb�Ƶ�B��δ�ϲ����ֵ���һ�����

		}	
	}
	   //���´���2���ȳ������
	   //����һ�������Ѿ���������һ������δ������
	   //Ԫ��ֵ����С���Ѿ����벿�֣�ֱ�ӽ��뼴��
	if(pa!=A)  //B�ѽ�����Aδ������ֱ������Aʣ�²��֣����ѹ��ɵ�ѭ��
		R->next=pa ;
	else   //A�ѽ�����Bδ��������Ҫ��R�Ƶ�B���һ����㣬����nextΪA���γ�ѭ��
	{
		R->next=pb ; 
		while(R->next!=B)
		{
			R=R->next;
		}
		R->next=A;
	}
	
	//�γ��µ�ѭ������
	//R->next=A;

	delete(B);  //ɾ�� B ͷ���
}

int main(int argc, char* argv[])
{
	node *A, *B, *p, *Pa, *Pb;
	cout<<"��������������A-->>"<<endl;
	A=createListR3();  //��������������A
	cout<<"��������������B-->>"<<endl;
	B=createListR3();  //��������������B

	         //��ӡԭʼ����
    cout<<"����A������"<<endl;
	printList(A);
    cout<<endl;

	cout<<"����B������"<<endl;
	printList(B);
    cout<<endl;


	interSet(A,B);  //��A=A��B

	cout<<"A=A��B���Ϊ��"<<endl;
	printList(A);
    cout<<endl;

	destroyList(A);
	destroyList(B);

	return 0;
}
