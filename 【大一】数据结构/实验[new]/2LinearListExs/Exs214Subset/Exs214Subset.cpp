// Exs214Subset.cpp : Defines the entry point for the console application.
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

int SubSetDecision( node* A, node* B )
{
	node *pa, *pb;
	pa=A->next;  //pa��pb�ֱ�ָ��A��B�����Ԫ�ؽ��
	pb=B->next;
	while(pa!=NULL && pb!=NULL)
	{
		if(pa->data==pb->data)  //paָʾԪ����B�У����ǽ�ϣ�pa��pbͬʱ����һ�����
		{
			pa=pa->next;
			pb=pb->next;
		}
		else if(pa->data>pb->data)  //paָʾԪ�ؿ�����B��pbָʾ��Ԫ�غ��棬����pb
			pb=pb->next;
		else
			return 0;  //��ʱ��pa->data<pb->data��paָʾԪ�ز�������B�У�A��B�Ӽ�
	}
	  //�������A��B�Ľ���������ж�A�Ƿ�B���Ӽ�
	if(pa==NULL)
		return 1;  //A����β��ȫ��Ԫ����B�У���B���Ӽ�
	else
		return 0;  //B����β��Aδ����β��A����Ԫ�ز���B�У�A��B���Ӽ�

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
	p=A->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;
	cout<<"����B������"<<endl;
	p=B->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;


	    //�����Ӽ��ж�����
	if(SubSetDecision(A,B))
		cout<<"A �� B ���Ӽ���"<<endl;
	else
		cout<<"A ���� B �Ӽ���"<<endl;


	destroyList(A);  //���������ͷ�heap�ڴ�
	destroyList(B);



	return 0;
}
