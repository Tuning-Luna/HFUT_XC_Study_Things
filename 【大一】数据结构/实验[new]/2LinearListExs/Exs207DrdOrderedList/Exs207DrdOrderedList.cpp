// Exs207DrdOrderedList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


//ɾ����������˳����е��ظ�Ԫ��
//���㷨˼�롿
//��Ԫ�طֳ��������֣���߲���Ϊ�Ѿ�ȡ�õ���ЧԪ�ز��֣���һ���±�iָʾ���һ��Ԫ�ء�
//�ұ߲����Ǵ�����Ĳ��֣���һ���±�jָʾ��һ��Ԫ�ء�
//����±�С��i��Ԫ���Ѿ�������ظ�������i�ǵ�ǰ���ڴ����Ԫ�أ�
//��data[i]��data[j]���бȽϣ���������������
//data[i]==data[j]��˵��jָʾ����i���ظ�Ԫ�أ���������j����һ��Ԫ�أ���ִ��j++
//data[i]<data[j]�� ˵��jָʾԪ����i��Ԫ�ز�ͬ�����i+1!=j����jԪ�ظ��Ƶ�i+1����ִ��j++
//ѭ��ִ������������ֱ����β��
//ʱ�临�Ӷȣ�O(n)
#include "stdafx.h"
#include "iostream.h"

#include "..\seqList.h"

//ɾ������˳����ظ�Ԫ��
void DeleteRepeatData(seqList & L)
{
	int i,j;  //�ֱ�ָ���Ѵ��������Ԫ�غ�δ�����ֵ�һ��Ԫ�أ���Ϊ�����±�

	if(L.listLen<2)
		return;   //����2��Ԫ�أ�ֱ���˳�
	i=0;  //��ʼ��iָ���һ��Ԫ��
	j=1;  //jָ��ڶ���Ԫ��
	while(j<L.listLen)
	{
		if(L.data[i]==L.data[j])  //jΪ�ظ�Ԫ�أ�j����
			j++;
		else  //��ΪLΪ����������ʣ�������L.data[i]<L.data[j]��jΪ������Ч����
		{
			    //���j==i+1��˵��j������i�������ƶ�Ԫ�أ�ֱ�Ӱ�i��jͬʱ����һλ����
			if((i+1)!=j)
				L.data[i+1]=L.data[j];  //jԪ�ظ��Ƶ�i+1

			i++;  //�����������������Ҫͬʱ����i��j
			j++;
		}
	}
	L.listLen=i+1;  //�޸ı��ʵ�ʳ���
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

