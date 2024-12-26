// Exs212ScrLlMerge.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdlib.h"
#include "iostream.h"
#include "time.h"

#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "..\scLinkedList.h"


//��ͷ��㵥ѭ������ϲ�
//�ϲ�����A�����ͷ��㣬A��β����B����Ԫ�ؽ��
//B��β����nextָ��A
//����B��ͷ���
void scMergeAB(node *&A, node *&B)
{
	node *u,*p; 
    p=B;
	if(p->next==B)            //B��Ϊ�ձ�ɾ��B��ͷ����������
	{
		delete B;
		return;
	}
	      //����ΪB�������
	p=A;
	while(p->next!=A)         //pָ��A��β���
	{
		p=p->next;	
	}	
	p->next=B->next;          //A��β����B����Ԫ�ؽ��
	
	p=B;
	while(p->next!=B)         //pָ��B��β���
	{
		p=p->next;
	}
	p->next=A;                //�γ�ѭ��	
	delete B;                 //�ͷ�B��ͷ���
}

int main(int argc, char* argv[])
{
	node *A,*B;
	cout<<"������ѭ������A��"<<endl;
	createSCLLR(A);
	cout<<"������ѭ������B��"<<endl;
	createSCLLR(B);
	
	cout<<"A��";
	printList(A);
	cout<<"B��";
	printList(B);
	    //�ϲ���A��B
	scMergeAB(A,B);
	cout<<"A��B�ϲ����";
	printList(A);
	
     destroyList(A);  //�ͷźϲ���ı�
	_CrtDumpMemoryLeaks();  //debug ģʽ�¼���Ƿ��ڴ�й©	
	return 0;
}

