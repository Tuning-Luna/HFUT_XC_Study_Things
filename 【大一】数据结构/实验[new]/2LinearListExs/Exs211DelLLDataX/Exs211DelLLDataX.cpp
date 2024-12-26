// Exs211DelLLDataX.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "iostream.h"
#include "stdlib.h"
#include "time.h"

#include "..\linkedList.h" 

//��2.11������㷨��ɾ��������ֵΪx��Ԫ�ؽ�㡣
//��������
//��������������ͬ��ֻ������Ҫ���ݽ���Ԫ��ֵ��������ɾ��Ŀ�����ָ�롣
//�����ɾ�����ƣ�������һ��ָ��pָ���ɾ������ǰ������p->nextָ��Ŀ���㡣
//��ʼ��ʱ��p=L����ָ��ͷ��㡣
//ѭ����p->next->data��x���бȽϣ�����ȣ�p->nextָʾ�ļ��Ǵ�ɾ����㡣
//���⣬Ҫ����x���ڱ��������

//ֻɾ���ҵ���һ��x���
bool listDeleteX(node* L, elementType x)
{
	node* u;
	node* p=L;   //pָ���ɾ������ǰ������ʼ��ָ��ͷ���
	int succ=0;  //�Ƿ�ɾ���ɹ���ǣ��ɹ�ɾ��succ=1��ʧ��succ=0��
	while(p->next)
	{
		if(p->next->data==x)
		{     //�ҵ�Ŀ���㣬ɾ���˽��
			u=p->next;  //pָ��Ŀ�����ǰ����p->nextָ���ɾ��Ŀ���㣬����u���档 
			p->next=u->next;  //ai-1��nextָ��ai+1�ڵ㣬��Ϊ�գ�ai-1Ϊ���ڵ㣩
			delete u;   //�ͷ�Ŀ����ռ�ݵĿռ�
			succ=1;     //��ǽ��ɹ�ɾ��

			break; //�˳�ѭ��
		}
		p=p->next;
	}

	if(succ==1)
		return true;   //�ɹ�ɾ�������� true
	else
		return false;  //ɾ��ʧ�ܣ����� false��x���������С�	
}

//ɾ������Ԫ��ֵΪx�Ľ��
bool listDeleteX1(node* L, elementType x)
{
	node* u;
	node* p=L;   //pָ���ɾ������ǰ������ʼ��ָ��ͷ���
	int succ=0;  //�Ƿ�ɾ���ɹ���ǣ��ɹ�ɾ��succ=1��ʧ��succ=0��
	while(p->next)
	{
		if(p->next->data==x)
		{      //�ҵ�Ŀ���㣬ɾ���˽��
			u=p->next;  //pָ��Ŀ�����ǰ����p->nextָ���ɾ��Ŀ���㣬����u���档 
			p->next=u->next;  //ai-1��nextָ��ai+1�ڵ㣬��Ϊ�գ�ai-1Ϊ���ڵ㣩
			delete u;   //�ͷ�Ŀ����ռ�ݵĿռ�
			succ=1;     //��ǽ��ɹ�ɾ��
		}               //ɾ������p->nextΪһ���µĽ�㣬����ֵҲΪx����p���ƶ�
		else
			p=p->next;
	}

	if(succ==1)
		return true;   //�ɹ�ɾ�������� true
	else
		return false;  //ɾ��ʧ�ܣ����� false��x���������С�	
}

//����ɾ��һ�����
bool listDeleteX2(node* L, elementType x)
{
	node* u;
	node* p=L;   //pָ���ɾ������ǰ������ʼ��ָ��ͷ���

	while(p->next && p->next->data!=x)
	{
		p=p->next;  //��λԪ��ֵΪx�Ľ��
	}
	if(p==NULL || p->next==NULL)
		return false;
	else
	{
		u=p->next;  //pָ��Ŀ�����ǰ����p->nextָ���ɾ��Ŀ���㣬����u���档 
		p->next=u->next;  //ai-1��nextָ��ai+1�ڵ㣬��Ϊ�գ�ai-1Ϊ���ڵ㣩
		delete u;   //�ͷ�Ŀ����ռ�ݵĿռ�
		return true;
	}
}



int main(int argc, char* argv[])
{
	node *L,*p;
	elementType x;

	cout<<"β�巨--������ͷ���ĵ�����-->>"<<endl;
	L=createListR3();

		//��ӡ����������
	cout<<"���������"<<endl;
	p=L->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;


	cout<<"�������ɾ����Ԫ�أ���������x=";
	cin>>x;

	//����ɾ��������������Ƿ�ɾ���ɹ�
	if(listDeleteX(L, x))
		cout<<"ɾ������Ԫ�� "<<x<<" �ڵ�ɹ���"<<endl;
	else
		cout<<"��Ǹ��Ԫ��"<<x<<"���ڱ��У�ɾ��ʧ�ܣ�"<<endl;


	//��ӡɾ���ڵ�������
	cout<<"ɾ���ڵ�������Ԫ�أ�"<<endl;
	p=L->next;
	while(p)
	{
		cout<<p->data<<",";
		p=p->next;
	}
    cout<<endl;

	destroyList(L);  //���������ͷ�heap�ڴ�
	return 0;
}
