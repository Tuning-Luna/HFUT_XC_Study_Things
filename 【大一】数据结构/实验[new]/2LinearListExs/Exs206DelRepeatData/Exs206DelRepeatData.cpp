// Exs206DelRepeatData.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "iostream.h"

#include "..\seqList.h"

//ɾ���ظ�Ԫ��
void DeleteRepeatData(seqList & L)
{
	int i,j;
	if(L.listLen==0)
	{
		cout<<"��ǰ˳���գ�"<<endl;
		return;
	}
	if(L.listLen==1)
	{
		cout<<"��ǰ˳���ֻ��һ��Ԫ�أ�"<<endl;
		return;
	}
	i=0;	
	while(i<L.listLen-1)
	{
		for(j=L.listLen-1; j>i; j--)  //�Ӻ���ǰɾ����Ч�ʽϸ�
		{
			if(L.data[i]==L.data[j])  //Ԫ���ظ�������ɾ��
			{
				listDelete(&L,j+1); //����ɾ���������±��1������+1
				//int k;
				//for(k=j; k<L.listLen-1; k++)
				//	L.data[k]=L.data[k+1];
				//L.listLen--;
			}
		}

		i++;
	}

  
}


int main(int argc, char* argv[])
{
	seqList L;
	int i,j;
	elementType x;

	initialList(&L);  //��ʼ��˳���

	cout<<"����˳���==>����������Ԫ�أ�'-100'��������"<<endl;
	cin>>x;
	i=0;
	while(x!=-100)
	{
		L.data[i]=x;
		i++;
		L.listLen++;  //����+1

		cin>>x;
	}
	

	//��ӡ��������
	cout<<"��ǰ��Ԫ�أ�";
	for(i=0;i<L.listLen;i++)
		cout<<L.data[i]<<", ";
	cout<<endl;


	DeleteRepeatData(L);  //����ɾ���ظ�Ԫ��
	
	//��ӡɾ���ظ�Ԫ�غ��˳���
	cout<<"ɾ���ظ�Ԫ�غ�˳���";
	for(i=0; i<L.listLen; i++)
		cout<<L.data[i]<<", ";
	cout<<endl;

	return 0;
}

