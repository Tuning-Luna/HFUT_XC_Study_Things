// Exs213LlSubset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++ͷ�ļ�

#include "..\linkedList.h" 

int llSubset( node* A, node* B )
{
	node *pa, *pb;
	int suc=0;
	pa=A->next;        //paָ��A����Ԫ�ؽ��	
	while(pa!=NULL)
	{
		pb=B->next;    //pbָ��B�����Ԫ�ؽ��
		suc=0;
		while(pb!=NULL)
		{
			if(pa->data==pb->data)
			{
				suc=1;
				break;
			}
			else
				pb=pb->next;		
		}
		if(suc==0)    //pa->data����B�У�A����B���Ӽ�������0
			break;	

		pa=pa->next;	
	}
	return suc;
}

int main(int argc, char* argv[])
{
	node *A, *B, *p;

	cout<<"β�巨��������A-->>"<<endl;
	A=createListR3();

	cout<<"β�巨��������B-->>"<<endl;
	B=createListR3();

		//��ӡ����������
	cout<<"����A������"<<endl;
	printList(A);
    cout<<endl;
	cout<<"����B������"<<endl;
	printList(B);
    cout<<endl;

	    //�����Ӽ��ж�����
	if(llSubset(A,B))
		cout<<"A �� B ���Ӽ���"<<endl;
	else
		cout<<"A ���� B �Ӽ���"<<endl;

	destroyList(A);  //���������ͷ�heap�ڴ�
	destroyList(B);
	return 0;
}

