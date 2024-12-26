// Exs221ListReverse.cpp : Defines the entry point for the console application.
//

//����������
#include "stdafx.h"


#define _CRTDBG_MAP_ALLOC //�ڴ�й©�����
#include<stdlib.h> 
#include<crtdbg.h> 

#include "stdio.h"
#include "stdlib.h"

#include "time.h"

#include "iostream.h"  //C++ͷ�ļ�

#include "..\linkedList.h" 

//������ľ͵����� 
void ListReverse(node *L)
{      
    node *p,*u;   
    p=L->next;      //pָ����Ԫ�ؽ��
	L->next=NULL;   //ͷ������������next�ÿ�
    while(p!=NULL)
	{   
        u=p;  //ȡ��δ���ò��ֵĵ�һ����㣬ָ��Ϊu
		p=p->next;  //p����һ����㣬����δ���ò��ֵ�ͷָ��
		      //����uָ��Ľ�㣬����ͷ�巨��������
        u->next=L->next;   
        L->next=u;           
    } 
}  


int main(int argc, char* argv[])
{
	node *L,*p;
	cout<<"������ͷ���ĵ�����L-->>"<<endl;
	L=createListR3();

	//��ӡ����������
	cout<<"����L������㣺"<<endl;
	p=L->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;
	//��ӡ���ý��
	
	cout<<"�������ý��Ϊ��"<<endl;
	ListReverse(L);
	p=L->next;   
    while(p!=NULL)
	{   
		cout<<p->data<<",";
		p=p->next;   
	}
	destroyList(L);  //���������ͷ�heap�ڴ�
	cout<<endl;


	return 0;
}

