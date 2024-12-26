// Exs220MergeOrderedLists.cpp : Defines the entry point for the console application.
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


void MergeLinkedList(node* La,node *Lb)
{
	//�ϲ�����LaΪͷ���ָ�룬Lbָʾ��ͷ���ɾ��
	node*pa,*pb,*R;  //pa��pbΪLa��Lb��ǰ���ָ��
	                 //RΪ�Ѿ����ֵ�βָ�� 
	pa=La->next;
	pb=Lb->next;
	R=La;
	while(pa!=NULL && pb!=NULL)
	{
		if(pa->data<=pb->data)
		{
			R->next=pa;  //paָʾ��㣬�ӵ��ϲ���β
			R=pa;  //�ƶ�βָ��
			pa=pa->next;  //paָ����һ�����
		}
		else
		{
			R->next=pb; //pb���ӵ��ϲ���β
			R=pb;  //�ƶ�βָ��
			pb=pb->next;  //pb�Ƶ���һ�����

		}	
	}
	   //���´���2���ȳ������
	   //����һ�������Ѿ���������һ������δ������
	   //Ԫ��ֵ����С���Ѿ����벿�֣�ֱ�ӽ��뼴��
	if(pa!=NULL)  //Lb�ѽ�����Laδ����
		R->next=pa ;
	else   //La�ѽ�����Lbδ����
		R->next=pb ;  

	delete(Lb);  //ɾ�� Lb ͷ���
}

void main(int argc, char* argv[])
{
	
	node *La,*Lb;
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


	cout<<"����B������"<<endl;
	pb=Lb->next;
	while(pb)
	{
		cout<<pb->data<<",";
		pb=pb->next;
	}
    cout<<endl;
	
	MergeLinkedList(La,Lb);  //���úϲ�����

	cout<<"���ӽ��Ϊ��"<<endl;
	pa=La->next;
	while(pa!=NULL)
	{
		cout<<pa->data<<",";
		pa=pa->next;
	}
    cout<<endl;

	//��ӡ���
	destroyList(La);  //���������ͷ�heap�ڴ�
	

}

