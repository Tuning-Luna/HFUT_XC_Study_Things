// Exs218Merge2SCR.cpp : Defines the entry point for the console application.
//
//�ϲ�2����������ѭ������A��B��ʹ��Ϊ1����������ѭ������A=A��B

#include "stdafx.h"

#include "iostream.h"
#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"


#include "..\scLinkedList.h" 



//��C=A��B
//�����в�ͬ����⣬
//��һ����ͬ���������������ǵ����ģ�Ҳ�ɲ���Ҫ��
//�ڶ�������Ĳ�ͬ����B��Ԫ�ؿ�������B����ؽӣ��ͷ�B�Ķ����㣬Ҳ������B��Ԫ��ֵ�����½�㣬B���ֲ���
//Ŀ���Ҫ�������B��Ԫ�������½�㣬B���ֲ��䡣
void unionSet(node *A, node *B)
{
	node*pa,*pb,*u,*R;  //pa��pbΪA��B��ǰ���ָ��
	pa=A;
	pb=B->next;
	while(pa->next!=A && pb!=B)
	{
		if(pa->next->data<pb->data)        //paΪ�������
			pa=pa->next;
		else if(pa->next->data>pb->data)   //pa�п�����pb�ĺ��棬pb�ǽ�����㣬�����½�����pa֮��
		{                            //pa����
			u=new node;
			u->data=pb->data;
			u->next=pa->next;
			pa->next=u;	
			pb=pb->next;
		}
		else    //pa->data==pb->data��pa��pbͬʱ����
		{
			pa=pa->next;
			pb=pb->next;		
		}	
	}
	    //���Bû�н�����pb��������Ϊ������㣬����A��
	while(pb!=B)
	{
		u=new node;
		u->data=pb->data;
		u->next=pa->next;
		pa->next=u;
		pa=u;           //paָβ���
		pb=pb->next;	
	}
}

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
	cout<<"������ѭ������-->>"<<endl;
	createSCLLR(A);  //������ѭ������A
	createSCLLR(B);  //������ѭ������B

	printList(A);
	printList(B);

	unionSet(A,B);

	printList(A);
	

	destroyList(A);
	destroyList(B);

	return 0;
}

