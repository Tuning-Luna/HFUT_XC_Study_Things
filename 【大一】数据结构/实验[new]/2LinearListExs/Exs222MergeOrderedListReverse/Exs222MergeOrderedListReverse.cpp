// Exs222MergeOrderedListReverse.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//���������A��B���ϲ�Ϊ�ݼ������
#include "stdafx.h"

#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++ͷ�ļ�

#include "..\linkedList.h" 

node* MergeAndReverse(node *A, node *B)
{  
	node *pa,*pb,*p,*L;
	      //pa��pb�ֱ�ָ��A��B�����õĽ��
	L=A;  //��LΪ�ϲ����ͷ���ָ�룬��A��ͷ���ָ��
	pa=A->next;
	pb=B->next;
	L->next=NULL;
	while(pa!=NULL && pb!=NULL)
	{
		if(pa->data<pb->data)
		{
			p=pa->next; //pָ��A��һ��δ���ý��
			pa->next=L->next;  
			L->next=pa;  //pa��Ϊ�Ѿ��ϲ����ֵĵ�һ����㣬�����������
			pa=p;  //paָ��A��һ��δ���ý��
		}
		else
		{
			p=pb->next;  //pָ��B��һ��δ���ý��
			pb->next=L->next;  //pb->nextָ���Ѿ��ϲ����ֵĵ�һ�����ݽ��
			L->next=pb;  //pb��Ϊ�Ѿ��ϲ����ֵĵ�һ����㣬�����������
			pb=p; //pbָ��A��һ��δ���ý��
		}
	}
	//���´���A��B���ȳ����
	while(pa!=NULL && pb==NULL)  //Aδ������B�Ѿ�����
	{  
        p=pa->next;
		pa->next=L->next;
		L->next=pa;
		pa=p;
	}
	while(pa==NULL && pb!=NULL)  //A�ѽ�����Bδ����
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
	cout<<"������������ϲ�-->>"<<endl;
	La=createListR3();
	Lb=createListR3();
	//��ӡ����������
	node *pa,*pb;
	cout<<"����A������"<<endl;
	pa=La->next;
	while(pa)
	{
		cout<<pa->data<<",";
		pa=pa->next;
	}
    cout<<endl;
	free(pa);

	cout<<"����B������"<<endl;
	pb=Lb->next;
	while(pb)
	{
		cout<<pb->data<<",";
		pb=pb->next;
	}
    cout<<endl;
	free(pb);
	//��ӡ���
	cout<<"�ϲ��������Ϊ:"<<endl;
	L=MergeAndReverse(La,Lb)->next;  //����ϲ��������������
	while(L)
	{
		cout<<L->data<<",";
		L=L->next;
	}
    cout<<endl;
	
	destroyList(L);  //���������ͷ�heap�ڴ�

	return 0;
}

